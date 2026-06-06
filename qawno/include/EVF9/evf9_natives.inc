/*
 * EVF9 - Extended Vehicle Framework 9
 *
 * Created by EAST9
 * Open.MP / SA-MP Community
 *
 * GitHub: https://github.com/east9-777
 * Discord: leivison0230_58048
 * Comunidade Discord: https://discord.gg/FJBWAesQV8
 *
 * Copyright (c) EAST9
 * Do not remove this notice.
 */

#if defined _evf9_natives_included
    #endinput
#endif
#define _evf9_natives_included

#include <EVF9/evf9_flags>
#include <EVF9/evf9_config>

// ---------------------------------------------------------------------------
// Core
// ---------------------------------------------------------------------------

native EVF9_GetVersion();
native EVF9_GetBuild();

// ---------------------------------------------------------------------------
// *** INTERNAL NATIVES (called by Pawn stocks — do not call directly) ***
//
// These C++ natives manage plugin state only. The Pawn stocks below handle
// the SA-MP/OMP server API calls (CreateVehicle, DestroyVehicle, etc.)
// ---------------------------------------------------------------------------

native EVF9_RegisterVehicle(vehicleid, modelid,
    Float:x, Float:y, Float:z, Float:angle,
    color1, color2, worldid, interiorid);

native EVF9_UnregisterVehicle(vehicleid);
native EVF9_RespawnVehicleInternal(vehicleid);
native EVF9_SetVehicleHealthInternal(vehicleid, Float:health);

// Called by the gamemode to keep plugin position cache in sync:
//   EVF9_UpdateVehiclePosition(vehicleid, x, y, z)
// Triggers EVF9_OnVehicleLocationUpdate if the vehicle is tracked.
native EVF9_UpdateVehiclePosition(vehicleid, Float:x, Float:y, Float:z);

// ---------------------------------------------------------------------------
// Pool — PUBLIC API (Pawn stocks that wrap server API + internal natives)
// ---------------------------------------------------------------------------

/**
 * CreateManagedVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2)
 *
 * Creates a vehicle on the server (CreateVehicle) and registers it with EVF9.
 * Fires EVF9_OnVehicleCreate on success.
 * Returns the vehicleid (same as SA-MP CreateVehicle) or INVALID_VEHICLE_ID.
 *
 * NOTE: This is a Pawn stock, not a native, so the server call and the plugin
 * registration happen atomically in Pawn. No C++ re-entrancy issues.
 */
stock CreateManagedVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2)
{
    new vehicleid = CreateVehicle(modelid, x, y, z, angle, color1, color2, -1);
    if (vehicleid == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
    if (!EVF9_RegisterVehicle(vehicleid, modelid, x, y, z, angle, color1, color2, 0, 0))
    {
        DestroyVehicle(vehicleid);
        return INVALID_VEHICLE_ID;
    }
    return vehicleid;
}

/**
 * CreateManagedVehicleEx — extended version with world and interior.
 */
stock CreateManagedVehicleEx(modelid, Float:x, Float:y, Float:z, Float:angle,
    color1, color2, worldid, interiorid)
{
    new vehicleid = CreateVehicle(modelid, x, y, z, angle, color1, color2, -1);
    if (vehicleid == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
    if (!EVF9_RegisterVehicle(vehicleid, modelid, x, y, z, angle, color1, color2, worldid, interiorid))
    {
        DestroyVehicle(vehicleid);
        return INVALID_VEHICLE_ID;
    }
    SetVehicleVirtualWorld(vehicleid, worldid);
    SetVehicleInterior(vehicleid, interiorid);
    return vehicleid;
}

/**
 * DestroyManagedVehicle(vehicleid)
 * Removes vehicle from EVF9 pool and destroys it on the server.
 */
stock DestroyManagedVehicle(vehicleid)
{
    if (!IsManagedVehicle(vehicleid)) return 0;
    EVF9_UnregisterVehicle(vehicleid);
    return DestroyVehicle(vehicleid);
}

/**
 * RespawnManagedVehicle(vehicleid)
 * Resets plugin state (health, fuel, flags) and respawns vehicle on server.
 */
stock RespawnManagedVehicle(vehicleid)
{
    if (!IsManagedVehicle(vehicleid)) return 0;
    EVF9_RespawnVehicleInternal(vehicleid);
    SetVehicleToRespawn(vehicleid);
    return 1;
}

/**
 * CloneManagedVehicle(vehicleid)
 * Creates an identical managed vehicle at the same spawn position.
 * Fires EVF9_OnVehicleClone(original, newvehicleid).
 */
stock CloneManagedVehicle(vehicleid)
{
    if (!IsManagedVehicle(vehicleid)) return INVALID_VEHICLE_ID;
    new modelid = EVF9_GetVehicleModel(vehicleid);
    new Float:x, Float:y, Float:z, Float:angle;
    EVF9_GetVehicleSpawnPos(vehicleid, x, y, z, angle);
    new worldid    = EVF9_GetVehicleWorld(vehicleid);
    new interiorid = EVF9_GetVehicleInterior(vehicleid);
    new newid = CreateManagedVehicleEx(modelid, x, y, z, angle, -1, -1, worldid, interiorid);
    if (newid != INVALID_VEHICLE_ID)
        CallLocalFunction("EVF9_OnVehicleClone", "ii", vehicleid, newid);
    return newid;
}

/**
 * EVF9_SetVehicleHealth(vehicleid, Float:health)
 * Updates plugin state AND syncs to the server (SetVehicleHealth).
 */
stock EVF9_SetVehicleHealth(vehicleid, Float:health)
{
    if (!IsManagedVehicle(vehicleid)) return 0;
    EVF9_SetVehicleHealthInternal(vehicleid, health);
    SetVehicleHealth(vehicleid, health);
    return 1;
}

// ---------------------------------------------------------------------------
// Iterator (linked list — never loops MAX_VEHICLES)
// ---------------------------------------------------------------------------

native EVF9_GetFirstVehicle();
native EVF9_GetNextVehicle(vehicleid);

/**
 * EVF9_FOREACH_VEHICLE(var)
 * Iterates over all managed vehicles using the internal linked list.
 * O(n) over managed vehicles only — never scans MAX_VEHICLES slots.
 *
 * Example:
 *   EVF9_FOREACH_VEHICLE(vid) {
 *       printf("Vehicle: %d model: %d", vid, EVF9_GetVehicleModel(vid));
 *   }
 */
#define EVF9_FOREACH_VEHICLE(%0) \
    for(new %0 = EVF9_GetFirstVehicle(); %0 != INVALID_VEHICLE_ID; %0 = EVF9_GetNextVehicle(%0))

// ---------------------------------------------------------------------------
// Entity getters
// ---------------------------------------------------------------------------

native EVF9_GetVehicleModel(vehicleid);
native EVF9_GetVehicleSpawnPos(vehicleid, &Float:x, &Float:y, &Float:z, &Float:angle);
native EVF9_GetVehicleWorld(vehicleid);
native EVF9_GetVehicleInterior(vehicleid);
native E_VEHICLE_FLAGS:EVF9_GetVehicleFlags(vehicleid);
native EVF9_GetVehicleCreatedAt(vehicleid);
native EVF9_GetVehicleLastDriver(vehicleid);
native EVF9_GetVehicleLastOccupant(vehicleid);
native Float:EVF9_GetVehicleDistanceTravelled(vehicleid);
native EVF9_GetVehicleLifetime(vehicleid);
native EVF9_IsVehicleSpawned(vehicleid);
native EVF9_IsVehicleManaged(vehicleid);
native EVF9_GetVehicleHealth(vehicleid, &Float:health);
native EVF9_SetVehicleTag(vehicleid, const tag[]);
native EVF9_GetVehicleTag(vehicleid, tag[], len);

// UIDs — permanent across respawns
native EVF9_GetVehicleUID(vehicleid);
native EVF9_GetVehicleByUID(uid);

// ---------------------------------------------------------------------------
// Pool queries
// ---------------------------------------------------------------------------

native IsManagedVehicle(vehicleid);
native CountManagedVehicles();

// ---------------------------------------------------------------------------
// Flags
// ---------------------------------------------------------------------------

native EVF9_SetVehicleFlag(vehicleid, E_VEHICLE_FLAGS:flag);
native EVF9_ClearVehicleFlag(vehicleid, E_VEHICLE_FLAGS:flag);
native EVF9_HasVehicleFlag(vehicleid, E_VEHICLE_FLAGS:flag);

// ---------------------------------------------------------------------------
// Persistence — save state machine
// ---------------------------------------------------------------------------

enum EVF9_SAVE_STATE
{
    EVF9_STATE_CLEAN  = 0,
    EVF9_STATE_DIRTY  = 1,
    EVF9_STATE_SAVING = 2
};

native EVF9_BeginSave(vehicleid);
native EVF9_EndSave(vehicleid);
native EVF9_IsVehicleSaving(vehicleid);
native EVF9_SAVE_STATE:EVF9_GetVehicleSaveState(vehicleid);

// ---------------------------------------------------------------------------
// Distance & speed (position cache — feed via EVF9_UpdateVehiclePosition)
// ---------------------------------------------------------------------------

native ResetVehicleDistance(vehicleid);
native GetVehicleLifetimeDistance(vehicleid, &Float:dist);
native GetVehicleTripDistance(vehicleid, &Float:dist);
native GetNearestManagedVehicle(Float:x, Float:y, Float:z, &Float:dist);
native CountVehiclesByModel(modelid);
native CountVehiclesByOwner(playerid);

// ---------------------------------------------------------------------------
// State queries
// ---------------------------------------------------------------------------

native IsVehicleLocked(vehicleid);
native IsVehicleEngineOn(vehicleid);
native IsVehicleTracked(vehicleid);
native IsVehicleAbandoned(vehicleid);

// ---------------------------------------------------------------------------
// Metadata (7 slots)
// ---------------------------------------------------------------------------

enum EVF9_METADATA
{
    META_JOB      = 0,
    META_COMPANY,
    META_GROUP,
    META_EVENT,
    META_CUSTOM_1,
    META_CUSTOM_2,
    META_CUSTOM_3
};

native SetVehicleMeta(vehicleid, EVF9_METADATA:key, value);
native GetVehicleMeta(vehicleid, EVF9_METADATA:key);
native ResetVehicleMeta(vehicleid, EVF9_METADATA:key);
native HasVehicleMeta(vehicleid, EVF9_METADATA:key);

// ---------------------------------------------------------------------------
// Custom typed properties (dynamic schema)
// ---------------------------------------------------------------------------

enum EVF9_PROPERTY_TYPE
{
    EVF9_PROP_INT    = 0,
    EVF9_PROP_FLOAT,
    EVF9_PROP_STRING
};

native RegisterVehicleProperty(const name[], EVF9_PROPERTY_TYPE:type);
native SetVehiclePropertyInt(vehicleid, propertyid, value);
native SetVehiclePropertyFloat(vehicleid, propertyid, Float:value);
native SetVehiclePropertyString(vehicleid, propertyid, const value[]);
native GetVehiclePropertyInt(vehicleid, propertyid);
native GetVehiclePropertyFloat(vehicleid, propertyid, &Float:value);
native GetVehiclePropertyString(vehicleid, propertyid, value[], len);
native HasVehicleProperty(vehicleid, propertyid);

// ---------------------------------------------------------------------------
// Addon system
// ---------------------------------------------------------------------------

#define INVALID_ADDON_ID (-1)
native EVF9_RegisterAddon(const name[], const version[]);
native EVF9_UnregisterAddon(addonid);
native EVF9_GetAddonInfo(addonid, name[], namelen, version[], verlen);

// ---------------------------------------------------------------------------
// Performance & memory
// ---------------------------------------------------------------------------

native EVF9_GetMemoryUsage();
native EVF9_GetVehicleMemoryUsage(vehicleid);
native EVF9_GetModuleMemoryUsage(module[]);
native EVF9_StartProfiler();
native EVF9_StopProfiler();
native EVF9_GetProfilerData(data[], len);

native EVF9_GetTotalVehicles();
native EVF9_GetManagedVehicles();
native EVF9_GetSpawnedVehicles();
native EVF9_GetDestroyedVehicles();
native EVF9_GetAbandonedVehicles();

// ---------------------------------------------------------------------------
// Fuel module
// ---------------------------------------------------------------------------

native SetVehicleFuel(vehicleid, Float:fuel);
native GetVehicleFuel(vehicleid, &Float:fuel);
native AddVehicleFuel(vehicleid, Float:amount);
native RemoveVehicleFuel(vehicleid, Float:amount);
native IsVehicleOutOfFuel(vehicleid);
native GetFuelConsumptionRate(vehicleid, &Float:rate);
native SetFuelConsumptionRate(vehicleid, Float:rate);

// ---------------------------------------------------------------------------
// Ownership module
// ---------------------------------------------------------------------------

native SetVehicleOwner(vehicleid, playerid);
native GetVehicleOwner(vehicleid);
native GetVehicleOwnerName(vehicleid, name[], len);
native RemoveVehicleOwner(vehicleid);
native TransferVehicleOwnership(vehicleid, newownerid);
native IsVehicleOwner(vehicleid, playerid);
native GetOwnedVehicleCount(playerid);

// ---------------------------------------------------------------------------
// Damage module
// ---------------------------------------------------------------------------

native RepairVehicleDamage(vehicleid, playerid = -1);
native DestroyVehiclePart(vehicleid, partid);
native RepairVehiclePart(vehicleid, partid, playerid = -1);
native GetVehicleDamageData(vehicleid, &panels, &doors, &lights, &tires);
native ResetVehicleDamage(vehicleid);

// ---------------------------------------------------------------------------
// Keys & lock module
// ---------------------------------------------------------------------------

native GiveVehicleKey(vehicleid, playerid);
native RemoveVehicleKey(vehicleid, playerid);
native HasVehicleKey(vehicleid, playerid);
native LockVehicle(vehicleid, playerid = -1);
native UnlockVehicle(vehicleid, playerid = -1);
native GetVehicleKeyCount(vehicleid);
native ClearVehicleKeys(vehicleid);

// ---------------------------------------------------------------------------
// Fleet module
// ---------------------------------------------------------------------------

#define INVALID_FLEET_ID (-1)
native CreateFleet(const name[], ownerid);
native DestroyFleet(fleetid);
native AssignVehicleToFleet(vehicleid, fleetid);
native RemoveVehicleFromFleet(vehicleid);
native GetFleetVehicleCount(fleetid);
native GetVehicleFleet(vehicleid);
native GetFleetOwner(fleetid);
native IsVehicleInFleet(vehicleid);

// ---------------------------------------------------------------------------
// Tracking module
// ---------------------------------------------------------------------------

native TrackVehicle(vehicleid, playerid = -1);
native UntrackVehicle(vehicleid, playerid = -1);
native GetVehicleLastPosition(vehicleid, &Float:x, &Float:y, &Float:z);
native GetVehicleTravelDistance(vehicleid, &Float:dist);
//em ajustes
//native GetVehicleDistanceFromPoint(vehicleid, Float:x, Float:y, Float:z, &Float:dist);

// ---------------------------------------------------------------------------
// Insurance module
// ---------------------------------------------------------------------------

native InsureVehicle(vehicleid, duration);
native CancelInsurance(vehicleid);
native IsVehicleInsured(vehicleid);
native GetInsuranceExpiration(vehicleid);
native RenewInsurance(vehicleid, duration);
