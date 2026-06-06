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
 * Example: Fuel System
 * Demonstrates fuel management, consumption, and events.
 */

#include <open.mp>
#include <EVF9>
#include <EVF9/modules/evf9_fuel>

static g_FuelTimer;

public OnGameModeInit()
{
    new vehicleid = CreateManagedVehicle(405, 50.0, 50.0, 3.0, 0.0, 3, 3);
    SetVehicleFuel(vehicleid, 80.0);
    SetFuelConsumptionRate(vehicleid, 0.5); // 0.5 units per tick

    g_FuelTimer = SetTimer("ConsumeFuelTick", 1000, true);
    return 1;
}

forward ConsumeFuelTick();
public ConsumeFuelTick()
{
    EVF9_FOREACH_VEHICLE(vid)
    {
        if(GetVehicleDriver(vid) != INVALID_PLAYER_ID)
            RemoveVehicleFuel(vid, 0.5);
    }
}

EVF9_OnVehicleFuelLow(vehicleid, Float:fuel)
{
    new driver = GetVehicleDriver(vehicleid);
    if(driver != INVALID_PLAYER_ID)
        SendClientMessage(driver, 0xFFFF00FF, "Warning: Fuel low!");
    return EVF9_CONTINUE;
}

EVF9_OnVehicleFuelEmpty(vehicleid)
{
    EVF9_SetVehicleFlag(vehicleid, VEHICLE_FLAG_ENGINE); // kill engine
    new driver = GetVehicleDriver(vehicleid);
    if(driver != INVALID_PLAYER_ID)
        SendClientMessage(driver, 0xFF0000FF, "Out of fuel! Engine off.");
    return EVF9_CONTINUE;
}

// /refuel — refuel at current position (full example would check a fuel station)
public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/refuel", true))
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(!IsManagedVehicle(vehicleid)) return SendClientMessage(playerid, -1, "Not in a managed vehicle.");
        SetVehicleFuel(vehicleid, 100.0);
        SendClientMessage(playerid, 0x00FF00FF, "Vehicle refuelled!");
        return 1;
    }
    return 0;
}
