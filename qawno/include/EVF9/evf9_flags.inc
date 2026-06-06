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

#if defined _evf9_flags_included
    #endinput
#endif
#define _evf9_flags_included

/*
 * Vehicle flags — bitwise enum.
 *
 * ⚠ VEHICLE_FLAG_NONE is 0 — calling EVF9_HasVehicleFlag(vehicleid, VEHICLE_FLAG_NONE)
 *   always returns 0 (flags & 0 == 0). To check if a vehicle has NO flags set,
 *   compare the full bitmask: if(EVF9_GetVehicleFlags(vehicleid) == VEHICLE_FLAG_NONE)
 *
 * ⚠ VEHICLE_FLAG_INSURED and VEHICLE_FLAG_TRACKED are owned by their modules.
 *   Never call EVF9_SetVehicleFlag(vid, VEHICLE_FLAG_INSURED) directly.
 *   Use InsureVehicle/CancelInsurance and TrackVehicle/UntrackVehicle respectively.
 */

enum E_VEHICLE_FLAGS (<<= 1)
{
    VEHICLE_FLAG_NONE      = 0,     // sentinel — do NOT use in EVF9_HasVehicleFlag
    VEHICLE_FLAG_LOCKED    = 1,
    VEHICLE_FLAG_ENGINE,            // 2
    VEHICLE_FLAG_LIGHTS,            // 4
    VEHICLE_FLAG_INSURED,           // 8  — managed by evf9_insurance.inc only
    VEHICLE_FLAG_TRACKED,           // 16 — managed by evf9_tracking.inc only
    VEHICLE_FLAG_ABANDONED,         // 32
    VEHICLE_FLAG_SPAWNED            // 64
};

// Convenience macros for multi-flag checks
#define EVF9_CheckFlags(%0,%1)    (bool:(EVF9_GetVehicleFlags(%0) & (%1)) == (%1))
#define EVF9_AnyFlag(%0,%1)       (bool:(EVF9_GetVehicleFlags(%0) & (%1)))
