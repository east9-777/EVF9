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
 * Example: Fleet System
 * Demonstrates fleet creation, vehicle assignment, and events.
 */

#include <open.mp>
#include <EVF9>
#include <EVF9/modules/evf9_fleet>

static g_FleetID = -1;

public OnGameModeInit()
{
    g_FleetID = CreateFleet("City Taxi Co.", 0); // owned by player 0
    printf("[EVF9] Fleet created id=%d", g_FleetID);

    // Assign 5 taxis to the fleet
    for(new i = 0; i < 5; i++)
    {
        new vid = CreateManagedVehicle(420, 300.0 + float(i) * 5.0, 300.0, 3.0, 0.0, 3, 3);
        AssignVehicleToFleet(vid, g_FleetID);
    }

    printf("[EVF9] Fleet %d has %d vehicles", g_FleetID, GetFleetVehicleCount(g_FleetID));
    return 1;
}

EVF9_OnFleetCreated(fleetid, ownerid)
{
    printf("[EVF9] Fleet %d created by player %d", fleetid, ownerid);
    return EVF9_CONTINUE;
}

EVF9_OnVehicleFleetAssigned(vehicleid, fleetid)
{
    printf("[EVF9] Vehicle %d assigned to fleet %d", vehicleid, fleetid);
    return EVF9_CONTINUE;
}
