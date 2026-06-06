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

#include "uid.h"
#include "vehicle_system.h"

UIDManager& UIDManager::getInstance()
{
    static UIDManager instance;
    return instance;
}

uint32_t UIDManager::generate()
{
    return VehicleSystem::getInstance().generateUID();
}

int UIDManager::getVehicleByUID(uint32_t uid) const
{
    return VehicleSystem::getInstance().getByUID(uid);
}
