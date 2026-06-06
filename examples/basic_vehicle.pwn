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
 * Example: Basic Vehicle
 * Demonstrates CreateManagedVehicle, iteration, UIDs, and flags.
 */

#include <a_samp>
#include <EVF9>

public OnGameModeInit()
{
    // Create a managed vehicle (Infernus at LS center)
    new vehicleid = CreateManagedVehicle(411, 0.0, 0.0, 3.0, 0.0, 1, 1);

    if(vehicleid == INVALID_VEHICLE_ID)
    {
        printf("[EVF9] Failed to create vehicle (pool full?)");
        return 1;
    }

    printf("[EVF9] Created vehicle id=%d uid=%d", vehicleid, EVF9_GetVehicleUID(vehicleid));

    // Set a label
    EVF9_SetVehicleTag(vehicleid, "Demo Infernus");

    // Lock it
    EVF9_SetVehicleFlag(vehicleid, VEHICLE_FLAG_LOCKED);

    printf("[EVF9] Total managed vehicles: %d", CountManagedVehicles());

    // Iterate all managed vehicles using the linked list
    EVF9_FOREACH_VEHICLE(vid)
    {
        new tag[65];
        EVF9_GetVehicleTag(vid, tag, sizeof(tag));
        printf("[EVF9] Vehicle %d — model=%d tag=%s locked=%d",
            vid,
            EVF9_GetVehicleModel(vid),
            tag,
            EVF9_HasVehicleFlag(vid, VEHICLE_FLAG_LOCKED)
        );
    }

    return 1;
}

public OnGameModeExit()
{
    // Destroy all managed vehicles
    EVF9_FOREACH_VEHICLE(vid)
    {
        DestroyManagedVehicle(vid);
    }
    return 1;
}
