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

#pragma once

/**
 * Persistence state constants and helpers.
 * States per vehicle: CLEAN -> DIRTY -> SAVING -> CLEAN
 * Plugin never permanently destroys a vehicle in SAVING state.
 */

enum class PersistenceState : uint8_t
{
    CLEAN  = 0,
    DIRTY  = 1,
    SAVING = 2
};
