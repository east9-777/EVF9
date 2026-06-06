/*
 * EVF9 - Extended Vehicle Framework 9 — Tracking Module
 *
 * Created by EAST9
 * Copyright (c) EAST9. Do not remove this notice.
 */

#if defined _evf9_tracking_included
    #endinput
#endif
#define _evf9_tracking_included

#include <EVF9/evf9_natives>
#include <EVF9/evf9_events>

/*
 * EVF9 Tracking module — interface reference
 *
 * Tracking uses the position CACHE maintained by EVF9_UpdateVehiclePosition.
 * Call it from OnPlayerStateChange / a timer to keep positions current.
 *
 * Natives:
 *   TrackVehicle(vehicleid, playerid = -1)
 *   UntrackVehicle(vehicleid, playerid = -1)
 *   GetVehicleLastPosition(vehicleid, &Float:x, &Float:y, &Float:z)
 *   GetVehicleTravelDistance(vehicleid, &Float:dist)   → lifetime km
 *   GetVehicleDistanceFromPoint(vehicleid, Float:x, Float:y, Float:z, &Float:dist)
 *   EVF9_UpdateVehiclePosition(vehicleid, Float:x, Float:y, Float:z)
 *   ResetVehicleDistance(vehicleid)                    → resets trip counter
 *   GetVehicleTripDistance(vehicleid, &Float:dist)
 *   GetVehicleLifetimeDistance(vehicleid, &Float:dist)
 *
 * Events:
 *   EVF9_OnVehicleTrackStart(vehicleid, playerid)
 *   EVF9_OnVehicleTrackStop(vehicleid, playerid)
 *   EVF9_OnVehicleLocationUpdate(vehicleid, Float:x, Float:y, Float:z)
 *     — fired automatically by EVF9_UpdateVehiclePosition when tracked
 */
