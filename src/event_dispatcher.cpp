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

#include "event_dispatcher.h"

AMX* EventDispatcher::s_amx = nullptr;

void EventDispatcher::setAMX(AMX* amx)
{
    s_amx = amx;
}

AMX* EventDispatcher::getAMX()
{
    return s_amx;
}

/*
 * Internal helper: push params right-to-left, exec, return Pawn result.
 * Returns 1 (EVF9_CONTINUE) if the public is not found.
 */
static int exec_event(AMX* amx, const char* name, const std::vector<cell>& args)
{
    if (!amx) return 1;

    int idx;
    if (amx_FindPublic(amx, name, &idx) != AMX_ERR_NONE)
        return 1; /* not found = EVF9_CONTINUE (gamemode hasn't hooked it) */

    /* Push arguments RIGHT to LEFT per AMX convention */
    for (int i = (int)args.size() - 1; i >= 0; --i)
        amx_Push(amx, args[i]);

    cell result = 1; /* default: EVF9_CONTINUE */
    amx_Exec(amx, &result, idx);
    return (int)result;
}

int EventDispatcher::dispatch1(int arg0, const char* event)
{
    return exec_event(s_amx, event, { (cell)arg0 });
}

int EventDispatcher::dispatch2(int arg0, int arg1, const char* event)
{
    return exec_event(s_amx, event, { (cell)arg0, (cell)arg1 });
}

int EventDispatcher::dispatch3(int arg0, int arg1, int arg2, const char* event)
{
    return exec_event(s_amx, event, { (cell)arg0, (cell)arg1, (cell)arg2 });
}

int EventDispatcher::dispatch1f(int arg0, float farg1, const char* event)
{
    cell fc = amx_ftoc(farg1);
    return exec_event(s_amx, event, { (cell)arg0, fc });
}

int EventDispatcher::dispatch3f(int arg0, float fx, float fy, float fz, const char* event)
{
    return exec_event(s_amx, event, {
        (cell)arg0,
        amx_ftoc(fx),
        amx_ftoc(fy),
        amx_ftoc(fz)
    });
}
