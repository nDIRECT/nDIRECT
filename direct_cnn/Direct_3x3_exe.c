#include "NDIRECT_direct.h"

void Direct_conv3x3_run(int H, int W, int N, int C, float *input, int K, int R, int S, float *filter, int padh, int padw, int stride, float* output){
    
    if(R==3){
        if(stride==1){
            LIB_R3_s1(H, W, N , C, input, K, R, S, filter, padh, padw, stride, output);
        }
        if(stride==2){
            
            LIB_R3_s2(H, W, N , C, input, K, R, S, filter, padh, padw, stride, output);
        }
    }
    
    else if(R==7){
        
        LIB_R7_s2(H, W, N , C, input, K, R, S, filter, padh, padw, stride, output);
        
    }
}