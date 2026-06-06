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

#if defined _evf9_streamer_included
    #endinput
#endif
#define _evf9_streamer_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_streamer"
#endif

#if !defined STREAMER_TYPE_OBJECT
    #error "evf9_streamer requires streamer plugin. Include <streamer> before <EVF9/extensions/evf9_streamer>"
#endif

/*
 * Extension: Streamer Integration
 * Attach/detach dynamic objects to managed vehicles.
 *
 * Events fired:
 *   EVF9_OnVehicleObjectAttach(vehicleid, objectid)
 *   EVF9_OnVehicleObjectDetach(vehicleid, objectid)
 */

forward EVF9_OnVehicleObjectAttach(vehicleid, objectid);
forward EVF9_OnVehicleObjectDetach(vehicleid, objectid);

native AttachDynamicObjectToVehicleEx(vehicleid, modelid, Float:ox, Float:oy, Float:oz);
native DetachDynamicObjectFromVehicle(vehicleid, objectid);
native DestroyAttachedVehicleObjects(vehicleid);
native GetAttachedVehicleObjects(vehicleid, objects[], len);
