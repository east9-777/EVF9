/*
 * EVF9 - Extended Vehicle Framework 9 — Ownership Module
 *
 * Created by EAST9
 * Copyright (c) EAST9. Do not remove this notice.
 */

#if defined _evf9_ownership_included
    #endinput
#endif
#define _evf9_ownership_included

#include <EVF9/evf9_natives>
#include <EVF9/evf9_events>

/*
 * EVF9 Ownership module — interface reference
 *
 * Natives:
 *   SetVehicleOwner(vehicleid, playerid)
 *   GetVehicleOwner(vehicleid)              → playerid or -1
 *   GetVehicleOwnerName(vehicleid, name[], len)
 *   RemoveVehicleOwner(vehicleid)
 *   TransferVehicleOwnership(vehicleid, newownerid)
 *   IsVehicleOwner(vehicleid, playerid)     → bool
 *   GetOwnedVehicleCount(playerid)
 *
 * Events:
 *   EVF9_OnVehicleOwnerAssigned(vehicleid, playerid)
 *   EVF9_OnVehicleOwnerRemoved(vehicleid, playerid)
 *   EVF9_OnVehicleOwnerChange(vehicleid, oldowner, newowner)
 *   EVF9_OnVehicleSold(vehicleid, sellerid, buyerid, price)    — fire manually
 *   EVF9_OnVehiclePurchased(vehicleid, buyerid, price)         — fire manually
 */
