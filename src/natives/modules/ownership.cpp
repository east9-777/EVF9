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
 * ownership.cpp — Vehicle ownership module natives.
 *
 * MIGRATION NOTE — MAX_PLAYER_NAME
 * =================================
 * The previous version referenced MAX_PLAYER_NAME which was defined by the
 * SA-MP SDK headers (plugincommon.h / a_players.inc pulled in transitively).
 * The Open.MP SDK does NOT define this constant in the C++ layer.
 *
 * We define it locally as 24, which is the universally agreed maximum player
 * name length for both SA-MP and Open.MP (the server enforces this limit).
 * If a future Open.MP version exposes a larger limit via its C++ headers,
 * replace this constant accordingly.
 *
 * MIGRATION NOTE — amx_FindNative / amx_Allot / amx_Exec
 * ========================================================
 * GetVehicleOwnerName calls the server-provided Pawn native GetPlayerName()
 * from within a C++ native handler.  The technique (amx_FindNative →
 * amx_Allot → amx_Push → amx_Exec → amx_Release) uses only the standard AMX
 * bytecode API, which is unchanged and fully available in the Open.MP SDK's
 * amx/amx.h.  No changes were required for this pattern.
 */

#include "../../vehicle_system.h"
#include "../../event_dispatcher.h"
#include "../../amx_helpers.h"
#include <cstring>

/* Open.MP / SA-MP maximum player name length (enforced by the server). */
#ifndef MAX_PLAYER_NAME
#define MAX_PLAYER_NAME 24
#endif

/* native SetVehicleOwner(vehicleid, playerid); */
cell AMX_NATIVE_CALL n_SetVehicleOwner(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int oldowner = v->ownerid;
    int newowner = (int)params[2];
    v->ownerid = newowner;
    VehicleSystem::getInstance().markDirty(params[1]);
    if (oldowner == -1)
        EventDispatcher::dispatch2(params[1], newowner, "EVF9_OnVehicleOwnerAssigned");
    else
        EventDispatcher::dispatch3(params[1], oldowner, newowner, "EVF9_OnVehicleOwnerChange");
    return 1;
}

/* native GetVehicleOwner(vehicleid); */
cell AMX_NATIVE_CALL n_GetVehicleOwner(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->ownerid : -1;
}

/*
 * native GetVehicleOwnerName(vehicleid, name[], len);
 *
 * Retrieves the online owner player's name by invoking the Pawn native
 * GetPlayerName() from within this C++ native.  If the owner is offline
 * the name cannot be resolved and the function returns 0.
 *
 * Pattern: amx_FindNative → amx_Allot → amx_Push (right-to-left) →
 *           amx_Exec → amx_GetString → amx_Release.
 * This is pure AMX API — fully supported in Open.MP.
 */
cell AMX_NATIVE_CALL n_GetVehicleOwnerName(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v || v->ownerid < 0) return 0;

    int natIdx;
    if (amx_FindNative(amx, "GetPlayerName", &natIdx) != AMX_ERR_NONE)
        return 0;

    int len = (int)params[3];

    /* Allocate a temporary output buffer in Pawn heap space. */
    cell amxAddr;
    cell* physAddr;
    if (amx_Allot(amx, len, &amxAddr, &physAddr) != AMX_ERR_NONE)
        return 0;

    /* Push GetPlayerName args right-to-left: (playerid, name[], size) */
    amx_Push(amx, (cell)len);
    amx_Push(amx, amxAddr);
    amx_Push(amx, (cell)v->ownerid);

    cell ret = 0;
    amx_Exec(amx, &ret, natIdx);

    if (ret)
    {
        char tmp[MAX_PLAYER_NAME + 1];
        amx_GetString(tmp, physAddr, 0, sizeof(tmp));
        evf9_SetStr(amx, params[2], tmp, len);
    }

    amx_Release(amx, amxAddr);
    return ret;
}

/* native RemoveVehicleOwner(vehicleid); */
cell AMX_NATIVE_CALL n_RemoveVehicleOwner(AMX* amx, const cell* params)
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
cell AMX_NATIVE_CALL n_TransferVehicleOwnership(AMX* amx, const cell* params)
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
cell AMX_NATIVE_CALL n_IsVehicleOwner(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && v->ownerid == (int)params[2]) ? 1 : 0;
}

/* native GetOwnedVehicleCount(playerid); */
cell AMX_NATIVE_CALL n_GetOwnedVehicleCount(AMX* amx, const cell* params)
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
