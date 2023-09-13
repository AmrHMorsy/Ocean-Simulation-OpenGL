#include "Renderer.h"



class Builder{
    
private:
    
    cl_int err ;
    progressbar * bar ;
    cl_context context ;
    cl_program program ;
    Renderer * renderer ;
    size_t program_size ;
    GLFWwindow * window ;
    cl_device_id device ;
    cl_command_queue queue ;
    cl_platform_id platform ;
    const char * program_buffer ;
    cl_kernel compute_FourierAmplitudes_Kernel, compute_inverse_Fourier_Transform ;

public:
    
    Builder() ;
    void initialize_GLFW() ;
    void set_window_attributes() ;
    void create_window() ;
    void initialize_GLEW() ;
    void setup_progress_bar() ;
    void access_openCL_platform() ;
    void access_openCL_devices() ;
    void create_openCL_context() ;
    void read_kernel_sourceCode() ;
    void create_program() ;
    void build_program() ;
    void create_kernels() ;
    void create_command_queue() ;
    void initialize_SDL() ;
    void launch_game() ;
    void generate_error( std::string msg ) ;
    ~Builder() ;
    
} ;
