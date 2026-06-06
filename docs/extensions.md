<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Extensions

Extensions are optional persistence and integration bridges.  
Each extension checks with `#if defined` for its dependency before compiling.

## MySQL

```pawn
#include <a_mysql>
#include <EVF9>
#include <EVF9/extensions/evf9_mysql>

EVF9_SetMySQLHandle(g_SQL);     // provide connection — EVF9 never opens its own
EVF9_InitializeMySQL();         // CREATE TABLE IF NOT EXISTS + migrations

EVF9_SaveVehicleMySQL(vehicleid);
EVF9_LoadVehicleMySQL(vehicleid);
EVF9_DeleteVehicleMySQL(vehicleid);
```

**Schema versioning:** EVF9 maintains an internal `evf9_schema` table. Migrations run automatically on `EVF9_InitializeMySQL()` — never manual SQL after updates.

## SQLite

```pawn
#include <EVF9/extensions/evf9_sqlite>

EVF9_SetSQLiteDB("evf9_vehicles.db");
EVF9_InitializeSQLite();

EVF9_SaveVehicleSQLite(vehicleid);
EVF9_LoadVehicleSQLite(vehicleid);
```

## JSON

```pawn
#include <EVF9/extensions/evf9_json>

EVF9_ExportVehicleJSON(vehicleid, "backups/vehicle_1234.json");
EVF9_ImportVehicleJSON(vehicleid, "backups/vehicle_1234.json");
```

## Streamer

```pawn
#include <streamer>
#include <EVF9/extensions/evf9_streamer>

AttachDynamicObjectToVehicleEx(vehicleid, modelid, ox, oy, oz);
DetachDynamicObjectFromVehicle(vehicleid, objectid);
DestroyAttachedVehicleObjects(vehicleid);
```

## Discord / HTTP

```pawn
#include <EVF9/extensions/evf9_discord>
SendVehicleLog(vehicleid, "Engine started");

#include <EVF9/extensions/evf9_http>
SendVehicleWebhook(vehicleid, "https://your.webhook/url");
```
