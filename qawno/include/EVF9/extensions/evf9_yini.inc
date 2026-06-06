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

#if defined _evf9_yini_included
    #endinput
#endif
#define _evf9_yini_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including EVF9/extensions/evf9_yini"
#endif

#if !defined _INC_y_ini
    #error "evf9_yini requires YSI y_ini. Include y_ini before <EVF9/extensions/evf9_yini>"
#endif

/*
 * Extension: Y_INI Persistence
 * Suitable for simple servers or configuration-based storage.
 */

native EVF9_SaveVehicleINI(vehicleid, file[]);
native EVF9_LoadVehicleINI(vehicleid, file[]);
