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

/**
 * Example: Tracking System
 * Demonstrates vehicle tracking, position updates.
 */

#include <open.mp>
#include <EVF9>
#include <EVF9/modules/evf9_tracking>

public OnGameModeInit()
{
    new vehicleid = CreateManagedVehicle(560, 0.0, 0.0, 3.0, 0.0, 1, 1);
    TrackVehicle(vehicleid);
    printf("[EVF9] Tracking vehicle %d: %d", vehicleid, EVF9_HasVehicleFlag(vehicleid, VEHICLE_FLAG_TRACKED));
    return 1;
}

EVF9_OnVehicleTrackStart(vehicleid, playerid)
{
    printf("[EVF9] Tracking started on vehicle %d", vehicleid);
    return EVF9_CONTINUE;
}

EVF9_OnVehicleLocationUpdate(vehicleid, Float:x, Float:y, Float:z)
{
    printf("[EVF9] Vehicle %d at %.1f, %.1f, %.1f", vehicleid, x, y, z);
    return EVF9_CONTINUE;
}

// /trackvehicle
public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/trackvehicle", true))
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(!IsManagedVehicle(vehicleid)) return 1;
        new Float:x, Float:y, Float:z;
        GetVehicleLastPosition(vehicleid, x, y, z);
        new str[128];
        format(str, sizeof(str), "Last position: %.1f, %.1f, %.1f", x, y, z);
        SendClientMessage(playerid, -1, str);
        return 1;
    }
    return 0;
}
