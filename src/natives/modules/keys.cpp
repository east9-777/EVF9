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
#include <unordered_map>
#include <unordered_set>

/*
 * Key registry: vehicleid → set of playerids that hold a key.
 * The owner (v->ownerid) always has implicit access regardless of this set.
 */
static std::unordered_map<int, std::unordered_set<int>> g_vehicleKeys;

/* native GiveVehicleKey(vehicleid, playerid); */
cell AMX_NATIVE_CALL n_GiveVehicleKey(AMX* amx, const cell* params)
{
    if (!VehicleSystem::getInstance().isManaged(params[1])) return 0;
    int giver = (params[0] / sizeof(cell) >= 3) ? (int)params[3] : -1;
    g_vehicleKeys[params[1]].insert(params[2]);
    EventDispatcher::dispatch3(params[1], giver, params[2], "EVF9_OnVehicleKeyGiven");
    return 1;
}

/* native RemoveVehicleKey(vehicleid, playerid); */
cell AMX_NATIVE_CALL n_RemoveVehicleKey(AMX* amx, const cell* params)
{
    if (!VehicleSystem::getInstance().isManaged(params[1])) return 0;
    int remover = (params[0] / sizeof(cell) >= 3) ? (int)params[3] : -1;
    auto it = g_vehicleKeys.find(params[1]);
    if (it != g_vehicleKeys.end()) it->second.erase(params[2]);
    EventDispatcher::dispatch3(params[1], remover, params[2], "EVF9_OnVehicleKeyRemoved");
    return 1;
}

/* native HasVehicleKey(vehicleid, playerid); */
cell AMX_NATIVE_CALL n_HasVehicleKey(AMX* amx, const cell* params)
{
    /* Owner always has key */
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (v && v->ownerid == (int)params[2]) return 1;
    auto it = g_vehicleKeys.find(params[1]);
    if (it == g_vehicleKeys.end()) return 0;
    return it->second.count(params[2]) ? 1 : 0;
}

/* native LockVehicle(vehicleid, playerid = -1); */
cell AMX_NATIVE_CALL n_LockVehicle(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->flags |= VEHICLE_FLAG_LOCKED;
    VehicleSystem::getInstance().markDirty(params[1]);
    int locker = (params[0] / sizeof(cell) >= 2) ? (int)params[2] : -1;
    EventDispatcher::dispatch2(params[1], locker, "EVF9_OnVehicleLock");
    return 1;
}

/* native UnlockVehicle(vehicleid, playerid = -1); */
cell AMX_NATIVE_CALL n_UnlockVehicle(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->flags &= ~VEHICLE_FLAG_LOCKED;
    VehicleSystem::getInstance().markDirty(params[1]);
    int unlocker = (params[0] / sizeof(cell) >= 2) ? (int)params[2] : -1;
    EventDispatcher::dispatch2(params[1], unlocker, "EVF9_OnVehicleUnlock");
    return 1;
}

/* native GetVehicleKeyCount(vehicleid); */
cell AMX_NATIVE_CALL n_GetVehicleKeyCount(AMX* amx, const cell* params)
{
    auto it = g_vehicleKeys.find(params[1]);
    if (it == g_vehicleKeys.end()) return 0;
    return (cell)it->second.size();
}

/* native ClearVehicleKeys(vehicleid); */
cell AMX_NATIVE_CALL n_ClearVehicleKeys(AMX* amx, const cell* params)
{
    g_vehicleKeys.erase(params[1]);
    return 1;
}
