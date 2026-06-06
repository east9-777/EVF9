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

/**
 * Example: JSON Export
 * Demonstrates vehicle data export to JSON files for backup/migration.
 */

#include <a_samp>
#include <EVF9>
#include <EVF9/extensions/evf9_json>

public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/exportvehicle", true))
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(!IsManagedVehicle(vehicleid)) return 1;
        new file[64];
        format(file, sizeof(file), "backups/vehicle_%d.json", EVF9_GetVehicleUID(vehicleid));
        EVF9_ExportVehicleJSON(vehicleid, file);
        SendClientMessage(playerid, -1, "Vehicle exported to JSON.");
        return 1;
    }
    return 0;
}
