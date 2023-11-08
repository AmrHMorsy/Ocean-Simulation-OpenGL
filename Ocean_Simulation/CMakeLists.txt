cmake_minimum_required(VERSION 3.8)
project(Ocean_Simulation)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH /encs/opt/local)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(SDL2_MIXER_INCLUDE_DIRS "/usr/local/include/SDL2")
set(SDL2_MIXER_LIBRARIES "/usr/local/lib/libSDL2_mixer.dylib")

# Find the required packages
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenCL REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake/clFFT") # Add the path of clFFT's CMake files
find_package(clFFT REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Adjust the include directories
include_directories(${CMAKE_SOURCE_DIR}/external/Include)
include_directories(src/)

aux_source_directory(src SOURCE)

add_executable(Ocean_Simulation src/Main.cpp ${SOURCE})

# Adjusted the linking order
target_link_libraries(Ocean_Simulation 
                      OpenCL::OpenCL 
                      OpenGL::GL 
                      GLEW::glew 
                      glfw 
                      ${ASSIMP_LIBRARIES}
                      /usr/local/lib/libclFFT.dylib
                      ${SDL2_MIXER_LIBRARIES}
                      ${SDL2_LIBRARIES})

target_include_directories(Ocean_Simulation PRIVATE 
                           ${FREETYPE_INCLUDE_DIRS} 
                           ${OPENCL_INCLUDE_DIR} 
                           ${clFFT_INCLUDE_DIRS}
                           ${ASSIMP_INCLUDE_DIRS}
                           ${SDL2_INCLUDE_DIRS} 
                           ${SDL2_MIXER_INCLUDE_DIRS})
