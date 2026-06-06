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
 * amx_helpers.h
 * Correct wrappers for the AMX API used in SA-MP/Open.MP plugins.
 *
 * The standard amx.h defines:
 *   amx_ctof(c)  — cell bits → float   (macro, *(float*)&(c))
 *   amx_ftoc(f)  — float → cell bits   (macro, *(cell*)&(f))
 *   amx_GetAddr(amx, param, &addr)  — get pointer to Pawn cell
 *   amx_GetString(dest, src_addr, use_wchar, len)
 *   amx_SetString(dest_addr, src, pack, use_wchar, len)
 *
 * These helpers wrap the above correctly so every native can use
 * evf9_amx_* instead of the raw API.
 */

#include <amx/amx.h>
#include <cstring>

/* ---------- float by value (params[N] directly) ---------- */
static inline float evf9_GetFloat(cell param)
{
    return amx_ctof(param);
}

/* ---------- float by reference (&Float:x) ---------- */
static inline float evf9_GetFloatRef(AMX* amx, cell param)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    return addr ? amx_ctof(*addr) : 0.0f;
}

static inline void evf9_SetFloat(AMX* amx, cell param, float value)
{
    cell* addr = nullptr;
    amx_GetAddr(amx, param, &addr);
    if (addr) *addr = amx_ftoc(value);
}

/* ---------- int by reference (&count) ---------- */
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
