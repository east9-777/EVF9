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
#include <string>

#define INVALID_FLEET_ID (-1)

/* native CreateFleet(const name[], ownerid); */
cell AMX_NATIVE_CALL n_CreateFleet(AMX* amx, cell* params)
{
    char name[128];
    evf9_GetStr(amx, params[1], name, sizeof(name));
    int fleetid = VehicleSystem::getInstance().createFleet(name, (int)params[2]);
    return (cell)fleetid;
}

/* native DestroyFleet(fleetid); */
cell AMX_NATIVE_CALL n_DestroyFleet(AMX* amx, cell* params)
{
    return VehicleSystem::getInstance().destroyFleet((int)params[1]) ? 1 : 0;
}

/* native AssignVehicleToFleet(vehicleid, fleetid); */
cell AMX_NATIVE_CALL n_AssignVehicleToFleet(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->fleetid = (int)params[2];
    VehicleSystem::getInstance().markDirty(params[1]);
    EventDispatcher::dispatch2(params[1], params[2], "EVF9_OnVehicleFleetAssigned");
    return 1;
}

/* native RemoveVehicleFromFleet(vehicleid); */
cell AMX_NATIVE_CALL n_RemoveVehicleFromFleet(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int old = v->fleetid;
    v->fleetid = -1;
    VehicleSystem::getInstance().markDirty(params[1]);
    EventDispatcher::dispatch2(params[1], old, "EVF9_OnVehicleFleetRemoved");
    return 1;
}

/* native GetFleetVehicleCount(fleetid); */
cell AMX_NATIVE_CALL n_GetFleetVehicleCount(AMX* amx, cell* params)
{
    int fleetid = (int)params[1], count = 0;
    int vid = VehicleSystem::getInstance().getFirst();
    while (vid != -1)
    {
        auto* v = VehicleSystem::getInstance().get(vid);
        if (v && v->fleetid == fleetid) count++;
        vid = VehicleSystem::getInstance().getNext(vid);
    }
    return count;
}

/* native GetVehicleFleet(vehicleid); */
cell AMX_NATIVE_CALL n_GetVehicleFleet(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->fleetid : INVALID_FLEET_ID;
}

/* native GetFleetOwner(fleetid); */
cell AMX_NATIVE_CALL n_GetFleetOwner(AMX* amx, cell* params)
{
    const FleetEntry* f = VehicleSystem::getInstance().getFleet((int)params[1]);
    return f ? (cell)f->ownerid : -1;
}

/* native IsVehicleInFleet(vehicleid); */
cell AMX_NATIVE_CALL n_IsVehicleInFleet(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && v->fleetid >= 0) ? 1 : 0;
}
