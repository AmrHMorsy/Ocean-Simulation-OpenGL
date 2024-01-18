#include "Builder.h"



Builder::Builder()
{
    initialize_GLFW() ;
    access_openCL_platform() ;
    renderer = new Renderer(window, bar) ;
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0,0,scrWidth,scrHeight) ;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS) ;
}

void Builder::initialize_GLFW()
{
    if( glfwInit() == GL_FALSE )
        generate_error( "ERROR::Failed to Initialize GLFW" ) ;
    
    set_window_attributes() ;
}

void Builder::set_window_attributes()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) ;
    
    create_window() ;
}

void Builder::create_window()
{
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    window = glfwCreateWindow(mode->width, mode->height, "Ocean Simulation", primary, NULL);
    glViewport(0,0,mode->width, mode->height) ;
    if( window == NULL )
        generate_error( "ERROR::Failed to Create Window" ) ;
    glfwMakeContextCurrent(window);
    glClearColor(0.00f, 0.00f, 0.00f, 1.0f );

    initialize_GLEW() ;
}

void Builder::initialize_GLEW()
{
    glewExperimental = true ;
    if( glewInit() != GLEW_OK )
        generate_error( "ERROR::Failed to Initialize GLEW" ) ;
    
    setup_progress_bar() ;
}

void Builder::setup_progress_bar()
{
    bar = new progressbar(NUM_PROGRESS_BARS) ;
    bar->set_done_char("\033[1;32m█\033[0m") ;
    bar->set_opening_bracket_char("[") ;
    bar->set_closing_bracket_char("]") ;
    std::cout << std::endl ;
}

void Builder::access_openCL_platform()
{
    err = clGetPlatformIDs(1, &platform, NULL) ;
    if( err < 0 )
        generate_error("Couldn't Find Any Platforms for OpenCL") ;
        
    access_openCL_devices() ;
}

void Builder::access_openCL_devices()
{
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL) ;
    if( err < 0 )
        generate_error("Couldn't Find Any Devices For OpenCL") ;
    
    create_openCL_context() ;
}

void Builder::create_openCL_context()
{
    CGLContextObj glContext = CGLGetCurrentContext() ;
    CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext) ;
    cl_context_properties properties[] = { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)shareGroup, 0 } ;

    context = clCreateContext(properties, 1, &device, NULL, NULL, &err);
    if( err < 0 )
        generate_error("Couldn't Create OpenCL Context") ;
    
    read_kernel_sourceCode() ;
}
    
void Builder::read_kernel_sourceCode()
{
    std::ifstream f ;
    std::stringstream s ;
    f.open("../external/kernels/waves.cl", std::fstream::in) ;
    if( !f.is_open() )
        generate_error("ERROR: FAILURE TO OPEN OPENCL KERNEL FILE") ;
    s << f.rdbuf() ;
    program_size = (size_t) f.tellg() ;
    f.close() ;
    std::string g = (s.str()) ;
    program_buffer = g.c_str() ;
    
    create_program() ;
}
    
void Builder::create_program()
{
    program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err ) ;
    if( err < 0 )
        generate_error("Couldn't Create OpenCL Program") ;
    
    build_program() ;
}
    
void Builder::build_program() {
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if(err < 0) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
        free(log);

        // You might want to throw an exception or terminate here since the build failed
        generate_error("Couldn't build OpenCL program. Check build log for details.");
    } else {
        create_kernels();
    }
}

//void Builder::build_program()
//{
//    err = clBuildProgram( program, 1, &device, NULL, NULL, NULL) ;
//    if( err < 0 )
//        generate_error("Couldn't Build OpenCL Program") ;
//    
//    create_kernels() ;
//}
//    
void Builder::create_kernels()
{
    compute_FourierAmplitudes_Kernel = clCreateKernel(program, "compute_FourierAmplitudes", &err);
    if( err < 0 )
        generate_error("Couldn't Find compute_FourierAmplitudes Kernel") ;
    compute_inverse_Fourier_Transform = clCreateKernel(program, "inverse_Fourier_Transform", &err);
    if( err < 0 )
        generate_error("Couldn't Find inverse_Fourier_Transform Kernel") ;
    
    create_command_queue() ;
}
 
void Builder::create_command_queue()
{
    queue = clCreateCommandQueue(context, device, 0, &err) ;
    if( err < 0 )
        generate_error("Couldn't create a command queue") ;
    
    initialize_SDL() ;
}

void Builder::initialize_SDL()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        generate_error("Couldn't Initialize SDL") ;
}

void Builder::launch_game()
{
    renderer->set(context, compute_FourierAmplitudes_Kernel, compute_inverse_Fourier_Transform, queue);
    renderer->start_game_loop() ;
}

void Builder::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Builder::~Builder()
{
    delete bar ;
    bar = NULL ; 
    delete renderer ;
    renderer = NULL ;
    glfwDestroyWindow(window) ;
    glfwTerminate() ;
}
