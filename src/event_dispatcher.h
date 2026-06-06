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
#include <amx/amx.h>
#include <string>
#include <vector>

/*
 * EventDispatcher
 *
 * Calls Pawn public functions from C++ using the standard AMX API:
 *   amx_FindPublic() — locates the public function index
 *   amx_Push()       — pushes arguments onto the AMX stack (right to left)
 *   amx_Exec()       — executes the public and retrieves return value
 *
 * Return values from Pawn:
 *   EVF9_CONTINUE (1) — allow action
 *   EVF9_BREAK    (0) — cancel action (cancelable events only)
 *
 * Usage (C++ native calling a Pawn event):
 *   int result = EventDispatcher::dispatch1(params[1], "EVF9_OnVehicleCreate");
 *   // or with playerid arg:
 *   int result = EventDispatcher::dispatch2(params[1], params[2], "EVF9_OnVehicleEngineStart");
 *   if (result == 0) return 0; // cancelled
 */

class EventDispatcher
{
public:
    /* Store the AMX pointer — called in AmxLoad/onAmxLoad */
    static void setAMX(AMX* amx);
    static AMX* getAMX();

    /*
     * dispatch1(arg0, event)
     * Fires a Pawn public with 1 int argument: public EVF9_OnXxx(vehicleid)
     * Returns the Pawn return value (1=continue, 0=cancel). Defaults to 1 if not found.
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
     * farg1 is the float, passed as its cell-bit representation.
     */
    static int dispatch1f(int arg0, float farg1, const char* event);

    /*
     * dispatch3f(arg0, fx, fy, fz, event)
     * Fires: public EVF9_OnXxx(vehicleid, Float:x, Float:y, Float:z)
     */
    static int dispatch3f(int arg0, float fx, float fy, float fz, const char* event);

private:
    static AMX* s_amx;
};
