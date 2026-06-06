<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Addons

Addons are Pawn includes installed manually in `addons/`.  
They communicate with EVF9 **exclusively via the public API** — events, natives, metadata, and custom properties.

## Using Official Addons

```pawn
#include <EVF9>
#include <addons/evf9_racing>
#include <addons/evf9_police>
#include <addons/evf9_jobs>

public OnGameModeInit()
{
    EVF9_Racing_Init();
    EVF9_Police_Init();
    EVF9_Jobs_Init();
    return 1;
}
```

## Creating a Custom Addon

```pawn
// addons/my_addon.inc

static g_AddonID = INVALID_ADDON_ID;
static g_PropNOS = -1;

stock MyAddon_Init()
{
    g_AddonID = EVF9_RegisterAddon("my_addon", "1.0.0");
    g_PropNOS  = RegisterVehicleProperty("nos_amount", EVF9_PROP_FLOAT);
}

EVF9_OnVehicleEngineStart(vehicleid, playerid)
{
    new Float:nos;
    GetVehiclePropertyFloat(vehicleid, g_PropNOS, nos);
    if(nos > 0.0) SetVehiclePropertyFloat(vehicleid, g_PropNOS, nos - 5.0);
    return EVF9_CONTINUE;
}
```

See `examples/addon_example.pwn` for a complete template.

## Addon Rules

- Register via `EVF9_RegisterAddon(name, version)` on init
- Use only: events (`EVF9_On*`), public natives, metadata, custom properties
- Never access plugin internals directly
- Never declare state that duplicates plugin state
