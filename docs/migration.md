<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Migration Guide

## From raw SA-MP vehicle management

| Before | After |
|---|---|
| `CreateVehicle(...)` | `CreateManagedVehicle(...)` |
| `DestroyVehicle(vehicleid)` | `DestroyManagedVehicle(vehicleid)` |
| `for(i = 0; i < MAX_VEHICLES; i++)` | `EVF9_FOREACH_VEHICLE(vid)` |
| `vehicleid` as DB key | `EVF9_GetVehicleUID(vehicleid)` as DB key |
| Custom HP array | `EVF9_SetVehicleHealth / EVF9_GetVehicleHealth` |

## Key principle

**Never use `vehicleid` as a database primary key.**  
Use `EVF9_GetVehicleUID(vehicleid)` — UIDs survive respawns, server restarts, and slot reassignments.

## MySQL Schema Migration

When upgrading EVF9 versions: call `EVF9_InitializeMySQL()` on startup.  
Migrations run automatically. No manual SQL needed.

```pawn
public OnGameModeInit()
{
    EVF9_SetMySQLHandle(g_SQL);
    EVF9_InitializeMySQL();  // auto-migrates on version change
    return 1;
}
```
