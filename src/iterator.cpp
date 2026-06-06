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

#include "iterator.h"
#include "vehicle_system.h"

int VehicleIterator::getFirst()
{
    return VehicleSystem::getInstance().getFirst();
}

int VehicleIterator::getNext(int vehicleid)
{
    return VehicleSystem::getInstance().getNext(vehicleid);
}
