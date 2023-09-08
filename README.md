# Forgotten Waves

## Description 

"Forgotten Waves" is an immersive virtual environment that captures the deep introspective journey of an individual amidst the vastness of the ocean. The project represents the intricate feelings of solitude, introspection, and the haunting beauty of internal struggles. Set against the expansive backdrop of the ocean, the game delivers a profound narrative experience through its meticulous graphics, real-time effects, and interactive elements.

## Implementation 

This project is implemented using C/C++, OpenGL and OpenCL. 

## Features

### PBR (Physically Based Rendering)

Implements a sophisticated shading model that simulates the interaction of light with surfaces in a physically accurate manner, enhancing the realism and detail of the environment.

### Dynamic Ocean
Uses FFT (Fast Fourier Transform) in conjunction with Jerry Tessendorf's seminal paper "Simulating Ocean Water" to generate realistic, animated ocean waves.
OpenCL acceleration for efficient and fast wave simulation.

### Atmospheric Effects
Skybox HDR: A high dynamic range skybox, adding depth and realism to the game's sky.
Fog Effect: A dense and immersive fog that envelops the environment, heightening the sense of isolation.
Image-Based Lighting: Lighting techniques based on real-world imagery, providing a natural and authentic look to the game.

### Optimizations
Frustum Culling: Efficiently renders only the portions of the game world that are currently visible, enhancing performance.

## To-Do

. Animating/Moving Boat: Enhance the boat's realism by allowing it to move and interact dynamically with the waves.
. Narrative Elements: Introduce poems, texts, and diaries to enrich the narrative.
. Weather Dynamics: Implement changing weather patterns, introducing elements like rain, thunder, and storms to intensify the emotional atmosphere.
. Introductory Scene: Create an engaging scene that sets the narrative tone and provides context to the game.
. Dynamic Shadows: Incorporate shadows for added realism.
. Trailer Creation: Design a compelling trailer to showcase the essence of "Forgotten Waves".

 
## Installation

1. Clone the repository:
```
git clone https://github.com/AmrHMorsy/Forgotten_Waves-.git
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
  
## Results 

## License

This project is licensed under the terms of the MIT license. For more information, see the LICENSE file.

## Acknowledgments

This project has been a product of continuous learning and inspiration, made possible by a multitude of invaluable resources.

A significant acknowledgement goes to the LearnOpenGL.com website as well as Jerry's Tessendofr's Paper "Simulating Ocean Water". This resource has provided the fundamental knowledge and understanding needed to bring this project to life. Its comprehensive and accessible approach to explaining the concept of ray tracing has been instrumental in the development of this application.

Last but not least, I am indebted to the open-source community for consistently providing resources, inspiration, and a platform to learn, create, and share.

Your contributions to my journey in computer graphics are immensely appreciated.


## Support

For any questions or concerns, please open an issue, and I'll get back to you as soon as possible.

## Author

Amr Morsy

Enjoy the project, and happy coding!
