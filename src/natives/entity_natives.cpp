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

#include "../vehicle_system.h"
#include "../amx_helpers.h"
#include <amx/amx.h>
#include <cstring>
#include <algorithm>
#include <cmath>

#define INVALID_VEHICLE_ID (-1)

/* native EVF9_GetVehicleModel(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleModel(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? v->modelid : 0;
}

/* native EVF9_GetVehicleSpawnPos(vehicleid, &Float:x, &Float:y, &Float:z, &Float:angle); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleSpawnPos(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->spawnX);
    evf9_SetFloat(amx, params[3], v->spawnY);
    evf9_SetFloat(amx, params[4], v->spawnZ);
    evf9_SetFloat(amx, params[5], v->spawnAngle);
    return 1;
}

/* native EVF9_GetVehicleWorld(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleWorld(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? v->worldid : 0;
}

/* native EVF9_GetVehicleInterior(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleInterior(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? v->interiorid : 0;
}

/* native E_VEHICLE_FLAGS:EVF9_GetVehicleFlags(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleFlags(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->flags : 0;
}

/* native EVF9_GetVehicleCreatedAt(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleCreatedAt(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->createdAt : 0;
}

/* native EVF9_GetVehicleLastDriver(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleLastDriver(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? v->lastDriver : INVALID_VEHICLE_ID;
}

/* native EVF9_GetVehicleLastOccupant(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleLastOccupant(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? v->lastOccupant : INVALID_VEHICLE_ID;
}

/* native Float:EVF9_GetVehicleDistanceTravelled(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleDistanceTravelled(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    float d = v ? v->lifetimeDistance : 0.0f;
    return amx_ftoc(d);
}

/* native EVF9_IsVehicleSpawned(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_IsVehicleSpawned(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && (v->flags & VEHICLE_FLAG_SPAWNED)) ? 1 : 0;
}

/* native EVF9_IsVehicleManaged(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_IsVehicleManaged(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().isManaged(params[1]) ? 1 : 0;
}

/*
 * native EVF9_GetVehicleHealth(vehicleid, &Float:health);
 * Returns the plugin-stored health (authoritative).
 */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleHealth(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->health);
    return 1;
}

/*
 * native EVF9_SetVehicleHealthInternal(vehicleid, Float:health);
 * Updates plugin state only. The Pawn stock EVF9_SetVehicleHealth also
 * calls SetVehicleHealth(vehicleid, health) to sync with the server.
 */
cell AMX_NATIVE_CALL n_EVF9_SetVehicleHealthInternal(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    float health = evf9_GetFloat(params[2]);
    v->health = std::max(0.0f, std::min(1000.0f, health));
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}

/* native EVF9_SetVehicleTag(vehicleid, const tag[]); */
cell AMX_NATIVE_CALL n_EVF9_SetVehicleTag(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    char buf[65];
    evf9_GetStr(amx, params[2], buf, sizeof(buf));
    strncpy(v->tag, buf, 64);
    v->tag[64] = '\0';
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}

/* native EVF9_GetVehicleTag(vehicleid, tag[], len); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleTag(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetStr(amx, params[2], v->tag, params[3]);
    return 1;
}

/* native EVF9_GetVehicleUID(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleUID(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->uid : 0;
}

/* native EVF9_GetVehicleByUID(uid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleByUID(AMX* amx, const cell* params)
{
    int vid = VehicleSystem::getInstance().getByUID((uint32_t)params[1]);
    return vid != -1 ? vid : INVALID_VEHICLE_ID;
}

/* native EVF9_GetVehicleLifetime(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleLifetime(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    auto tp = std::chrono::system_clock::now().time_since_epoch();
    int64_t now = std::chrono::duration_cast<std::chrono::seconds>(tp).count();
    return (cell)(now - v->createdAt);
}

/* native ResetVehicleDistance(vehicleid); */
cell AMX_NATIVE_CALL n_ResetVehicleDistance(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->tripDistance = 0.0f;
    return 1;
}

/* native GetVehicleLifetimeDistance(vehicleid, &Float:dist); */
cell AMX_NATIVE_CALL n_GetVehicleLifetimeDistance(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->lifetimeDistance);
    return 1;
}

/* native GetVehicleTripDistance(vehicleid, &Float:dist); */
cell AMX_NATIVE_CALL n_GetVehicleTripDistance(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->tripDistance);
    return 1;
}

/* native IsVehicleLocked(vehicleid); */
cell AMX_NATIVE_CALL n_IsVehicleLocked(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && (v->flags & VEHICLE_FLAG_LOCKED)) ? 1 : 0;
}

/* native IsVehicleEngineOn(vehicleid); */
cell AMX_NATIVE_CALL n_IsVehicleEngineOn(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && (v->flags & VEHICLE_FLAG_ENGINE)) ? 1 : 0;
}

/* native IsVehicleTracked(vehicleid); */
cell AMX_NATIVE_CALL n_IsVehicleTracked(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && (v->flags & VEHICLE_FLAG_TRACKED)) ? 1 : 0;
}

/* native IsVehicleAbandoned(vehicleid); */
cell AMX_NATIVE_CALL n_IsVehicleAbandoned(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && (v->flags & VEHICLE_FLAG_ABANDONED)) ? 1 : 0;
}

/* native CountVehiclesByModel(modelid); */
cell AMX_NATIVE_CALL n_CountVehiclesByModel(AMX* amx, const cell* params)
{
    int model = params[1], count = 0;
    int vid = VehicleSystem::getInstance().getFirst();
    while (vid != -1)
    {
        auto* v = VehicleSystem::getInstance().get(vid);
        if (v && v->modelid == model) count++;
        vid = VehicleSystem::getInstance().getNext(vid);
    }
    return count;
}

/* native CountVehiclesByOwner(playerid); */
cell AMX_NATIVE_CALL n_CountVehiclesByOwner(AMX* amx, const cell* params)
{
    int owner = params[1], count = 0;
    int vid = VehicleSystem::getInstance().getFirst();
    while (vid != -1)
    {
        auto* v = VehicleSystem::getInstance().get(vid);
        if (v && v->ownerid == owner) count++;
        vid = VehicleSystem::getInstance().getNext(vid);
    }
    return count;
}

/* native GetNearestManagedVehicle(Float:x, Float:y, Float:z, &Float:dist); */
cell AMX_NATIVE_CALL n_GetNearestManagedVehicle(AMX* amx, const cell* params)
{
    float qx = evf9_GetFloat(params[1]);
    float qy = evf9_GetFloat(params[2]);
    float qz = evf9_GetFloat(params[3]);
    float minDist = 1e9f;
    int nearestId = INVALID_VEHICLE_ID;

    int vid = VehicleSystem::getInstance().getFirst();
    while (vid != INVALID_VEHICLE_ID)
    {
        auto* v = VehicleSystem::getInstance().get(vid);
        if (v)
        {
            float dx = v->lastPosX - qx;
            float dy = v->lastPosY - qy;
            float dz = v->lastPosZ - qz;
            float d  = sqrtf(dx*dx + dy*dy + dz*dz);
            if (d < minDist) { minDist = d; nearestId = vid; }
        }
        vid = VehicleSystem::getInstance().getNext(vid);
    }
    evf9_SetFloat(amx, params[4], minDist < 1e8f ? minDist : 0.0f);
    return nearestId;
}
