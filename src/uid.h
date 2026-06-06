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
#include <cstdint>

/**
 * UID Manager — generates and maps permanent unique IDs.
 * vehicleid is temporary; UID is permanent across respawns.
 */
class UIDManager
{
public:
    static UIDManager& getInstance();

    uint32_t generate();
    int      getVehicleByUID(uint32_t uid) const;

private:
    UIDManager() = default;
};
