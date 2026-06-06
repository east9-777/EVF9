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

#if defined _evf9_json_included
    #endinput
#endif
#define _evf9_json_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_json"
#endif

/*
 * Extension: JSON Export/Import
 * Useful for data migration, backups, and external tooling.
 */

native EVF9_ExportVehicleJSON(vehicleid, file[]);
native EVF9_ImportVehicleJSON(vehicleid, file[]);
