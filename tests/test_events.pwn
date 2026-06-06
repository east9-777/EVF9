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
 * Test: Events
 * Validates: lifecycle, cancelable, and state events fire correctly.
 */

#include <a_samp>
#include <EVF9>
#include <EVF9/modules/evf9_fuel>
#include <EVF9/modules/evf9_keys>

static g_CreateFired    = 0;
static g_DestroyFired   = 0;
static g_RespawnFired   = 0;
static g_FuelEmptyFired = 0;
static g_LockFired      = 0;
static g_CancelCount    = 0;

EVF9_OnVehicleCreate(vehicleid)    { g_CreateFired++;    return EVF9_CONTINUE; }
EVF9_OnVehicleDestroy(vehicleid)   { g_DestroyFired++;   return EVF9_CONTINUE; }
EVF9_OnVehicleRespawn(vehicleid)   { g_RespawnFired++;   return EVF9_CONTINUE; }
EVF9_OnVehicleFuelEmpty(vehicleid) { g_FuelEmptyFired++; return EVF9_CONTINUE; }

// Cancelable: always cancel lock
EVF9_OnVehicleLock(vehicleid, playerid) { g_LockFired++; g_CancelCount++; return EVF9_BREAK; }

#define TEST_ASSERT(%0,%1) \
    if(%0) printf("[PASS] %1"); \
    else    printf("[FAIL] %1");

public OnGameModeInit()
{
    printf("--- EVF9 Event Tests ---");

    new vid = CreateManagedVehicle(411, 0.0, 0.0, 3.0, 0.0, 1, 1);
    TEST_ASSERT(g_CreateFired == 1, "EVF9_OnVehicleCreate fired")

    RespawnManagedVehicle(vid);
    TEST_ASSERT(g_RespawnFired == 1, "EVF9_OnVehicleRespawn fired")

    SetVehicleFuel(vid, 0.0);
    TEST_ASSERT(g_FuelEmptyFired == 1, "EVF9_OnVehicleFuelEmpty fired")

    // Test cancelable: LockVehicle should be cancelled
    LockVehicle(vid);
    TEST_ASSERT(g_LockFired == 1, "EVF9_OnVehicleLock fired")
    TEST_ASSERT(!EVF9_HasVehicleFlag(vid, VEHICLE_FLAG_LOCKED), "Lock cancelled — flag not set")

    DestroyManagedVehicle(vid);
    TEST_ASSERT(g_DestroyFired == 1, "EVF9_OnVehicleDestroy fired")

    printf("--- Event tests complete ---");
    return 1;
}
