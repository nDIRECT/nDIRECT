#include "NDIRECT_direct.h"

void verify(float *output, float *output1, int N, int K, int H, int W){

        int out_num = N * K * H * W;
        
        
        
        int k_flag = -1;
        for(int i=0; i<out_num; i++){
            if(abs(output[i] - output1[i]) > 0.001){
                printf("Direct result error ! i: %d, i_out: %f, d_out: %f\n", i, output[i], output1[i]);
                k_flag = 0;
            }
        }
        if (k_flag == -1)
            printf("Right !\n");
    
}
