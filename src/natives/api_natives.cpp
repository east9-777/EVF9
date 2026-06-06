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
#include "../amx_helpers.h"
#include <amx/amx.h>

#define INVALID_VEHICLE_ID (-1)

/* native EVF9_GetFirstVehicle(); */
cell AMX_NATIVE_CALL n_EVF9_GetFirstVehicle(AMX* amx, const cell* params)
{
    int vid = VehicleSystem::getInstance().getFirst();
    return vid != -1 ? vid : INVALID_VEHICLE_ID;
}

/* native EVF9_GetNextVehicle(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetNextVehicle(AMX* amx, const cell* params)
{
    int vid = VehicleSystem::getInstance().getNext(params[1]);
    return vid != -1 ? vid : INVALID_VEHICLE_ID;
}

/* native EVF9_BeginSave(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_BeginSave(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().beginSave(params[1]) ? 1 : 0;
}

/* native EVF9_EndSave(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_EndSave(AMX* amx, const cell* params)
{
    return VehicleSystem::getInstance().endSave(params[1]) ? 1 : 0;
}

/* native EVF9_IsVehicleSaving(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_IsVehicleSaving(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return (v && v->saveState == SaveState::SAVING) ? 1 : 0;
}

/* native EVF9_SAVE_STATE:EVF9_GetVehicleSaveState(vehicleid); */
cell AMX_NATIVE_CALL n_EVF9_GetVehicleSaveState(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    return v ? (cell)v->saveState : 0;
}

/* native SetVehicleMeta(vehicleid, EVF9_METADATA:key, value); */
cell AMX_NATIVE_CALL n_SetVehicleMeta(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int key = (int)params[2];
    if (key < 0 || key >= 7) return 0;
    v->meta[key] = (int)params[3];
    return 1;
}

/* native GetVehicleMeta(vehicleid, EVF9_METADATA:key); */
cell AMX_NATIVE_CALL n_GetVehicleMeta(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int key = (int)params[2];
    if (key < 0 || key >= 7) return 0;
    return (cell)v->meta[key];
}

/* native ResetVehicleMeta(vehicleid, EVF9_METADATA:key); */
cell AMX_NATIVE_CALL n_ResetVehicleMeta(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int key = (int)params[2];
    if (key < 0 || key >= 7) return 0;
    v->meta[key] = 0;
    return 1;
}

/* native HasVehicleMeta(vehicleid, EVF9_METADATA:key); */
cell AMX_NATIVE_CALL n_HasVehicleMeta(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int key = (int)params[2];
    if (key < 0 || key >= 7) return 0;
    return v->meta[key] != 0 ? 1 : 0;
}

/* native RegisterVehicleProperty(const name[], EVF9_PROPERTY_TYPE:type); */
cell AMX_NATIVE_CALL n_RegisterVehicleProperty(AMX* amx, const cell* params)
{
    char name[128];
    evf9_GetStr(amx, params[1], name, sizeof(name));
    PropType type = (PropType)(int)params[2];
    return (cell)VehicleSystem::getInstance().registerProperty(name, type);
}

/* native SetVehiclePropertyInt(vehicleid, propertyid, value); */
cell AMX_NATIVE_CALL n_SetVehiclePropertyInt(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int propid = (int)params[2];
    if (propid < 0 || propid >= (int)v->props.size()) return 0;
    v->props[propid].val.ival = (int)params[3];
    v->props[propid].set      = true;
    return 1;
}

/* native SetVehiclePropertyFloat(vehicleid, propertyid, Float:value); */
cell AMX_NATIVE_CALL n_SetVehiclePropertyFloat(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int propid = (int)params[2];
    if (propid < 0 || propid >= (int)v->props.size()) return 0;
    v->props[propid].val.fval = evf9_GetFloat(params[3]);
    v->props[propid].set      = true;
    return 1;
}

/* native SetVehiclePropertyString(vehicleid, propertyid, const value[]); */
cell AMX_NATIVE_CALL n_SetVehiclePropertyString(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int propid = (int)params[2];
    if (propid < 0 || propid >= (int)v->props.size()) return 0;
    char buf[256];
    evf9_GetStr(amx, params[3], buf, sizeof(buf));
    v->props[propid].sval = buf;
    v->props[propid].set  = true;
    return 1;
}

/* native GetVehiclePropertyInt(vehicleid, propertyid); */
cell AMX_NATIVE_CALL n_GetVehiclePropertyInt(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int propid = (int)params[2];
    if (propid < 0 || propid >= (int)v->props.size()) return 0;
    return (cell)v->props[propid].val.ival;
}

/* native GetVehiclePropertyFloat(vehicleid, propertyid, &Float:value); */
cell AMX_NATIVE_CALL n_GetVehiclePropertyFloat(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int propid = (int)params[2];
    if (propid < 0 || propid >= (int)v->props.size()) return 0;
    evf9_SetFloat(amx, params[3], v->props[propid].val.fval);
    return 1;
}

/* native GetVehiclePropertyString(vehicleid, propertyid, value[], len); */
cell AMX_NATIVE_CALL n_GetVehiclePropertyString(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int propid = (int)params[2];
    if (propid < 0 || propid >= (int)v->props.size()) return 0;
    evf9_SetStr(amx, params[3], v->props[propid].sval.c_str(), (int)params[4]);
    return 1;
}

/* native HasVehicleProperty(vehicleid, propertyid); */
cell AMX_NATIVE_CALL n_HasVehicleProperty(AMX* amx, const cell* params)
{
    auto* v = VehicleSystem::getInstance().get(params[1]);
    if (!v) return 0;
    int propid = (int)params[2];
    if (propid < 0 || propid >= (int)v->props.size()) return 0;
    return v->props[propid].set ? 1 : 0;
}
