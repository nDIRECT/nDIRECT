#include <malloc.h>
#include <sys/time.h>
#include <stdio.h>
//#include <cblas.h>
#include "NDIRECT_direct.h"

static double gtod_ref_time_sec = 0.0;

double dclock()
{
	double the_time, norm_sec;
	struct timeval tv;

	gettimeofday( &tv, NULL );

	if ( gtod_ref_time_sec == 0.0 )
		gtod_ref_time_sec = ( double ) tv.tv_sec;

	norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;

	the_time = norm_sec + tv.tv_usec * 1.0e-6;

	return the_time;
}

void random_matrix( int m, int n, float *a)
{
	double drand48();
	int i, j;

	for ( i = 0; i < m; i++ )
		for ( j = 0; j < n; j++ )
		{
			a[i * n + j] = 2.0 * (float)drand48( ) - 1.0 + 0.000001 * (i + j);
		}
}

int main(int argc, char* argv[])
{

	//openblas_set_num_threads(NUM);
	int i, j, k, pc;
	int loop = 20;
	double start, cost;
	double gflops;
	long lda, ldb, ldc;
	float temp = -1;
	float *filter, *input, *output, *output1;

	int H = 56, W = 56, N = 64, C = 256;
	int K = 512, R = 1, S = 1;
	int padh = 0, padw = 0, stride = 2;

	FILE *fp;
	if ( (fp = fopen("NDIRECT_CONV_FT_scalable1.txt", "a+")) == NULL )
	{
		puts("Fail to open file!");
		exit(0);
	}


	i = 1;
	if (argc > i) C      	= atoi(argv[i++]);
	if (argc > i) K        	= atoi(argv[i++]);
	if (argc > i) H        	= atoi(argv[i++]);
	if (argc > i) W       	= atoi(argv[i++]);
	if (argc > i) R       	= atoi(argv[i++]);
	if (argc > i) S       	= atoi(argv[i++]);
	if (argc > i) stride    = atoi(argv[i++]);
	if (argc > i) padh      = atoi(argv[i++]);
	if (argc > i) padw      = atoi(argv[i++]);


	for ( j = 0 ; j < 1; j++)
	{
		lda = C;
		ldb = H * W;
		ldc = H * W;

		double ops = (double) N * K * (H / stride) * (W/ stride) * C * R * S * 1.0e-09 * 2;

		fprintf(fp, "%d %d %d %d %d %d %d %d %d",C, K, H, W, R, S, stride, padh, padw);
		for (pc = 0; pc < 5; pc++)
		{

			filter = ( float * ) malloc( C * K * R *S *sizeof( float ) );
			input = ( float * ) malloc(N * C * H * W * sizeof( float ) );
			output = ( float * ) malloc(N * K * (H/stride * W/stride )* sizeof( float ) );
			output1 = ( float * ) malloc(N * K * H * W * sizeof( float ) );
			random_matrix(K, C*R*S, filter);
			random_matrix(C, N * H * W, input);

			NDIRECT_dnn_conv_fwd_exec(H, W, N , C, input,
			                            K, R, S, filter,
			                            padh, padw, stride, output);
			//NDIRECT_flush();
			//start = omp_get_wtime();
			start = dclock();
			for ( i = 0; i < loop ; i++)
				NDIRECT_dnn_conv_fwd_exec(H, W, N, C, input,
				                            K, R, S, filter,
				                            padh, padw, stride, output);
			//cost = (omp_get_wtime() - start) / loop;
			cost = (dclock() - start) / loop;

			printf("NDIRECT:  C = %d K= %d H=%d W=%d R = %d S = %d stride = %d padh = %d padw = %d \
					Gflops = %lf effic= %.3lf %\n",
			       C, K, H, W, R, S, stride, padh, padw,  ops / cost, ops / cost / 17.6 * 100 / NUM);

			fprintf(fp, " %.3f", ops / cost);
/*

			for (int nn = 0; nn < N ; nn++)
			{
				cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, K, H * W, C,
				            1.0, filter, lda, &input[nn *  C * H * W ], ldb, 0.0,
				            &output1[nn * H * W * K], ldc);
			}
			if(stride == 2)
				Check_result_s2(output, output1, N, K ,  H / stride, W / stride);
			else
				Check_result_s1(output, output1, N, K ,  H / stride, W / stride);

			*/

			free(filter);
			free(input);
			free(output);
			free(output1);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;

}
