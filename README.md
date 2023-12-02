# Ocean Simulation

Developed in C++ and OpenGL, this simulation uses the Fast Fourier Transform method (FFT) with the assistance of OpenCL to simulate waves, inspired by the paper ["Simulating Ocean Water"](https://people.computing.clemson.edu/~jtessen/reports/papers_files/coursenotes2004.pdf) by Jerry Tessendorf. Additionally, it employs Physically-Based Rendering (PBR) and image-based lighting (IBL) techniques to enhance the realism of the oceanic visuals

![image 2](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/0ac42cf6-1602-4f77-a293-f23f99f24ba7)

## Features

### Graphics and Rendering:
- Powered by OpenGL, the project offers advanced graphics rendering.
- Utilized Physically-Based Rendering (PBR) techniques to achieve lifelike shading and materials, resulting in realistic reflections, refractions, and lighting effects on the water surface.
- Enhanced visual fidelity with Image-based Lighting, providing richer reflections and accurate illumination.
- Frustum culling ensures optimized performance, rendering only the elements within the player's perspective.

### Ocean Simulation:
- Employed Fast Fourier Transform (FFT) for the realistic depiction of ocean waves, providing a compelling representation of ocean dynamics, inspired by the paper ["Simulating Ocean Water"](https://people.computing.clemson.edu/~jtessen/reports/papers_files/coursenotes2004.pdf) by Jerry Tessendorf. 
- Integrated OpenCL to efficiently parallelize wave computations, bolstering the simulation's performance.
- Incorporated the Phillips Spectrum for detailed statistical modeling of wave energy distribution across various frequencies.
- Features adjustable ocean temperament settings, allowing transitions between serene waters and turbulent seas.
- Wake effects intricately modify the wave height field, further enhancing the simulation's realism.

![Screenshot 2023-11-08 at 10 41 20](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/6e515374-3d8b-47e1-a4f6-2b6747b0851e)

### Environmental Effects:
- Introduced an HDR Skybox, presenting a dynamic, immersive sky that accentuates the visual experience.
- Atmospheric fog effects have been implemented, adding depth and a captivating ambiance to the scene.
- The Infinite Ocean feature ensures that the water landscape stretches endlessly, providing a consistent and immersive backdrop.

![1](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/29a98de4-4cdf-485f-80c9-bb70f09f6fc0)

## Installation

1. Clone the repository:
```
git clone https://github.com/AmrHMorsy/ocean_simulation-.git
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
./ocean_simulation
```
