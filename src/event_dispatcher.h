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

/*
 * event_dispatcher.h — Calls Pawn public callbacks from C++.
 *
 * ARCHITECTURE (Open.MP modern)
 * ==============================
 * Open.MP supports multiple simultaneously-loaded Pawn scripts (gamemode +
 * zero or more filterscripts).  Each script has its own IPawnScript instance.
 *
 * The component's onAmxLoad / onAmxUnload methods call addScript / removeScript
 * so every script receives EVF9 events.  Events are fired on ALL loaded scripts;
 * if ANY returns EVF9_BREAK (0), the result is 0.
 *
 * Pawn callbacks are invoked via IPawnScript::Call() — the SDK's built-in
 * wrapper that handles FindPublic + argument push + Exec in one call.
 * This removes all direct dependencies on the raw AMX C API (amx_FindPublic,
 * amx_Push, amx_Exec) from the event dispatcher.
 *
 * Return values from Pawn:
 *   EVF9_CONTINUE (1) — allow action
 *   EVF9_BREAK    (0) — cancel action (cancelable events only)
 */

#include <Server/Components/Pawn/pawn.hpp>
#include <unordered_set>

class EventDispatcher
{
public:
    /*
     * Called from EVF9Component::onAmxLoad.
     * Registers a script with the dispatcher so it receives callbacks.
     */
    static void addScript(IPawnScript* script);

    /*
     * Called from EVF9Component::onAmxUnload.
     * Removes a script — events are no longer fired to it.
     */
    static void removeScript(IPawnScript* script);

    /*
     * dispatch1(arg0, event)
     * Fires: public EVF9_OnXxx(vehicleid)
     * Returns 0 if ANY script returns EVF9_BREAK; 1 otherwise.
     */
    static int dispatch1(int arg0, const char* event);

    /*
     * dispatch2(arg0, arg1, event)
     * Fires: public EVF9_OnXxx(vehicleid, playerid)
     */
    static int dispatch2(int arg0, int arg1, const char* event);

    /*
     * dispatch3(arg0, arg1, arg2, event)
     * Fires: public EVF9_OnXxx(vehicleid, arg1, arg2)
     */
    static int dispatch3(int arg0, int arg1, int arg2, const char* event);

    /*
     * dispatch1f(arg0, farg1, event)
     * Fires: public EVF9_OnXxx(vehicleid, Float:value)
     */
    static int dispatch1f(int arg0, float farg1, const char* event);

    /*
     * dispatch3f(arg0, fx, fy, fz, event)
     * Fires: public EVF9_OnXxx(vehicleid, Float:x, Float:y, Float:z)
     */
    static int dispatch3f(int arg0, float fx, float fy, float fz, const char* event);

    /* All currently-loaded Pawn script instances. */
    static std::unordered_set<IPawnScript*> s_instances;
};
