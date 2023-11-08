__constant float gravity = 9.81 ;


float calculate_length( float2 a )
{
    return sqrt(dot(a, a)) ;
}

float dispersion_relation( float2 wavevector )
{
    return sqrt( gravity * calculate_length(wavevector) ) ;
}

__kernel void compute_FourierAmplitudes( __global float2 *fourier_grid, __global float2 *wavevectors, __global float4 *fourier_amplitudes, float time, int N )
{
    uint i = get_global_id(0) ;
    uint j = get_global_id(1) ;

    if( (i<N) && (j<N) ){
        float phase = dispersion_relation(wavevectors[i * N + j]) * time ;
        float current_fourier_real = fourier_grid[i * N + j].x ;
        float current_fourier_imag = fourier_grid[i * N + j].y ;
        float ky = wavevectors[i * N + j].y ;
        float kx = wavevectors[i * N + j].x ;
        fourier_amplitudes[i * N + j].x = current_fourier_real * cos(phase) - current_fourier_imag * sin(phase) ;
        fourier_amplitudes[i * N + j].y = current_fourier_imag * cos(phase) + current_fourier_real * sin(phase) ;
        fourier_amplitudes[i * N + j].z = -kx * current_fourier_imag ;
        fourier_amplitudes[i * N + j].w = -ky * current_fourier_real ;
        
        if ((i == 0 && j == 0) || (i == N/2 && j == N/2))
            fourier_amplitudes[i * N + j].y = 0.0f;
    }
}

__kernel void inverse_Fourier_Transform( __global float4 *fourier_amplitudes, write_only image2d_t height_map, __global float2 *wavevectors, float time, int N, float L )
{
    uint i = get_global_id(0) ;
    uint j = get_global_id(1) ;
    
    if( (i<N) && (j<N) ){
        float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f) ;
        float2 spatial_position = (float2)( ((i-N/2.0f)*(L/N)), ((j-N/2.0f)*(L/N)) ) ;
        for( int m = 0 ; m < N ; m++ ){
            for ( int n = 0 ; n < N ; n++ ){
                float phase = dot( wavevectors[(m*N)+n], spatial_position ) ;
                float2 exponential = (float2)( cos(phase), sin(phase) ) ;
                sum.x += (fourier_amplitudes[(m*N)+n].x * exponential.x) - (fourier_amplitudes[(m*N)+n].y * exponential.y) ;
                sum.y += (fourier_amplitudes[(m*N)+n].x * exponential.y) + (fourier_amplitudes[(m*N)+n].y * exponential.x) ;
                sum.z += (fourier_amplitudes[(m*N)+n].z * exponential.x) - (fourier_amplitudes[(m*N)+n].w * exponential.y) ;
                sum.w += (fourier_amplitudes[(m*N)+n].z * exponential.y) + (fourier_amplitudes[(m*N)+n].w * exponential.x) ;
            }
        }
        sum = sum / (float)(N*N) ;
        float4 data = (float4)( sum.x, sum.z, sum.w, 1.0f ) ;
        write_imagef(height_map, (int2)(i, j), data);
    }
}
