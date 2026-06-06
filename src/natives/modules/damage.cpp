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

/* native RepairVehicleDamage(vehicleid, playerid = -1); */
cell AMX_NATIVE_CALL n_RepairVehicleDamage(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->health = 1000.0f;
    VehicleSystem::getInstance().markDirty(params[1]);
    int repairer = (params[0] / sizeof(cell) >= 2) ? (int)params[2] : -1;
    EventDispatcher::dispatch2(params[1], repairer, "EVF9_OnVehicleRepair");
    return 1;
}

/* native DestroyVehiclePart(vehicleid, partid); */
cell AMX_NATIVE_CALL n_DestroyVehiclePart(AMX* amx, const cell* params)
{
    EventDispatcher::dispatch2(params[1], params[2], "EVF9_OnVehiclePartDamaged");
    return 1;
}

/* native RepairVehiclePart(vehicleid, partid, playerid = -1); */
cell AMX_NATIVE_CALL n_RepairVehiclePart(AMX* amx, const cell* params)
{
    int repairer = (params[0] / sizeof(cell) >= 3) ? (int)params[3] : -1;
    EventDispatcher::dispatch3(params[1], params[2], repairer, "EVF9_OnVehiclePartRepaired");
    return 1;
}

/*
 * native GetVehicleDamageData(vehicleid, &panels, &doors, &lights, &tires);
 * Calls GetVehicleDamageStatus via the AMX native table to get live server data.
 */
cell AMX_NATIVE_CALL n_GetVehicleDamageData(AMX* amx, const cell* params)
{
    if (!VehicleSystem::getInstance().isManaged(params[1])) return 0;

    int natIdx;
    if (amx_FindNative(amx, "GetVehicleDamageStatus", &natIdx) != AMX_ERR_NONE)
    {
        /* Stub: write zeroes */
        evf9_SetInt(amx, params[2], 0);
        evf9_SetInt(amx, params[3], 0);
        evf9_SetInt(amx, params[4], 0);
        evf9_SetInt(amx, params[5], 0);
        return 1;
    }

    /* Allocate 4 temporary cells for the output values */
    cell panelsAddr, doorsAddr, lightsAddr, tiresAddr;
    cell *panelsPhy, *doorsPhy, *lightsPhy, *tiresPhy;
    if (amx_Allot(amx, 1, &panelsAddr, &panelsPhy) != AMX_ERR_NONE) return 0;
    if (amx_Allot(amx, 1, &doorsAddr,  &doorsPhy)  != AMX_ERR_NONE) { amx_Release(amx, panelsAddr); return 0; }
    if (amx_Allot(amx, 1, &lightsAddr, &lightsPhy) != AMX_ERR_NONE) { amx_Release(amx, doorsAddr);  amx_Release(amx, panelsAddr); return 0; }
    if (amx_Allot(amx, 1, &tiresAddr,  &tiresPhy)  != AMX_ERR_NONE) { amx_Release(amx, lightsAddr); amx_Release(amx, doorsAddr); amx_Release(amx, panelsAddr); return 0; }

    /* Push args right to left: (vehicleid, &panels, &doors, &lights, &tires) */
    amx_Push(amx, tiresAddr);
    amx_Push(amx, lightsAddr);
    amx_Push(amx, doorsAddr);
    amx_Push(amx, panelsAddr);
    amx_Push(amx, (cell)params[1]);

    cell ret = 0;
    amx_Exec(amx, &ret, natIdx);

    evf9_SetInt(amx, params[2], (int)*panelsPhy);
    evf9_SetInt(amx, params[3], (int)*doorsPhy);
    evf9_SetInt(amx, params[4], (int)*lightsPhy);
    evf9_SetInt(amx, params[5], (int)*tiresPhy);

    amx_Release(amx, tiresAddr);
    amx_Release(amx, lightsAddr);
    amx_Release(amx, doorsAddr);
    amx_Release(amx, panelsAddr);
    return ret;
}

/* native ResetVehicleDamage(vehicleid); */
cell AMX_NATIVE_CALL n_ResetVehicleDamage(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->health = 1000.0f;
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}
