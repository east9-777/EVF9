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
#include <cstring>

/* native SetVehicleOwner(vehicleid, playerid); */
cell AMX_NATIVE_CALL n_SetVehicleOwner(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int oldowner  = v->ownerid;
    int newowner  = (int)params[2];
    v->ownerid = newowner;
    VehicleSystem::getInstance().markDirty(params[1]);
    if (oldowner == -1)
        EventDispatcher::dispatch2(params[1], newowner, "EVF9_OnVehicleOwnerAssigned");
    else
        EventDispatcher::dispatch3(params[1], oldowner, newowner, "EVF9_OnVehicleOwnerChange");
    return 1;
}

/* native GetVehicleOwner(vehicleid); */
cell AMX_NATIVE_CALL n_GetVehicleOwner(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->ownerid : -1;
}

/* native GetVehicleOwnerName(vehicleid, name[], len);
 * Retrieves the owner player's name via the Pawn AMX GetPlayerName call-back.
 * If the owner is offline, the name cannot be resolved and the buffer is
 * cleared (returns 0). If you store names persistently, cache them yourself.
 */
cell AMX_NATIVE_CALL n_GetVehicleOwnerName(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v || v->ownerid < 0) return 0;

    /*
     * Call GetPlayerName(ownerid, buffer, len) from Pawn.
     * We use amx_FindNative + amx_Exec to invoke the server native.
     * This is safe to call from within a native handler in SA-MP/Open.MP.
     */
    int natIdx;
    if (amx_FindNative(amx, "GetPlayerName", &natIdx) != AMX_ERR_NONE)
        return 0;

    int len = (int)params[3];

    /* Allocate a temporary AMX string cell via amx_Allot */
    cell amxAddr;
    cell* physAddr;
    if (amx_Allot(amx, len, &amxAddr, &physAddr) != AMX_ERR_NONE)
        return 0;

    /* Push GetPlayerName args right to left: (playerid, name[], size) */
    amx_Push(amx, (cell)len);
    amx_Push(amx, amxAddr);
    amx_Push(amx, (cell)v->ownerid);

    cell ret = 0;
    amx_Exec(amx, &ret, natIdx);

    if (ret)
    {
        /* Copy result to caller's output param */
        char tmp[MAX_PLAYER_NAME + 1];
        amx_GetString(tmp, physAddr, 0, sizeof(tmp));
        evf9_SetStr(amx, params[2], tmp, len);
    }

    amx_Release(amx, amxAddr);
    return ret;
}

/* native RemoveVehicleOwner(vehicleid); */
cell AMX_NATIVE_CALL n_RemoveVehicleOwner(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int old = v->ownerid;
    v->ownerid = -1;
    VehicleSystem::getInstance().markDirty(params[1]);
    EventDispatcher::dispatch2(params[1], old, "EVF9_OnVehicleOwnerRemoved");
    return 1;
}

/* native TransferVehicleOwnership(vehicleid, newownerid); */
cell AMX_NATIVE_CALL n_TransferVehicleOwnership(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int old = v->ownerid;
    v->ownerid = (int)params[2];
    VehicleSystem::getInstance().markDirty(params[1]);
    EventDispatcher::dispatch3(params[1], old, v->ownerid, "EVF9_OnVehicleOwnerChange");
    return 1;
}

/* native IsVehicleOwner(vehicleid, playerid); */
cell AMX_NATIVE_CALL n_IsVehicleOwner(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && v->ownerid == (int)params[2]) ? 1 : 0;
}

/* native GetOwnedVehicleCount(playerid); */
cell AMX_NATIVE_CALL n_GetOwnedVehicleCount(AMX* amx, cell* params)
{
    int count = 0;
    int vid = VehicleSystem::getInstance().getFirst();
    while (vid != -1)
    {
        auto* v = VehicleSystem::getInstance().get(vid);
        if (v && v->ownerid == (int)params[1]) count++;
        vid = VehicleSystem::getInstance().getNext(vid);
    }
    return count;
}
