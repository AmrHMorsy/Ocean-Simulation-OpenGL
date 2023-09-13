#include "Waves.h"



Waves::Waves(): N(64), M(64), Lx(128), Ly(128), gravity(9.81f), wind_speed(10.0f), phillips_constant(1.0f), dampening_constant(1.0f), wind_direction(glm::normalize(glm::vec2(16.0f,0.0f))), fourier_grid(M*N), fourier_amplitudes(M*N), wavevectors(M*N)
{
    height_map.resize(M*N) ;
        
    create_texture() ;
}

void Waves::set_time( float time )
{
    this->time = time ;
    err = clSetKernelArg(compute_FourierAmplitudes_kernel, 3, sizeof(float), &time) ;
    err = clSetKernelArg(compute_inverseFourierTransform_kernel, 3, sizeof(float), &time) ;
}

void Waves::set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue )
{
    this->queue = queue ;
    this->context = context ;
    compute_FourierAmplitudes_kernel = FA ;
    compute_inverseFourierTransform_kernel = IFT ;
}

void Waves::create_texture()
{
    glGenTextures(1, &fourier_grid_texture) ;
    glBindTexture(GL_TEXTURE_2D, fourier_grid_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, N, M, 0, GL_RGBA, GL_FLOAT, NULL) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;

    initialize_FourierGrid() ;
}

void Waves::initialize_FourierGrid()
{
    glm::vec2 amplitude ;
    float a, b, magnitude, philip_spectrum ;
    for( float i = 0 ; i < N ; i++ ){
        for( float j = 0 ; j < M ; j++ ){
            a = gaussian_random(0.0f, 1.0f) ;
            b = gaussian_random(0.0f, 1.0f) ;
            wavevectors[(i*N)+j] = generate_wavevector(i,j) ;
            philip_spectrum = phillips_spectrum(wavevectors[(i*N)+j]) ;
            if( philip_spectrum < 0 )
                generate_error("phillips_spectrum is less than 0") ;
            magnitude = (1.0f / std::sqrt(2.0f)) * std::sqrt(philip_spectrum) ;
            amplitude.x = a * magnitude ;
            amplitude.y = b * magnitude ;
            fourier_grid[(i*N)+j] = amplitude ;
        }
    }
}

void Waves::set_kernel_arguments()
{
    cl_mem fourierGrid = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR, N*M*2*sizeof(float), fourier_grid.data() , &err) ;
    cl_mem wave_vectors = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR, N*M*2*sizeof(float), wavevectors.data() , &err) ;
    fourierAmplitudes = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR, N*M*4*sizeof(float), fourier_amplitudes.data(), &err) ;
    heightMapCLImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, fourier_grid_texture, &err);
    err = clSetKernelArg(compute_FourierAmplitudes_kernel, 0, sizeof(cl_mem), &fourierGrid) ;
    err |= clSetKernelArg(compute_FourierAmplitudes_kernel, 1, sizeof(cl_mem), &wave_vectors) ;
    err |= clSetKernelArg(compute_FourierAmplitudes_kernel, 2, sizeof(cl_mem), &fourierAmplitudes) ;
    err |= clSetKernelArg(compute_FourierAmplitudes_kernel, 3, sizeof(float), &time) ;
    err = clSetKernelArg(compute_inverseFourierTransform_kernel, 0, sizeof(cl_mem), &fourierAmplitudes) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 1, sizeof(cl_mem), &heightMapCLImage) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 2, sizeof(cl_mem), &wave_vectors) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 3, sizeof(float), &time) ;
}

void Waves::compute_waves()
{
    glFinish() ;
    size_t global_work_size[2] = {static_cast<size_t>(N), static_cast<size_t>(M)};
    size_t local[2] = {16, 16};
    cl_event event_for_kernel1;
    err = clEnqueueNDRangeKernel(queue, compute_FourierAmplitudes_kernel, 2, NULL, global_work_size, local, 0, NULL, &event_for_kernel1) ;
    if (err != CL_SUCCESS)
        std::cout << "compute_FourierAmplitudes_kernel failed" << std::endl ;
    clFinish(queue) ;
    cl_event event_for_kernel2 ;
    err = clEnqueueNDRangeKernel(queue, compute_inverseFourierTransform_kernel, 2, NULL, global_work_size, local, 1, &event_for_kernel1, NULL) ;
    clFinish(queue) ;
    activate_waves() ;
}

float Waves::gaussian_random( float min, float max )
{
    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    static std::normal_distribution<float> distribution(min, max);
    return distribution(generator);
}

float Waves::phillips_spectrum( glm::vec2 wavevector )
{
    float wavevector_magnitude = glm::length(wavevector) ;
    float windspeed_squared = (wind_speed*wind_speed) ;
    float largest_wave = windspeed_squared / gravity ;
    float kdotw = glm::dot(wavevector, wind_direction) ;
    float cosine = pow( kdotw, 2 ) ;
    float mull = wavevector_magnitude*largest_wave ;
    float mullpow = pow(mull,2) ;
    float expon = exp( -1 / mullpow ) ;
    float powr = pow(wavevector_magnitude,4) ;
    float frac = (1/powr) ;
    float exponential =  expon * frac ;
    float f1 = wavevector_magnitude*wavevector_magnitude ;
    float f2 = dampening_constant*dampening_constant ;
    float dampening_factor = exp(-f1*f2) ;
    float phillips_spectrum = phillips_constant * exponential * cosine ;
    phillips_spectrum = phillips_spectrum * dampening_factor ;
    return phillips_spectrum ;
}

float Waves::dispersion_relation( glm::vec2 wavevector )
{
    return sqrt( gravity * glm::length(wavevector)) ;
}

glm::vec2 Waves::generate_wavevector(float i, float j)
{
    float dkx = 2.0f * M_PI / Lx;
    float dky = 2.0f * M_PI / Ly;
    float kx = ((i-N) / 2.0f) * dkx;
    float ky = ((j-M) / 2.0f) * dky;
    return glm::vec2(kx, ky);
}

void Waves::activate_waves()
{
    glActiveTexture(GL_TEXTURE8) ;
    glBindTexture(GL_TEXTURE_2D, fourier_grid_texture) ;
}

void Waves::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Waves::~Waves()
{
    glDeleteTextures(1, &fourier_grid_texture) ;
}
