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

Lighting is currently **under development** — as visible in the night scene, some shading calculations are off.  
This will be fixed once the light propagation and ambient occlusion systems are implemented.

---

## Screenshots
<img width="1275" height="749" alt="Screenshot 2025-12-03 183809" src="https://github.com/user-attachments/assets/155f5ba2-1073-4ea1-a195-ee26509599ab" />
<img width="1277" height="747" alt="Screenshot 2025-12-03 184825" src="https://github.com/user-attachments/assets/9fa84dc5-0b95-4818-aa26-8f45ce1abab5" />
<img width="1276" height="704" alt="Screenshot 2025-12-03 185120" src="https://github.com/user-attachments/assets/3766dd76-6b32-43ae-ad30-bd722bbb3f53" />

---

## Roadmap

- [ ] Fix lighting system (sunlight propagation)
- [ ] Add ambient occlusion and soft shadows
- [ ] Implement water and sky reflections
- [ ] Introduce biomes (plains, mountains, snow, etc.)
- [ ] Player controls & collision
- [ ] Save/load chunks from disk

