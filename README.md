Developed in C++ and OpenGL, this simulation uses the Fast Fourier Transform method (FFT) with the assistance of OpenCL to simulate waves. Additionally, it employs Physically-Based Rendering and image-based lighting techniques to enhance the realism of the oceanic visuals

![image copy](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/dea0ab4b-059d-4ec4-a18d-99e5c77f1057)

## Features

### Graphics and Rendering:
- Powered by OpenGL, the project offers advanced graphics rendering.
- Utilized Physically-Based Rendering (PBR) techniques to achieve lifelike shading and materials, resulting in realistic reflections, refractions, and lighting effects on the water surface.
- Enhanced visual fidelity with Image-based Lighting, providing richer reflections and accurate illumination.
- Frustum culling ensures optimized performance, rendering only the elements within the player's perspective.

### Ocean Simulation:
- Employed Fast Fourier Transform (FFT) for the realistic depiction of ocean waves, providing a compelling representation of ocean dynamics.
- Integrated OpenCL to efficiently parallelize wave computations, bolstering the simulation's performance.
- Incorporated the Phillips Spectrum for detailed statistical modeling of wave energy distribution across various frequencies.
- Features adjustable ocean temperament settings, allowing transitions between serene waters and turbulent seas.
- Wake effects intricately modify the wave height field, further enhancing the simulation's realism.

![image](https://github.com/AmrHMorsy/Ocean-Simulation/assets/56271967/bca13d29-e97e-4bc9-92b1-acf6f150ddd6)

### Environmental Effects:
- Introduced an HDR Skybox, presenting a dynamic, immersive sky that accentuates the visual experience.
- Atmospheric fog effects have been implemented, adding depth and a captivating ambiance to the scene.
- The Infinite Ocean feature ensures that the water landscape stretches endlessly, providing a consistent and immersive backdrop.

### Additional Features:
- The project boasts dynamic shadows, lending a heightened sense of realism.
- An animated boat, responding to the simulation's oceanic dynamics, moves seamlessly across the water.
