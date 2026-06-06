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
#include "../module_registry.h"
#include "../amx_helpers.h"
#include <amx/amx.h>
#include <cstring>

#define EVF9_VERSION 100
#define EVF9_BUILD   1

/* native EVF9_GetVersion(); */
cell AMX_NATIVE_CALL n_EVF9_GetVersion(AMX* amx, const cell* params)
{
    return EVF9_VERSION;
}

/* native EVF9_GetBuild(); */
cell AMX_NATIVE_CALL n_EVF9_GetBuild(AMX* amx, const cell* params)
{
    return EVF9_BUILD;
}

/* native EVF9_RegisterAddon(const name[], const version[]); */
cell AMX_NATIVE_CALL n_EVF9_RegisterAddon(AMX* amx, const cell* params)
{
    char name[64] = {}, version[32] = {};
    evf9_GetStr(amx, params[1], name, sizeof(name));
    evf9_GetStr(amx, params[2], version, sizeof(version));
    return (cell)ModuleRegistry::getInstance().registerModule(name, version);
}

/* native EVF9_UnregisterAddon(addonid); */
cell AMX_NATIVE_CALL n_EVF9_UnregisterAddon(AMX* amx, const cell* params)
{
    return ModuleRegistry::getInstance().unregisterModule((int)params[1]) ? 1 : 0;
}

/* native EVF9_GetAddonInfo(addonid, name[], namelen, version[], verlen); */
cell AMX_NATIVE_CALL n_EVF9_GetAddonInfo(AMX* amx, const cell* params)
{
    std::string name, version;
    if (!ModuleRegistry::getInstance().getInfo((int)params[1], name, version))
        return 0;
    evf9_SetStr(amx, params[2], name.c_str(),    (int)params[3]);
    evf9_SetStr(amx, params[4], version.c_str(), (int)params[5]);
    return 1;
}

/* native EVF9_GetMemoryUsage(); */
cell AMX_NATIVE_CALL n_EVF9_GetMemoryUsage(AMX* amx, const cell* params)
{
    /* Approximate: main vehicle array + linked list */
    return (cell)(sizeof(VehicleSystem));
}

/* native EVF9_GetVehicleMemoryUsage(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleMemoryUsage(AMX* amx, const cell* params)
{
    return (cell)sizeof(VehicleEntity);
}

/* native EVF9_GetModuleMemoryUsage(module[]); */
cell AMX_NATIVE_CALL n_EVF9_GetModuleMemoryUsage(AMX* amx, const cell* params)
{
    return (cell)sizeof(ModuleRegistry);
}

/* native EVF9_GetTotalVehicles(); */
cell AMX_NATIVE_CALL n_EVF9_GetTotalVehicles(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().count();
}

/* native EVF9_GetManagedVehicles(); */
cell AMX_NATIVE_CALL n_EVF9_GetManagedVehicles(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().count();
}

/* native EVF9_GetSpawnedVehicles(); */
cell AMX_NATIVE_CALL n_EVF9_GetSpawnedVehicles(AMX* amx, const cell* params)
{
    int spawned = 0;
    int vid = VehicleSystem::getInstance().getFirst();
    while (vid != -1)
    {
        auto* v = VehicleSystem::getInstance().get(vid);
        if (v && (v->flags & VEHICLE_FLAG_SPAWNED)) spawned++;
        vid = VehicleSystem::getInstance().getNext(vid);
    }
    return spawned;
}

/* native EVF9_GetDestroyedVehicles(); — not tracked across resets, returns 0 */
cell AMX_NATIVE_CALL n_EVF9_GetDestroyedVehicles(AMX* amx, const cell* params)
{
    return 0;
}

/* native EVF9_GetAbandonedVehicles(); */
cell AMX_NATIVE_CALL n_EVF9_GetAbandonedVehicles(AMX* amx, const cell* params)
{
    int count = 0;
    int vid = VehicleSystem::getInstance().getFirst();
    while (vid != -1)
    {
        auto* v = VehicleSystem::getInstance().get(vid);
        if (v && (v->flags & VEHICLE_FLAG_ABANDONED)) count++;
        vid = VehicleSystem::getInstance().getNext(vid);
    }
    return count;
}

/* native EVF9_StartProfiler(); */
cell AMX_NATIVE_CALL n_EVF9_StartProfiler(AMX* amx, const cell* params) { return 1; }

/* native EVF9_StopProfiler(); */
cell AMX_NATIVE_CALL n_EVF9_StopProfiler(AMX* amx, const cell* params)  { return 1; }

/* native EVF9_GetProfilerData(data[], len); */
cell AMX_NATIVE_CALL n_EVF9_GetProfilerData(AMX* amx, const cell* params)
{
    evf9_SetStr(amx, params[1], "[EVF9] Profiler not active.", (int)params[2]);
    return 1;
}
