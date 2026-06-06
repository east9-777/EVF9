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

#if defined _evf9_sqlite_included
    #endinput
#endif
#define _evf9_sqlite_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_sqlite"
#endif

/*
 * Extension: SQLite Persistence
 * Parity with MySQL extension — external handle + auto table creation.
 *
 * Usage:
 *   EVF9_SetSQLiteDB("evf9_vehicles.db");
 *   EVF9_InitializeSQLite();
 */

native EVF9_SetSQLiteDB(const filename[]);
native EVF9_InitializeSQLite();
native EVF9_SaveVehicleSQLite(vehicleid);
native EVF9_LoadVehicleSQLite(vehicleid);
native EVF9_DeleteVehicleSQLite(vehicleid);
