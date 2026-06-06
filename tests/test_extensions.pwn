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
 * Test: Extensions
 * Validates: persistence state machine, DIRTY/SAVING/CLEAN transitions.
 * NOTE: MySQL/SQLite require actual plugin — this tests the state machine only.
 */

#include <a_samp>
#include <EVF9>

#define TEST_ASSERT(%0,%1) \
    if(%0) printf("[PASS] %1"); \
    else    printf("[FAIL] %1");

public OnGameModeInit()
{
    printf("--- EVF9 Extension / Persistence Tests ---");

    new vid = CreateManagedVehicle(411, 0.0, 0.0, 3.0, 0.0, 1, 1);

    // Initial state: CLEAN
    TEST_ASSERT(EVF9_GetVehicleSaveState(vid) == EVF9_STATE_CLEAN, "Initial state is CLEAN")

    // Modify to trigger DIRTY
    EVF9_SetVehicleHealth(vid, 500.0);
    TEST_ASSERT(EVF9_GetVehicleSaveState(vid) == EVF9_STATE_DIRTY, "State is DIRTY after modification")

    // Begin save
    EVF9_BeginSave(vid);
    TEST_ASSERT(EVF9_GetVehicleSaveState(vid) == EVF9_STATE_SAVING, "State is SAVING after BeginSave")
    TEST_ASSERT(EVF9_IsVehicleSaving(vid), "IsVehicleSaving returns 1")

    // End save
    EVF9_EndSave(vid);
    TEST_ASSERT(EVF9_GetVehicleSaveState(vid) == EVF9_STATE_CLEAN, "State is CLEAN after EndSave")
    TEST_ASSERT(!EVF9_IsVehicleSaving(vid), "IsVehicleSaving returns 0")

    // Metadata
    SetVehicleMeta(vid, META_JOB, 42);
    TEST_ASSERT(GetVehicleMeta(vid, META_JOB) == 42, "SetVehicleMeta / GetVehicleMeta")
    TEST_ASSERT(HasVehicleMeta(vid, META_JOB), "HasVehicleMeta returns 1")
    ResetVehicleMeta(vid, META_JOB);
    TEST_ASSERT(GetVehicleMeta(vid, META_JOB) == 0, "ResetVehicleMeta")

    // Custom Properties
    new propInt   = RegisterVehicleProperty("test_int",   EVF9_PROP_INT);
    new propFloat = RegisterVehicleProperty("test_float", EVF9_PROP_FLOAT);
    SetVehiclePropertyInt(vid, propInt, 99);
    TEST_ASSERT(GetVehiclePropertyInt(vid, propInt) == 99, "SetVehiclePropertyInt / GetVehiclePropertyInt")
    SetVehiclePropertyFloat(vid, propFloat, 3.14);
    new Float:fVal;
    GetVehiclePropertyFloat(vid, propFloat, fVal);
    TEST_ASSERT(fVal > 3.1 && fVal < 3.2, "SetVehiclePropertyFloat / GetVehiclePropertyFloat")
    TEST_ASSERT(HasVehicleProperty(vid, propInt), "HasVehicleProperty returns 1")

    DestroyManagedVehicle(vid);
    printf("--- Extension tests complete ---");
    return 1;
}
