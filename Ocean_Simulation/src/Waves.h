#include "Skybox.h"



class Waves{
    
private:
    
    cl_int err ;
    int N, num_bits ;
    cl_context context ;
    cl_command_queue queue ;
    unsigned int height_map ;
    glm::vec2 wind_direction ;
    std::vector<glm::vec2> fourier_grid, wavevectors ;
    std::vector<glm::vec4> fourier_amplitudes, output ;
    cl_kernel compute_FourierAmplitudes_kernel, compute_inverseFourierTransform_kernel ;
    cl_mem heightMapCLImage, fourierAmplitudes, wave_vectors, fourierGrid, output_buffer ;
    float dampening_constant, gravity, wind_speed, phillips_constant, L, time, fourier_constant ;
    
public:
    
    Waves() ;
    void set_time( float time ) ;
    void set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue ) ;
    void create_texture() ;
    void initialize_Fourier_grid() ;
    void set_kernel_arguments() ;
    void compute_waves() ;
    float generate_gaussian_random( float mean, float standard_deviation ) ;
    float compute_phillips_spectrum( glm::vec2 wavevector, float wavevector_magnitude ) ;
    glm::vec2 generate_wavevector(float i, float j) ;
    void activate_waves() ;
    void generate_error( std::string error_msg ) ;
    ~Waves() ;
} ;
