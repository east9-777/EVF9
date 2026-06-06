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
 * main.cpp — Open.MP Component entry point (modern SDK, 64-bit only).
 *
 * MIGRATION SUMMARY
 * =================
 * The previous implementation used the SA-MP plugin ABI:
 *   - plugincommon.h / amx/amx.h (legacy headers)
 *   - PLUGIN_EXPORT / PLUGIN_CALL macros
 *   - Supports() / Load() / Unload() free functions
 *   - AmxLoad() / AmxUnload() free functions
 *   - pAMXFunctions / ppPluginData global pointers
 *   - Single AMX* stored in EventDispatcher (no filterscript support)
 *
 * This file now implements the Open.MP component interface exclusively:
 *   - #include <sdk.hpp>  (from the Open.MP SDK)
 *   - EVF9Component inherits IComponent + PawnEventHandler
 *   - COMPONENT_ENTRY_POINT() replaces PLUGIN_EXPORT + Supports/Load/Unload
 *   - onAmxLoad / onAmxUnload replace AmxLoad / AmxUnload
 *   - Natives registered via script.addNative() per-script instead of a
 *     single amx_Register() call — this correctly handles filterscripts
 *   - EventDispatcher now maintains a set of ALL loaded AMX instances so
 *     events reach every script (gamemode + filterscripts)
 *   - No pAMXFunctions, no ppPluginData, no PLUGIN_DATA_AMX_EXPORTS
 *
 * NATIVE REGISTRATION
 * ===================
 * Each native is forward-declared here (extern linkage, defined in its own
 * .cpp file).  Inside onAmxLoad(), script.addNative() is called once per
 * native — Open.MP registers it into the given script's native table.
 *
 * FLOAT CONVENTION (unchanged from SA-MP)
 * ========================================
 * Float by value:  params[N] holds the bit-pattern; use amx_ctof(params[N]).
 * Float by ref:    evf9_SetFloat(amx, params[N], val) — see amx_helpers.h.
 *
 * EVENT DISPATCHER
 * ================
 * EventDispatcher::addAMX(amx) is called for every script that loads so the
 * dispatcher can fire amx_FindPublic + amx_Exec on ALL of them.
 */

#include <sdk.hpp>
#include <Server/Components/Pawn/pawn.hpp>

#include "event_dispatcher.h"
#include "vehicle_system.h"

/* =========================================================================
 * Forward declarations — one per native function.
 * The native files define each function without `static` (external linkage).
 * ========================================================================= */

/* pool_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_RegisterVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_UnregisterVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_RespawnVehicleInternal(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsManagedVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_CountManagedVehicles(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_UpdateVehiclePosition(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleLastDriver(AMX*, const cell*);

/* entity_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleModel(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleSpawnPos(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleWorld(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleInterior(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleFlags(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleCreatedAt(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleLastDriver(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleLastOccupant(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleDistanceTravelled(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_IsVehicleSpawned(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_IsVehicleManaged(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleHealth(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleHealthInternal(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleTag(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleTag(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleUID(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleByUID(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleLifetime(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_ResetVehicleDistance(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleLifetimeDistance(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleTripDistance(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleLocked(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleEngineOn(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleTracked(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleAbandoned(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_CountVehiclesByModel(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_CountVehiclesByOwner(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetNearestManagedVehicle(AMX*, const cell*);

/* flags_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleFlag(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_ClearVehicleFlag(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_HasVehicleFlag(AMX*, const cell*);

/* api_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_GetFirstVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetNextVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_BeginSave(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_EndSave(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_IsVehicleSaving(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleSaveState(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_SetVehicleMeta(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleMeta(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_ResetVehicleMeta(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_HasVehicleMeta(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_RegisterVehicleProperty(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_SetVehiclePropertyInt(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_SetVehiclePropertyFloat(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_SetVehiclePropertyString(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehiclePropertyInt(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehiclePropertyFloat(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehiclePropertyString(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_HasVehicleProperty(AMX*, const cell*);

/* core_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_GetVersion(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetBuild(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_RegisterAddon(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_UnregisterAddon(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetAddonInfo(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetMemoryUsage(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleMemoryUsage(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetModuleMemoryUsage(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetTotalVehicles(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetManagedVehicles(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetSpawnedVehicles(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetDestroyedVehicles(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetAbandonedVehicles(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_StartProfiler(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_StopProfiler(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetProfilerData(AMX*, const cell*);

/* modules/fuel.cpp */
extern cell AMX_NATIVE_CALL n_SetVehicleFuel(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleFuel(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_AddVehicleFuel(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleFuel(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleOutOfFuel(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetFuelConsumptionRate(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_SetFuelConsumptionRate(AMX*, const cell*);

/* modules/ownership.cpp */
extern cell AMX_NATIVE_CALL n_SetVehicleOwner(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleOwner(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleOwnerName(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleOwner(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_TransferVehicleOwnership(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleOwner(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetOwnedVehicleCount(AMX*, const cell*);

/* modules/damage.cpp */
extern cell AMX_NATIVE_CALL n_RepairVehicleDamage(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_DestroyVehiclePart(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_RepairVehiclePart(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleDamageData(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_ResetVehicleDamage(AMX*, const cell*);

/* modules/keys.cpp */
extern cell AMX_NATIVE_CALL n_GiveVehicleKey(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleKey(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_HasVehicleKey(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_LockVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_UnlockVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleKeyCount(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_ClearVehicleKeys(AMX*, const cell*);

/* modules/fleet.cpp */
extern cell AMX_NATIVE_CALL n_CreateFleet(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_DestroyFleet(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_AssignVehicleToFleet(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleFromFleet(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetFleetVehicleCount(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleFleet(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetFleetOwner(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleInFleet(AMX*, const cell*);

/* modules/tracking.cpp */
extern cell AMX_NATIVE_CALL n_TrackVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_UntrackVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleLastPosition(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleTravelDistance(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleDistanceFromPoint(AMX*, const cell*);

/* modules/insurance.cpp */
extern cell AMX_NATIVE_CALL n_InsureVehicle(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_CancelInsurance(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleInsured(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_GetInsuranceExpiration(AMX*, const cell*);
extern cell AMX_NATIVE_CALL n_RenewInsurance(AMX*, const cell*);

/* =========================================================================
 * EVF9Component — the Open.MP component class
 *
 * Inherits:
 *   IComponent      — lifecycle hooks (onLoad, onInit, onReady, onFree, free, reset)
 *   PawnEventHandler — Pawn-specific hooks (onAmxLoad, onAmxUnload)
 * ========================================================================= */
class EVF9Component final : public IComponent, public PawnEventHandler
{
public:
    /*
     * PROVIDE_UID assigns the component a globally-unique 64-bit identifier.
     * Open.MP uses UIDs to locate components at runtime via queryComponent<T>().
     * This value MUST be unique across all loaded components.
     * "EVF9COMP" encoded as ASCII bytes: 0x45564639434F4D50
     */
    PROVIDE_UID(0x45564639434F4D50ULL);

    /* -----------------------------------------------------------------------
     * IComponent metadata
     * --------------------------------------------------------------------- */
    StringView componentName() const override { return "EVF9"; }

    SemanticVersion componentVersion() const override
    {
        return { 1, 0, 0, 0 };
    }

    /* -----------------------------------------------------------------------
     * onLoad — called once when the server loads the component.
     * ICore* gives access to logging, config, timers, etc.
     * No other components are available yet; do NOT call queryComponent here.
     * --------------------------------------------------------------------- */
    void onLoad(ICore* c) override
    {
        core_ = c;
        core_->printLn(" ");
        core_->printLn(" *** EVF9 - Extended Vehicle Framework 9 ***");
        core_->printLn(" *** Created by EAST9 — github.com/east9-777 ***");
        core_->printLn(" *** Version 1.0.0 loaded (Open.MP modern). ***");
        core_->printLn(" ");
    }

    /* -----------------------------------------------------------------------
     * onInit — called after ALL components have been loaded.
     * Safe to call queryComponent here to obtain peer component interfaces.
     * --------------------------------------------------------------------- */
    void onInit(IComponentList* components) override
    {
        components_ = components;

        /* Locate the Pawn component (Open.MP's Pawn scripting runtime). */
        pawn_ = components_->queryComponent<IPawnComponent>();
        if (!pawn_)
        {
            core_->printLn("[EVF9] WARNING: Pawn component not found. "
                           "Natives and events will be unavailable.");
            return;
        }

        /* Register ourselves as a Pawn event handler so we receive
         * onAmxLoad / onAmxUnload for every script that the server loads. */
        pawn_->getEventDispatcher().addEventHandler(this);
    }

    /* -----------------------------------------------------------------------
     * onReady — called once all components have finished initialising.
     * Good place for actions that depend on the full server state.
     * --------------------------------------------------------------------- */
    void onReady() override { /* nothing required at this stage */ }

    /* -----------------------------------------------------------------------
     * onAmxLoad — called by the Pawn runtime whenever a new Pawn script is
     * loaded (gamemode OR filterscript).
     *
     * Replaces SA-MP's AmxLoad(AMX* amx).
     *
     * Key differences from SA-MP:
     *   1. This is called once per script — there may be several scripts.
     *   2. Natives are registered per-script via script.addNative(), not via
     *      a single amx_Register() on a global native table.
     *   3. The raw AMX* is obtained via script.getAMX() and given to the
     *      EventDispatcher so events reach every loaded script.
     * --------------------------------------------------------------------- */
    void onAmxLoad(IPawnScript& script) override
    {
        /* Register the AMX instance for event dispatching. */
        EventDispatcher::addScript(&script);

        /* ---- Internal pool management (called by Pawn stocks) ---- */
        script.Register("EVF9_RegisterVehicle",        n_EVF9_RegisterVehicle);
        script.Register("EVF9_UnregisterVehicle",      n_EVF9_UnregisterVehicle);
        script.Register("EVF9_RespawnVehicleInternal", n_EVF9_RespawnVehicleInternal);
        script.Register("EVF9_UpdateVehiclePosition",  n_EVF9_UpdateVehiclePosition);
        script.Register("EVF9_SetVehicleLastDriver",   n_EVF9_SetVehicleLastDriver);

        /* ---- Pool queries ---- */
        script.Register("IsManagedVehicle",            n_IsManagedVehicle);
        script.Register("CountManagedVehicles",        n_CountManagedVehicles);

        /* ---- Entity getters ---- */
        script.Register("EVF9_GetVehicleModel",          n_EVF9_GetVehicleModel);
        script.Register("EVF9_GetVehicleSpawnPos",       n_EVF9_GetVehicleSpawnPos);
        script.Register("EVF9_GetVehicleWorld",          n_EVF9_GetVehicleWorld);
        script.Register("EVF9_GetVehicleInterior",       n_EVF9_GetVehicleInterior);
        script.Register("EVF9_GetVehicleFlags",          n_EVF9_GetVehicleFlags);
        script.Register("EVF9_GetVehicleCreatedAt",      n_EVF9_GetVehicleCreatedAt);
        script.Register("EVF9_GetVehicleLastDriver",     n_EVF9_GetVehicleLastDriver);
        script.Register("EVF9_GetVehicleLastOccupant",   n_EVF9_GetVehicleLastOccupant);
        script.Register("EVF9_GetVehicleDistanceTravelled", n_EVF9_GetVehicleDistanceTravelled);
        script.Register("EVF9_IsVehicleSpawned",         n_EVF9_IsVehicleSpawned);
        script.Register("EVF9_IsVehicleManaged",         n_EVF9_IsVehicleManaged);
        script.Register("EVF9_GetVehicleHealth",         n_EVF9_GetVehicleHealth);
        script.Register("EVF9_SetVehicleHealthInternal", n_EVF9_SetVehicleHealthInternal);
        script.Register("EVF9_SetVehicleTag",            n_EVF9_SetVehicleTag);
        script.Register("EVF9_GetVehicleTag",            n_EVF9_GetVehicleTag);
        script.Register("EVF9_GetVehicleUID",            n_EVF9_GetVehicleUID);
        script.Register("EVF9_GetVehicleByUID",          n_EVF9_GetVehicleByUID);
        script.Register("EVF9_GetVehicleLifetime",       n_EVF9_GetVehicleLifetime);

        /* ---- Distance ---- */
        script.Register("ResetVehicleDistance",          n_ResetVehicleDistance);
        script.Register("GetVehicleLifetimeDistance",    n_GetVehicleLifetimeDistance);
        script.Register("GetVehicleTripDistance",        n_GetVehicleTripDistance);
        script.Register("CountVehiclesByModel",          n_CountVehiclesByModel);
        script.Register("CountVehiclesByOwner",          n_CountVehiclesByOwner);
        script.Register("GetNearestManagedVehicle",      n_GetNearestManagedVehicle);

        /* ---- State queries ---- */
        script.Register("IsVehicleLocked",               n_IsVehicleLocked);
        script.Register("IsVehicleEngineOn",             n_IsVehicleEngineOn);
        script.Register("IsVehicleTracked",              n_IsVehicleTracked);
        script.Register("IsVehicleAbandoned",            n_IsVehicleAbandoned);

        /* ---- Bitwise flags ---- */
        script.Register("EVF9_SetVehicleFlag",           n_EVF9_SetVehicleFlag);
        script.Register("EVF9_ClearVehicleFlag",         n_EVF9_ClearVehicleFlag);
        script.Register("EVF9_HasVehicleFlag",           n_EVF9_HasVehicleFlag);

        /* ---- Linked-list iterator ---- */
        script.Register("EVF9_GetFirstVehicle",          n_EVF9_GetFirstVehicle);
        script.Register("EVF9_GetNextVehicle",           n_EVF9_GetNextVehicle);

        /* ---- Save state machine ---- */
        script.Register("EVF9_BeginSave",                n_EVF9_BeginSave);
        script.Register("EVF9_EndSave",                  n_EVF9_EndSave);
        script.Register("EVF9_IsVehicleSaving",          n_EVF9_IsVehicleSaving);
        script.Register("EVF9_GetVehicleSaveState",      n_EVF9_GetVehicleSaveState);

        /* ---- Metadata ---- */
        script.Register("SetVehicleMeta",                n_SetVehicleMeta);
        script.Register("GetVehicleMeta",                n_GetVehicleMeta);
        script.Register("ResetVehicleMeta",              n_ResetVehicleMeta);
        script.Register("HasVehicleMeta",                n_HasVehicleMeta);

        /* ---- Custom typed properties ---- */
        script.Register("RegisterVehicleProperty",       n_RegisterVehicleProperty);
        script.Register("SetVehiclePropertyInt",         n_SetVehiclePropertyInt);
        script.Register("SetVehiclePropertyFloat",       n_SetVehiclePropertyFloat);
        script.Register("SetVehiclePropertyString",      n_SetVehiclePropertyString);
        script.Register("GetVehiclePropertyInt",         n_GetVehiclePropertyInt);
        script.Register("GetVehiclePropertyFloat",       n_GetVehiclePropertyFloat);
        script.Register("GetVehiclePropertyString",      n_GetVehiclePropertyString);
        script.Register("HasVehicleProperty",            n_HasVehicleProperty);

        /* ---- Core / version / addon system ---- */
        script.Register("EVF9_GetVersion",               n_EVF9_GetVersion);
        script.Register("EVF9_GetBuild",                 n_EVF9_GetBuild);
        script.Register("EVF9_RegisterAddon",            n_EVF9_RegisterAddon);
        script.Register("EVF9_UnregisterAddon",          n_EVF9_UnregisterAddon);
        script.Register("EVF9_GetAddonInfo",             n_EVF9_GetAddonInfo);
        script.Register("EVF9_GetMemoryUsage",           n_EVF9_GetMemoryUsage);
        script.Register("EVF9_GetVehicleMemoryUsage",    n_EVF9_GetVehicleMemoryUsage);
        script.Register("EVF9_GetModuleMemoryUsage",     n_EVF9_GetModuleMemoryUsage);
        script.Register("EVF9_GetTotalVehicles",         n_EVF9_GetTotalVehicles);
        script.Register("EVF9_GetManagedVehicles",       n_EVF9_GetManagedVehicles);
        script.Register("EVF9_GetSpawnedVehicles",       n_EVF9_GetSpawnedVehicles);
        script.Register("EVF9_GetDestroyedVehicles",     n_EVF9_GetDestroyedVehicles);
        script.Register("EVF9_GetAbandonedVehicles",     n_EVF9_GetAbandonedVehicles);
        script.Register("EVF9_StartProfiler",            n_EVF9_StartProfiler);
        script.Register("EVF9_StopProfiler",             n_EVF9_StopProfiler);
        script.Register("EVF9_GetProfilerData",          n_EVF9_GetProfilerData);

        /* ---- Fuel module ---- */
        script.Register("SetVehicleFuel",                n_SetVehicleFuel);
        script.Register("GetVehicleFuel",                n_GetVehicleFuel);
        script.Register("AddVehicleFuel",                n_AddVehicleFuel);
        script.Register("RemoveVehicleFuel",             n_RemoveVehicleFuel);
        script.Register("IsVehicleOutOfFuel",            n_IsVehicleOutOfFuel);
        script.Register("GetFuelConsumptionRate",        n_GetFuelConsumptionRate);
        script.Register("SetFuelConsumptionRate",        n_SetFuelConsumptionRate);

        /* ---- Ownership module ---- */
        script.Register("SetVehicleOwner",               n_SetVehicleOwner);
        script.Register("GetVehicleOwner",               n_GetVehicleOwner);
        script.Register("GetVehicleOwnerName",           n_GetVehicleOwnerName);
        script.Register("RemoveVehicleOwner",            n_RemoveVehicleOwner);
        script.Register("TransferVehicleOwnership",      n_TransferVehicleOwnership);
        script.Register("IsVehicleOwner",                n_IsVehicleOwner);
        script.Register("GetOwnedVehicleCount",          n_GetOwnedVehicleCount);

        /* ---- Damage module ---- */
        script.Register("RepairVehicleDamage",           n_RepairVehicleDamage);
        script.Register("DestroyVehiclePart",            n_DestroyVehiclePart);
        script.Register("RepairVehiclePart",             n_RepairVehiclePart);
        script.Register("GetVehicleDamageData",          n_GetVehicleDamageData);
        script.Register("ResetVehicleDamage",            n_ResetVehicleDamage);

        /* ---- Keys & lock module ---- */
        script.Register("GiveVehicleKey",                n_GiveVehicleKey);
        script.Register("RemoveVehicleKey",              n_RemoveVehicleKey);
        script.Register("HasVehicleKey",                 n_HasVehicleKey);
        script.Register("LockVehicle",                   n_LockVehicle);
        script.Register("UnlockVehicle",                 n_UnlockVehicle);
        script.Register("GetVehicleKeyCount",            n_GetVehicleKeyCount);
        script.Register("ClearVehicleKeys",              n_ClearVehicleKeys);

        /* ---- Fleet module ---- */
        script.Register("CreateFleet",                   n_CreateFleet);
        script.Register("DestroyFleet",                  n_DestroyFleet);
        script.Register("AssignVehicleToFleet",          n_AssignVehicleToFleet);
        script.Register("RemoveVehicleFromFleet",        n_RemoveVehicleFromFleet);
        script.Register("GetFleetVehicleCount",          n_GetFleetVehicleCount);
        script.Register("GetVehicleFleet",               n_GetVehicleFleet);
        script.Register("GetFleetOwner",                 n_GetFleetOwner);
        script.Register("IsVehicleInFleet",              n_IsVehicleInFleet);

        /* ---- Tracking module ---- */
        script.Register("TrackVehicle",                  n_TrackVehicle);
        script.Register("UntrackVehicle",                n_UntrackVehicle);
        script.Register("GetVehicleLastPosition",        n_GetVehicleLastPosition);
        script.Register("GetVehicleTravelDistance",      n_GetVehicleTravelDistance);
        script.Register("GetVehicleDistanceFromPoint",   n_GetVehicleDistanceFromPoint);

        /* ---- Insurance module ---- */
        script.Register("InsureVehicle",                 n_InsureVehicle);
        script.Register("CancelInsurance",               n_CancelInsurance);
        script.Register("IsVehicleInsured",              n_IsVehicleInsured);
        script.Register("GetInsuranceExpiration",        n_GetInsuranceExpiration);
        script.Register("RenewInsurance",                n_RenewInsurance);
    }

    /* -----------------------------------------------------------------------
     * onAmxUnload — called when a Pawn script is unloaded.
     * Replaces SA-MP's AmxUnload(AMX* amx).
     * --------------------------------------------------------------------- */
    void onAmxUnload(IPawnScript& script) override
    {
        EventDispatcher::removeScript(&script);
    }

    /* -----------------------------------------------------------------------
     * onFree — called when another component is about to be freed.
     * We null out any pointer to it to prevent dangling references.
     * --------------------------------------------------------------------- */
    void onFree(IComponent* component) override
    {
        if (component == pawn_)
        {
            pawn_->getEventDispatcher().removeEventHandler(this);
            pawn_ = nullptr;
        }
    }

    /* -----------------------------------------------------------------------
     * free — called by Open.MP when it wants to unload this component.
     * --------------------------------------------------------------------- */
    void free() override { delete this; }

    /* -----------------------------------------------------------------------
     * reset — called when the server resets (gamemode change, etc.).
     * EVF9 state is managed externally via Pawn calls so no reset needed here.
     * --------------------------------------------------------------------- */
    void reset() override { /* nothing required */ }

private:
    ICore*          core_       = nullptr;
    IComponentList* components_ = nullptr;
    IPawnComponent* pawn_       = nullptr;
};

/* =========================================================================
 * COMPONENT_ENTRY_POINT — replaces PLUGIN_EXPORT + Supports/Load/Unload.
 *
 * Expands to:
 *   extern "C" PLUGIN_EXPORT IComponent* PLUGIN_CALL RegisterComponent()
 *
 * Open.MP discovers components by calling RegisterComponent() from the
 * shared library.  PLUGIN_EXPORT and PLUGIN_CALL are redefined by sdk.hpp
 * for 64-bit Windows/Linux with correct ABI — they are NOT the same as the
 * SA-MP plugincommon.h versions.
 * ========================================================================= */
COMPONENT_ENTRY_POINT()
{
    return new EVF9Component();
}
