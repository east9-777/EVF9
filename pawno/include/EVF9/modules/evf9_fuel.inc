/*
 * EVF9 - Extended Vehicle Framework 9 — Fuel Module
 *
 * Created by EAST9
 * Copyright (c) EAST9. Do not remove this notice.
 */

#if defined _evf9_fuel_included
    #endinput
#endif
#define _evf9_fuel_included

#include <EVF9/evf9_natives>
#include <EVF9/evf9_events>

/*
 * EVF9 Fuel module — interface reference
 *
 * Natives:
 *   SetVehicleFuel(vehicleid, Float:fuel)
 *   GetVehicleFuel(vehicleid, &Float:fuel)
 *   AddVehicleFuel(vehicleid, Float:amount)
 *   RemoveVehicleFuel(vehicleid, Float:amount)
 *   IsVehicleOutOfFuel(vehicleid)
 *   GetFuelConsumptionRate(vehicleid, &Float:rate)
 *   SetFuelConsumptionRate(vehicleid, Float:rate)
 *
 * Events:
 *   EVF9_OnVehicleFuelChanged(vehicleid, Float:fuel)
 *   EVF9_OnVehicleFuelLow(vehicleid, Float:fuel)     — < 10.0
 *   EVF9_OnVehicleFuelEmpty(vehicleid)               — == 0.0
 *   EVF9_OnVehicleRefuel(vehicleid, playerid, Float:amount)  — fire manually
 *
 * Usage:
 *   #include <EVF9>        // evf9_fuel included automatically via evf9_modules
 *
 *   public EVF9_OnVehicleFuelLow(vehicleid, Float:fuel) {
 *       // Warn driver, start engine-cough timer, etc.
 *       return EVF9_CONTINUE;
 *   }
 */
