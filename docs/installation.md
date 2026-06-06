<!--
 * EVF9 - Extended Vehicle Framework 9
 * Created by EAST9 — Do not remove this notice.
-->

# Installation Guide

## Open.MP (Primary)

### 1. Build the Plugin

```sh
git clone https://github.com/east9-777/EVF9
cd EVF9
git submodule update --init --recursive   # downloads Open.MP SDK to sdk/omp
./build.sh                                # compiles evf9.so
```

### 2. Copy Files

```
build/evf9.so            →  <server>/plugins/evf9.so
pawno/include/EVF9.inc   →  <server>/qawno/include/EVF9.inc
pawno/include/EVF9/      →  <server>/qawno/include/EVF9/
```

### 3. Configure Server

In `config.json`:

```json
{
    "pawn": {
        "legacy_plugins": ["evf9"]
    }
}
```

### 4. Include in Gamemode

```pawn
#include <EVF9>
```

---

## SA-MP 0.3.7 (Partial Support)

### 1. Build 32-bit

```sh
# Requires 32-bit toolchain: apt install gcc-multilib g++-multilib
./build.sh --samp
```

### 2. Copy Files

```
build/evf9.so            →  <server>/plugins/evf9.so
pawno/include/EVF9.inc   →  <server>/pawno/include/EVF9.inc
pawno/include/EVF9/      →  <server>/pawno/include/EVF9/
```

### 3. Configure Server

In `server.cfg`:

```
plugins evf9
```

---

## Termux (Android)

```sh
./build_termux.sh
```

Note: Termux builds target your device architecture. For deployment, build on Linux (x86_64).

---

## Verify Installation

Add to your gamemode:

```pawn
public OnGameModeInit()
{
    printf("[EVF9] Version: %d Build: %d", EVF9_GetVersion(), EVF9_GetBuild());
    return 1;
}
```

Expected output: `[EVF9] Version: 100 Build: 1`
