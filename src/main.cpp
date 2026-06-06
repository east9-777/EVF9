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
 * main.cpp — Plugin entry point for Open.MP and SA-MP.
 *
 * HOW NATIVES ARE REGISTERED
 * ===========================
 * Each native is defined WITHOUT the `static` keyword in its own .cpp file
 * (the removed `static` makes the function external-linkage).
 * Here we forward-declare every one, build a single AMX_NATIVE_INFO array,
 * and call amx_Register() inside AmxLoad / onAmxLoad.
 *
 * FLOAT CONVENTION
 * ================
 * Float by value:  params[N] holds the bit-pattern; use amx_ctof(params[N]).
 * Float by ref:    evf9_SetFloat(amx, params[N], val) (see amx_helpers.h).
 *
 * EVENT DISPATCHER
 * ================
 * EventDispatcher::setAMX() is called in AmxLoad so the dispatcher can call
 * amx_FindPublic + amx_Exec for every Pawn public callback.
 */

#include "event_dispatcher.h"
#include "vehicle_system.h"
#include <amx/amx.h>
#include <plugincommon.h>   /* SA-MP / Open.MP plugin API */
#include <cstdio>
#include <cstring>

/* -------------------------------------------------------------------------
 * Forward declarations — one per native function across all .cpp files.
 * Remove `static` was done in each native file so they are extern here.
 * ------------------------------------------------------------------------- */

/* pool_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_RegisterVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_UnregisterVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_RespawnVehicleInternal(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsManagedVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_CountManagedVehicles(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_UpdateVehiclePosition(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleLastDriver(AMX*, cell*);

/* entity_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleModel(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleSpawnPos(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleWorld(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleInterior(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleFlags(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleCreatedAt(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleLastDriver(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleLastOccupant(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleDistanceTravelled(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_IsVehicleSpawned(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_IsVehicleManaged(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleHealth(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleHealthInternal(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleTag(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleTag(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleUID(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleByUID(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleLifetime(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_ResetVehicleDistance(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleLifetimeDistance(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleTripDistance(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleLocked(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleEngineOn(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleTracked(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleAbandoned(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_CountVehiclesByModel(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_CountVehiclesByOwner(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetNearestManagedVehicle(AMX*, cell*);

/* flags_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_SetVehicleFlag(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_ClearVehicleFlag(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_HasVehicleFlag(AMX*, cell*);

/* api_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_GetFirstVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetNextVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_BeginSave(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_EndSave(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_IsVehicleSaving(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleSaveState(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_SetVehicleMeta(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleMeta(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_ResetVehicleMeta(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_HasVehicleMeta(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_RegisterVehicleProperty(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_SetVehiclePropertyInt(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_SetVehiclePropertyFloat(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_SetVehiclePropertyString(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehiclePropertyInt(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehiclePropertyFloat(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehiclePropertyString(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_HasVehicleProperty(AMX*, cell*);

/* core_natives.cpp */
extern cell AMX_NATIVE_CALL n_EVF9_GetVersion(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetBuild(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_RegisterAddon(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_UnregisterAddon(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetAddonInfo(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetMemoryUsage(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetVehicleMemoryUsage(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetModuleMemoryUsage(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetTotalVehicles(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetManagedVehicles(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetSpawnedVehicles(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetDestroyedVehicles(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetAbandonedVehicles(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_StartProfiler(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_StopProfiler(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_EVF9_GetProfilerData(AMX*, cell*);

/* modules/fuel.cpp */
extern cell AMX_NATIVE_CALL n_SetVehicleFuel(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleFuel(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_AddVehicleFuel(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleFuel(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleOutOfFuel(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetFuelConsumptionRate(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_SetFuelConsumptionRate(AMX*, cell*);

/* modules/ownership.cpp */
extern cell AMX_NATIVE_CALL n_SetVehicleOwner(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleOwner(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleOwnerName(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleOwner(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_TransferVehicleOwnership(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleOwner(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetOwnedVehicleCount(AMX*, cell*);

/* modules/damage.cpp */
extern cell AMX_NATIVE_CALL n_RepairVehicleDamage(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_DestroyVehiclePart(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_RepairVehiclePart(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleDamageData(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_ResetVehicleDamage(AMX*, cell*);

/* modules/keys.cpp */
extern cell AMX_NATIVE_CALL n_GiveVehicleKey(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleKey(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_HasVehicleKey(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_LockVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_UnlockVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleKeyCount(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_ClearVehicleKeys(AMX*, cell*);

/* modules/fleet.cpp */
extern cell AMX_NATIVE_CALL n_CreateFleet(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_DestroyFleet(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_AssignVehicleToFleet(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_RemoveVehicleFromFleet(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetFleetVehicleCount(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleFleet(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetFleetOwner(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleInFleet(AMX*, cell*);

/* modules/tracking.cpp */
extern cell AMX_NATIVE_CALL n_TrackVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_UntrackVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleLastPosition(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleTravelDistance(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetVehicleDistanceFromPoint(AMX*, cell*);

/* modules/insurance.cpp */
extern cell AMX_NATIVE_CALL n_InsureVehicle(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_CancelInsurance(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_IsVehicleInsured(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_GetInsuranceExpiration(AMX*, cell*);
extern cell AMX_NATIVE_CALL n_RenewInsurance(AMX*, cell*);

/* -------------------------------------------------------------------------
 * Native table — maps Pawn native names to C++ function pointers.
 * The table MUST end with {nullptr, nullptr}.
 * ------------------------------------------------------------------------- */
static AMX_NATIVE_INFO evf9_natives[] =
{
    /* --- Internal pool management (called by Pawn stocks) --- */
    { "EVF9_RegisterVehicle",           n_EVF9_RegisterVehicle           },
    { "EVF9_UnregisterVehicle",         n_EVF9_UnregisterVehicle         },
    { "EVF9_RespawnVehicleInternal",    n_EVF9_RespawnVehicleInternal    },
    { "EVF9_UpdateVehiclePosition",     n_EVF9_UpdateVehiclePosition     },
    { "EVF9_SetVehicleLastDriver",      n_EVF9_SetVehicleLastDriver      },

    /* --- Pool queries --- */
    { "IsManagedVehicle",               n_IsManagedVehicle               },
    { "CountManagedVehicles",           n_CountManagedVehicles           },

    /* --- Entity getters --- */
    { "EVF9_GetVehicleModel",           n_EVF9_GetVehicleModel           },
    { "EVF9_GetVehicleSpawnPos",        n_EVF9_GetVehicleSpawnPos        },
    { "EVF9_GetVehicleWorld",           n_EVF9_GetVehicleWorld           },
    { "EVF9_GetVehicleInterior",        n_EVF9_GetVehicleInterior        },
    { "EVF9_GetVehicleFlags",           n_EVF9_GetVehicleFlags           },
    { "EVF9_GetVehicleCreatedAt",       n_EVF9_GetVehicleCreatedAt       },
    { "EVF9_GetVehicleLastDriver",      n_EVF9_GetVehicleLastDriver      },
    { "EVF9_GetVehicleLastOccupant",    n_EVF9_GetVehicleLastOccupant    },
    { "EVF9_GetVehicleDistanceTravelled",n_EVF9_GetVehicleDistanceTravelled},
    { "EVF9_IsVehicleSpawned",          n_EVF9_IsVehicleSpawned          },
    { "EVF9_IsVehicleManaged",          n_EVF9_IsVehicleManaged          },
    { "EVF9_GetVehicleHealth",          n_EVF9_GetVehicleHealth          },
    { "EVF9_SetVehicleHealthInternal",  n_EVF9_SetVehicleHealthInternal  },
    { "EVF9_SetVehicleTag",             n_EVF9_SetVehicleTag             },
    { "EVF9_GetVehicleTag",             n_EVF9_GetVehicleTag             },
    { "EVF9_GetVehicleUID",             n_EVF9_GetVehicleUID             },
    { "EVF9_GetVehicleByUID",           n_EVF9_GetVehicleByUID           },
    { "EVF9_GetVehicleLifetime",        n_EVF9_GetVehicleLifetime        },

    /* --- Distance --- */
    { "ResetVehicleDistance",           n_ResetVehicleDistance           },
    { "GetVehicleLifetimeDistance",     n_GetVehicleLifetimeDistance     },
    { "GetVehicleTripDistance",         n_GetVehicleTripDistance         },
    { "CountVehiclesByModel",           n_CountVehiclesByModel           },
    { "CountVehiclesByOwner",           n_CountVehiclesByOwner           },
    { "GetNearestManagedVehicle",       n_GetNearestManagedVehicle       },

    /* --- State queries --- */
    { "IsVehicleLocked",                n_IsVehicleLocked                },
    { "IsVehicleEngineOn",              n_IsVehicleEngineOn              },
    { "IsVehicleTracked",               n_IsVehicleTracked               },
    { "IsVehicleAbandoned",             n_IsVehicleAbandoned             },

    /* --- Flags --- */
    { "EVF9_SetVehicleFlag",            n_EVF9_SetVehicleFlag            },
    { "EVF9_ClearVehicleFlag",          n_EVF9_ClearVehicleFlag          },
    { "EVF9_HasVehicleFlag",            n_EVF9_HasVehicleFlag            },

    /* --- Iterator --- */
    { "EVF9_GetFirstVehicle",           n_EVF9_GetFirstVehicle           },
    { "EVF9_GetNextVehicle",            n_EVF9_GetNextVehicle            },

    /* --- Save state machine --- */
    { "EVF9_BeginSave",                 n_EVF9_BeginSave                 },
    { "EVF9_EndSave",                   n_EVF9_EndSave                   },
    { "EVF9_IsVehicleSaving",           n_EVF9_IsVehicleSaving           },
    { "EVF9_GetVehicleSaveState",       n_EVF9_GetVehicleSaveState       },

    /* --- Metadata --- */
    { "SetVehicleMeta",                 n_SetVehicleMeta                 },
    { "GetVehicleMeta",                 n_GetVehicleMeta                 },
    { "ResetVehicleMeta",               n_ResetVehicleMeta               },
    { "HasVehicleMeta",                 n_HasVehicleMeta                 },

    /* --- Custom properties --- */
    { "RegisterVehicleProperty",        n_RegisterVehicleProperty        },
    { "SetVehiclePropertyInt",          n_SetVehiclePropertyInt          },
    { "SetVehiclePropertyFloat",        n_SetVehiclePropertyFloat        },
    { "SetVehiclePropertyString",       n_SetVehiclePropertyString       },
    { "GetVehiclePropertyInt",          n_GetVehiclePropertyInt          },
    { "GetVehiclePropertyFloat",        n_GetVehiclePropertyFloat        },
    { "GetVehiclePropertyString",       n_GetVehiclePropertyString       },
    { "HasVehicleProperty",             n_HasVehicleProperty             },

    /* --- Core / version --- */
    { "EVF9_GetVersion",                n_EVF9_GetVersion                },
    { "EVF9_GetBuild",                  n_EVF9_GetBuild                  },
    { "EVF9_RegisterAddon",             n_EVF9_RegisterAddon             },
    { "EVF9_UnregisterAddon",           n_EVF9_UnregisterAddon           },
    { "EVF9_GetAddonInfo",              n_EVF9_GetAddonInfo              },
    { "EVF9_GetMemoryUsage",            n_EVF9_GetMemoryUsage            },
    { "EVF9_GetVehicleMemoryUsage",     n_EVF9_GetVehicleMemoryUsage     },
    { "EVF9_GetModuleMemoryUsage",      n_EVF9_GetModuleMemoryUsage      },
    { "EVF9_GetTotalVehicles",          n_EVF9_GetTotalVehicles          },
    { "EVF9_GetManagedVehicles",        n_EVF9_GetManagedVehicles        },
    { "EVF9_GetSpawnedVehicles",        n_EVF9_GetSpawnedVehicles        },
    { "EVF9_GetDestroyedVehicles",      n_EVF9_GetDestroyedVehicles      },
    { "EVF9_GetAbandonedVehicles",      n_EVF9_GetAbandonedVehicles      },
    { "EVF9_StartProfiler",             n_EVF9_StartProfiler             },
    { "EVF9_StopProfiler",              n_EVF9_StopProfiler              },
    { "EVF9_GetProfilerData",           n_EVF9_GetProfilerData           },

    /* --- Fuel module --- */
    { "SetVehicleFuel",                 n_SetVehicleFuel                 },
    { "GetVehicleFuel",                 n_GetVehicleFuel                 },
    { "AddVehicleFuel",                 n_AddVehicleFuel                 },
    { "RemoveVehicleFuel",              n_RemoveVehicleFuel              },
    { "IsVehicleOutOfFuel",             n_IsVehicleOutOfFuel             },
    { "GetFuelConsumptionRate",         n_GetFuelConsumptionRate         },
    { "SetFuelConsumptionRate",         n_SetFuelConsumptionRate         },

    /* --- Ownership module --- */
    { "SetVehicleOwner",                n_SetVehicleOwner                },
    { "GetVehicleOwner",                n_GetVehicleOwner                },
    { "GetVehicleOwnerName",            n_GetVehicleOwnerName            },
    { "RemoveVehicleOwner",             n_RemoveVehicleOwner             },
    { "TransferVehicleOwnership",       n_TransferVehicleOwnership       },
    { "IsVehicleOwner",                 n_IsVehicleOwner                 },
    { "GetOwnedVehicleCount",           n_GetOwnedVehicleCount           },

    /* --- Damage module --- */
    { "RepairVehicleDamage",            n_RepairVehicleDamage            },
    { "DestroyVehiclePart",             n_DestroyVehiclePart             },
    { "RepairVehiclePart",              n_RepairVehiclePart              },
    { "GetVehicleDamageData",           n_GetVehicleDamageData           },
    { "ResetVehicleDamage",             n_ResetVehicleDamage             },

    /* --- Keys & lock module --- */
    { "GiveVehicleKey",                 n_GiveVehicleKey                 },
    { "RemoveVehicleKey",               n_RemoveVehicleKey               },
    { "HasVehicleKey",                  n_HasVehicleKey                  },
    { "LockVehicle",                    n_LockVehicle                    },
    { "UnlockVehicle",                  n_UnlockVehicle                  },
    { "GetVehicleKeyCount",             n_GetVehicleKeyCount             },
    { "ClearVehicleKeys",               n_ClearVehicleKeys               },

    /* --- Fleet module --- */
    { "CreateFleet",                    n_CreateFleet                    },
    { "DestroyFleet",                   n_DestroyFleet                   },
    { "AssignVehicleToFleet",           n_AssignVehicleToFleet           },
    { "RemoveVehicleFromFleet",         n_RemoveVehicleFromFleet         },
    { "GetFleetVehicleCount",           n_GetFleetVehicleCount           },
    { "GetVehicleFleet",                n_GetVehicleFleet                },
    { "GetFleetOwner",                  n_GetFleetOwner                  },
    { "IsVehicleInFleet",               n_IsVehicleInFleet               },

    /* --- Tracking module --- */
    { "TrackVehicle",                   n_TrackVehicle                   },
    { "UntrackVehicle",                 n_UntrackVehicle                 },
    { "GetVehicleLastPosition",         n_GetVehicleLastPosition         },
    { "GetVehicleTravelDistance",       n_GetVehicleTravelDistance       },
    { "GetVehicleDistanceFromPoint",    n_GetVehicleDistanceFromPoint    },

    /* --- Insurance module --- */
    { "InsureVehicle",                  n_InsureVehicle                  },
    { "CancelInsurance",                n_CancelInsurance                },
    { "IsVehicleInsured",               n_IsVehicleInsured               },
    { "GetInsuranceExpiration",         n_GetInsuranceExpiration         },
    { "RenewInsurance",                 n_RenewInsurance                 },

    /* --- Sentinel --- */
    { nullptr, nullptr }
};

/* =========================================================================
 * SA-MP plugin interface
 * ========================================================================= */

void** ppPluginData = nullptr;

extern void* pAMXFunctions;
void*        pAMXFunctions = nullptr;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void** pluginData)
{
    ppPluginData  = pluginData;
    pAMXFunctions = pluginData[PLUGIN_DATA_AMX_EXPORTS];

    /* Print banner to server console */
    typedef void (*logprintf_t)(const char* format, ...);
    logprintf_t logprintf = (logprintf_t)pluginData[PLUGIN_DATA_LOGPRINTF];
    logprintf(" ");
    logprintf(" *** EVF9 - Extended Vehicle Framework 9 ***");
    logprintf(" *** Created by EAST9 — github.com/east9-777 ***");
    logprintf(" *** Version 1.0.0 loaded successfully. ***");
    logprintf(" ");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
    /* Nothing to tear down — all state is in static singletons */
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
    /* Store the AMX pointer for event dispatching.
     * If multiple gamemodes are loaded (filterscripts), only the first amx
     * that defines EVF9 callbacks will receive events. For filterscript
     * support, maintain a list of AMX pointers (extension task). */
    if (!EventDispatcher::getAMX())
        EventDispatcher::setAMX(amx);

    /* Register all natives with this AMX instance */
    return amx_Register(amx, evf9_natives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
    if (EventDispatcher::getAMX() == amx)
        EventDispatcher::setAMX(nullptr);
    return AMX_ERR_NONE;
}

/* ProcessTick is called every server tick.
 * Currently used as a hook point for future timer / distance-update logic.
 * Keep it lightweight — avoid any heavy allocation here. */
PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
    /* Reserved for future use (e.g. periodic fuel drain, position sync) */
}
