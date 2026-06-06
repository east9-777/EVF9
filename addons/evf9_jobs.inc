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

#if defined _evf9_jobs_included
    #endinput
#endif
#define _evf9_jobs_included

#if !defined _evf9_core_included
    #error "Include <EVF9> before including addons/evf9_jobs"
#endif

/*
 * Addon: Jobs
 * Vehicle-based job system — delivery, transport, cargo missions.
 * Uses EVF9_METADATA (META_JOB, META_COMPANY) and custom properties.
 */

#define EVF9_ADDON_JOBS_VERSION "1.0.0"

static g_JobsAddonID = INVALID_ADDON_ID;

stock EVF9_Jobs_Init()
{
    g_JobsAddonID = EVF9_RegisterAddon("evf9_jobs", EVF9_ADDON_JOBS_VERSION);
    printf("[EVF9:Jobs] Addon registered (id=%d)", g_JobsAddonID);
    return 1;
}

forward EVF9_OnJobStarted(vehicleid, playerid, jobid);
forward EVF9_OnJobCompleted(vehicleid, playerid, jobid, reward);
forward EVF9_OnJobFailed(vehicleid, playerid, jobid, reason);

native AssignJob(vehicleid, playerid, jobid);
native CompleteJob(vehicleid, playerid);
native FailJob(vehicleid, playerid, reason);
native GetVehicleJob(vehicleid);
native IsVehicleOnJob(vehicleid);
native GetJobReward(jobid);
