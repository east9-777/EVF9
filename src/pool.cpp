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

#include "pool.h"

VehiclePool& VehiclePool::getInstance()
{
    static VehiclePool instance;
    return instance;
}

bool VehiclePool::acquire(int vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_SIZE) return false;
    m_slots[vehicleid] = true;
    return true;
}

bool VehiclePool::release(int vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_SIZE) return false;
    m_slots[vehicleid] = false;
    return true;
}

bool VehiclePool::has(int vehicleid) const
{
    if (vehicleid < 0 || vehicleid >= MAX_SIZE) return false;
    return m_slots[vehicleid];
}
