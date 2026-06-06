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

#include "../../vehicle_system.h"
#include "../../event_dispatcher.h"
#include "../../amx_helpers.h"
#include <amx/amx.h>
#include <cmath>

/* native TrackVehicle(vehicleid, playerid = -1); */
cell AMX_NATIVE_CALL n_TrackVehicle(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->flags |= VEHICLE_FLAG_TRACKED;
    VehicleSystem::getInstance().markDirty(params[1]);
    int placer = (params[0] / sizeof(cell) >= 2) ? (int)params[2] : -1;
    EventDispatcher::dispatch2(params[1], placer, "EVF9_OnVehicleTrackStart");
    return 1;
}

/* native UntrackVehicle(vehicleid, playerid = -1); */
cell AMX_NATIVE_CALL n_UntrackVehicle(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->flags &= ~VEHICLE_FLAG_TRACKED;
    VehicleSystem::getInstance().markDirty(params[1]);
    int remover = (params[0] / sizeof(cell) >= 2) ? (int)params[2] : -1;
    EventDispatcher::dispatch2(params[1], remover, "EVF9_OnVehicleTrackStop");
    return 1;
}

/* native GetVehicleLastPosition(vehicleid, &Float:x, &Float:y, &Float:z); */
cell AMX_NATIVE_CALL n_GetVehicleLastPosition(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->lastPosX);
    evf9_SetFloat(amx, params[3], v->lastPosY);
    evf9_SetFloat(amx, params[4], v->lastPosZ);
    return 1;
}

/* native GetVehicleTravelDistance(vehicleid, &Float:dist); */
cell AMX_NATIVE_CALL n_GetVehicleTravelDistance(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->lifetimeDistance);
    return 1;
}

/* native GetVehicleDistanceFromPoint(vehicleid, Float:x, Float:y, Float:z, &Float:dist); */
cell AMX_NATIVE_CALL n_GetVehicleDistanceFromPoint(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    float px = evf9_GetFloat(params[2]);
    float py = evf9_GetFloat(params[3]);
    float pz = evf9_GetFloat(params[4]);
    float dx = v->lastPosX - px;
    float dy = v->lastPosY - py;
    float dz = v->lastPosZ - pz;
    float d  = sqrtf(dx*dx + dy*dy + dz*dz);
    evf9_SetFloat(amx, params[5], d);
    return 1;
}
