# QuirkyVoxel

**QuirkyVoxel** is a Minecraft-style sandbox game built entirely from scratch in **C++** and **OpenGL**. 

It began as a project to understand how games like Minecraft are built internally from **terrain generation** and **chunk systems** to **rendering, UI, and gameplay systems**.

While the current focus is on procedural world generation and efficient rendering, the long-term goal is to evolve QuirkyVoxel into a complete sandbox experience with **collisions, entities, UI, and interactive world mechanics**.

This project serves as a deep dive into learning how complex systems rendering, threading, asset management, and world simulation work together in a game engine environment.

---

## What's Implemented

- **Procedural Terrain Generation** using layered noise functions  
- **Chunk-based World System** with dynamic mesh generation  
- **Threaded Mesh Builder** for background terrain updates  
- **Day-Night Cycle** with real-time sun and moon movement  
- **Sky Gradient Rendering** based on time of day  
- **Batched Terrain Rendering** using **MultiDraw** with a single shared **VBO** 

---

## Core Systems

### Application
Manages the main loop, window creation, and ImGui context.  
`GameLayer` connects the world and renderer each frame.

### Renderer
Handles all rendering operations:
- **GameRenderer** – High-level rendering entry point.  
- **WorldRenderer / ChunkRenderManager** – Batches visible chunks using **MultiDraw** and a shared **VBO**.  
- **MeshManager** – Builds and uploads chunk meshes in the background.  
- **Camera & SkyboxRenderer** – Handles view setup and sky rendering based on world time.

### World
Implements world logic and terrain generation:
- **ChunkManager / TerrainGeneration** – Procedural terrain creation and chunk lifecycle.  
- **World / WorldTime / Sky** – Manages world state, time progression, and lighting data.  
- **Block / Biome** – Defines block types and biome behavior.

### Other Systems
- **Events** – Input and window event handling.  
- **Player** – Camera control and future movement system.  
- **Utils** – Shared helper utilities across modules.

---

## Known Issues

Lighting is currently **under development** as visible in the night scene, some shading calculations are off.  
This will be fixed once the light propagation and ambient occlusion systems are implemented.

---

## Screenshots


| Day | Night | Terrain |
|:---:|:---:|:---:|
| ![Day](https://github.com/user-attachments/assets/155f5ba2-1073-4ea1-a195-ee26509599ab) | ![Terrain](https://github.com/user-attachments/assets/9fa84dc5-0b95-4818-aa26-8f45ce1abab5) | ![Night](https://github.com/user-attachments/assets/3766dd76-6b32-43ae-ad30-bd722bbb3f53) |

---

## Roadmap

- [ ] Fix lighting system (sunlight propagation)
- [ ] Add ambient occlusion and soft shadows
- [ ] Implement water and sky reflections
- [ ] Introduce biomes (plains, mountains, snow, etc.)
- [ ] Player controls & collision
- [ ] Save/load chunks from disk

---

## Build & Run

### Requirements
- **CMake** (3.21+ recommended)  
- **C++20** compiler  
- **Windows / MSVC (VS 2022)** – currently tested here  
  > Other platforms should be possible (no platform-specific code), but are **untested** for now.

### Clone with submodules
```bash
git clone --recurse-submodules https://github.com/Jayantkumar56/QuirkyVoxel
# If you already cloned without recurse:
git submodule update --init --recursive
```

### Windows (Visual Studio 2022)

Option A:- use the script
```bash
GenerateProject.bat
```
This creates a Build/ directory and generates a Visual Studio 17 2022 solution using CMake.

Option B:- manual CMake
```bash
mkdir Build && cd Build
cmake .. -G "Visual Studio 17 2022"
```

Then open the generated solution and build either the Debug or Release configuration.

### Assets

Assets are not automatically copied to the executable directory yet.
If you run from the IDE, make sure the working directory is set to the project root
or manually copy the `Assets` folder next to the executable or in the working directory.

---

## Acknowledgments

QuirkyVoxel uses several open-source libraries to handle core functionality:

| Library | Purpose |
|----------|----------|
| [**GLFW**](https://github.com/glfw/glfw) | Window creation, input, and OpenGL context management |
| [**GLAD**](https://github.com/Dav1dde/glad) | OpenGL function loading |
| [**ImGui**](https://github.com/ocornut/imgui) | In-engine UI and debug tools |
| [**glm**](https://github.com/g-truc/glm) | Mathematics library for vectors, matrices, and transforms |
| [**stb**](https://github.com/nothings/stb) | Image loading utilities |
| [**FastNoiseLite**](https://github.com/Auburn/FastNoiseLite) | Procedural noise generation for terrain |
| [**yaml-cpp**](https://github.com/jbeder/yaml-cpp) | YAML parsing for configuration and project settings |
| [**spdlog**](https://github.com/gabime/spdlog) | Fast and modern C++ logging system |
| [**Tracy**](https://github.com/wolfpld/tracy) | Real-time performance profiling and instrumentation |

All libraries are included as part of the project’s `Vendor/` folder for easy build integration.
