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
 * Example: SQLite Persistence
 * Demonstrates saving/loading vehicles with SQLite.
 */

#include <open.mp>
#include <EVF9>
#include <EVF9/extensions/evf9_sqlite>

public OnGameModeInit()
{
    EVF9_SetSQLiteDB("evf9_vehicles.db");
    EVF9_InitializeSQLite();
    return 1;
}

EVF9_OnVehicleDirty(vehicleid)
{
    EVF9_SaveVehicleSQLite(vehicleid);
    return EVF9_CONTINUE;
}
