# Terraria-Style Sandbox Game (C + Raylib)

An educational, step-by-step recreation of a Terraria-like 2D sandbox game,
built in plain C (C99) with Raylib. Each development phase is built and
verified before moving to the next one.

## Current status: Phase 7 - Tools and progressive mining complete

What exists right now:

- Everything from Phases 1 through 5
- A `Player` struct with position, velocity, size, and grounded state
- Horizontal movement, jumping, gravity, and maximum fall speed
- Axis-separated collision against solid world tiles
- World-boundary collision
- A camera that follows the player's center and stays inside the world
- A small input module that keeps keyboard choices separate from physics
- Mouse tile targeting with a yellow in-range or red out-of-range outline
- Left-click mining with colored block-break particles
- Right-click selected-block placement on empty tiles beside solid tiles
- Five-tile interaction reach and protection against placing inside the player
- A ten-slot hotbar with 99 items per stack
- Dirt, grass, stone, wood, leaves, and copper ore inventory items
- Collectible item drops created by mining blocks
- Placement that consumes one item from the selected stack
- Mouse-wheel and number-key hotbar selection
- A freshly randomized world on every game launch
- Variable dirt depth with stone underneath
- Random-walk cave tunnels below the protected surface
- Copper ore veins placed inside stone
- Trees made from collectible wood and leaf blocks
- Non-solid trees that the player and dropped items can pass through
- Tree breaking from the mined trunk block upward, while lower wood remains
- Generated-surface player spawning with a clear starting area
- Hold-to-mine interaction with different mining times for each tile type
- Wooden pickaxe with faster stone and copper mining
- Wooden axe with faster wood and leaf mining
- A world-space mining progress bar over the targeted block
- Automatic stepping over one-tile obstacles while walking

Controls:

- `A` / `D` or Left / Right arrows - move
- `Space` - jump
- Mouse wheel - change the selected hotbar slot
- Number keys `1` through `0` - select a hotbar slot directly
- `Ctrl` + Mouse wheel - zoom in/out
- Hold left click - mine the targeted block
- Right click - place the selected block

## Project structure

```text
src/
|-- main.c                 - entry point, owns the game loop
|-- game.c / game.h        - ties subsystems together
|-- camera_controller.c/.h - player-following Camera2D and zoom
|-- input.c / input.h      - maps keyboard state to player commands
|-- interaction.c/.h       - targeting, reach, mining, and placement
|-- inventory.c/.h         - hotbar, selection, and item stacking
|-- item.c / item.h        - item types and tile/item conversion
|-- dropped_item.c/.h      - collectible world item entities
|-- particle.c/.h          - fixed-pool block-break particles
|-- player.c / player.h    - player physics, collision, and drawing
|-- world.c / world.h      - 2D tile grid, visible-tile rendering
|-- world_generation.c/.h  - terrain, caves, ore veins, and trees
|-- tile.c / tile.h        - TileType enum and per-tile properties
`-- constants.h            - shared tunable values

assets/
|-- textures/
|-- sounds/
`-- fonts/
```

The `texture_manager` module will be added when assets are introduced in a
later phase, following the same one-module-per-responsibility style.

## Building in Visual Studio (CMake)

This project includes a `CMakeLists.txt` that downloads and builds Raylib
automatically the first time you configure. You do not need to install Raylib
yourself for this path.

1. Open Visual Studio 2022 with the **Desktop development with C++** workload.
2. Choose **File -> Open -> Folder...** and select the folder containing
   `CMakeLists.txt`.
3. Wait for Visual Studio to finish configuring CMake. The first run downloads
   and builds Raylib, so it can take a minute or two.
4. Select `terraria_clone.exe` as the startup item and press **Ctrl+F5**.

To generate a Visual Studio solution instead:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
```

Then open `build/terraria_clone.sln`.

Notes:

- The first configure needs an internet connection to fetch Raylib 5.5.
- CMake 3.15 or newer is required.
- The executable is written to the build directory's `bin` folder.
- Visual Studio runs the game with the project root as its working directory,
  so future `assets/...` paths resolve correctly.

## Building with Make

The Makefile expects Raylib to already be installed on the system:

```bash
make
make run
make clean
```

## Roadmap

- [x] Phase 1 - Engine (window, game loop, Camera2D, test grid)
- [x] Phase 2 - Tile world (tile enum, 2D array, visible-tile rendering)
- [x] Phase 3 - Player (movement, gravity, collision, camera follow)
- [x] Phase 4 - Interaction (mining, placing, reach distance, particles)
- [x] Phase 5 - Inventory (hotbar, stacking, pickup, selection)
- [x] Phase 6 - Procedural world (surface, caves, trees, ores)
- [x] Phase 7 - Tools, progressive mining, and one-tile auto-step
