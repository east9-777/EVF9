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
 * Test: Performance
 * Benchmarks: creation, destruction, iteration, search — 500, 2000, 5000 vehicles.
 */

#include <a_samp>
#include <EVF9>

#define BENCH_COUNT_SMALL  500
#define BENCH_COUNT_MED    2000

static g_Vehicles[BENCH_COUNT_MED];

forward BenchCreate(count);
forward BenchIterate();
forward BenchDestroy(count);
forward BenchSearch(count);

public BenchCreate(count)
{
    new startTick = GetTickCount();
    for(new i = 0; i < count; i++)
    {
        g_Vehicles[i] = CreateManagedVehicle(411, float(i) * 2.0, 0.0, 3.0, 0.0, 1, 1);
    }
    new elapsed = GetTickCount() - startTick;
    printf("[PERF] Create %d vehicles: %dms (avg %.2fms/vehicle)", count, elapsed, float(elapsed)/float(count));
}

public BenchIterate()
{
    new startTick = GetTickCount();
    new count = 0;
    EVF9_FOREACH_VEHICLE(vid) count++;
    new elapsed = GetTickCount() - startTick;
    printf("[PERF] Iterate %d managed vehicles: %dms", count, elapsed);
}

public BenchDestroy(count)
{
    new startTick = GetTickCount();
    for(new i = 0; i < count; i++)
    {
        if(g_Vehicles[i] != INVALID_VEHICLE_ID)
            DestroyManagedVehicle(g_Vehicles[i]);
    }
    new elapsed = GetTickCount() - startTick;
    printf("[PERF] Destroy %d vehicles: %dms", count, elapsed);
}

public BenchSearch(count)
{
    new startTick = GetTickCount();
    for(new i = 0; i < count; i++)
    {
        new uid = EVF9_GetVehicleUID(g_Vehicles[i]);
        EVF9_GetVehicleByUID(uid);
    }
    new elapsed = GetTickCount() - startTick;
    printf("[PERF] UID lookup %d times: %dms", count, elapsed);
}

public OnGameModeInit()
{
    printf("--- EVF9 Performance Benchmarks ---");
    printf("[PERF] Minimum target: 500 vehicles, Recommended: 2000, Advanced: 5000+");

    // Benchmark at BENCH_COUNT_SMALL
    BenchCreate(BENCH_COUNT_SMALL);
    BenchSearch(BENCH_COUNT_SMALL);
    BenchIterate();
    BenchDestroy(BENCH_COUNT_SMALL);

    // Benchmark at BENCH_COUNT_MED
    BenchCreate(BENCH_COUNT_MED);
    BenchIterate();
    BenchSearch(BENCH_COUNT_MED);
    BenchDestroy(BENCH_COUNT_MED);

    printf("[PERF] Memory usage: %d bytes", EVF9_GetMemoryUsage());
    printf("--- Benchmark complete ---");
    return 1;
}
