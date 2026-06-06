<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Performance Guide

## Targets

| Scenario | Vehicles |
|---|---|
| Minimum | 500 |
| Recommended | 2,000 |
| Advanced | 5,000+ |

## Internal Techniques

| Technique | Purpose |
|---|---|
| Linked List iterator | O(n) over managed vehicles only — never loops MAX_VEHICLES |
| Bitwise flags | Compact storage, fast queries |
| Pool allocator | Pre-allocated vehicle slots |
| Per-vehicle cache | Position, velocity, occupancy cached and updated at `EVF9_CACHE_INTERVAL` |
| Typed properties | No string parsing in hot paths |

## Configuration

```pawn
// Update cache every 1 second instead of 500ms for lighter servers
#define EVF9_CACHE_INTERVAL 1000
#include <EVF9>
```

## Anti-patterns

```pawn
// BAD — iterates all MAX_VEHICLES slots (2000 iterations with empty slots)
for(new i = 0; i < MAX_VEHICLES; i++) { ... }

// GOOD — iterates only managed vehicles (O(n) real count)
EVF9_FOREACH_VEHICLE(vid) { ... }
```

```pawn
// BAD — strings as identifiers in loops
if(strcmp(someString, "vehicle_type") == 0) { ... }

// GOOD — typed metadata or int enum
if(GetVehicleMeta(vid, META_CUSTOM_1) == TYPE_TAXI) { ... }
```

## Memory Usage

```pawn
printf("Total memory: %d bytes", EVF9_GetMemoryUsage());
printf("Vehicle 42:   %d bytes", EVF9_GetVehicleMemoryUsage(42));
```

## Profiler

```pawn
#define EVF9_ENABLE_PROFILER 1
#include <EVF9>

EVF9_StartProfiler();
// ... operations ...
EVF9_StopProfiler();
new data[512];
EVF9_GetProfilerData(data, sizeof(data));
printf("%s", data);
```
