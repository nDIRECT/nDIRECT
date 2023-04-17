#ifndef __DIRECT__ 
#define __DIRECT__ 

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>
#include <iostream>
#include <vector>


using namespace std;
#define NUM 64
#define CONV_K 512
#define CONV_HW 4096
#define CONV_C 256

extern vector<int>vec;
extern int Tm, Tn;

void NDIRECT_flush();
void Check_result_s1(float *output, float *output1, int N, int K, int H, int W);
void Check_result_s2(float *output, float *output1, int N, int K, int H, int W);
void NDIRECT_copy_KCRS_to_KCRS_cb_kb_fp32(float* filter, float* NDIRECT_filter,
                long Kb, long Cb, long C, long R, long S);

void NDIRECT_conv_direct_1x1s2_fwd_exec(int H, int W, int N, int C, float *input,
                                        int K, int R, int S, float *filter,
                                        int padh, int padw, int stride, float *output);

void NDIRECT_conv_direct_1x1_fwd_exec(int H, int W, int N, int C, float *input,
                                        int K, int R, int S, float *filter,
                                        int padh, int padw, int stride, float *output);

void NDIRECT_dnn_conv_fwd_exec(int H, int W, int N, int C, float *input,
                                 int K, int R, int S, float *filter,
                                 int padh, int padw, int stride, float* output);

void Dete_grad_N_threads_nums(int N, int K, int H, int W, int stride);

void LIB_R3_s1(int H, int W, int N, int C, float *input, 
            int K, int R, int S, float *filter, int padh, int padw, int stride, float* output);
void LIB_R3_s2(int H, int W, int N, int C, float *input, 
    int K, int R, int S, float *filter, int padh, int padw, int stride, float* output);
void LIB_R7_s2(int H, int W, int N, int C, float *input, 
    int K, int R, int S, float *filter, int padh, int padw, int stride, float* output);
void Direct_conv3x3_run(int H, int W, int N, int C, float *input, 
    int K, int R, int S, float *filter, int padh, int padw, int stride, float* output);
void verify(float *output, float *output1, int N, int K, int H, int W);

#endif
