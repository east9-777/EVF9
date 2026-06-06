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

#if defined _evf9_events_included
    #endinput
#endif
#define _evf9_events_included

/*
 * EVF9 Event System (ALS — Advanced Library System)
 * Intercepts callbacks natively without external dependencies.
 *
 * Return EVF9_CONTINUE to allow action, EVF9_BREAK to cancel (cancelable events only).
 *
 * Distinction:
 *   EVF9_OnVehicleDestroy(vehicleid)              — vehicle removed from framework
 *   EVF9_OnVehicleDestroyed(vehicleid, killerid)  — vehicle exploded/destroyed in game
 */

#define EVF9_CONTINUE (1)
#define EVF9_BREAK    (0)

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleCreate(vehicleid);
forward EVF9_OnVehicleDestroy(vehicleid);
forward EVF9_OnVehicleSpawn(vehicleid);
forward EVF9_OnVehicleRespawn(vehicleid);
forward EVF9_OnVehicleDespawn(vehicleid);
forward EVF9_OnVehicleClone(vehicleid, newvehicleid);
forward EVF9_OnVehicleRecover(vehicleid);
forward EVF9_OnVehicleAbandoned(vehicleid, playerid);
forward EVF9_OnVehicleRecovered(vehicleid, playerid);

// ---------------------------------------------------------------------------
// Occupancy
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleEnter(vehicleid, playerid, seatid);
forward EVF9_OnVehicleExit(vehicleid, playerid);
forward EVF9_OnVehicleSeatChange(vehicleid, playerid, oldseat, newseat);
forward EVF9_OnVehicleDriverChange(vehicleid, olddriver, newdriver);
forward EVF9_OnVehiclePassengerAdded(vehicleid, playerid, seatid);
forward EVF9_OnVehiclePassengerRemoved(vehicleid, playerid);

// ---------------------------------------------------------------------------
// Engine [cancelable]
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleEngineStart(vehicleid, playerid);    // [cancelable]
forward EVF9_OnVehicleEngineStop(vehicleid, playerid);     // [cancelable]
forward EVF9_OnVehicleEngineFail(vehicleid);
forward EVF9_OnVehicleEngineDamage(vehicleid, Float:amount);
forward EVF9_OnVehicleEngineRepair(vehicleid, playerid);

// ---------------------------------------------------------------------------
// Fuel
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleFuelChanged(vehicleid, Float:oldfuel, Float:newfuel);
forward EVF9_OnVehicleFuelLow(vehicleid, Float:fuel);
forward EVF9_OnVehicleFuelEmpty(vehicleid);
forward EVF9_OnVehicleRefuel(vehicleid, playerid, Float:amount);

// ---------------------------------------------------------------------------
// Damage
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleDamage(vehicleid, Float:amount);
forward EVF9_OnVehicleRepair(vehicleid, playerid);
forward EVF9_OnVehicleExplode(vehicleid);
forward EVF9_OnVehicleFlip(vehicleid);
forward EVF9_OnVehiclePartDamaged(vehicleid, partid);
forward EVF9_OnVehiclePartRepaired(vehicleid, partid, playerid);
forward EVF9_OnVehicleDestroyed(vehicleid, killerid);

// ---------------------------------------------------------------------------
// Security [cancelable]
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleLock(vehicleid, playerid);           // [cancelable]
forward EVF9_OnVehicleUnlock(vehicleid, playerid);         // [cancelable]
forward EVF9_OnVehicleKeyGiven(vehicleid, playerid, targetid);
forward EVF9_OnVehicleKeyRemoved(vehicleid, playerid, targetid);
forward EVF9_OnVehicleTheftAttempt(vehicleid, playerid);   // [cancelable]

// ---------------------------------------------------------------------------
// Ownership
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleOwnerAssigned(vehicleid, playerid);
forward EVF9_OnVehicleOwnerRemoved(vehicleid, playerid);
forward EVF9_OnVehicleOwnerChange(vehicleid, oldowner, newowner);
forward EVF9_OnVehicleSold(vehicleid, sellerid, buyerid, price);
forward EVF9_OnVehiclePurchased(vehicleid, buyerid, price);

// ---------------------------------------------------------------------------
// Tracking
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleTrackStart(vehicleid, playerid);
forward EVF9_OnVehicleTrackStop(vehicleid, playerid);
forward EVF9_OnVehicleLocationUpdate(vehicleid, Float:x, Float:y, Float:z);

// ---------------------------------------------------------------------------
// Fleet
// ---------------------------------------------------------------------------

forward EVF9_OnFleetCreated(fleetid, ownerid);
forward EVF9_OnFleetDestroyed(fleetid);
forward EVF9_OnVehicleFleetAssigned(vehicleid, fleetid);
forward EVF9_OnVehicleFleetRemoved(vehicleid, fleetid);

// ---------------------------------------------------------------------------
// Persistence
// ---------------------------------------------------------------------------

forward EVF9_OnVehicleDirty(vehicleid);
forward EVF9_OnVehicleSave(vehicleid);
forward EVF9_OnVehicleSaved(vehicleid);
forward EVF9_OnVehicleLoad(vehicleid);
forward EVF9_OnVehicleDelete(vehicleid);
forward EVF9_OnVehicleDataReset(vehicleid);

// ---------------------------------------------------------------------------
// ALS Hook macros — used internally by modules
// ---------------------------------------------------------------------------

#define ALS_R_EVF9_EngineStart  \
    EVF9_OnVehicleEngineStart(vehicleid, playerid)

#define ALS_R_EVF9_EngineStop   \
    EVF9_OnVehicleEngineStop(vehicleid, playerid)

#define ALS_R_EVF9_Lock         \
    EVF9_OnVehicleLock(vehicleid, playerid)

#define ALS_R_EVF9_Unlock       \
    EVF9_OnVehicleUnlock(vehicleid, playerid)

#define ALS_R_EVF9_TheftAttempt \
    EVF9_OnVehicleTheftAttempt(vehicleid, playerid)
