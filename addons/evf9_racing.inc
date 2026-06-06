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

#if defined _evf9_racing_included
    #endinput
#endif
#define _evf9_racing_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including addons/evf9_racing"
#endif

/*
 * Addon: Racing
 * Race management, lap tracking, leaderboard, and race events.
 * Communicates with EVF9 exclusively via API, events, and metadata.
 */

#define INVALID_RACE_ID (-1)
#define EVF9_ADDON_RACING_VERSION "1.0.0"

static g_RacingAddonID = INVALID_ADDON_ID;

stock EVF9_Racing_Init()
{
    g_RacingAddonID = EVF9_RegisterAddon("evf9_racing", EVF9_ADDON_RACING_VERSION);
    if(g_RacingAddonID == INVALID_ADDON_ID)
    {
        printf("[EVF9:Racing] Failed to register addon.");
        return 0;
    }
    printf("[EVF9:Racing] Addon registered (id=%d)", g_RacingAddonID);
    return 1;
}

forward EVF9_OnRaceStart(raceid);
forward EVF9_OnRaceEnd(raceid, winnerid);
forward EVF9_OnRaceLap(raceid, vehicleid, lap);
forward EVF9_OnRaceCheckpoint(raceid, vehicleid, checkpoint);

// Racing API — uses metadata and custom properties internally
native CreateRace(const name[], maxlaps);
native DestroyRace(raceid);
native AddRaceVehicle(raceid, vehicleid);
native RemoveRaceVehicle(raceid, vehicleid);
native StartRace(raceid);
native EndRace(raceid);
native GetRaceLeader(raceid);
native GetVehicleLap(raceid, vehicleid);
native GetVehicleRacePosition(raceid, vehicleid);
