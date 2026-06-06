/*
 * EVF9 - Extended Vehicle Framework 9 — Damage Module
 *
 * Created by EAST9
 * Copyright (c) EAST9. Do not remove this notice.
 */

#if defined _evf9_damage_included
    #endinput
#endif
#define _evf9_damage_included

#include <EVF9/evf9_natives>
#include <EVF9/evf9_events>

/*
 * EVF9 Damage module — interface reference
 *
 * Natives:
 *   RepairVehicleDamage(vehicleid, playerid = -1)
 *   DestroyVehiclePart(vehicleid, partid)
 *   RepairVehiclePart(vehicleid, partid, playerid = -1)
 *   GetVehicleDamageData(vehicleid, &panels, &doors, &lights, &tires)
 *     — calls GetVehicleDamageStatus internally; returns live server data
 *   ResetVehicleDamage(vehicleid)
 *
 * Events (fire manually from the game hooks you know best):
 *   EVF9_OnVehicleDamage(vehicleid, Float:amount)
 *   EVF9_OnVehicleRepair(vehicleid, playerid)
 *   EVF9_OnVehicleExplode(vehicleid)
 *   EVF9_OnVehicleFlip(vehicleid)
 *   EVF9_OnVehiclePartDamaged(vehicleid, partid)
 *   EVF9_OnVehiclePartRepaired(vehicleid, partid, playerid)
 *   EVF9_OnVehicleDestroyed(vehicleid, killerid)
 */
