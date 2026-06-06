<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Events Reference

Return `EVF9_CONTINUE` (1) to allow, `EVF9_BREAK` (0) to cancel (cancelable events only).

## Lifecycle

| Event | Cancelable |
|---|---|
| `EVF9_OnVehicleCreate(vehicleid)` | No |
| `EVF9_OnVehicleDestroy(vehicleid)` | No |
| `EVF9_OnVehicleSpawn(vehicleid)` | No |
| `EVF9_OnVehicleRespawn(vehicleid)` | No |
| `EVF9_OnVehicleDespawn(vehicleid)` | No |
| `EVF9_OnVehicleClone(vehicleid, newvehicleid)` | No |
| `EVF9_OnVehicleAbandoned(vehicleid, playerid)` | No |
| `EVF9_OnVehicleRecovered(vehicleid, playerid)` | No |

> **Distinction:** `EVF9_OnVehicleDestroy` = removed from framework. `EVF9_OnVehicleDestroyed` = exploded in game.

## Engine

| Event | Cancelable |
|---|---|
| `EVF9_OnVehicleEngineStart(vehicleid, playerid)` | **Yes** |
| `EVF9_OnVehicleEngineStop(vehicleid, playerid)` | **Yes** |
| `EVF9_OnVehicleEngineFail(vehicleid)` | No |
| `EVF9_OnVehicleEngineDamage(vehicleid, Float:amount)` | No |
| `EVF9_OnVehicleEngineRepair(vehicleid, playerid)` | No |

## Fuel

| Event | Description |
|---|---|
| `EVF9_OnVehicleFuelChanged(vehicleid, Float:old, Float:new)` | Any fuel change |
| `EVF9_OnVehicleFuelLow(vehicleid, Float:fuel)` | Fired at ≤ 10.0 |
| `EVF9_OnVehicleFuelEmpty(vehicleid)` | Fired at 0.0 |
| `EVF9_OnVehicleRefuel(vehicleid, playerid, Float:amount)` | After refuel |

## Damage

| Event | Cancelable |
|---|---|
| `EVF9_OnVehicleDamage(vehicleid, Float:amount)` | No |
| `EVF9_OnVehicleRepair(vehicleid, playerid)` | No |
| `EVF9_OnVehicleExplode(vehicleid)` | No |
| `EVF9_OnVehicleFlip(vehicleid)` | No |
| `EVF9_OnVehiclePartDamaged(vehicleid, partid)` | No |
| `EVF9_OnVehiclePartRepaired(vehicleid, partid, playerid)` | No |
| `EVF9_OnVehicleDestroyed(vehicleid, killerid)` | No |

## Security

| Event | Cancelable |
|---|---|
| `EVF9_OnVehicleLock(vehicleid, playerid)` | **Yes** |
| `EVF9_OnVehicleUnlock(vehicleid, playerid)` | **Yes** |
| `EVF9_OnVehicleKeyGiven(vehicleid, playerid, targetid)` | No |
| `EVF9_OnVehicleKeyRemoved(vehicleid, playerid, targetid)` | No |
| `EVF9_OnVehicleTheftAttempt(vehicleid, playerid)` | **Yes** |

## Ownership, Tracking, Fleet, Persistence

See `pawno/include/EVF9/evf9_events.inc` for the complete list.

## Example

```pawn
EVF9_OnVehicleEngineStart(vehicleid, playerid)
{
    new Float:fuel;
    GetVehicleFuel(vehicleid, fuel);
    if(fuel <= 0.0)
    {
        SendClientMessage(playerid, -1, "Sem combustível.");
        return EVF9_BREAK;
    }
    return EVF9_CONTINUE;
}
```
