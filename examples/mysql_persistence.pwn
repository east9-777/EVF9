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
 * Example: MySQL Persistence
 * Demonstrates saving and loading vehicles from MySQL.
 *
 * Requires: a_mysql plugin (BlueG or pBlueG fork)
 */

#include <a_samp>
#include <a_mysql>
#include <EVF9>
#include <EVF9/modules/evf9_ownership>
#include <EVF9/modules/evf9_fuel>
#include <EVF9/extensions/evf9_mysql>

new MySQL:g_SQL;

public OnGameModeInit()
{
    // Connect to MySQL — EVF9 never opens its own connection
    g_SQL = mysql_connect("127.0.0.1", "root", "evf9_db", "password");

    // Provide the handle to EVF9 — required before any MySQL operation
    EVF9_SetMySQLHandle(g_SQL);

    // Create/migrate tables automatically (idempotent, safe on existing DBs)
    EVF9_InitializeMySQL();

    return 1;
}

// Save vehicle on dirty event (debounce: save 5s after last change)
EVF9_OnVehicleDirty(vehicleid)
{
    // In production, debounce with SetTimerEx to avoid per-field saves
    if(!EVF9_IsVehicleSaving(vehicleid))
        EVF9_SaveVehicleMySQL(vehicleid);
    return EVF9_CONTINUE;
}

// Mark save complete in your MySQL query callback
public OnVehicleSaveResult(vehicleid)
{
    EVF9_EndSave(vehicleid);
}
