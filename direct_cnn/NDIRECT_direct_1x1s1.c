//Authors: 
#include "./kernel/kernel.h"
#include "NDIRECT_direct.h"

void NDIRECT_conv_direct_1x1_fwd_exec(int H, int W, int N, int C, float *input,
                                        int K, int R, int S, float *filter,
                                        int padh, int padw, int stride, float *output)
{

	//omp_set_num_threads(num);
	void *ptr, *ptr1;
	int Tn = NUM;
	int Tm = NUM / Tn;
	posix_memalign(&ptr, 64, NUM * CONV_C * 12 * sizeof( float ));
	posix_memalign(&ptr1, 64, CONV_C * Tm * CONV_K * sizeof( float ));
	float *NDIRECT_input = (float *)ptr;
	float *NDIRECT_filter = (float *)ptr1;

	int input_HW_size = H * W;


	// -----------note------------------
	int Num_K_block = (K / Tm + CONV_K - 1) / CONV_K;
	int Num_C_block = (C + CONV_C - 1) / CONV_C;

	int dim_size = Num_K_block * Num_C_block * Tn;


	bool *Barrier_tag1 = ( bool * ) malloc( Tm * dim_size * sizeof(bool));
	bool *Barrier_tag2 = ( bool * ) malloc( Tm * dim_size *sizeof(bool));

	int thread_num_inputs = N / Tn;
	int img_Tn = Tn / N;
	if(thread_num_inputs > 1)
		img_Tn = 1;
	else
		thread_num_inputs = 1;

	int gride_HW_size = input_HW_size / img_Tn;
	int gride_K_size = K / Tm;
	int gride_HW_e = input_HW_size % img_Tn;
	int gride_K_e = K % Tm;


	#pragma omp parallel num_threads(NUM)
	{

		int i, j, k, C_index, K_index, pre_index;
		int id = omp_get_thread_num();
		int ii, jj, cc, nn, iis, jjs, ccs, HW_to, K_to, C_to;
		int HWb, Kb, Cb;
		int N_id = id / Tn;

		for ( i = id; i < Tm * dim_size ; i = i + NUM )
		{
			Barrier_tag1[ i ] = 0;
			Barrier_tag2[ i ] = 0;
		}

		if (id  % img_Tn < gride_HW_e)
		{
			jjs = (id % img_Tn) * (gride_HW_size + 1);
			HW_to = jjs + (gride_HW_size + 1);
		}
		else
		{
			jjs = (id % img_Tn) * gride_HW_size + gride_HW_e;
			HW_to = jjs + gride_HW_size;
		}

		iis = N_id * gride_K_size;
		K_to = iis + gride_K_size;

		ccs = 0;
		C_to = C;

		#pragma omp barrier

		for ( jj = jjs; jj < HW_to; jj = jj + HWb)
		{
			HWb = CONV_HW;
			if (HW_to - jj < CONV_HW)
			{
				HWb = HW_to - jj;
			}

			for (cc = ccs; cc < C_to; cc = cc + Cb)
			{

				C_index = cc / CONV_C;
				Cb = CONV_C;
				if (C_to - cc < CONV_C)
					Cb = C_to - cc;

				float *buffer_input = input + ((id % Tn) / img_Tn)  * thread_num_inputs*
									  C * input_HW_size + cc * input_HW_size + jj;
				for ( ii = iis ; ii < K_to; ii = ii + Kb)
				{
					
					pre_index = C_index * Num_K_block * Tn + (ii - iis) / CONV_K * Tn;
					Kb = CONV_K;
					if (K_to - ii < CONV_K)
					{
						Kb = K_to - ii;
					}

					float *buffer_filter = filter + ii * C + cc;
					float *buffer_output = output + ( (id % Tn) / img_Tn) *thread_num_inputs  
											* K * input_HW_size + ii * input_HW_size + jj ;

					int Num_K_tiles = (Kb + 7) / 8;
					float *temp_NDIRECT_filter = NDIRECT_filter + (id / Tn)
					                               * CONV_C * CONV_K;
					int EDGE_Kb = Kb % 8;


					for (i = id % Tn; i < Num_K_tiles; i = i + Tn)
					{
						float *temp_buffer_filter = buffer_filter + i * 8 * C;
						int panel_size = 8;
						if (EDGE_Kb != 0 && i == Num_K_tiles - 1)
							panel_size = EDGE_Kb;
						NDIRECT_copy_KCRS_to_KCRS_cb_kb_fp32(temp_buffer_filter,
						                                       temp_NDIRECT_filter + i * 8 * Cb,
						                                       panel_size, Cb, C, R, S);
					}

					
					Barrier_tag1 [N_id * dim_size + pre_index + id % Tn] = 1;

					for (j = 0 ; j < Tn; j++)
					{
						while (Barrier_tag1[N_id * dim_size + pre_index + j] == 0)
						{
							asm volatile("nop \n");
						};
					}
					
					for(nn = 0; nn < thread_num_inputs; nn++)
					{
						float *buffer_output1 = buffer_output + nn * K * input_HW_size;
						float *buffer_input1 = buffer_input + nn * C * input_HW_size;
						int EDGE_HWb = HWb % 12;
						int LEN_HWb = HWb - EDGE_HWb;
						if (LEN_HWb > 0)
							SMM(buffer_output1, temp_NDIRECT_filter, buffer_input1,
							    Kb, LEN_HWb, Cb, input_HW_size, C, &NDIRECT_input[id * CONV_C * 12], cc);
						if (EDGE_HWb != 0)
						{
							float *temp_buffer_input = buffer_input1 + LEN_HWb;
							float *temp_buffer_output = buffer_output1 + LEN_HWb;

							if (EDGE_HWb >= 8)
							{
								SMM_NN_KERNEL8x8(temp_buffer_output, temp_NDIRECT_filter,
								                 temp_buffer_input, Kb, 8, Cb, input_HW_size, C,
								                 &NDIRECT_input[id * CONV_C * 12], cc);
								EDGE_HWb = EDGE_HWb - 8;
								temp_buffer_input = temp_buffer_input + 8;
								temp_buffer_output = temp_buffer_output + 8;
							}

							if (EDGE_HWb >= 4)
							{

								SMM_NN_KERNEL8x4(temp_buffer_output, temp_NDIRECT_filter,
								                 temp_buffer_input, Kb, 4, Cb, input_HW_size, C,
								                 &NDIRECT_input[id * CONV_C * 12], cc);
								EDGE_HWb = EDGE_HWb - 4;
								temp_buffer_input = temp_buffer_input + 4;
								temp_buffer_output = temp_buffer_output + 4;
							}

							if (EDGE_HWb >= 2)
							{
								SMM_NN_KERNEL8x2(temp_buffer_output, temp_NDIRECT_filter,
								                 temp_buffer_input, Kb, 2, Cb, input_HW_size, C,
								                 &NDIRECT_input[id * CONV_C * 12], cc);

								EDGE_HWb = EDGE_HWb - 2;
								temp_buffer_input = temp_buffer_input + 2;
								temp_buffer_output = temp_buffer_output + 2;
							}

							if (EDGE_HWb >= 1)
							{
								SMM_NN_KERNEL8x1(temp_buffer_output, temp_NDIRECT_filter,
								                 temp_buffer_input, Kb, 1, Cb, input_HW_size, C,
								                 &NDIRECT_input[id * CONV_C * 12], cc);
							}
						}
					}


					Barrier_tag2 [N_id * dim_size + pre_index + id % Tn] = 1;

					for (j = 0 ; j < Tn; j++)
					{
						while (Barrier_tag2[N_id * dim_size + pre_index + j] == 0)
						{
							asm volatile("nop \n");
						};
					}

				}
			}

		}
		
	}

	free(NDIRECT_input);
	free(NDIRECT_filter);
	
	free(Barrier_tag1);
	free(Barrier_tag2);
	
}

