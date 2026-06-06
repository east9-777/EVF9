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

#if defined _evf9_mysql_included
    #endinput
#endif
#define _evf9_mysql_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_mysql"
#endif

#if !defined mysql_included
    #error "evf9_mysql requires a_mysql plugin. Include <a_mysql> before <EVF9/extensions/evf9_mysql>"
#endif

/*
 * Extension: MySQL Persistence
 *
 * EVF9 NEVER opens its own database connection.
 * The gamemode owns the connection; pass the handle via EVF9_SetMySQLHandle().
 *
 * Usage:
 *   #include <a_mysql>
 *   #include <EVF9>
 *   #include <EVF9/extensions/evf9_mysql>
 *
 *   new MySQL:g_SQL;
 *
 *   public OnGameModeInit() {
 *       g_SQL = mysql_connect(...);
 *       EVF9_SetMySQLHandle(g_SQL);    // required before any MySQL EVF9 op
 *       EVF9_InitializeMySQL();        // creates/migrates tables automatically
 *       return 1;
 *   }
 *
 * Schema version table (internal):
 *   CREATE TABLE IF NOT EXISTS evf9_schema (version INT NOT NULL DEFAULT 0);
 *
 * Migrations are idempotent (ADD COLUMN IF NOT EXISTS). Never destructive.
 * Existing servers upgrade schema automatically on EVF9 version update.
 */

/**
 * Function:    EVF9_SetMySQLHandle(MySQL:handle)
 * Description: Provides the MySQL connection handle to the EVF9 extension.
 *              Must be called before EVF9_InitializeMySQL.
 * Returns:     1 on success
 */
native EVF9_SetMySQLHandle(MySQL:handle);

/**
 * Function:    EVF9_InitializeMySQL()
 * Description: Creates all required tables (CREATE TABLE IF NOT EXISTS)
 *              and runs schema migrations if the DB is behind current version.
 * Returns:     1 on success
 */
native EVF9_InitializeMySQL();

/**
 * Function:    EVF9_SaveVehicleMySQL(vehicleid)
 * Description: Persists the vehicle entity to MySQL.
 *              Calls EVF9_BeginSave internally. Call EVF9_EndSave in your query callback.
 * Returns:     1 on success
 * Notes:       Listen for EVF9_OnVehicleDirty to schedule saves.
 *              Debounce saves (e.g. 5s delay) to avoid per-field queries.
 */
native EVF9_SaveVehicleMySQL(vehicleid);

/**
 * Function:    EVF9_LoadVehicleMySQL(vehicleid)
 * Description: Loads vehicle data from MySQL into the plugin state.
 * Returns:     1 on success
 */
native EVF9_LoadVehicleMySQL(vehicleid);

/**
 * Function:    EVF9_DeleteVehicleMySQL(vehicleid)
 * Description: Deletes the vehicle record from MySQL permanently.
 * Returns:     1 on success
 */
native EVF9_DeleteVehicleMySQL(vehicleid);
