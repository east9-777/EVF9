<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# FAQ

**Q: Do I need YSI to use EVF9?**  
A: No. EVF9 implements its own ALS hooks and iterators. YSI is not required.

**Q: Can I use EVF9 with PawnPlus / coroutines?**  
A: Yes — EVF9 is compatible. It doesn't use or require PawnPlus.

**Q: How many vehicles can EVF9 handle?**  
A: Minimum 500, recommended 2000, advanced 5000+. See [performance.md](performance.md).

**Q: Can two modules conflict?**  
A: No. Modules are independent and communicate only via events and public API.

**Q: What happens if I call DestroyManagedVehicle during a MySQL save?**  
A: Destruction is deferred until `EVF9_EndSave` is called. No data is lost.

**Q: Should I use vehicleid or UID in my database?**  
A: Always use UID. vehicleid can change across respawns.

**Q: Is VEHICLE_FLAG_INSURED safe to set directly?**  
A: No. Only `evf9_insurance.inc` (via `InsureVehicle`) should set it. Setting it directly creates state inconsistency.

**Q: Does EVF9 open a database connection?**  
A: Never. The gamemode owns the connection; you pass the handle via `EVF9_SetMySQLHandle`.

**Q: Can I contribute an addon?**  
A: Yes. See [CONTRIBUTING.md](../.github/CONTRIBUTING.md).
