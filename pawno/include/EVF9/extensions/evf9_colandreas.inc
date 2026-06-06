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

#if defined _evf9_colandreas_included
    #endinput
#endif
#define _evf9_colandreas_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_colandreas"
#endif

#if !defined CA_Init
    #error "evf9_colandreas requires ColAndreas plugin. Include <ColAndreas> before this extension."
#endif

forward EVF9_OnVehicleCollision(vehicleid);

native FindSafeVehiclePosition(vehicleid, &Float:x, &Float:y, &Float:z);
native GetVehicleGroundPosition(vehicleid, &Float:z);
native CheckVehicleCollision(vehicleid);
