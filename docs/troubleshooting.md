<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Troubleshooting

## Plugin doesn't load

- Verify `evf9.so` is in `<server>/plugins/`
- Check `config.json` has `"evf9"` in `pawn.legacy_plugins`
- For SA-MP: check `server.cfg` has `plugins evf9`
- Ensure the build matches server architecture (x86_64 for Open.MP, x86 for SA-MP)

## "sdk/omp not found" at compile time

```sh
git submodule update --init --recursive
```

## Vehicle health not syncing

Use `EVF9_SetVehicleHealth(vehicleid, health)` — not `SetVehicleHealth` directly.  
EVF9 propagates the change to the server automatically.

## Lock event fires but vehicle stays unlocked

`EVF9_OnVehicleLock` is cancelable. Check if another script returns `EVF9_BREAK`.

## VEHICLE_FLAG_INSURED not updating after InsureVehicle

Never call `EVF9_SetVehicleFlag(vid, VEHICLE_FLAG_INSURED)` directly.  
Use `InsureVehicle(vehicleid, duration)` — it sets the flag and expiry together.

## Enable debug logging

```pawn
#define EVF9_ENABLE_DEBUG 1
#include <EVF9>
```

Check server console for `[EVF9]` prefixed messages.

## MySQL save race condition / vehicle destroyed during save

EVF9 handles this automatically. `DestroyManagedVehicle` during an active save (SAVING state)  
defers destruction until `EVF9_EndSave` is called — no data loss.
