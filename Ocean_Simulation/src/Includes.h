#include <iostream>
#include <clFFT.h>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <chrono>
#include <string>
#include <stdio.h>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <OpenCL/opencl.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#include <OpenGL/OpenGL.h>
#pragma clang diagnostic pop
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../external/Include/irrKlang.h"
#include "../external/Include/progressbar.h"

using namespace std ;
using namespace irrklang;

#define NUM_PROGRESS_BARS 100
