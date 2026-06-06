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
 * Test: Modules
 * Validates: ownership, fuel, damage, fleet, keys, tracking, insurance.
 */

#include <open.mp>
#include <EVF9>
#include <EVF9/modules/evf9_ownership>
#include <EVF9/modules/evf9_fuel>
#include <EVF9/modules/evf9_damage>
#include <EVF9/modules/evf9_fleet>
#include <EVF9/modules/evf9_keys>
#include <EVF9/modules/evf9_tracking>
#include <EVF9/modules/evf9_insurance>

#define TEST_ASSERT(%0,%1) \
    if(%0) printf("[PASS] %1"); \
    else    printf("[FAIL] %1");

public OnGameModeInit()
{
    printf("--- EVF9 Module Tests ---");
    new vid = CreateManagedVehicle(411, 0.0, 0.0, 3.0, 0.0, 1, 1);

    // Ownership
    SetVehicleOwner(vid, 5);
    TEST_ASSERT(GetVehicleOwner(vid) == 5, "SetVehicleOwner / GetVehicleOwner")
    TEST_ASSERT(IsVehicleOwner(vid, 5),    "IsVehicleOwner returns 1")
    TEST_ASSERT(!IsVehicleOwner(vid, 6),   "IsVehicleOwner returns 0 for non-owner")
    TEST_ASSERT(GetOwnedVehicleCount(5) >= 1, "GetOwnedVehicleCount >= 1")
    TransferVehicleOwnership(vid, 7);
    TEST_ASSERT(GetVehicleOwner(vid) == 7, "TransferVehicleOwnership")
    RemoveVehicleOwner(vid);
    TEST_ASSERT(GetVehicleOwner(vid) == -1, "RemoveVehicleOwner")

    // Fuel
    SetVehicleFuel(vid, 50.0);
    new Float:fuel;
    GetVehicleFuel(vid, fuel);
    TEST_ASSERT(fuel == 50.0, "SetVehicleFuel / GetVehicleFuel")
    AddVehicleFuel(vid, 20.0);
    GetVehicleFuel(vid, fuel);
    TEST_ASSERT(fuel == 70.0, "AddVehicleFuel")
    RemoveVehicleFuel(vid, 80.0);
    TEST_ASSERT(IsVehicleOutOfFuel(vid), "IsVehicleOutOfFuel after RemoveVehicleFuel")

    // Damage
    EVF9_SetVehicleHealth(vid, 500.0);
    RepairVehicleDamage(vid);
    new Float:hp;
    EVF9_GetVehicleHealth(vid, hp);
    TEST_ASSERT(hp == 1000.0, "RepairVehicleDamage restores health to 1000")

    // Fleet
    new fleetid = CreateFleet("Test Fleet", 0);
    TEST_ASSERT(fleetid >= 0, "CreateFleet returns valid id")
    AssignVehicleToFleet(vid, fleetid);
    TEST_ASSERT(GetVehicleFleet(vid) == fleetid, "AssignVehicleToFleet / GetVehicleFleet")
    TEST_ASSERT(GetFleetVehicleCount(fleetid) >= 1, "GetFleetVehicleCount >= 1")
    RemoveVehicleFromFleet(vid);
    TEST_ASSERT(GetVehicleFleet(vid) == -1, "RemoveVehicleFromFleet")

    // Keys
    GiveVehicleKey(vid, 3);
    TEST_ASSERT(HasVehicleKey(vid, 3),  "GiveVehicleKey / HasVehicleKey")
    TEST_ASSERT(!HasVehicleKey(vid, 4), "HasVehicleKey returns 0 for non-holder")
    RemoveVehicleKey(vid, 3);
    TEST_ASSERT(!HasVehicleKey(vid, 3), "RemoveVehicleKey")

    // Tracking
    TrackVehicle(vid);
    TEST_ASSERT(EVF9_HasVehicleFlag(vid, VEHICLE_FLAG_TRACKED), "TrackVehicle sets VEHICLE_FLAG_TRACKED")
    UntrackVehicle(vid);
    TEST_ASSERT(!EVF9_HasVehicleFlag(vid, VEHICLE_FLAG_TRACKED), "UntrackVehicle clears VEHICLE_FLAG_TRACKED")

    // Insurance
    InsureVehicle(vid, 86400);
    TEST_ASSERT(IsVehicleInsured(vid),              "InsureVehicle / IsVehicleInsured")
    TEST_ASSERT(GetInsuranceExpiration(vid) > 0,    "GetInsuranceExpiration > 0")
    CancelInsurance(vid);
    TEST_ASSERT(!IsVehicleInsured(vid), "CancelInsurance")

    DestroyManagedVehicle(vid);
    printf("--- Module tests complete ---");
    return 1;
}
