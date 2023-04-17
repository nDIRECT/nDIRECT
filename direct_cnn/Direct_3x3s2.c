//Authors: pengyu wang
#include "./kernel/3x3s2.h"

#include "NDIRECT_direct.h"
void LIB_R3_s2(int H, int W, int N, int C, float *input, int K, int R, int S, float *filter, int padh, int padw, int stride, float* output){
    
    int out_h = (H - R + 2*padh)/stride + 1;
    
    int out_w = out_h;
    
    int per_out_batch = K * out_h * out_w;
    
    int per_img_batch = C * H * W;
    
    int kc_tile = (K<128) ? K:128;

    int ic_tile = (C<32) ? C:32;

    int ic_out_tile = C; 
    
    int kc_out_tile = (C<512)? K : 128; 
    
    int threads = K / kc_out_tile;

    
    float *out_kernel=(float*)malloc(K* C * R * S * sizeof(float));
    
    int i;
    
    transform_filter_paral_ass_s2(K, C, R, S, filter, out_kernel, kc_tile, ic_tile, ic_out_tile, kc_out_tile, threads);
                
               
#   pragma omp parallel for num_threads(N)
    for(i=0; i<N; i++){
        direct_3x3s2(out_kernel, input + i*per_img_batch, output + i*per_out_batch, C, K, H, out_h, R, padh, stride, kc_tile, ic_tile, ic_out_tile, kc_out_tile);
    }


    
    free(out_kernel);

}



