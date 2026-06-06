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

/*
 * event_dispatcher.cpp — Multi-script Pawn event firing (Open.MP modern).
 *
 * Uses IPawnScript::Call() to invoke Pawn public functions.
 * This is the correct Open.MP component API — no raw AMX C functions needed.
 *
 * IPawnScript::Call(name, DefaultReturnValue_True, arg1, arg2, ...) internally:
 *   1. Calls FindPublic(name, &idx) — returns AMX_ERR_NONE if public exists
 *   2. Pushes arguments right-to-left via Push()
 *   3. Executes with Exec(&ret, idx)
 *   4. Returns the Pawn return value, or the default if public is not found
 *
 * Arguments are passed as-is; floats are handled by the PushOne<float>
 * specialisation in pawn.hpp which calls amx_ftoc() internally.
 */

#include "event_dispatcher.h"

std::unordered_set<IPawnScript*> EventDispatcher::s_instances;

void EventDispatcher::addScript(IPawnScript* script)
{
    if (script)
        s_instances.insert(script);
}

void EventDispatcher::removeScript(IPawnScript* script)
{
    s_instances.erase(script);
}

/*
 * Fire event on ALL loaded scripts.
 * Returns 0 (EVF9_BREAK) if any script cancelled the event; 1 otherwise.
 * Scripts that do not have the public function return the default (1 = continue).
 */
static int fire1(const char* name, int a0)
{
    int result = 1;
    for (IPawnScript* script : EventDispatcher::s_instances)
    {
        cell ret = script->Call(name, DefaultReturnValue_True, a0);
        if ((int)ret == 0) result = 0;
    }
    return result;
}

static int fire2(const char* name, int a0, int a1)
{
    int result = 1;
    for (IPawnScript* script : EventDispatcher::s_instances)
    {
        cell ret = script->Call(name, DefaultReturnValue_True, a0, a1);
        if ((int)ret == 0) result = 0;
    }
    return result;
}

static int fire3(const char* name, int a0, int a1, int a2)
{
    int result = 1;
    for (IPawnScript* script : EventDispatcher::s_instances)
    {
        cell ret = script->Call(name, DefaultReturnValue_True, a0, a1, a2);
        if ((int)ret == 0) result = 0;
    }
    return result;
}

static int fire1f(const char* name, int a0, float f1)
{
    int result = 1;
    for (IPawnScript* script : EventDispatcher::s_instances)
    {
        cell ret = script->Call(name, DefaultReturnValue_True, a0, f1);
        if ((int)ret == 0) result = 0;
    }
    return result;
}

static int fire3f(const char* name, int a0, float fx, float fy, float fz)
{
    int result = 1;
    for (IPawnScript* script : EventDispatcher::s_instances)
    {
        cell ret = script->Call(name, DefaultReturnValue_True, a0, fx, fy, fz);
        if ((int)ret == 0) result = 0;
    }
    return result;
}

int EventDispatcher::dispatch1(int arg0, const char* event)
{
    return fire1(event, arg0);
}

int EventDispatcher::dispatch2(int arg0, int arg1, const char* event)
{
    return fire2(event, arg0, arg1);
}

int EventDispatcher::dispatch3(int arg0, int arg1, int arg2, const char* event)
{
    return fire3(event, arg0, arg1, arg2);
}

int EventDispatcher::dispatch1f(int arg0, float farg1, const char* event)
{
    return fire1f(event, arg0, farg1);
}

int EventDispatcher::dispatch3f(int arg0, float fx, float fy, float fz, const char* event)
{
    return fire3f(event, arg0, fx, fy, fz);
}
