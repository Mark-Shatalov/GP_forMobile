# Terraria-Style Sandbox Game (C + Raylib)

An educational, step-by-step recreation of a Terraria-like 2D sandbox
game, built in plain C (C99) with Raylib. Each development phase is
built and verified before moving to the next one.

## Current status: Phase 1 — Engine ✅

What exists right now:
- A Raylib window running at 60 FPS
- A proper game loop (Update → Draw) separated from `main.c`
- A `Camera2D` wrapped in its own module (`camera_controller`)
- A simple 16x16 test grid drawn in world space, to visually confirm
  the coordinate system and camera work correctly

Controls (Phase 1 only — temporary, for testing):
- `WASD` / Arrow keys — pan the camera
- Mouse wheel — zoom in/out

## Project structure

```
src/
├── main.c                 - entry point, owns the game loop
├── game.c / game.h        - ties subsystems together
├── camera_controller.c/.h - wraps Raylib's Camera2D
└── constants.h            - all shared tunable values (screen size, tile size, etc.)

assets/
├── textures/
├── sounds/
└── fonts/
```

Files for `world`, `tile`, `player`, `texture_manager`, and `input`
will be added in their respective phases (2–6), following the same
one-module-per-responsibility style.

## Building in Visual Studio (CMake)

This project includes a `CMakeLists.txt` that downloads and builds raylib
automatically the first time you configure — you do **not** need to
install raylib yourself for this path.

1. Open Visual Studio 2022 (with the "Desktop development with C++"
   workload, which includes CMake support).
2. **File → Open → Folder...** and select this project's root folder
   (the one containing `CMakeLists.txt`).
3. Visual Studio detects `CMakeLists.txt` and runs CMake automatically.
   The first run will take a minute or two — it's cloning and building
   raylib in the background (see the Output window).
4. Once configuration finishes, set the startup item at the top toolbar
   to `terraria_clone.exe`, then press **Ctrl+F5** (or the green Run
   arrow) to build and run.

If you'd rather generate a `.sln` file instead of using Open Folder:
```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
```
Then open `build/terraria_clone.sln` in Visual Studio as usual.

Notes:
- Needs an internet connection the first time (to fetch raylib from
  GitHub) and a reasonably recent CMake (3.15+, bundled with VS 2022).
- The executable is written to `build/bin/` and its working directory
  is set to the project root, so relative paths like `assets/...` will
  resolve correctly once later phases start loading textures.

## Installing raylib (manual / non-Visual-Studio path)

If you're building with plain `make` instead of CMake/Visual Studio,
you need raylib's development files installed first.

**Ubuntu/Debian:**
```bash
sudo apt install build-essential git \
    libasound2-dev mesa-common-dev libx11-dev libxrandr-dev \
    libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install RAYLIB_LIBTYPE=STATIC
```

**macOS (Homebrew):**
```bash
brew install raylib
```

**Windows:**
The easiest route is [w64devkit](https://github.com/skeeto/w64devkit)
(comes with GCC + raylib pre-set-up), or install raylib via
[vcpkg](https://github.com/microsoft/vcpkg):
```powershell
vcpkg install raylib
```

## Building and running

Once raylib is installed system-wide:

```bash
make        # builds to build/game
make run    # builds and runs
make clean  # removes build artifacts
```

If `gcc` can't find raylib, either adjust the `LDFLAGS`/`CFLAGS` in the
`Makefile` to point at wherever you installed it, or (Linux) run
`sudo ldconfig` after `make install` above.

## Roadmap

- [x] Phase 1 — Engine (window, game loop, Camera2D, test grid)
- [ ] Phase 2 — Tile world (tile enum, 2D array, visible-tile rendering)
- [ ] Phase 3 — Player (movement, gravity, collision, camera follow)
- [ ] Phase 4 — Interaction (mining, placing, reach distance, particles)
- [ ] Phase 5 — Inventory (hotbar, stacking, pickup, selection)
- [ ] Phase 6 — Procedural world (surface, caves, trees, ores)
