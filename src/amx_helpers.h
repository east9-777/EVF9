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
 * amx_helpers.h — Thin wrappers over the AMX C API.
 *
 * The Open.MP SDK ships its own copy of amx/amx.h (the same low-level AMX
 * bytecode interface that SA-MP used).  These helpers wrap the standard API
 * functions so native implementations can exchange Pawn values cleanly
 * without repeating boilerplate.
 *
 * Standard macros used here (all defined in amx/amx.h):
 *   amx_ctof(c)               — cell bit-pattern → float
 *   amx_ftoc(f)               — float → cell bit-pattern
 *   amx_GetAddr(amx,p,&addr)  — resolve reference parameter to cell*
 *   amx_GetString(dst,src,wchar,len)
 *   amx_SetString(dst,src,pack,wchar,len)
 */

#include <amx/amx.h>
#include <cstring>

/*
 * Safe float<->cell conversion using memcpy to avoid strict-aliasing UB.
 * The pawn amx.h macros (amx_ctof / amx_ftoc) use direct pointer casts which
 * trigger -Wstrict-aliasing under GCC/Clang.  These inline functions are the
 * standard-conforming replacement and compile to the same single instruction.
 */
static inline float evf9_cell_to_float(cell c)
{
    float f;
    static_assert(sizeof(cell) == sizeof(float), "cell and float must be same size");
    std::memcpy(&f, &c, sizeof(f));
    return f;
}

static inline cell evf9_float_to_cell(float f)
{
    cell c;
    static_assert(sizeof(cell) == sizeof(float), "cell and float must be same size");
    std::memcpy(&c, &f, sizeof(c));
    return c;
}

/* ---------- Float by value: params[N] carries the bit-pattern ---------- */
static inline float evf9_GetFloat(cell param)
{
    return evf9_cell_to_float(param);
}

/* ---------- Float by reference (&Float:x) ---------- */
static inline float evf9_GetFloatRef(AMX* amx, cell param)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    return addr ? evf9_cell_to_float(*addr) : 0.0f;
}

static inline void evf9_SetFloat(AMX* amx, cell param, float value)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    if (addr) *addr = evf9_float_to_cell(value);
}

/* ---------- Integer by reference (&count) ---------- */
static inline int evf9_GetIntRef(AMX* amx, cell param)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    return addr ? (int)*addr : 0;
}

static inline void evf9_SetInt(AMX* amx, cell param, int value)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    if (addr) *addr = (cell)value;
}

/* ---------- Pawn string → C string ---------- */
static inline void evf9_GetStr(AMX* amx, cell param, char* buf, int maxlen)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    if (addr)
        amx_GetString(buf, addr, 0, maxlen);
    else
        buf[0] = '\0';
}

/* ---------- C string → Pawn string ---------- */
static inline void evf9_SetStr(AMX* amx, cell param, const char* str, int maxlen)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    if (addr)
        amx_SetString(addr, str, 0, 0, maxlen);
}
