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

#include "vehicle_system.h"
#include "event_dispatcher.h"
#include <cstring>
#include <chrono>
#include <algorithm>

VehicleSystem& VehicleSystem::getInstance()
{
    static VehicleSystem instance;
    return instance;
}

static int64_t nowSeconds()
{
    auto tp = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(tp).count();
}

VehicleEntity* VehicleSystem::get(int vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_V) return nullptr;
    if (!m_vehicles[vehicleid].active) return nullptr;
    return &m_vehicles[vehicleid];
}

const VehicleEntity* VehicleSystem::get(int vehicleid) const
{
    if (vehicleid < 0 || vehicleid >= MAX_V) return nullptr;
    if (!m_vehicles[vehicleid].active) return nullptr;
    return &m_vehicles[vehicleid];
}

bool VehicleSystem::registerVehicle(int vehicleid, int modelid,
    float x, float y, float z, float angle,
    int color1, int color2,
    int worldid, int interiorid)
{
    if (vehicleid < 0 || vehicleid >= MAX_V) return false;
    if (m_vehicles[vehicleid].active) return false; /* already registered */

    auto& v          = m_vehicles[vehicleid];
    v.active         = true;
    v.vehicleid      = vehicleid;
    v.uid            = generateUID();
    v.modelid        = modelid;
    v.spawnX         = x;
    v.spawnY         = y;
    v.spawnZ         = z;
    v.spawnAngle     = angle;
    v.color1         = color1;
    v.color2         = color2;
    v.worldid        = worldid;
    v.interiorid     = interiorid;
    v.flags          = VEHICLE_FLAG_SPAWNED;
    v.health         = 1000.0f;
    v.fuel           = 100.0f;
    v.fuelRate       = 1.0f;
    v.ownerid        = -1;
    v.fleetid        = -1;
    v.saveState      = SaveState::CLEAN;
    v.pendingDestroy = false;
    v.lastDriver     = -1;
    v.lastOccupant   = -1;
    v.distanceTravelled = 0.0f;
    v.tripDistance   = 0.0f;
    v.lifetimeDistance  = 0.0f;
    v.insuranceExpiry   = 0;
    v.lastPosX       = x;
    v.lastPosY       = y;
    v.lastPosZ       = z;
    v.createdAt      = nowSeconds();
    v.tag[0]         = '\0';
    std::fill(std::begin(v.meta), std::end(v.meta), 0);

    /* Resize props vector to accommodate all registered property definitions */
    v.props.resize(m_propDefs.size());

    linkVehicle(vehicleid);
    m_count++;

    EventDispatcher::dispatch1(vehicleid, "EVF9_OnVehicleCreate");
    return true;
}

bool VehicleSystem::unregisterVehicle(int vehicleid)
{
    auto* v = get(vehicleid);
    if (!v) return false;

    /* If saving, defer destruction until EVF9_EndSave is called */
    if (v->saveState == SaveState::SAVING)
    {
        v->pendingDestroy = true;
        return true;
    }

    EventDispatcher::dispatch1(vehicleid, "EVF9_OnVehicleDestroy");

    unlinkVehicle(vehicleid);
    v->active = false;
    v->props.clear();
    m_count--;
    return true;
}

bool VehicleSystem::respawnInternal(int vehicleid)
{
    auto* v = get(vehicleid);
    if (!v) return false;

    EventDispatcher::dispatch1(vehicleid, "EVF9_OnVehicleRespawn");

    v->health         = 1000.0f;
    v->fuel           = 100.0f;
    v->flags         |= VEHICLE_FLAG_SPAWNED;
    v->tripDistance   = 0.0f;
    v->lastDriver     = -1;
    v->saveState      = SaveState::DIRTY;
    return true;
}

bool VehicleSystem::isManaged(int vehicleid) const
{
    if (vehicleid < 0 || vehicleid >= MAX_V) return false;
    return m_vehicles[vehicleid].active;
}

int VehicleSystem::count() const { return m_count; }

uint32_t VehicleSystem::generateUID() { return m_nextUID++; }

int VehicleSystem::getByUID(uint32_t uid) const
{
    /* Walk the linked list instead of scanning entire array */
    int vid = m_head;
    while (vid != -1)
    {
        if (m_vehicles[vid].uid == uid) return vid;
        vid = m_next[vid];
    }
    return -1;
}

int VehicleSystem::getFirst() const { return m_head; }

int VehicleSystem::getNext(int vehicleid) const
{
    if (vehicleid < 0 || vehicleid >= MAX_V) return -1;
    return m_next[vehicleid];
}

void VehicleSystem::markDirty(int vehicleid)
{
    auto* v = get(vehicleid);
    if (!v) return;
    if (v->saveState == SaveState::CLEAN)
    {
        v->saveState = SaveState::DIRTY;
        EventDispatcher::dispatch1(vehicleid, "EVF9_OnVehicleDirty");
    }
}

bool VehicleSystem::beginSave(int vehicleid)
{
    auto* v = get(vehicleid);
    if (!v) return false;
    v->saveState = SaveState::SAVING;
    EventDispatcher::dispatch1(vehicleid, "EVF9_OnVehicleSave");
    return true;
}

bool VehicleSystem::endSave(int vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_V) return false;
    auto& v = m_vehicles[vehicleid];
    if (!v.active && !v.pendingDestroy) return false;

    if (v.pendingDestroy)
    {
        v.saveState = SaveState::CLEAN;
        unregisterVehicle(vehicleid);
        return true;
    }

    v.saveState = SaveState::CLEAN;
    EventDispatcher::dispatch1(vehicleid, "EVF9_OnVehicleSaved");
    return true;
}

/* --- Property registry --- */

int VehicleSystem::registerProperty(const std::string& name, PropType type)
{
    /* Prevent duplicate registration */
    for (int i = 0; i < (int)m_propDefs.size(); ++i)
        if (m_propDefs[i].name == name) return i;

    m_propDefs.push_back({ name, type });
    int propid = (int)m_propDefs.size() - 1;

    /* Grow props vector on all existing vehicles */
    for (int i = 0; i < MAX_V; ++i)
        if (m_vehicles[i].active)
            m_vehicles[i].props.resize(m_propDefs.size());

    return propid;
}

int VehicleSystem::findProperty(const std::string& name) const
{
    for (int i = 0; i < (int)m_propDefs.size(); ++i)
        if (m_propDefs[i].name == name) return i;
    return -1;
}

const PropertyDef* VehicleSystem::getPropertyDef(int propid) const
{
    if (propid < 0 || propid >= (int)m_propDefs.size()) return nullptr;
    return &m_propDefs[propid];
}

int VehicleSystem::propertyCount() const { return (int)m_propDefs.size(); }

/* --- Fleet registry --- */

int VehicleSystem::createFleet(const std::string& name, int ownerid)
{
    m_fleets.push_back({ name, ownerid, true });
    int fleetid = (int)m_fleets.size() - 1;
    EventDispatcher::dispatch2(fleetid, ownerid, "EVF9_OnFleetCreated");
    return fleetid;
}

bool VehicleSystem::destroyFleet(int fleetid)
{
    if (fleetid < 0 || fleetid >= (int)m_fleets.size()) return false;
    m_fleets[fleetid].active = false;
    EventDispatcher::dispatch1(fleetid, "EVF9_OnFleetDestroyed");
    return true;
}

const FleetEntry* VehicleSystem::getFleet(int fleetid) const
{
    if (fleetid < 0 || fleetid >= (int)m_fleets.size()) return nullptr;
    return &m_fleets[fleetid];
}

int VehicleSystem::fleetCount() const { return (int)m_fleets.size(); }

/* --- Linked list --- */

void VehicleSystem::linkVehicle(int vehicleid)
{
    m_next[vehicleid] = m_head;
    m_head = vehicleid;
}

void VehicleSystem::unlinkVehicle(int vehicleid)
{
    if (m_head == vehicleid)
    {
        m_head = m_next[vehicleid];
        m_next[vehicleid] = -1;
        return;
    }
    int prev = m_head;
    while (prev != -1 && m_next[prev] != vehicleid)
        prev = m_next[prev];
    if (prev != -1)
    {
        m_next[prev] = m_next[vehicleid];
        m_next[vehicleid] = -1;
    }
}
