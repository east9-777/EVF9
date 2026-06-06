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
 * Example: Custom Addon
 * Demonstrates how to create a minimal EVF9 addon.
 * The addon registers itself, hooks events, and uses metadata/custom properties.
 */

#include <open.mp>
#include <EVF9>

// ---- Addon registration ----
static g_MyAddonID = INVALID_ADDON_ID;
static g_PropNOS    = -1;
static g_PropTurbo  = -1;

stock MyAddon_Init()
{
    g_MyAddonID = EVF9_RegisterAddon("my_addon", "1.0.0");
    if(g_MyAddonID == INVALID_ADDON_ID) return printf("[MyAddon] Registration failed.");

    // Register custom typed properties
    g_PropNOS   = RegisterVehicleProperty("nos_amount",   EVF9_PROP_FLOAT);
    g_PropTurbo = RegisterVehicleProperty("turbo_active", EVF9_PROP_INT);

    printf("[MyAddon] Initialized. addonid=%d", g_MyAddonID);
}

public OnGameModeInit()
{
    MyAddon_Init();
    new vehicleid = CreateManagedVehicle(411, 0.0, 0.0, 3.0, 0.0, 1, 1);
    SetVehiclePropertyFloat(vehicleid, g_PropNOS, 100.0);
    SetVehiclePropertyInt(vehicleid, g_PropTurbo, 1);
    SetVehicleMeta(vehicleid, META_CUSTOM_1, 999); // store custom game data
    return 1;
}

// Hook events exclusively — no direct plugin access
EVF9_OnVehicleEngineStart(vehicleid, playerid)
{
    new Float:nos;
    GetVehiclePropertyFloat(vehicleid, g_PropNOS, nos);
    if(HasVehicleProperty(vehicleid, g_PropTurbo))
        printf("[MyAddon] NOS ready: %.1f | Turbo: active", nos);
    return EVF9_CONTINUE; // allow engine to start
}
