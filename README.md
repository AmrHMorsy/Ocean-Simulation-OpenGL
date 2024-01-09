# Ocean Simulation

Developed in C++ and OpenGL, this real-time simulation uses the Fast Fourier Transform method (FFT) to simulate waves, inspired by the paper ["Simulating Ocean Water"](https://people.computing.clemson.edu/~jtessen/reports/papers_files/coursenotes2004.pdf) by Jerry Tessendorf. Additionally, it is parallelized using OpenCL and employs Physically-Based Rendering (PBR) and image-based lighting (IBL) techniques to enhance the realism of the oceanic visuals, and an HDR skybox to present dynamic and an immersive sky. 

![image-2 copy](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/5ebb1fef-45e0-46bf-b247-d71636ce0f0e)

## Features

### Graphics and Rendering

#### Advanced Graphics Rendering with OpenGL

This project harnesses the power of OpenGL, a widely-used graphics API, to render complex oceanic environments with high efficiency. OpenGL's versatility allows for the creation of detailed and dynamic graphics, offering users a visually rich simulation experience.

#### Lifelike Shading with Physically-Based Rendering (PBR)

PBR techniques are employed to simulate realistic material properties. This approach ensures that the water surface in the simulation accurately reflects and refracts light, mimicking the way light interacts with natural water. The result is a stunningly realistic depiction of the ocean, complete with nuanced lighting effects.

#### Image-Based Lighting for Enhanced Realism

To further improve visual fidelity, Image-Based Lighting (IBL) is used. This technique utilizes real-world imagery to provide environmental lighting, ensuring that the simulation's lighting conditions are based on actual atmospheric lighting. This results in richer reflections and more accurate illumination across the simulated ocean.

#### Optimized Performance with Frustum Culling

Frustum culling is a technique used to enhance the performance of the simulation. By rendering only the elements that are within the player's field of view, it significantly reduces the processing load. This optimization ensures smooth and responsive performance, even when rendering complex scenes.

![image 2](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/0ac42cf6-1602-4f77-a293-f23f99f24ba7)

### Ocean Simulation

#### Realistic Ocean Waves with FFT

The Fast Fourier Transform (FFT) method is pivotal in simulating realistic ocean waves. This mathematical approach allows for a compelling representation of ocean dynamics, closely inspired by Jerry Tessendorf's paper ["Simulating Ocean Water"](https://people.computing.clemson.edu/~jtessen/reports/papers_files/coursenotes2004.pdf). The FFT algorithm creates dynamic, lifelike wave patterns that faithfully mimic real oceanic conditions.


#### Performance Boost with OpenCL Parallelization

The integration of OpenCL facilitates efficient parallelization of wave computations. This means that wave simulations can be processed simultaneously across multiple computing units, greatly enhancing the overall performance and responsiveness of the simulation.


#### Phillips Spectrum for Wave Energy Modeling

The simulation incorporates the Phillips Spectrum, a mathematical model that provides a detailed statistical representation of wave energy distribution. By using this model, the simulation can accurately depict the varying energy levels across different wave frequencies, adding to the authenticity of the ocean wave behavior.

#### Adjustable Ocean Temperament Settings

Users have the flexibility to adjust the ocean's temperament in the simulation. This feature allows for a seamless transition between calm, serene waters and more turbulent, stormy seas, offering a varied and interactive experience.

#### Enhanced Realism with Wake Effects

Wake effects are meticulously implemented to modify the wave height field, adding another layer of realism to the simulation. These effects are particularly noticeable when objects interact with the water surface, creating realistic ripples and waves that enhance the overall authenticity of the ocean environment.

![image-2](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/d2234e09-ed69-4a88-8974-3bb09ba8bb23)

### Environmental Effects

#### Dynamic HDR Skybox 

An HDR (High Dynamic Range) Skybox is introduced to present a more dynamic and immersive sky. This feature captures the vast range of luminance of real-world skies, from the brightest clouds to the darkest nights, creating a more lifelike backdrop that enhances the visual experience.

#### Atmospheric Fog for Depth and Ambiance

Atmospheric fog effects are implemented to add depth and a captivating ambiance to the scene. This not only contributes to the visual appeal of the simulation but also adds a layer of realism by mimicking the way fog interacts with light and the environment in natural settings.

#### Consistent Backdrop with the Infinite Ocean

The Infinite Ocean feature ensures that the ocean landscape extends endlessly, providing a consistent and immersive backdrop. This feature is crucial in creating a believable and boundless oceanic environment, where the horizon seamlessly meets the sky, just like in the real world.

![image](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/ea42b491-76d7-4abc-b233-601a4113fb28)

## Future Work

### Enhanced Fourier Transform Algorithm

Transition to Cooley-Tukey FFT Algorithm: The current implementation of the ocean simulation utilizes the inverse discrete Fourier transform, a more straightforward yet computationally intensive approach. To significantly improve performance and efficiency, the project will transition to the Cooley-Tukey Fast Fourier Transform (FFT) algorithm. This algorithm, renowned for its reduced computational complexity, will enable faster and more dynamic wave simulations, making the ocean environment even more realistic and responsive.

### Expanded Environmental Elements

Incorporation of Wildlife: To enhance the immersion and realism of the oceanic environment, the simulation will introduce animated birds. These birds will not only add life to the sky but also interact with the ocean environment, creating a more vibrant and organic ocean ecosystem.

### Character Animation Integration

Future updates will include character animation, allowing users to experience the simulation from a more personal and interactive perspective. This addition aims to create a more engaging user experience, where the simulation can be explored and appreciated from different viewpoints.

### Strengthened Wake Effects

Recognizing the importance of wake effects in contributing to the realism of ocean simulations, the project will focus on enhancing these effects. The improved wake system will more accurately depict the interaction between objects and the water surface, allowing for more intricate and realistic wave patterns.

### Implementation of an Infinite Ocean

To provide a boundless and uninterrupted oceanic experience, the simulation will incorporate an 'Infinite Ocean' feature. This will ensure that the water landscape seamlessly stretches into the horizon, offering users an unbroken and expansive view of the ocean.

### Foam Generation

A significant addition will be the simulation of ocean foams. This feature will enhance the visual fidelity of the water, particularly in turbulent areas, adding a layer of realism to the wave crests and interactions.

### Rendering Improvements

A continuous effort will be made to improve the rendering capabilities of the simulation. This includes refining the Physically-Based Rendering (PBR) techniques, optimizing light interactions, and enhancing material representations. These improvements aim to achieve even more lifelike and visually stunning water and environmental effects.

These future enhancements are aimed at pushing the boundaries of realism in ocean simulation, creating an experience that is not only visually spectacular but also rich in dynamics and interactivity. By implementing these upgrades, the project will not only become a state-of-the-art tool for simulating ocean environments but also a platform for immersive exploration and discovery.

![image copy 2](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/b6e61dd7-eb12-4c48-9f93-d48f10ba4d5b)


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
<br>

![image copy](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/c18bee8f-1a24-4ec3-8300-6918dd2d9109)
