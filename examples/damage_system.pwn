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
 * Example: Damage System
 * Demonstrates vehicle damage tracking and repair.
 */

#include <open.mp>
#include <EVF9>
#include <EVF9/modules/evf9_damage>

EVF9_OnVehicleDamage(vehicleid, Float:amount)
{
    new Float:hp;
    EVF9_GetVehicleHealth(vehicleid, hp);
    printf("[EVF9] Vehicle %d took %.1f damage. HP: %.1f", vehicleid, amount, hp);
    return EVF9_CONTINUE;
}

EVF9_OnVehicleDestroyed(vehicleid, killerid)
{
    printf("[EVF9] Vehicle %d destroyed by player %d", vehicleid, killerid);
    return EVF9_CONTINUE;
}

// /repair
public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/repair", true))
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(!IsManagedVehicle(vehicleid)) return 1;
        RepairVehicleDamage(vehicleid);
        SendClientMessage(playerid, 0x00FF00FF, "Vehicle repaired!");
        return 1;
    }
    return 0;
}
