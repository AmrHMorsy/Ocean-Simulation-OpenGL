# Raytracer

## Description 

This project, developed in C++, is an advanced Raytracer application that utilizes OpenMP for parallel computing to enhance performance. The project features motion blur, global illumination, and depth of field, designed to encapsulate and illustrate the intricate processes of rendering photorealistic images from 3D models and scenes, effectively demonstrating the power and capabilities of computer graphics programming.

## OpenMP Parallelization

This project incorporates OpenMP (Open Multi-Processing), a parallel programming model for shared-memory multiprocessor architectures, to achieve faster performance. By distributing computations across multiple CPU cores, the application can render complex scenes more quickly and efficiently.

## Important Note on Academic Integrity

While this repository is open to the public for learning and inspiration, it is expected that anyone viewing or using this code will respect the rules of academic integrity. This code should not be copied for use in academic or professional projects without proper understanding and attribution. Please use this repository as a reference and inspiration, not a source for plagiarism.

## Features

### Motion Blur

A feature that adds cinematic qualities to the rendered scenes, the motion blur function simulates the photographic phenomenon where fast-moving objects appear blurred along the direction of relative motion.

### Global Illumination 

By emulating how light interplays with objects within a scene, the global illumination feature provides richer and more lifelike visual outputs. It models the details of how light bounces around an environment and scatters in various directions, capturing the subtle nuances of natural light and shade.

### Depth of Field

This attribute allows for the simulation of the lens focus effect seen in real-world cameras. By adjusting the depth of field, users can emphasize specific subjects in the scene while artistically blurring other elements, creating a strikingly realistic and depth-perceptive visualization.

### Soft Shadows 

The Soft Shadows feature simulates the effect of shadows when light is obstructed by objects in the scene. The 'soft' refers to the non-uniform, irregular shape of the shadows, which contribute to the realistic rendering of scenes by simulating the scattering and diffraction of light.


### Phong Local Illumination

The Phong Local Illumination model calculates the color of an object based on its diffuse, specular, and ambient color, as well as the light present in the scene. This feature contributes to more accurate and visually pleasing renderings by considering the local properties of the objects and the lighting conditions.



## Installation

1. Clone the repository:
```
git clone https://github.com/AmrHMorsy/Raytracer-.git
```
2. Navigate to the project directory: 
```
cd raytracer
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
./raytracer <Model_Filepath>.json
```
  
## Results 

![jumping_ball](https://github.com/AmrHMorsy/Raytracer-/assets/56271967/548bd100-d515-401c-880d-fe52c0427db0)
![TripleB](https://github.com/AmrHMorsy/Raytracer-/assets/56271967/7a8be9a4-eb5d-49a2-8570-50b5362e4ebb)
![cornell_box_](https://github.com/AmrHMorsy/Raytracer-/assets/56271967/0ff224f0-9637-4f56-9a25-273a2a581e37)



## Contribution

This project is open to contributions. If you have suggestions or improvements, feel free to fork the project, make your changes, and open a pull request.

## License

This project is licensed under the terms of the MIT license. For more information, see the LICENSE file.

## Acknowledgments

This project has been a product of continuous learning and inspiration, made possible by a multitude of invaluable resources.

A significant acknowledgement goes to the Ray Tracing in One Weekend series https://raytracing.github.io. This resource has provided the fundamental knowledge and understanding needed to bring this project to life. Its comprehensive and accessible approach to explaining the concept of ray tracing has been instrumental in the development of this application.

I would also like to express my gratitude to the course COMP 371 - Computer Graphics at Concordia University. The theoretical framework and practical exercises provided throughout the course have immensely contributed to my understanding and skills in computer graphics, which have been vital in developing this project. The guidance from the faculty and the comprehensive curriculum helped transform a complex subject into a manageable and fascinating one.

Last but not least, I am indebted to the open-source community for consistently providing resources, inspiration, and a platform to learn, create, and share.

Your contributions to my journey in computer graphics are immensely appreciated.


## Support

For any questions or concerns, please open an issue, and I'll get back to you as soon as possible.

## Author

Amr Morsy

Enjoy the project, and happy coding!
