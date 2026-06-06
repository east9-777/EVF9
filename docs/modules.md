<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Modules

Modules are optional Pawn includes. Each is independent — no module depends on another.

Include individually:

```pawn
#include <EVF9/modules/evf9_ownership>
```

## Ownership

```pawn
SetVehicleOwner(vehicleid, playerid)
GetVehicleOwner(vehicleid)               // returns playerid or -1
RemoveVehicleOwner(vehicleid)
TransferVehicleOwnership(vehicleid, newownerid)
IsVehicleOwner(vehicleid, playerid)
GetVehicleOwnerName(vehicleid, name[], len)
GetOwnedVehicleCount(playerid)
```

## Fuel

```pawn
SetVehicleFuel(vehicleid, Float:fuel)    // 0.0 – 100.0
GetVehicleFuel(vehicleid, &Float:fuel)
AddVehicleFuel(vehicleid, Float:amount)
RemoveVehicleFuel(vehicleid, Float:amount)
IsVehicleOutOfFuel(vehicleid)
GetFuelConsumptionRate(vehicleid, &Float:rate)
SetFuelConsumptionRate(vehicleid, Float:rate)
```

## Damage

```pawn
RepairVehicleDamage(vehicleid)
DestroyVehiclePart(vehicleid, partid)
RepairVehiclePart(vehicleid, partid)
GetVehicleDamageData(vehicleid, &panels, &doors, &lights, &tires)
ResetVehicleDamage(vehicleid)
```

## Keys

```pawn
GiveVehicleKey(vehicleid, playerid)
RemoveVehicleKey(vehicleid, playerid)
HasVehicleKey(vehicleid, playerid)
LockVehicle(vehicleid)       // fires EVF9_OnVehicleLock [cancelable]
UnlockVehicle(vehicleid)     // fires EVF9_OnVehicleUnlock [cancelable]
```

## Fleet

```pawn
CreateFleet(name[], ownerid)             // returns fleetid
DestroyFleet(fleetid)
AssignVehicleToFleet(vehicleid, fleetid)
RemoveVehicleFromFleet(vehicleid)
GetFleetVehicleCount(fleetid)
GetVehicleFleet(vehicleid)
GetFleetOwner(fleetid)
```

## Tracking

```pawn
TrackVehicle(vehicleid)       // sets VEHICLE_FLAG_TRACKED
UntrackVehicle(vehicleid)     // clears VEHICLE_FLAG_TRACKED
GetVehicleLastPosition(vehicleid, &Float:x, &Float:y, &Float:z)
GetVehicleTravelDistance(vehicleid, &Float:dist)
```

## Insurance

```pawn
InsureVehicle(vehicleid, duration)    // duration in seconds
CancelInsurance(vehicleid)
IsVehicleInsured(vehicleid)
GetInsuranceExpiration(vehicleid)     // Unix timestamp
```
