#include "Waves.h"



Waves::Waves(): N(64), L(128), gravity(9.81f), wind_speed(100.0f), phillips_constant(1.0f), wind_direction(glm::normalize(glm::vec2(1.0f,1.0f))), fourier_grid(N*N), fourier_amplitudes(N*N), wavevectors(N*N), fourier_constant( (1.0f / std::sqrt(2.0f)) ), dampening_constant(1.5f)
{
    create_texture() ;
    initialize_Fourier_grid() ;
}

void Waves::set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue )
{
    this->queue = queue ;
    this->context = context ;
    compute_FourierAmplitudes_kernel = FA ;
    compute_inverseFourierTransform_kernel = IFT ;
}

void Waves::set_time( float time )
{
    this->time = time ;
    err = clSetKernelArg(compute_FourierAmplitudes_kernel, 3, sizeof(float), &time) ;
    err = clSetKernelArg(compute_inverseFourierTransform_kernel, 3, sizeof(float), &time) ;
}

void Waves::create_texture()
{
    glGenTextures(1, &height_map) ;
    glBindTexture(GL_TEXTURE_2D, height_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, N, N, 0, GL_RGBA, GL_FLOAT, NULL) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
}

glm::vec2 Waves::generate_wavevector( float i, float j )
{
    float dkx = 2.0f * M_PI / L ;
    float dky = 2.0f * M_PI / L ;
    float ni = ( i < (N/2) ) ? i : (i-N) ;
    float nj = ( j < (N/2) ) ? j : (j-N) ;
    float kx = ni * dkx ;
    float ky = nj * dky ;
    return glm::vec2(kx, ky) ;
}

float Waves::generate_gaussian_random( float mean, float standard_deviation )
{
    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    static std::normal_distribution<float> distribution(mean, standard_deviation);
    return distribution(generator);
}

float Waves::compute_phillips_spectrum( glm::vec2 wavevector, float wavevector_magnitude )
{
    if( wavevector_magnitude == 0 )
        return 0.0f ;
    float cosine = glm::dot(wavevector, wind_direction) ;
    cosine = pow(cosine, 2) ;
    float largest_wave = pow(wind_speed,2) / gravity ;
    float exponential = wavevector_magnitude * largest_wave ;
    exponential = pow(exponential,2) ;
    exponential = exp( -1 / exponential ) ;
    exponential = exponential / pow(wavevector_magnitude,4) ;
    float dampening_factor = wavevector_magnitude * dampening_constant ;
    dampening_factor = pow( dampening_factor, 2 ) ;
    dampening_factor = exp( - dampening_factor ) ;
    float phillips_spectrum = phillips_constant * exponential * cosine * dampening_factor ;
    if( phillips_spectrum < 0 )
        generate_error("ERROR: Phillips Spectrum < 0") ;
    return phillips_spectrum ;
}

void Waves::initialize_Fourier_grid()
{
    float gaussian_random_real, gaussian_random_imag, philip_spectrum ;
    for( float i = 0 ; i < N ; i++ ){
        for( float j = 0 ; j < N ; j++ ){
            wavevectors[(i*N)+j] = generate_wavevector(i,j) ;
            gaussian_random_real = generate_gaussian_random(0.0f, 1.0f) ;
            gaussian_random_imag = generate_gaussian_random(0.0f, 1.0f) ;
            philip_spectrum = compute_phillips_spectrum( wavevectors[(i*N)+j], glm::length(wavevectors[(i*N)+j]) ) ;
            fourier_grid[(i*N)+j].x = gaussian_random_real * fourier_constant * std::sqrt(philip_spectrum) ;
            fourier_grid[(i*N)+j].y = gaussian_random_imag * fourier_constant * std::sqrt(philip_spectrum) ;
        }
    }
}

void Waves::set_kernel_arguments()
{
    cl_mem fourierGrid = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR, N*N*2*sizeof(float), fourier_grid.data() , &err) ;
    cl_mem wave_vectors = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR, N*N*2*sizeof(float), wavevectors.data() , &err) ;
    fourierAmplitudes = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR, N*N*4*sizeof(float), fourier_amplitudes.data(), &err) ;
    heightMapCLImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, height_map, &err);
    err  = clSetKernelArg(compute_FourierAmplitudes_kernel, 0, sizeof(cl_mem), &fourierGrid) ;
    err |= clSetKernelArg(compute_FourierAmplitudes_kernel, 1, sizeof(cl_mem), &wave_vectors) ;
    err |= clSetKernelArg(compute_FourierAmplitudes_kernel, 2, sizeof(cl_mem), &fourierAmplitudes) ;
    err |= clSetKernelArg(compute_FourierAmplitudes_kernel, 3, sizeof(float), &time) ;
    err |= clSetKernelArg(compute_FourierAmplitudes_kernel, 4, sizeof(int), &N) ;
    err  = clSetKernelArg(compute_inverseFourierTransform_kernel, 0, sizeof(cl_mem), &fourierAmplitudes) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 1, sizeof(cl_mem), &heightMapCLImage) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 2, sizeof(cl_mem), &wave_vectors) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 3, sizeof(float), &time) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 4, sizeof(int), &N) ;
    err |= clSetKernelArg(compute_inverseFourierTransform_kernel, 5, sizeof(float), &L) ;
}

void Waves::compute_waves()
{
    glFinish() ;
    size_t global_work_size[2] = {static_cast<size_t>(N), static_cast<size_t>(N)};
    size_t local[2] = {8,8} ;
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

void Waves::activate_waves()
{
    glActiveTexture(GL_TEXTURE8) ;
    glBindTexture(GL_TEXTURE_2D, height_map) ;
}

void Waves::generate_error( std::string error_msg )
{
    std::cout << "\033[1;31m" << error_msg << "\033[0m" << std::endl;
    exit(1) ;
}

Waves::~Waves()
{
    glDeleteTextures(1, &height_map) ;
}
