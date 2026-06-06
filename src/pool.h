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
 * VehiclePool — lightweight bitset tracking which vehicleids are managed.
 * Complements VehicleSystem: quick O(1) isManaged check without touching
 * the full VehicleEntity struct.
 */
class VehiclePool
{
public:
    static VehiclePool& getInstance();

    bool acquire(int vehicleid);
    bool release(int vehicleid);
    bool has(int vehicleid) const;

private:
    VehiclePool() = default;
    static constexpr int MAX_SIZE = 2000;
    bool m_slots[MAX_SIZE] = {};
};
