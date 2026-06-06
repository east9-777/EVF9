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
#include <chrono>

static int64_t nowSec()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

/* native InsureVehicle(vehicleid, duration); */
cell AMX_NATIVE_CALL n_InsureVehicle(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->insuranceExpiry = nowSec() + (int64_t)params[2];
    v->flags |= VEHICLE_FLAG_INSURED;
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}

/* native CancelInsurance(vehicleid); */
cell AMX_NATIVE_CALL n_CancelInsurance(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->insuranceExpiry = 0;
    v->flags &= ~VEHICLE_FLAG_INSURED;
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}

/* native IsVehicleInsured(vehicleid); */
cell AMX_NATIVE_CALL n_IsVehicleInsured(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    return (v->insuranceExpiry > nowSec()) ? 1 : 0;
}

/* native GetInsuranceExpiration(vehicleid); */
cell AMX_NATIVE_CALL n_GetInsuranceExpiration(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->insuranceExpiry : 0;
}

/* native RenewInsurance(vehicleid, duration); */
cell AMX_NATIVE_CALL n_RenewInsurance(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int64_t base = (v->insuranceExpiry > nowSec()) ? v->insuranceExpiry : nowSec();
    v->insuranceExpiry = base + (int64_t)params[2];
    v->flags |= VEHICLE_FLAG_INSURED;
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}
