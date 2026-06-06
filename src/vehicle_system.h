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
#include <cstdint>
#include <array>
#include <chrono>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef EVF9_MAX_VEHICLES
#define EVF9_MAX_VEHICLES 2000
#endif

enum class SaveState : uint8_t
{
    CLEAN  = 0,
    DIRTY  = 1,
    SAVING = 2
};

enum VehicleFlags : uint32_t
{
    VEHICLE_FLAG_NONE      = 0,
    VEHICLE_FLAG_LOCKED    = 1 << 0,
    VEHICLE_FLAG_ENGINE    = 1 << 1,
    VEHICLE_FLAG_LIGHTS    = 1 << 2,
    VEHICLE_FLAG_INSURED   = 1 << 3,
    VEHICLE_FLAG_TRACKED   = 1 << 4,
    VEHICLE_FLAG_ABANDONED = 1 << 5,
    VEHICLE_FLAG_SPAWNED   = 1 << 6
};

/* Custom typed property value (union style) */
enum class PropType : uint8_t { INT = 0, FLOAT = 1, STRING = 2 };

struct PropertyDef
{
    std::string name;
    PropType    type;
};

union PropValue
{
    int         ival;
    float       fval;
};

struct VehicleProp
{
    PropValue   val  = { 0 };
    std::string sval;
    bool        set  = false;
};

struct VehicleEntity
{
    bool        active         = false;
    int         vehicleid      = -1;
    uint32_t    uid            = 0;
    int         modelid        = 0;
    float       spawnX         = 0.0f;
    float       spawnY         = 0.0f;
    float       spawnZ         = 0.0f;
    float       spawnAngle     = 0.0f;
    int         color1         = -1;
    int         color2         = -1;
    int         worldid        = 0;
    int         interiorid     = 0;
    uint32_t    flags          = VEHICLE_FLAG_NONE;
    float       health         = 1000.0f;
    float       distanceTravelled = 0.0f;
    int         lastDriver     = -1;
    int         lastOccupant   = -1;
    int64_t     createdAt      = 0;
    SaveState   saveState      = SaveState::CLEAN;
    char        tag[65]        = {};
    bool        pendingDestroy = false;

    /* Fuel module */
    float       fuel           = 100.0f;
    float       fuelRate       = 1.0f;

    /* Ownership module */
    int         ownerid        = -1;

    /* Fleet module */
    int         fleetid        = -1;

    /* Insurance module */
    int64_t     insuranceExpiry = 0;

    /* Tracking module */
    float       lastPosX       = 0.0f;
    float       lastPosY       = 0.0f;
    float       lastPosZ       = 0.0f;
    float       lifetimeDistance = 0.0f;

    /* Trip distance (resets on demand) */
    float       tripDistance   = 0.0f;

    /* Metadata array (7 slots: META_JOB, COMPANY, GROUP, EVENT, CUSTOM 1-3) */
    int         meta[7]        = {};

    /* Custom typed properties — indexed by propertyid */
    std::vector<VehicleProp> props;
};

/* Fleet data */
struct FleetEntry
{
    std::string name;
    int         ownerid = -1;
    bool        active  = false;
};

/* -------------------------------------------------------------------------
 * VehicleSystem — singleton, authoritative state for all managed vehicles
 * ------------------------------------------------------------------------- */
class VehicleSystem
{
public:
    static VehicleSystem& getInstance();

    /* Core */
    VehicleEntity* get(int vehicleid);
    const VehicleEntity* get(int vehicleid) const;

    /* Pool management — vehicleid comes FROM the server (CreateVehicle) */
    bool    registerVehicle(int vehicleid, int modelid,
                            float x, float y, float z, float angle,
                            int color1, int color2,
                            int worldid, int interiorid);
    bool    unregisterVehicle(int vehicleid);
    bool    respawnInternal(int vehicleid);

    bool    isManaged(int vehicleid) const;
    int     count() const;

    /* UID */
    uint32_t generateUID();
    int      getByUID(uint32_t uid) const;

    /* Linked list iterator */
    int getFirst() const;
    int getNext(int vehicleid) const;

    /* Save state */
    void markDirty(int vehicleid);
    bool beginSave(int vehicleid);
    bool endSave(int vehicleid);

    /* Property registry */
    int  registerProperty(const std::string& name, PropType type);
    int  findProperty(const std::string& name) const;
    const PropertyDef* getPropertyDef(int propid) const;
    int  propertyCount() const;

    /* Fleet registry */
    int  createFleet(const std::string& name, int ownerid);
    bool destroyFleet(int fleetid);
    const FleetEntry* getFleet(int fleetid) const;
    int  fleetCount() const;

private:
    VehicleSystem() { std::fill(std::begin(m_next), std::end(m_next), -1); }

    static constexpr int MAX_V = EVF9_MAX_VEHICLES;

    VehicleEntity   m_vehicles[MAX_V];
    int             m_next[MAX_V];
    int             m_head    = -1;
    int             m_count   = 0;
    uint32_t        m_nextUID = 1;

    std::vector<PropertyDef>  m_propDefs;
    std::vector<FleetEntry>   m_fleets;

    void linkVehicle(int vehicleid);
    void unlinkVehicle(int vehicleid);
};
