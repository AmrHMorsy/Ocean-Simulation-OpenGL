# Ocean Simulation

Developed in C++ and OpenGL, this simulation uses the Fast Fourier Transform method (FFT) with the assistance of OpenCL to simulate waves. Additionally, it employs Physically-Based Rendering to enhance the realism of the oceanic visuals

<img width="1426" alt="Screenshot 2023-09-08 at 19 59 39" src="https://github.com/AmrHMorsy/Forgotten-Waves/assets/56271967/f304d023-837d-4057-ae82-9f44c06a3d4b">

## Features

### Graphics and Rendering
- Developed using C++ for core game logic.
- Advanced graphics powered by OpenGL.
- Physically-Based Rendering (PBR) employed for lifelike shading and materials.
- Image-based Lighting for richer and more accurate reflections and illumination.

### Ocean Simulation
- Realistic oceanic visuals with waves simulated through Fast Fourier Transform (FFT).
- OpenCL integration for efficient parallelization of wave computations.
- Ability to adjust the ocean's temperament, transitioning between calm seas and stormy waters.

### Environmental Effects
- A visually arresting HDR Skybox for a dynamic and immersive sky.
- Atmospheric fog effect to add depth and ambiance to the scene.
- Frustum culling to optimize performance by rendering only what's within the player's field of view.

### Original Soundtrack
- A unique background score composed personally, accentuating the game's mood and themes, and enriching the sense of solitude and reflection.

<img width="1432" alt="Screenshot 2023-09-09 at 06 08 12" src="https://github.com/AmrHMorsy/Forgotten-Waves/assets/56271967/732caa22-246e-4def-91ec-6803573349bd">

## Installation

1. Clone the repository:
```
git clone https://github.com/AmrHMorsy/Ocean-Simulation.git
```
2. Navigate to the project directory: 
```
cd Ocean_Simulation
```
3. Create a build directory: 
```
mkdir build
```
4. Navigate to the build directory: 
```
cd build
```
5. Generate makefiles using cmake: 
```
cmake ../
```
6. compile using makefile: 
```
make
```
7. Run the program: 
```
./Ocean_Simulation
```


## Future Work

- **Animating/Moving Boat:** Allow the boat to row in different directions and to animate the oars with movement.
- **Weather Dynamics:** Implement dynamic weather patterns. This includes introducing elements like rain, thunder, and turbulent storms.
- **Dynamic Shadows:** Future implementations will focus on incorporating dynamic shadows, creating a richer and more lifelike environment.

## Acknowledgments

- **LearnOpenGL:** Many of the advanced rendering techniques and graphics integrations were guided by the comprehensive tutorials and articles available on LearnOpenGL. Their in-depth explanations and hands-on approach were instrumental in realizing the simulation's visual potential.
- **"Simulating Ocean Water" by Jerry Tessendorf:** The realistic ocean simulation in the game is largely influenced by the methods described in Jerry Tessendorf's seminal paper, "Simulating Ocean Water". This paper provided the foundational understanding of ocean wave simulations, and its techniques were crucial in the development of the game's oceanic environment.
- **CGTrader:** All of the 3D models used in the game were sourced from CGTrader. <br />


## Contributing
This project is open to contributions. If you have suggestions or improvements, feel free to fork the project, make your changes, and open a pull request.
