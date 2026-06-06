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

#if defined _evf9_core_included
    #endinput
#endif
#define _evf9_core_included

/*
 * EVF9 - Extended Vehicle Framework 9
 * Entry point — #include <EVF9> in your gamemode.
 *
 * Usage:
 *   // Optional: override defaults before including
 *   #define EVF9_ENABLE_DEBUG 1
 *   #include <EVF9>
 *
 *   // Optional modules (include individually as needed):
 *   #include <EVF9/modules/evf9_fuel>
 *   #include <EVF9/modules/evf9_ownership>
 *   #include <EVF9/modules/evf9_damage>
 *   #include <EVF9/modules/evf9_keys>
 *   #include <EVF9/modules/evf9_fleet>
 *   #include <EVF9/modules/evf9_tracking>
 *   #include <EVF9/modules/evf9_insurance>
 *
 *   // Optional extensions (persistence, integrations):
 *   #include <EVF9/extensions/evf9_mysql>
 *   #include <EVF9/extensions/evf9_sqlite>
 *   // etc.
 *
 * Plugin initialises automatically when loaded by the server.
 * No OnGameModeInit call required for the core.
 *
 * Layer diagram:
 *   Gamemode (Pawn)
 *       ↓  #include <EVF9>
 *   EVF9.inc  ←  evf9_natives.inc / evf9_events.inc / evf9_flags.inc / evf9_config.inc
 *       ↓  calls natives
 *   EVF9 Plugin (C++)
 *       └── VehicleSystem (authoritative state)
 *       └── EventDispatcher
 *       └── ModuleRegistry
 *       └── VehiclePool
 *       └── VehicleIterator (Linked List — never loops MAX_VEHICLES)
 */

#include <EVF9/evf9_config>
#include <EVF9/evf9_flags>
#include <EVF9/evf9_natives>
#include <EVF9/evf9_events>

#if EVF9_ENABLE_DEBUG
    #pragma message "EVF9: Debug mode enabled"
#endif
