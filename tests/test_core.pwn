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
 * Test: Core
 * Validates: creation, destruction, respawn, UIDs, pool, iterator.
 */

#include <a_samp>
#include <EVF9>

static g_Pass = 0;
static g_Fail = 0;

#define TEST_ASSERT(%0,%1) \
    if(%0) { printf("[PASS] %1"); g_Pass++; } \
    else    { printf("[FAIL] %1"); g_Fail++; }

public OnGameModeInit()
{
    printf("--- EVF9 Core Tests ---");

    // Test: version returns positive int
    TEST_ASSERT(EVF9_GetVersion() > 0, "EVF9_GetVersion() > 0")

    // Test: creation
    new vid = CreateManagedVehicle(411, 0.0, 0.0, 3.0, 0.0, 1, 1);
    TEST_ASSERT(vid != INVALID_VEHICLE_ID, "CreateManagedVehicle succeeds")
    TEST_ASSERT(IsManagedVehicle(vid),     "IsManagedVehicle after create")
    TEST_ASSERT(CountManagedVehicles() == 1, "CountManagedVehicles == 1")

    // Test: UID
    new uid = EVF9_GetVehicleUID(vid);
    TEST_ASSERT(uid != 0, "UID is non-zero")
    TEST_ASSERT(EVF9_GetVehicleByUID(uid) == vid, "GetVehicleByUID returns correct vehicleid")

    // Test: tag
    EVF9_SetVehicleTag(vid, "Test Vehicle");
    new tag[65];
    EVF9_GetVehicleTag(vid, tag, sizeof(tag));
    TEST_ASSERT(!strcmp(tag, "Test Vehicle"), "SetVehicleTag / GetVehicleTag round-trip")

    // Test: tag truncation > 64 chars
    EVF9_SetVehicleTag(vid, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"); // 65 A's
    EVF9_GetVehicleTag(vid, tag, sizeof(tag));
    TEST_ASSERT(strlen(tag) == 64, "Tag truncated to 64 chars")

    // Test: health
    EVF9_SetVehicleHealth(vid, 500.0);
    new Float:hp;
    EVF9_GetVehicleHealth(vid, hp);
    TEST_ASSERT(hp == 500.0, "SetVehicleHealth / GetVehicleHealth round-trip")

    // Test: iterator
    new count = 0;
    EVF9_FOREACH_VEHICLE(itv) count++;
    TEST_ASSERT(count == 1, "EVF9_FOREACH_VEHICLE count == 1")

    // Test: respawn
    TEST_ASSERT(RespawnManagedVehicle(vid) == 1, "RespawnManagedVehicle returns 1")

    // Test: clone
    new cloneVid = CloneManagedVehicle(vid);
    TEST_ASSERT(cloneVid != INVALID_VEHICLE_ID, "CloneManagedVehicle returns valid id")
    TEST_ASSERT(CountManagedVehicles() == 2, "CountManagedVehicles == 2 after clone")

    // Test: destroy
    DestroyManagedVehicle(vid);
    DestroyManagedVehicle(cloneVid);
    TEST_ASSERT(CountManagedVehicles() == 0, "CountManagedVehicles == 0 after destroy")
    TEST_ASSERT(EVF9_GetVehicleByUID(uid) == INVALID_VEHICLE_ID, "GetVehicleByUID returns INVALID after destroy")

    printf("--- Results: %d passed, %d failed ---", g_Pass, g_Fail);
    return 1;
}
