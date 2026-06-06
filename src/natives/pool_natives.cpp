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

/*
 * Pool natives — vehicle registration with real server vehicleids.
 *
 * ARCHITECTURE:
 *   The Pawn stock CreateManagedVehicle calls CreateVehicle (server native)
 *   to get a real vehicleid, then calls EVF9_RegisterVehicle (this C++ native)
 *   to register it in the plugin pool.
 *
 *   This design means the plugin NEVER creates a vehicle itself — it always
 *   manages a vehicleid that already exists in the server. This works
 *   identically on SA-MP and Open.MP with no SDK dependency.
 */

#include "../vehicle_system.h"
#include "../event_dispatcher.h"
#include "../amx_helpers.h"
#include <amx/amx.h>
#include <cmath>

#define INVALID_VEHICLE_ID (-1)

/* native EVF9_RegisterVehicle(vehicleid, modelid, Float:x, Float:y, Float:z,
                                Float:angle, color1, color2, worldid, interiorid); */
cell AMX_NATIVE_CALL n_EVF9_RegisterVehicle(AMX* amx, const cell* params)
{
    int   vehicleid  = params[1];
    int   modelid    = params[2];
    float x          = evf9_GetFloat(params[3]);
    float y          = evf9_GetFloat(params[4]);
    float z          = evf9_GetFloat(params[5]);
    float angle      = evf9_GetFloat(params[6]);
    int   color1     = params[7];
    int   color2     = params[8];
    int   worldid    = params[9];
    int   interiorid = params[10];

    bool ok = VehicleSystem::getInstance().registerVehicle(
        vehicleid, modelid, x, y, z, angle, color1, color2, worldid, interiorid);
    return ok ? 1 : 0;
}

/* native EVF9_UnregisterVehicle(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_UnregisterVehicle(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().unregisterVehicle(params[1]) ? 1 : 0;
}

/* native EVF9_RespawnVehicleInternal(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_RespawnVehicleInternal(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().respawnInternal(params[1]) ? 1 : 0;
}

/* native IsManagedVehicle(vehicleid); */
cell AMX_NATIVE_CALL n_IsManagedVehicle(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().isManaged(params[1]) ? 1 : 0;
}

/* native CountManagedVehicles(); */
cell AMX_NATIVE_CALL n_CountManagedVehicles(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().count();
}

/* native EVF9_UpdateVehiclePosition(vehicleid, Float:x, Float:y, Float:z); */
cell AMX_NATIVE_CALL n_EVF9_UpdateVehiclePosition(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    float nx = evf9_GetFloat(params[2]);
    float ny = evf9_GetFloat(params[3]);
    float nz = evf9_GetFloat(params[4]);
    /* Update distance tracking */
    float dx = nx - v->lastPosX;
    float dy = ny - v->lastPosY;
    float dz = nz - v->lastPosZ;
    float d  = sqrtf(dx*dx + dy*dy + dz*dz);
    v->lifetimeDistance += d;
    v->tripDistance     += d;
    v->lastPosX = nx;
    v->lastPosY = ny;
    v->lastPosZ = nz;
    if (v->flags & VEHICLE_FLAG_TRACKED)
        EventDispatcher::dispatch3f(params[1], nx, ny, nz, "EVF9_OnVehicleLocationUpdate");
    return 1;
}

/* native EVF9_SetVehicleLastDriver(vehicleid, playerid); */
cell AMX_NATIVE_CALL n_EVF9_SetVehicleLastDriver(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->lastDriver = params[2];
    return 1;
}
