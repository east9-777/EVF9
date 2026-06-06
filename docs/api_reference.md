<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# API Reference

## Core

| Native | Description |
|---|---|
| `EVF9_GetVersion()` | Returns framework version as integer (e.g. 100 = v1.0.0) |
| `EVF9_GetBuild()` | Returns build number |

## Pool

| Native | Description |
|---|---|
| `CreateManagedVehicle(modelid, x, y, z, angle, c1, c2)` | Creates vehicle, assigns UID, adds to pool |
| `CreateManagedVehicleEx(...)` | Like above + worldid, interiorid |
| `DestroyManagedVehicle(vehicleid)` | Removes vehicle from pool (deferred if SAVING) |
| `RespawnManagedVehicle(vehicleid)` | Respawns vehicle at spawn position |
| `CloneManagedVehicle(vehicleid)` | Creates copy of vehicle |
| `IsManagedVehicle(vehicleid)` | Returns 1 if in EVF9 pool |
| `CountManagedVehicles()` | Returns count of managed vehicles |

## Entity

| Native | Description |
|---|---|
| `EVF9_GetVehicleModel(vehicleid)` | Model ID |
| `EVF9_GetVehicleSpawnPos(vehicleid, &x, &y, &z, &angle)` | Spawn position |
| `EVF9_GetVehicleWorld(vehicleid)` | Virtual world |
| `EVF9_GetVehicleInterior(vehicleid)` | Interior ID |
| `EVF9_GetVehicleFlags(vehicleid)` | Full flag bitmask |
| `EVF9_GetVehicleHealth(vehicleid, &Float:hp)` | Current HP |
| `EVF9_SetVehicleHealth(vehicleid, Float:hp)` | Set HP + sync to server |
| `EVF9_SetVehicleTag(vehicleid, tag[])` | Set label (max 64 chars) |
| `EVF9_GetVehicleTag(vehicleid, tag[], len)` | Get label |
| `EVF9_GetVehicleUID(vehicleid)` | Permanent UID |
| `EVF9_GetVehicleByUID(uid)` | Find vehicleid by UID |
| `EVF9_IsVehicleSpawned(vehicleid)` | 1 if spawned |
| `EVF9_IsVehicleManaged(vehicleid)` | 1 if managed |

## Flags

| Native | Description |
|---|---|
| `EVF9_SetVehicleFlag(vehicleid, flag)` | Set a flag |
| `EVF9_ClearVehicleFlag(vehicleid, flag)` | Clear a flag |
| `EVF9_HasVehicleFlag(vehicleid, flag)` | Check a flag |

## Iterator

```pawn
// Macro — expands to linked list traversal
EVF9_FOREACH_VEHICLE(vid) { ... }

// Raw natives (used by macro)
EVF9_GetFirstVehicle()
EVF9_GetNextVehicle(vehicleid)
```

## Persistence State

| Native | Description |
|---|---|
| `EVF9_BeginSave(vehicleid)` | Mark as SAVING |
| `EVF9_EndSave(vehicleid)` | Mark as CLEAN |
| `EVF9_IsVehicleSaving(vehicleid)` | 1 if SAVING |
| `EVF9_GetVehicleSaveState(vehicleid)` | Returns EVF9_SAVE_STATE enum |

## Metadata

```pawn
SetVehicleMeta(vehicleid, EVF9_METADATA:key, value)
GetVehicleMeta(vehicleid, EVF9_METADATA:key)
ResetVehicleMeta(vehicleid, EVF9_METADATA:key)
HasVehicleMeta(vehicleid, EVF9_METADATA:key)
```

Keys: `META_JOB`, `META_COMPANY`, `META_GROUP`, `META_EVENT`, `META_CUSTOM_1/2/3`

## Custom Properties

```pawn
RegisterVehicleProperty(name[], EVF9_PROPERTY_TYPE:type)  // returns propertyid
SetVehiclePropertyInt(vehicleid, propertyid, value)
SetVehiclePropertyFloat(vehicleid, propertyid, Float:value)
SetVehiclePropertyString(vehicleid, propertyid, value[])
GetVehiclePropertyInt(vehicleid, propertyid)
GetVehiclePropertyFloat(vehicleid, propertyid, &Float:value)
GetVehiclePropertyString(vehicleid, propertyid, value[], len)
HasVehicleProperty(vehicleid, propertyid)
```

Full native list: see `pawno/include/EVF9/evf9_natives.inc`
