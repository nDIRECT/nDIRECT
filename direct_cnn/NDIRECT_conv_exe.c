#include "NDIRECT_direct.h"

void NDIRECT_dnn_conv_fwd_exec(int H, int W, int N, int C, float *input,
                                 int K, int R, int S, float *filter,
                                 int padh, int padw, int stride, float* output)
{

	if(R == 1)
	{

		Dete_grad_N_threads_nums(N, K, H, W, stride);
		if (R == 1 && S == 1 && stride == 2)
		{
			NDIRECT_conv_direct_1x1s2_fwd_exec(H, W, N, C, input,
			                                   K, 1, 1, filter,
			                                   padh, padw, 2, output);
		}
		else if(R == 1 && S == 1 && stride == 1)
		{
			NDIRECT_conv_direct_1x1_fwd_exec(H, W, N, C, input,
			                                   K, 1, 1, filter,
			                                   padh, padw, 1, output);
		}
	}
	else if(R == 3)
	{
        if(stride==1)
        {
            LIB_R3_s1(H, W, N , C, input, K, R, S, filter, 
            	padh, padw, stride, output);
        }
        if(stride==2)
        {
            
            LIB_R3_s2(H, W, N , C, input, K, R, S, filter, 
            	padh, padw, stride, output);
        }
    }
	else
	{
		LIB_R7_s2(H, W, N , C, input, K, R, S, filter, 
			padh, padw, stride, output);
	}
}
