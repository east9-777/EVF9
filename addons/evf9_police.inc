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

#if defined _evf9_police_included
    #endinput
#endif
#define _evf9_police_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including addons/evf9_police"
#endif

/*
 * Addon: Police
 * Police vehicle management — pursuit tracking, sirens, impound.
 * Uses EVF9 events, metadata, and custom properties exclusively.
 */

#define EVF9_ADDON_POLICE_VERSION "1.0.0"

static g_PoliceAddonID = INVALID_ADDON_ID;

stock EVF9_Police_Init()
{
    g_PoliceAddonID = EVF9_RegisterAddon("evf9_police", EVF9_ADDON_POLICE_VERSION);
    printf("[EVF9:Police] Addon registered (id=%d)", g_PoliceAddonID);
    return 1;
}

forward EVF9_OnVehicleImpounded(vehicleid, officerId);
forward EVF9_OnVehicleReleased(vehicleid, officerId);
forward EVF9_OnPursuitStart(vehicleid, suspectId);
forward EVF9_OnPursuitEnd(vehicleid, reason);

native ImpoundVehicle(vehicleid, officerId);
native ReleaseImpound(vehicleid, officerId);
native IsVehicleImpounded(vehicleid);
native StartPursuit(vehicleid, suspectId);
native EndPursuit(vehicleid, reason);
native IsVehicleInPursuit(vehicleid);
native GetPursuitSuspect(vehicleid);
native ActivateSirens(vehicleid);
native DeactivateSirens(vehicleid);
native IsVehicleSirensActive(vehicleid);
