#include "NDIRECT_direct.h"


void NDIRECT_flush()
{

  float *flush = ( float * ) malloc( 10000 * 10000* sizeof( float ) );
  float *dirty = ( float * ) malloc( 10000 * 10000* sizeof( float ) );
  #pragma omp parallel for num_threads(NUM)
  for(int i =0 ; i < 10000 * 10000; i ++)
  {
  	flush[i] = 0.5 + 0.00001 * (i % 100) ;
  	dirty[i] = 0.00001 * (i % 1000) + 0.123;
  }

  #pragma omp parallel for num_threads(NUM)
  for(int i =0 ; i < 10000 * 10000; i ++)
  {
  	dirty[i] = dirty[i % 64] + 0.1 * flush[i];
  }
  free(flush);
  free(dirty);
}


void Check_result_s1(float *output, float *output1, int N, int K, int H, int W)
{

	int i, j;
	int flag = 0;
	for ( i = 0; i < N; i++)
	{
		for ( j = 0 ; j < K * H * W; j++)
		{
			if (fabs(output[i * K * H * W + j] - output1[i * K * H * W + j]) > 1.0e-03)
			{
				printf("i = %d, j= %d\n", i , j );
				printf("output= %lf , output1= %lf\n", output[i * K * H * W + j], output1[i *K * H * W + j]);
				flag = 1;
				printf("error\n");
				break;
			}
		}
		if (flag == 1)
			break;
	}

	if (flag == 0)
		printf("结果正确\n");
}


void Check_result_s2(float *output, float *output1, int N, int K, int H, int W)
{

	int i, j, k;
	int flag = 0;

	float *output_sgemm = ( float * ) malloc(N * K * H * W * sizeof( float ) );


	for(i = 0; i < N * K; i++)
	{
		for(j = 0; j < H; j++)
		{
			for(k = 0; k < W; k++)
			{
				output_sgemm[i * H * W + j * W + k] = 
				output1[i * H * W * 4 + j * W * 4 + k *2];
			}
		}
	}


	
	//printf("H * W = %d\n", H * W);
	for ( i = 0; i < N * K; i++)
	{
		for ( j = 0 ; j < H * W; j++)
		{
			if (fabs(output[i * H * W + j] - output_sgemm[i * H * W + j]) > 1.0e-03)
			{
				printf("i = %d, j= %d\n", i , j );
				printf("output= %lf , output1= %lf\n", output[i * H * W + j], output1[i * H * W + j]);
				flag = 1;
				printf("error\n");
				break;
			}
		}
		if (flag == 1)
			break;
	}

	free(output_sgemm);
	if (flag == 0)
		printf("结果正确\n");
}


void NDIRECT_copy_KCRS_to_KCRS_cb_kb_fp32(float* filter, float* NDIRECT_filter,
                long Kb, long Cb, long C, long R, long S)
{

	int ii, jj, kk;
	long filter_stride = R * S * C;

	if (Kb == 8)
	{
		float *temp = filter;

		asm volatile(
		        "	ldr		x0, %[NDIRECT_filter]				\n"
		        "	ldr		x1, %[Cb]							\n"		//
		        "	ldr		x2, %[temp]							\n"		//
		        "	ldr		x30, %[filter_stride]				\n"

		        "	add		x3, x2, x30, lsl #2					\n"
		        "	add		x4, x2, x30, lsl #3					\n"
		        "	add		x5, x3, x30, lsl #3					\n"
		        "	add		x6, x4, x30, lsl #3					\n"
		        "	add		x7, x5, x30, lsl #3					\n"
		        "	add		x8, x6, x30, lsl #3					\n"
		        "	add		x9, x7, x30, lsl #3					\n"

		        "	lsr		x21, x1, #3							\n"
		        "	cmp		x21, #0								\n"
		        "	beq		Filter_copy_END						\n"

		        "Filter_copy:									\n"

		        "	prfm	PLDL1KEEP, [x2, #128]				\n"
		        "	prfm	PLDL1KEEP, [x3, #128]				\n"

		        "	ldr		q0, [x2], #16						\n"
		        "	ldr		q1, [x3], #16						\n"
		        "	ldr		q2, [x4], #16						\n"
		        "	ldr		q3, [x5], #16						\n"

		        "	prfm	PLDL1KEEP, [x4, #128]				\n"
		        "	prfm	PLDL1KEEP, [x5, #128]				\n"

		        "	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"

		        "	ldr		q4, [x6], #16								\n"
		        "	ldr		q5, [x7], #16								\n"
		        "	ldr		q6, [x8], #16								\n"
		        "	ldr		q7, [x9], #16								\n"

		        "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

		        "	prfm	PLDL1KEEP, [x6, #128]				\n"
		        "	prfm	PLDL1KEEP, [x7, #128]				\n"

		        "	ldr		q8, [x2], #16								\n"
		        "	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
		        "	ldr		q9, [x3], #16								\n"
		        "	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
		        "	ldr		q10, [x4], #16							\n"
		        "	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
		        "	ldr		q11, [x5], #16							\n"
		        "	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"

		        "	prfm	PLDL1KEEP, [x8, #128]				\n"
		        "	prfm	PLDL1KEEP, [x9, #128]				\n"


		        "	ldr		q12, [x6], #16							\n"
		        "	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16			\n"
		        "	ldr		q13, [x7], #16							\n"
		        "	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16			\n"
		        "	ldr		q14, [x8], #16							\n"
		        "	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x0], #16		\n"
		        "	ldr		q15, [x9], #16							\n"
		        "	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x0], #16		\n"

		        "	subs	x21, x21, #1							\n"

		        "	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x0], #16		\n"
		        "	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x0], #16		\n"
		        "	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x0], #16		\n"
		        "	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x0], #16		\n"
		        "	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x0], #16		\n"
		        "	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x0], #16		\n"

		        "	bgt		Filter_copy 						\n"

		        "Filter_copy_END:								\n"

		        :
		        :
		        [temp] "m" (temp),
		        [NDIRECT_filter] "m" (NDIRECT_filter),
		        [Cb] "m" (Cb),
		        [filter_stride] "m" (filter_stride)
		        :
		        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
		        "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
		        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
		        "x26", "x27", "x28", "x29", "x30",
		        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
		        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
		        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
		        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"

		);

		NDIRECT_filter = NDIRECT_filter + Cb * 8;
	}
	else
	{
		if (Kb >= 4)
		{

			float *temp = filter;

			asm volatile(
			        "	ldr		x0, %[NDIRECT_filter]				\n"
			        "	ldr		x1, %[Cb]							\n"
			        "	ldr		x2, %[temp]							\n"
			        "	ldr		x30, %[filter_stride]				\n"

			        "	add		x3, x2, x30, lsl #2					\n"
			        "	add		x4, x2, x30, lsl #3					\n"
			        "	add  	x5, x3, x30, lsl #3 				\n"

			        "	lsr		x21, x1, #2							\n"
			        "	cmp		x21, #0								\n"
			        "	beq		Filter_copy_Kb4_END					\n"

			        "Filter_copy_Kb4:								\n"

			        "	prfm	PLDL1KEEP, [x2, #64]				\n"
			        "	prfm	PLDL1KEEP, [x3, #64]				\n"

			        "	ldr		q0, [x2], #16						\n"
			        "	ldr		q1, [x3], #16						\n"
			        "	ldr		q2, [x4], #16						\n"
			        "	ldr 	q3, [x5], #16 						\n"

			        "	prfm	PLDL1KEEP, [x4, #64]				\n"
			        "	prfm	PLDL1KEEP, [x5, #64]				\n"

			        "	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16		\n"
			        "	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16		\n"
			        "	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16		\n"
			        "	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16		\n"

			        "	subs	x21, x21, #1						\n"
			        "	bgt		Filter_copy_Kb4 					\n"


			        "Filter_copy_Kb4_END:							\n"

			        :
			        :
			        [temp] "m" (temp),
			        [NDIRECT_filter] "m" (NDIRECT_filter),
			        [Cb] "m" (Cb),
			        [filter_stride] "m" (filter_stride)
			        :
			        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
			        "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
			        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
			        "x26", "x27", "x28", "x29", "x30",
			        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
			        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
			        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
			        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"

			);

			Kb = Kb - 4;
			filter = filter + 4 * filter_stride;
			NDIRECT_filter = NDIRECT_filter + 4 * Cb;

		}

		if (Kb >= 3)
		{
			float *temp = filter;

			asm volatile(
			        "	ldr		x0, %[NDIRECT_filter]				\n"
			        "	ldr		x1, %[Cb]							\n"
			        "	ldr		x2, %[temp]							\n"
			        "	ldr		x30, %[filter_stride]				\n"

			        "	add		x3, x2, x30, lsl #2					\n"
			        "	add		x4, x2, x30, lsl #3					\n"

			        "	lsr		x21, x1, #3							\n"
			        "	cmp		x21, #0								\n"
			        "	beq		Filter_copy_Kb3_END					\n"

			        "Filter_copy_Kb3:								\n"

			        "	prfm	PLDL1KEEP, [x2, #128]				\n"
			        "	prfm	PLDL1KEEP, [x3, #128]				\n"

			        "	ldr		q0, [x2], #16						\n"
			        "	ldr		q1, [x3], #16						\n"
			        "	ldr		q2, [x4], #16						\n"

			        "	prfm	PLDL1KEEP, [x4, #128]				\n"

			        "	st3		{v0.s, v1.s, v2.s}[0], [x0], #12	\n"


			        "	ldr		q8, [x2], #16						\n"
			        "	st3		{v0.s, v1.s, v2.s}[1], [x0], #12	\n"
			        "	ldr		q9, [x3], #16						\n"
			        "	ldr		q10, [x4], #16						\n"
			        "	st3		{v0.s, v1.s, v2.s}[2], [x0], #12	\n"
			        "	st3		{v0.s, v1.s, v2.s}[3], [x0], #12	\n"
			        "	st3		{v8.s, v9.s, v10.s}[0], [x0], #12	\n"

			        "	subs	x21, x21, #1						\n"

			        "	st3		{v8.s, v9.s, v10.s}[1], [x0], #12	\n"
			        "	st3		{v8.s, v9.s, v10.s}[2], [x0], #12   \n"
			        "	st3		{v8.s, v9.s, v10.s}[3], [x0], #12	\n"

			        "	bgt		Filter_copy_Kb3 					\n"

			        "Filter_copy_Kb3_END:							\n"

			        :
			        :
			        [temp] "m" (temp),
			        [NDIRECT_filter] "m" (NDIRECT_filter),
			        [Cb] "m" (Cb),
			        [filter_stride] "m" (filter_stride)
			        :
			        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
			        "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
			        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
			        "x26", "x27", "x28", "x29", "x30",
			        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
			        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
			        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
			        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"

			);

			Kb = Kb - 3;
			filter = filter + 3 * filter_stride;
			NDIRECT_filter = NDIRECT_filter + 3 * Cb;
		}

		if (Kb >= 2)
		{
			float *temp = filter;
			asm volatile(
			        "	ldr		x0, %[NDIRECT_filter]				\n"
			        "	ldr		x1, %[Cb]							\n"
			        "	ldr		x2, %[temp]							\n"
			        "	ldr		x30, %[filter_stride]				\n"


			        "	add		x3, x2, x30, lsl #2					\n"

			        "	lsr		x21, x1, #3							\n"
			        "	cmp		x21, #0								\n"
			        "	beq		Filter_copy_Kb2_END					\n"

			        "Filter_copy_Kb2:								\n"

			        "	prfm	PLDL1KEEP, [x2, #128]				\n"
			        "	prfm	PLDL1KEEP, [x3, #128]				\n"

			        "	ldr		q0, [x2], #16						\n"
			        "	ldr		q1, [x3], #16						\n"

			        "	st2		{v0.s, v1.s}[0], [x0], #8			\n"


			        "	ldr		q8, [x2], #16						\n"
			        "	st2		{v0.s, v1.s}[1], [x0], #8			\n"
			        "	ldr		q9, [x3], #16						\n"
			        "	st2		{v0.s, v1.s}[2], [x0], #8			\n"
			        "	st2		{v0.s, v1.s}[3], [x0], #8			\n"
			        "	st2		{v8.s, v9.s}[0], [x0], #8			\n"

			        "	subs	x21, x21, #1						\n"

			        "	st2		{v8.s, v9.s}[1], [x0], #8			\n"
			        "	st2		{v8.s, v9.s}[2], [x0], #8  			\n"
			        "	st2		{v8.s, v9.s}[3], [x0], #8			\n"

			        "	bgt		Filter_copy_Kb2 					\n"

			        "	ands	x22, x1, #7							\n"
			        "	beq		Filter_copy_Kb2_END					\n"

			        "Filter_copy_Kb2_END:							\n"

			        :
			        :
			        [temp] "m" (temp),
			        [NDIRECT_filter] "m" (NDIRECT_filter),
			        [Cb] "m" (Cb),
			        [filter_stride] "m" (filter_stride)
			        :
			        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
			        "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
			        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
			        "x26", "x27", "x28", "x29", "x30",
			        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
			        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
			        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
			        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"

			);

		}
	}

}