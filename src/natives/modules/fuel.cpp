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
#include <algorithm>

#define FUEL_LOW_THRESHOLD 10.0f

/* native SetVehicleFuel(vehicleid, Float:fuel); */
cell AMX_NATIVE_CALL n_SetVehicleFuel(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    float oldFuel = v->fuel;
    float newFuel = std::max(0.0f, std::min(100.0f, evf9_GetFloat(params[2])));
    v->fuel = newFuel;
    VehicleSystem::getInstance().markDirty(params[1]);
    EventDispatcher::dispatch1f(params[1], newFuel, "EVF9_OnVehicleFuelChanged");
    if (newFuel <= 0.0f && oldFuel > 0.0f)
        EventDispatcher::dispatch1(params[1], "EVF9_OnVehicleFuelEmpty");
    else if (newFuel <= FUEL_LOW_THRESHOLD && oldFuel > FUEL_LOW_THRESHOLD)
        EventDispatcher::dispatch1f(params[1], newFuel, "EVF9_OnVehicleFuelLow");
    return 1;
}

/* native GetVehicleFuel(vehicleid, &Float:fuel); */
cell AMX_NATIVE_CALL n_GetVehicleFuel(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->fuel);
    return 1;
}

/* native AddVehicleFuel(vehicleid, Float:amount); */
cell AMX_NATIVE_CALL n_AddVehicleFuel(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->fuel = std::min(100.0f, v->fuel + evf9_GetFloat(params[2]));
    VehicleSystem::getInstance().markDirty(params[1]);
    return 1;
}

/* native RemoveVehicleFuel(vehicleid, Float:amount); */
cell AMX_NATIVE_CALL n_RemoveVehicleFuel(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    float oldFuel = v->fuel;
    v->fuel = std::max(0.0f, v->fuel - evf9_GetFloat(params[2]));
    VehicleSystem::getInstance().markDirty(params[1]);
    if (v->fuel <= 0.0f && oldFuel > 0.0f)
        EventDispatcher::dispatch1(params[1], "EVF9_OnVehicleFuelEmpty");
    return 1;
}

/* native IsVehicleOutOfFuel(vehicleid); */
cell AMX_NATIVE_CALL n_IsVehicleOutOfFuel(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && v->fuel <= 0.0f) ? 1 : 0;
}

/* native GetFuelConsumptionRate(vehicleid, &Float:rate); */
cell AMX_NATIVE_CALL n_GetFuelConsumptionRate(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    evf9_SetFloat(amx, params[2], v->fuelRate);
    return 1;
}

/* native SetFuelConsumptionRate(vehicleid, Float:rate); */
cell AMX_NATIVE_CALL n_SetFuelConsumptionRate(AMX* amx, cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    v->fuelRate = evf9_GetFloat(params[2]);
    return 1;
}
