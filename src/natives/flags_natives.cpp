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
#include <amx/amx.h>

/* native EVF9_SetVehicleFlag(vehicleid, E_VEHICLE_FLAGS:flag); */
cell AMX_NATIVE_CALL n_EVF9_SetVehicleFlag(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->flags |= (uint32_t)params[2];
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}

/* native EVF9_ClearVehicleFlag(vehicleid, E_VEHICLE_FLAGS:flag); */
cell AMX_NATIVE_CALL n_EVF9_ClearVehicleFlag(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->flags &= ~(uint32_t)params[2];
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}

/* native EVF9_HasVehicleFlag(vehicleid, E_VEHICLE_FLAGS:flag); */
cell AMX_NATIVE_CALL n_EVF9_HasVehicleFlag(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    return (v->flags & (uint32_t)params[2]) ? 1 : 0;
}
