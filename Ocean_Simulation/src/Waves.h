#include "Boat.h"



class Waves{
    
private:
    
    float time ;
    float Lx, Ly ;
    cl_int err ;
    cl_context context ;
    cl_command_queue queue ;
    glm::vec2 wind_direction ;
    std::vector<glm::vec2> height_map ;
    float N, M, dampening_constant ;
    unsigned int fourier_grid_texture ;
    std::vector<glm::vec2> fourier_grid, wavevectors ;
    std::vector<glm::vec4> fourier_amplitudes ;
    float gravity, wind_speed, phillips_constant ;
    cl_kernel compute_FourierAmplitudes_kernel, compute_inverseFourierTransform_kernel ;
    cl_mem heightMapCLImage ;
    cl_mem fourierAmplitudes ;
    
public:
    
    Waves() ;
    void set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue ) ;
    void compute_waves() ;
    void set_time( float time ) ; 
    void create_texture() ;
    void initialize_FourierGrid() ;
    void set_kernel_arguments() ; 
    float gaussian_random( float min, float max ) ;
    float phillips_spectrum( glm::vec2 wavevector ) ;
    float dispersion_relation( glm::vec2 wavevector ) ;
    glm::vec2 generate_wavevector(float i, float j) ;
    void activate_waves() ;
    void generate_error( std::string msg ) ;
    ~Waves() ;
} ;
