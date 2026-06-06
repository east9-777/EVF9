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
 * Example: Ownership System
 * Demonstrates vehicle ownership assignment, transfer, and events.
 */

#include <open.mp>
#include <EVF9>
#include <EVF9/modules/evf9_ownership>

public OnGameModeInit()
{
    new vehicleid = CreateManagedVehicle(411, 100.0, 200.0, 3.0, 90.0, 6, 0);
    SetVehicleOwner(vehicleid, 0); // Player 0 owns it
    printf("[EVF9] Vehicle %d owned by player %d", vehicleid, GetVehicleOwner(vehicleid));
    printf("[EVF9] Player 0 owns %d vehicle(s)", GetOwnedVehicleCount(0));
    return 1;
}

EVF9_OnVehicleOwnerAssigned(vehicleid, playerid)
{
    new name[25];
    GetVehicleOwnerName(vehicleid, name, sizeof(name));
    printf("[EVF9] Vehicle %d assigned to player %d (%s)", vehicleid, playerid, name);
    return EVF9_CONTINUE;
}

EVF9_OnVehicleOwnerChange(vehicleid, oldowner, newowner)
{
    printf("[EVF9] Vehicle %d transferred from %d to %d", vehicleid, oldowner, newowner);
    return EVF9_CONTINUE;
}

// Command: /givevehicle <playerid>
public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!strcmp(cmdtext[0], "/givevehicle", true, 12))
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(!IsManagedVehicle(vehicleid)) return SendClientMessage(playerid, -1, "Not a managed vehicle.");
        if(!IsVehicleOwner(vehicleid, playerid)) return SendClientMessage(playerid, -1, "You don't own this vehicle.");
        new target = strval(cmdtext[13]);
        TransferVehicleOwnership(vehicleid, target);
        SendClientMessage(playerid, -1, "Vehicle transferred.");
        return 1;
    }
    return 0;
}
