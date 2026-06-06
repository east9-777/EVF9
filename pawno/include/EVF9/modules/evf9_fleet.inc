/*
 * EVF9 - Extended Vehicle Framework 9 — Fleet Module
 *
 * Created by EAST9
 * Copyright (c) EAST9. Do not remove this notice.
 */

#if defined _evf9_fleet_included
    #endinput
#endif
#define _evf9_fleet_included

#include <EVF9/evf9_natives>
#include <EVF9/evf9_events>

/*
 * EVF9 Fleet module — interface reference
 *
 * Natives:
 *   CreateFleet(const name[], ownerid)          → fleetid
 *   DestroyFleet(fleetid)
 *   AssignVehicleToFleet(vehicleid, fleetid)
 *   RemoveVehicleFromFleet(vehicleid)
 *   GetFleetVehicleCount(fleetid)
 *   GetVehicleFleet(vehicleid)                  → fleetid or INVALID_FLEET_ID
 *   GetFleetOwner(fleetid)                      → playerid
 *   IsVehicleInFleet(vehicleid)                 → bool
 *
 * Events:
 *   EVF9_OnFleetCreated(fleetid, ownerid)
 *   EVF9_OnFleetDestroyed(fleetid)
 *   EVF9_OnVehicleFleetAssigned(vehicleid, fleetid)
 *   EVF9_OnVehicleFleetRemoved(vehicleid, fleetid)
 *
 * Usage:
 *   new fleetid = CreateFleet("Taxi Co", ownerid);
 *   AssignVehicleToFleet(vehicleid, fleetid);
 *   EVF9_FOREACH_VEHICLE(vid) {
 *       if(GetVehicleFleet(vid) == fleetid) printf("Fleet vehicle: %d", vid);
 *   }
 */
