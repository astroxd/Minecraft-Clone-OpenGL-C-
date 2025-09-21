# OpenGL

## To-Do List
+ [ ] Bug Fixes
  - [ ] Chunks not being rebuilt
  - [ ] ImGui Abort error when closing the window 
+ [ ] Terrain Generation
  - [ ] Implement ThreadPool for generating chunks
  - [ ] Build chunks starting from player position growing around him
  - [ ] Blend different noises like Minecraft does
  - [ ] Add Trees and in-between chunks structures (different class like TerrainFeature?)
  - [ ] Ore generation
+ [ ] Containers
  - [ ] Abstract Inventory logic to a Container class
  - [ ] Implement a Container, like a Chest
  - [ ] Implement a Container with input/output slots, like a Furnace
+ [ ] Inventory
  - [ ] Add Pickup logic (fill the first empty slot or increment item quantity) 
+ [ ] Physics
+ [ ] Serialization
  - [ ] World save system
  - [ ] Load Block data from JSON file
+ [ ] Blocks
  - [ ] Add Transparent Blocks, like Glass ([article](https://learnopengl.com/Advanced-OpenGL/Blending))
  - [ ] Add Water
  - [ ] Add Billboard Blocks, like Grass, Wheat
  - [ ] Add Physics affected blocks, like Sand
  - [ ] Add various size blocks, like Bed, Door
+ [ ] GUI (static functions like ImGui?)
  - [ ] Add Text Rendering ([article](https://learnopengl.com/In-Practice/Text-Rendering))
  - [ ] Add Buttons
  - [ ] Add Text Field
  - [ ] Add Waila Mod
  - [ ] Add JEI Mod
    - [ ] Paginated List of Items
    - [ ] Search Bar
    - [ ] Item Giving
  - [ ] Add In-Game Info Mod ([article](https://github.com/Lunatrius/InGame-Info-XML))
+ [ ] Lighting 
+ [ ] Entity
  - [ ] Add Animal/Mob
  - [ ] Add Projectile, like Arrows
+ [ ] ECS??
+ [ ] 3D Renderer
  - [ ] Model Loading
  - [ ] Dropped Item 
+ [ ] Hephaestus (HPS) Game Engine (Detach Game Logic from Engine)
