/*
 * EVF9 - Extended Vehicle Framework 9 — Keys & Lock Module
 *
 * Created by EAST9
 * Copyright (c) EAST9. Do not remove this notice.
 */

#if defined _evf9_keys_included
    #endinput
#endif
#define _evf9_keys_included

#include <EVF9/evf9_natives>
#include <EVF9/evf9_events>

/*
 * EVF9 Keys & Lock module — interface reference
 *
 * Natives:
 *   GiveVehicleKey(vehicleid, playerid)
 *   RemoveVehicleKey(vehicleid, playerid)
 *   HasVehicleKey(vehicleid, playerid)      → bool (owner always has key)
 *   LockVehicle(vehicleid, playerid = -1)
 *   UnlockVehicle(vehicleid, playerid = -1)
 *   GetVehicleKeyCount(vehicleid)
 *   ClearVehicleKeys(vehicleid)
 *
 * Events:
 *   EVF9_OnVehicleLock(vehicleid, playerid)          [cancelable]
 *   EVF9_OnVehicleUnlock(vehicleid, playerid)        [cancelable]
 *   EVF9_OnVehicleKeyGiven(vehicleid, playerid, targetid)
 *   EVF9_OnVehicleKeyRemoved(vehicleid, playerid, targetid)
 *   EVF9_OnVehicleTheftAttempt(vehicleid, playerid)  [cancelable]
 *
 * Theft event must be fired by the gamemode (C++ cannot detect it automatically):
 *   CallLocalFunction("EVF9_OnVehicleTheftAttempt", "ii", vehicleid, playerid);
 */
