<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Contributing to EVF9

Thank you for your interest in contributing!

## How to Contribute

1. **Fork** the repository
2. Create a branch from `main`
3. Implement your change following the guidelines below
4. Run tests: `tests/test_core.pwn`, `tests/test_modules.pwn`, etc.
5. Submit a **Pull Request** using the PR template

## Code Guidelines

- All files must include the full copyright header (see any existing file)
- Indentation: **4 spaces** — never tabs
- No external dependencies in the core plugin
- Every public native needs documentation (Function/Description/Parameters/Returns/Example/Notes)
- Every event needs documentation (Event/Called/Parameters/Cancelable/Returns/Example)
- No `vehicleid` as database primary key — always use UID
- Modules must be independent (no inter-module dependencies)

## Architecture Rules

- State lives in the C++ plugin — Pawn never stores duplicate vehicle state
- Events are dispatched by the plugin — Pawn listens via ALS hooks
- Persistence is always optional — modules work without it
- Addons use only: events, public API, metadata, custom properties

## Naming Conventions

| Context | Pattern | Example |
|---|---|---|
| Core/Plugin | `EVF9_FunctionName` | `EVF9_GetVersion()` |
| Modules | `VerbNoun` | `GetVehicleFuel()` |
| Events | `EVF9_OnVehicleSomething` | `EVF9_OnVehicleEngineStart` |
| Macros | `EVF9_MACRO_NAME` | `EVF9_FOREACH_VEHICLE` |
| Constants | `EVF9_CONSTANT_NAME` | `EVF9_CONTINUE` |

## Commit Messages

```
feat(fuel): add GetFuelConsumptionRate native
fix(uid): handle INVALID_VEHICLE_ID in getByUID
docs(events): document EVF9_OnVehicleTheftAttempt cancelable behavior
perf(iterator): reduce linked list traversal overhead
```

## Contact

- Discord: leivison0230_58048
- Servidor: https://discord.gg/FJBWAesQV8
