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
 * LinkedList-based vehicle iterator.
 * Wraps VehicleSystem linked list traversal for Pawn-accessible iteration.
 * O(n) only over managed vehicles — never loops MAX_VEHICLES.
 */
class VehicleIterator
{
public:
    static int getFirst();
    static int getNext(int vehicleid);
};
