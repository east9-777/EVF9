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
 * Example: Insurance System
 * Demonstrates vehicle insurance with expiry timestamps.
 */

#include <a_samp>
#include <EVF9>
#include <EVF9/modules/evf9_insurance>

public OnGameModeInit()
{
    new vehicleid = CreateManagedVehicle(560, 0.0, 0.0, 3.0, 0.0, 1, 1);
    InsureVehicle(vehicleid, 86400); // 24 hours
    printf("[EVF9] Vehicle %d insured: %d", vehicleid, IsVehicleInsured(vehicleid));
    printf("[EVF9] Insurance expires at: %d", GetInsuranceExpiration(vehicleid));
    return 1;
}

// /insure
public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/insure", true))
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(!IsManagedVehicle(vehicleid)) return 1;
        InsureVehicle(vehicleid, 2592000); // 30 days
        SendClientMessage(playerid, 0x00FF00FF, "Vehicle insured for 30 days!");
        return 1;
    }
    return 0;
}
