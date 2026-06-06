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

#if defined _evf9_config_included
    #endinput
#endif
#define _evf9_config_included

/*
 * Global configuration for EVF9.
 * Define any of these BEFORE #include <EVF9> to override defaults.
 *
 * Example:
 *   #define EVF9_ENABLE_DEBUG 1
 *   #include <EVF9>
 */

#if !defined EVF9_MAX_VEHICLES
    #define EVF9_MAX_VEHICLES        MAX_VEHICLES
#endif

#if !defined EVF9_ENABLE_DEBUG
    #define EVF9_ENABLE_DEBUG        0   // 1 = active, 0 = disabled (use 1 or 0, never true/false)
#endif

#if !defined EVF9_ENABLE_EVENTS
    #define EVF9_ENABLE_EVENTS       1
#endif

#if !defined EVF9_ENABLE_POOLING
    #define EVF9_ENABLE_POOLING      1
#endif

#if !defined EVF9_ENABLE_PROFILER
    #define EVF9_ENABLE_PROFILER     0
#endif

#if !defined EVF9_ENABLE_MEMORY_STATS
    #define EVF9_ENABLE_MEMORY_STATS 0
#endif

#if !defined EVF9_CACHE_INTERVAL
    #define EVF9_CACHE_INTERVAL      500 // ms — internal cache update interval
#endif
