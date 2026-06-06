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

#if defined _evf9_fcnpc_included
    #endinput
#endif
#define _evf9_fcnpc_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_fcnpc"
#endif

#if !defined FCNPC_MOVE_TYPE_AUTO
    #error "evf9_fcnpc requires FCNPC plugin. Include <FCNPC> before <EVF9/extensions/evf9_fcnpc>"
#endif

/*
 * Extension: FCNPC Integration
 * Assign NPC drivers and route playback to managed vehicles.
 *
 * Events fired:
 *   EVF9_OnVehicleRouteStart(vehicleid, routeid)
 *   EVF9_OnVehicleRouteComplete(vehicleid, routeid)
 *   EVF9_OnVehicleNPCAssigned(vehicleid, npcid)
 */

forward EVF9_OnVehicleRouteStart(vehicleid, routeid);
forward EVF9_OnVehicleRouteComplete(vehicleid, routeid);
forward EVF9_OnVehicleNPCAssigned(vehicleid, npcid);

native AssignVehicleNPC(vehicleid, npcid);
native RemoveVehicleNPC(vehicleid);
native GetVehicleNPC(vehicleid);
native StartVehicleRoute(vehicleid, routeid);
native StopVehicleRoute(vehicleid);
