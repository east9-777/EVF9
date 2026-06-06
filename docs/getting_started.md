<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Getting Started — Your First Managed Vehicle in 5 Minutes

## Prerequisites

- Open.MP server (or SA-MP 0.3.7)
- Pawn compiler (qawno / pawno)
- EVF9 plugin installed (see [installation.md](installation.md))

## Step 1 — Include EVF9

```pawn
#include <EVF9>
```

That's all you need. Modules are optional.

## Step 2 — Create a Managed Vehicle

```pawn
public OnGameModeInit()
{
    // Create an Infernus at LS coords
    new vehicleid = CreateManagedVehicle(411, 1234.0, -2345.0, 13.0, 90.0, 1, 1);

    if(vehicleid == INVALID_VEHICLE_ID)
    {
        printf("[EVF9] Could not create vehicle — pool may be full.");
        return 1;
    }

    printf("[EVF9] Vehicle created: id=%d uid=%d", vehicleid, EVF9_GetVehicleUID(vehicleid));

    // Attach a human-readable label
    EVF9_SetVehicleTag(vehicleid, "Infernus #1");

    return 1;
}
```

## Step 3 — Iterate Managed Vehicles

```pawn
EVF9_FOREACH_VEHICLE(vid)
{
    new tag[65];
    EVF9_GetVehicleTag(vid, tag, sizeof(tag));
    printf("Vehicle %d | Model %d | Tag: %s", vid, EVF9_GetVehicleModel(vid), tag);
}
```

Never use `for(new i = 0; i < MAX_VEHICLES; i++)` — use the macro above.

## Step 4 — Listen to Events

```pawn
EVF9_OnVehicleEngineStart(vehicleid, playerid)
{
    new Float:fuel;
    GetVehicleFuel(vehicleid, fuel);       // requires evf9_fuel module
    if(fuel <= 0.0) return EVF9_BREAK;     // cancel engine start
    return EVF9_CONTINUE;
}
```

## Step 5 — Use UIDs for Persistence

```pawn
// Save the UID — not vehicleid — to your database
new uid = EVF9_GetVehicleUID(vehicleid);

// Later, find the vehicle by UID (safe after respawn/restart)
new vid = EVF9_GetVehicleByUID(uid);
```

## Next Steps

- [installation.md](installation.md) — full install guide
- [api_reference.md](api_reference.md) — all natives
- [modules.md](modules.md) — fuel, ownership, damage, fleet, tracking, insurance
- [events.md](events.md) — full event list
