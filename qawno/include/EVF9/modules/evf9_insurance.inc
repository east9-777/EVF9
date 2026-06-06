/*
 * EVF9 - Extended Vehicle Framework 9 — Insurance Module
 *
 * Created by EAST9
 * Copyright (c) EAST9. Do not remove this notice.
 */

#if defined _evf9_insurance_included
    #endinput
#endif
#define _evf9_insurance_included

#include <EVF9/evf9_natives>

/*
 * EVF9 Insurance module — interface reference
 *
 * Insurance is stored as a UNIX timestamp (seconds since epoch).
 * Use GetInsuranceExpiration() and compare with gettime() to check validity.
 *
 * Natives:
 *   InsureVehicle(vehicleid, duration)          → duration in seconds
 *   CancelInsurance(vehicleid)
 *   IsVehicleInsured(vehicleid)                 → bool (live expiry check)
 *   GetInsuranceExpiration(vehicleid)           → UNIX timestamp
 *   RenewInsurance(vehicleid, duration)         → extends from current expiry
 *
 * Usage:
 *   InsureVehicle(vehicleid, 86400 * 30);   // 30 days
 *   if(IsVehicleInsured(vehicleid)) {
 *       new expiry = GetInsuranceExpiration(vehicleid);
 *       printf("Insured until UNIX: %d", expiry);
 *   }
 */
