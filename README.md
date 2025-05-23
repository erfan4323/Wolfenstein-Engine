# Wolfenstein Engine

A simple implementation of the Wolfenstein 3D rendering style using C. This project serves as a foundational example of raycasting techniques inspired by the classic 1992 game.

## How It Works: The Wolfenstein 3D Engine

The original Wolfenstein 3D engine utilized a technique called **raycasting** to simulate a 3D environment on a 2D grid. In this method, for each vertical line on the screen, a ray is cast from the player's position into the game world to detect walls. When a ray intersects a wall, the engine calculates the distance to that wall and renders a vertical slice of the wall's texture accordingly. This creates the illusion of depth and perspective, allowing for real-time rendering even on the limited hardware of the early '90s.

This approach was efficient because it only required calculating the visible portions of the environment, rather than rendering the entire 3D scene. The game world was represented as a 2D grid, with each cell indicating the presence or absence of a wall, simplifying collision detection and rendering calculations.

## Features

- Basic raycasting engine for 3D-like rendering

- Wall rendering with texture mapping

- Sprite rendering

- Simple player movement and rotation

- Lightweight and minimal dependencies

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`)

- [Xmake](https://xmake.io/) build system

### Building the Project

1. Clone the repository:
   
   ```bash
   git clone https://github.com/erfan4323/Wolfenstein-Engine.git
   cd Wolfenstein-Engine
   ```

2. First generate the compile_commands.json in order to link with libraries:
   
   ```bash
   xmake project -k compile_commands
   ```

3. Run the executable:
   
   ```bash
   xmake -r; xmake r
   ```

## Reading Guide: Understanding the Project

To comprehend the workings of this project, consider the following steps:

1. **Start with `main.c`**: This is the entry point of the program, initializing the game loop and handling user input.

2. **Explore the Raycasting Logic**: Delve into the functions responsible for casting rays and rendering walls. Understanding how each ray is cast and how wall distances are calculated is crucial.

3. **Examine Texture Mapping**: Look into how textures are loaded and mapped onto walls. This will give insight into how the 2D textures contribute to the 3D illusion.

4. **Understand Player Movement**: Review how player input translates into movement and rotation within the game world.

5. **Review the Map Structure**: Analyze how the game world is represented as a 2D grid and how this grid is used in collision detection and rendering.

By following this sequence, you'll gain a comprehensive understanding of how the engine operates and how the various components interact to create the final rendered scene.

## Preview

![Preview](./readme-resources/Wolfenstein-Engine-Showcase.mp4)


