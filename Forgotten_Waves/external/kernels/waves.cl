__constant float gravity = 9.81 ;
__constant int N = 64 ;
__constant int M = 64 ;

float calculate_length( float2 a )
{
    return sqrt(dot(a, a)) ;
}

float dispersion_relation( float2 wavevector )
{
    return sqrt( gravity * calculate_length(wavevector)) ;
}

__kernel void compute_FourierAmplitudes(__global float2 *fourier_grid, __global float2 *wavevectors, __global float4 *fourier_amplitudes, float time )
{
    float frequency ;
    uint i = get_global_id(0) ;
    uint j = get_global_id(1) ;

    frequency = dispersion_relation(wavevectors[i * N + j]) * time ;
    float current_fourier_real = fourier_grid[i * N + j].x ;
    float current_fourier_imag = fourier_grid[i * N + j].y ;
    fourier_amplitudes[i * N + j].x = current_fourier_real * cos(frequency) + current_fourier_imag * sin(frequency) ;
    fourier_amplitudes[i * N + j].y = current_fourier_imag * cos(frequency) - current_fourier_real * sin(frequency) ;

    float ky = wavevectors[i * N + j].y ;
    float kx = wavevectors[i * N + j].x ;
    float fx = current_fourier_real ;
    float fy = current_fourier_imag  ;

    fourier_amplitudes[i * N + j].z = (-ky*fx) - (kx*fy) ;
    fourier_amplitudes[i * N + j].w = (kx*fx) - (ky*fy) ;
}

__kernel void inverse_Fourier_Transform( __global float4 *fourier_amplitudes, write_only image2d_t height_map, __global float2 *wavevectors, float time )
{
    uint i = get_global_id(0) ;
    uint j = get_global_id(1) ;
    float4 sum ;
    sum.x = 0.0f ;
    sum.y = 0.0f ;
    sum.z = 0.0f ;
    sum.w = 0.0f ;
    for( int m = 0 ; m < N ; m++ ){
        for ( int n = 0 ; n < M ; n++ ){
            float2 tmp ;
            tmp.x = i ;
            tmp.y = j ;
            float exponential = dot(wavevectors[(m*N)+n], tmp ) ;
            float2 exp ;
            exp.x = cos(exponential) ;
            exp.y = sin(exponential) ;
            sum.x += fourier_amplitudes[(m*N)+n].x * exp.x - fourier_amplitudes[(m*N)+n].y * exp.y;
            sum.y += fourier_amplitudes[(m*N)+n].x * exp.y + fourier_amplitudes[(m*N)+n].y * exp.x;
            sum.z += fourier_amplitudes[(m*N)+n].z * exp.x - fourier_amplitudes[(m*N)+n].w * exp.y;
            sum.w += fourier_amplitudes[(m*N)+n].z * exp.y + fourier_amplitudes[(m*N)+n].w * exp.x;
        }
    }
    sum = sum / (N*M) ;
    float slope_x = sum.z;
    float slope_z = sum.w ;
    float4 col = (float4)( sum.x, slope_x, slope_z, 1.0f ) ;
    int2 coord = (int2)(i, j);
    write_imagef(height_map, coord, col);
    
//    float3 normal = (float3)(100*slope_x, 1.0f, 100*slope_z) ;
//    float mag = sqrt( (normal.x*normal.x) + (normal.y*normal.y) + (normal.z*normal.z) ) ;
//    normal = normal / mag ;
//    printf("%f %f %f \n", normal.x, normal.y, normal.z) ;
    
//    float2 normal = (float2)(slope_x, slope_z) ;
//    float mag = sqrt( (normal.x*normal.x) + (normal.y*normal.y) ) ;
//    normal = normal / mag ;
//    printf("%f %f \n", slope_x, slope_z) ;
//
}
