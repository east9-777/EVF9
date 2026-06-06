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

#if defined _evf9_discord_included
    #endinput
#endif
#define _evf9_discord_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_discord"
#endif

/*
 * Extension: Discord Integration
 * Send vehicle and fleet logs to Discord webhooks.
 * Requires a Discord connector plugin (e.g. discord-connector).
 */

native SendVehicleLog(vehicleid, message[]);
native SendFleetLog(fleetid, message[]);
