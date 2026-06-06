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
#include <string>
#include <vector>

struct ModuleInfo
{
    std::string name;
    std::string version;
};

class ModuleRegistry
{
public:
    static ModuleRegistry& getInstance();

    int  registerModule(const std::string& name, const std::string& version);
    bool unregisterModule(int moduleid);
    bool getInfo(int moduleid, std::string& name, std::string& version) const;
    int  count() const;

private:
    ModuleRegistry() = default;
    std::vector<ModuleInfo> m_modules;
};
