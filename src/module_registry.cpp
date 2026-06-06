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

#include "module_registry.h"

ModuleRegistry& ModuleRegistry::getInstance()
{
    static ModuleRegistry instance;
    return instance;
}

int ModuleRegistry::registerModule(const std::string& name, const std::string& version)
{
    for (int i = 0; i < (int)m_modules.size(); ++i)
        if (m_modules[i].name == name) return i; /* already registered */
    m_modules.push_back({ name, version });
    return (int)m_modules.size() - 1;
}

bool ModuleRegistry::unregisterModule(int moduleid)
{
    if (moduleid < 0 || moduleid >= (int)m_modules.size()) return false;
    m_modules.erase(m_modules.begin() + moduleid);
    return true;
}

bool ModuleRegistry::getInfo(int moduleid, std::string& name, std::string& version) const
{
    if (moduleid < 0 || moduleid >= (int)m_modules.size()) return false;
    name    = m_modules[moduleid].name;
    version = m_modules[moduleid].version;
    return true;
}

int ModuleRegistry::count() const { return (int)m_modules.size(); }
