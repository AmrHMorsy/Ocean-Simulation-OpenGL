# Forgotten Waves 

Forgotten Waves is an immersive virtual environment that captures the deep introspective journey of an individual amidst the vastness of the ocean. The project represents the intricate feelings of solitude, introspection, and the haunting beauty of internal struggles.

<img width="1426" alt="Screenshot 2023-09-08 at 19 59 39" src="https://github.com/AmrHMorsy/Forgotten-Waves/assets/56271967/f304d023-837d-4057-ae82-9f44c06a3d4b">

## Implementation 

"Forgotten Waves" is a game developed in C++ and utilizes the graphics capabilities of OpenGL. It employs OpenCL to simulate waves using the Fast Fourier Transform method. The game also uses Physically-Based Rendering for its shading, enhancing the realism of the oceanic visuals.

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

### Game Elements and Interaction
- A solitary boat serving as the player's point of reference and interaction.
- A mysterious book inside the boat, furthering the game's narrative.
- Player controls for navigating the boat and interacting with in-game elements.

### Original Soundtrack
- A unique background score composed personally, accentuating the game's mood and themes, and enriching the sense of solitude and reflection.

### Narrative and Theme
- A thematic exploration of feeling lost and isolated amidst the vastness of society, inviting players to reflect and empathize.

With these features, "Forgotten Waves" offers a blend of technical prowess and emotional depth, setting it apart as a unique gaming experience.

<img width="1432" alt="Screenshot 2023-09-09 at 06 08 12" src="https://github.com/AmrHMorsy/Forgotten-Waves/assets/56271967/732caa22-246e-4def-91ec-6803573349bd">

## Installation

1. Clone the repository:
```
git clone https://github.com/AmrHMorsy/Forgotten_Waves.git
```
2. Navigate to the project directory: 
```
cd Forgotten_Waves
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
./Forgotten_Waves
```


## Future Work

- **Animating/Moving Boat:** To further the game's realism, there's a vision to allow the boat to row in different directions and to animate the oars with movement.
- **Narrative Elements:** To enhance the introspective journey, future versions might introduce poems, texts, and personal diary entries. These literary fragments could provide deeper layers of story, resonating with players on an emotional and intellectual level.
- **Weather Dynamics:** Plans are in place to implement dynamic weather patterns. This includes introducing elements like rain, thunder, and turbulent storms, all aiming to amplify the emotional ambiance of "Forgotten Waves".
- **Introductory Scene:** To seamlessly introduce players to the game's narrative and emotional undertones, an evocative introductory scene is on the horizon. This scene aims to set the tone and context for the entire experience.
- **Dynamic Shadows:** Realism is at the heart of "Forgotten Waves". To push this even further, future implementations will focus on incorporating dynamic shadows, creating a richer and more lifelike environment.
- **Trailer Creation:** For those yet to dive into the game, a compelling trailer is in the works. This trailer will encapsulate the ethos, beauty, and depth of "Forgotten Waves", inviting potential players into its captivating embrace.



## Acknowledgments
In the development of "Forgotten Waves", several resources were pivotal in achieving the game's technical and visual accomplishments:

- **LearnOpenGL:** Many of the advanced rendering techniques and graphics integrations were guided by the comprehensive tutorials and articles available on LearnOpenGL. Their in-depth explanations and hands-on approach were instrumental in realizing the game's visual potential.
- **"Simulating Ocean Water" by Jerry Tessendorf:** The realistic ocean simulation in the game is largely influenced by the methods described in Jerry Tessendorf's seminal paper, "Simulating Ocean Water". This paper provided the foundational understanding of ocean wave simulations, and its techniques were crucial in the development of the game's oceanic environment. <br />

A heartfelt thank you to these and other resources and communities that contributed, either directly or indirectly, to the making of "Forgotten Waves".


## Contributing
This project is open to contributions. If you have suggestions or improvements, feel free to fork the project, make your changes, and open a pull request.
