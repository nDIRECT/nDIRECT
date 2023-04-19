#include <malloc.h>
#include <sys/time.h>
#include <stdio.h>
//#include <cblas.h>
#include <stdlib.h>

void trans_ass_core_K77(float *out_start, float *start, long out_num_size, long loop_count){
    
    asm volatile(
        ".macro K77_M8N8_UKERNEL_LOOP            \n"
        
        "   st4     {v3.s, v4.s, v5.s, v6.s}[0], [x0], #16          \n"
        "   st4     {v7.s, v8.s, v9.s, v10.s}[0], [x0], #16          \n"
        
        "   st4     {v3.s, v4.s, v5.s, v6.s}[1], [x0], #16          \n"
        "   st4     {v7.s, v8.s, v9.s, v10.s}[1], [x0], #16          \n"
        
        "   st4     {v3.s, v4.s, v5.s, v6.s}[2], [x0], #16          \n"
        "   st4     {v7.s, v8.s, v9.s, v10.s}[2], [x0], #16          \n"
        
        "   st4     {v3.s, v4.s, v5.s, v6.s}[3], [x0], #16          \n"
        "   st4     {v7.s, v8.s, v9.s, v10.s}[3], [x0], #16          \n"

        
        "ldr        q3, [x1], #16                       \n"
        "ldr        q4, [x4], #16                       \n"
        "ldr        q5, [x5], #16                       \n"
        "ldr        q6, [x6], #16                       \n"

        "ldr        q7, [x7], #16                       \n"
        "ldr        q8, [x8], #16                       \n"
        "ldr        q9, [x9], #16                         \n"
        "ldr        q10, [x10], #16                        \n"
        
        ".endm                                  \n"
        
        //ENTRANCE
        
        "ldr    x1, %[start]            \n"
        "ldr    x0, %[out_start]       \n"
        "ldr    x2, %[out_num_size]          \n"
        "ldr    x3, %[loop_count]       \n"
        
        "ldr        q3, [x1]                       \n"
        "add        x4, x1, x2              \n"
        
        "add        x5, x4, x2              \n"
        "add        x6, x5, x2              \n"
        "ldr        q4, [x4]                       \n"
        
        
        "add        x7, x6, x2              \n"
        "add        x8, x7, x2              \n"
        "ldr        q5, [x5]                       \n"
        
        "add        x9, x8, x2              \n"
        "add        x10, x9, x2             \n"
        "ldr        q6, [x6]                       \n"
        "ldr        q7, [x7]                       \n"
        "ldr        q8, [x8]                       \n"
        "ldr        q9, [x9]                         \n"
        "ldr        q10, [x10]                        \n"
        
        /*
        "prfm       PLDLIKEEP, [x1, #256]            \n"
        "prfm       PLDLIKEEP, [x4, #256]            \n"
        "prfm       PLDLIKEEP, [x5, #256]            \n"
        "prfm       PLDLIKEEP, [x6, #256]            \n"
        "prfm       PLDLIKEEP, [x7, #256]            \n"
        "prfm       PLDLIKEEP, [x8, #256]            \n"
        "prfm       PLDLIKEEP, [x9, #256]            \n"
        "prfm       PLDLIKEEP, [x10, #256]            \n"
        */
        "add        x1, x1, #12     \n"

        "add        x4, x4, #12     \n"
        "add        x5, x5, #12     \n"
        "add        x6, x6, #12     \n"
        "add        x7, x7, #12     \n"
        "add        x8, x8, #12     \n"
        "add        x9, x9, #12     \n"
        "add        x10, x10, #12     \n"
        
        "   st4     {v3.s, v4.s, v5.s, v6.s}[0], [x0], #16          \n"
        "   st4     {v7.s, v8.s, v9.s, v10.s}[0], [x0], #16          \n"
        
        "   st4     {v3.s, v4.s, v5.s, v6.s}[1], [x0], #16          \n"
        "   st4     {v7.s, v8.s, v9.s, v10.s}[1], [x0], #16          \n"
        
        "   st4     {v3.s, v4.s, v5.s, v6.s}[2], [x0], #16          \n"
        "   st4     {v7.s, v8.s, v9.s, v10.s}[2], [x0], #16          \n"
        
        
        "ldr        q3, [x1], #16                       \n"
        "ldr        q4, [x4], #16                       \n"
        "ldr        q5, [x5], #16                       \n"
        "ldr        q6, [x6], #16                       \n"
        
        "ldr        q7, [x7], #16                       \n"
        "ldr        q8, [x8], #16                       \n"
        "ldr        q9, [x9], #16                         \n"
        "ldr        q10, [x10], #16                        \n"
        
        "K77_M8N8_PARAL_LOOP:                    \n"
        "   K77_M8N8_UKERNEL_LOOP                \n"
        "   subs    x3, x3, #1          \n"
        "   beq     K77_M8N8_END_LOOP            \n"
        "   b       K77_M8N8_PARAL_LOOP          \n"
        
        "K77_M8N8_END_LOOP:                      \n"
    
    :
    
    :
        [out_start] "m" (out_start),
        [start]     "m" (start),
        [out_num_size]   "m" (out_num_size),
        [loop_count]"m" (loop_count)
    
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10",
        
        "v3", "v4", "v5", "v6", "v7","v8","v9","v10"
    );
    
    
}


void transform_filter_paral_ass_K77(float* kernel, float* out_kernel, long outch, long kc_tile, long trans_threads)
{
    long i, j, k, ii, jj, kk, h, w;
    
    long kc, ic;
    
    long k_h = 7;
    
    long k_w = 7;
    
    long inch = 3;
    
    long st_0, st_1, st;

    long thread_used = trans_threads;

    long out_num = inch * k_h * k_w;
    
    long out_num_size = out_num * sizeof(float);
    
    long i_off_1;
    long j_off;
    
    float *start, *out_start;
    
    long loop_count = 36;  //147 / 4 =38  3   147 = 7*7*3

#pragma omp parallel for num_threads(thread_used) default(none) shared(kernel, out_kernel, out_num, outch, kc_tile, loop_count, out_num_size) private(kc, i, i_off_1, st_0, st_1, start, out_start)

    for(kc = 0; kc < outch; kc += kc_tile){
        
        st_0 = 147 * kc;
                    
            for(i=kc; i < kc + kc_tile; i += 8)
            {
                
                st_1 = st_0 + 147 * (i-kc);
                
                i_off_1 = i * out_num;
                
                start = kernel + i_off_1;
                
                out_start = out_kernel + st_1;
                
                trans_ass_core_K77(out_start, start, out_num_size, loop_count);
                
            }

    }
}



void direct_K77_M8N8_00_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_00_M8N8_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "movi v8.4s, #0            \n"
    "movi v12.4s, #0            \n"
    
    "movi v9.4s, #0            \n"
    "movi v13.4s, #0            \n"
    
    
    "   fmul    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmul    v11.4s, v0.4s, v2.s[3]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmul    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   fmul    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmul    v17.4s, v0.4s, v3.s[3]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmul    v19.4s, v0.4s, v4.s[3]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmul    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmul    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmul    v23.4s, v1.4s, v4.s[3]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    

    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_00_M8N8_LAYERS     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    //7
    
    
    
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   ldr q5, [x0], #16                      \n"
    
    "   ldr q6, [x0], #16                      \n"


    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_00_M8N8_LAYERS_END     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    "   add x3, x3, #672    \n"
    //7

    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_00_M8N8_STORE     \n"
    
    
    
    
    
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"

    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"

    //7
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"

    
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"

    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"

    
   

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_00_M8N8_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    "   add x3, x3, #672    \n"

    "   uKERNEL_K77_00_M8N8_BEGIN      \n"
    "   uKERNEL_K77_00_M8N8_LAYERS     \n"
    "   uKERNEL_K77_00_M8N8_LAYERS     \n"

 
    "   b K77_00_M8N8_ADD_ROW           \n"
    
    "K77_00_M8N8_LOOP:            \n"
    "   uKERNEL_K77_00_M8N8_LAYERS     \n"
    "   uKERNEL_K77_00_M8N8_LAYERS     \n"
    "   uKERNEL_K77_00_M8N8_LAYERS     \n"

    
    "K77_00_M8N8_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_00_M8N8_STORE       \n"
    "   uKERNEL_K77_00_M8N8_LAYERS_END     \n"
    "   b       K77_00_M8N8_LOOP       \n"
    
    "K77_00_M8N8_STORE:          \n"
    "   uKERNEL_K77_00_M8N8_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}


void direct_K77_M8N8_01_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_01_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
    "   ldr q7, [x0], #16                      \n"
    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_01_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_01_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    
    "   add x3, x3, #672    \n"
    
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_01_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_01_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    "   add x3, x3, #672    \n"

    "   uKERNEL_K77_M8N8_01_BEGIN      \n"
    "   uKERNEL_K77_M8N8_01_LAYERS     \n"
    "   uKERNEL_K77_M8N8_01_LAYERS     \n"


    "   b K77_M8N8_01_ADD_ROW           \n"
    
    "K77_M8N8_01_LOOP:            \n"
    
    
    "   uKERNEL_K77_M8N8_01_LAYERS     \n"
    "   uKERNEL_K77_M8N8_01_LAYERS     \n"
    "   uKERNEL_K77_M8N8_01_LAYERS     \n"

    
    "K77_M8N8_01_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_01_STORE       \n"
    "   uKERNEL_K77_M8N8_01_LAYERS_END     \n"
    "   b       K77_M8N8_01_LOOP       \n"
    
    "K77_M8N8_01_STORE:          \n"
    "   uKERNEL_K77_M8N8_01_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}



void direct_K77_M8N8_02_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_02_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_02_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q5, [x0], #16                      \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_02_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"


    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    
    "   add x3, x3, #672    \n"
    
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"

    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_02_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_02_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    "   add x3, x3, #672    \n"

    "   uKERNEL_K77_M8N8_02_BEGIN      \n"
    "   uKERNEL_K77_M8N8_02_LAYERS     \n"
    "   uKERNEL_K77_M8N8_02_LAYERS     \n"


    "   b K77_M8N8_02_ADD_ROW           \n"
    
    "K77_M8N8_02_LOOP:            \n"
    "   uKERNEL_K77_M8N8_02_LAYERS     \n"
    "   uKERNEL_K77_M8N8_02_LAYERS     \n"
    "   uKERNEL_K77_M8N8_02_LAYERS     \n"


    
    "K77_M8N8_02_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_02_STORE       \n"
    "   uKERNEL_K77_M8N8_02_LAYERS_END     \n"
    "   b       K77_M8N8_02_LOOP       \n"
    
    "K77_M8N8_02_STORE:          \n"
    "   uKERNEL_K77_M8N8_02_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_K77_M8N8_10_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_10_M8N8_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "movi v8.4s, #0            \n"
    "movi v12.4s, #0            \n"
    
    "movi v9.4s, #0            \n"
    "movi v13.4s, #0            \n"
    
    
    "   fmul    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmul    v11.4s, v0.4s, v2.s[3]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmul    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   fmul    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmul    v17.4s, v0.4s, v3.s[3]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmul    v19.4s, v0.4s, v4.s[3]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmul    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmul    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmul    v23.4s, v1.4s, v4.s[3]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    

    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_10_M8N8_LAYERS     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    //7
    
    
    
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    //"   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"

    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   ldr q6, [x0], #16                      \n"


    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_10_M8N8_LAYERS_END     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    "   add x3, x3, #224    \n"
    //7

    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    //"   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q2, [x0], #16                      \n"

    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    "   ldr q3, [x0], #16                      \n"
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_10_M8N8_STORE     \n"
    
    
    
    
    
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"

    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"

    //7
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"

    
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"

    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"

    
   

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_10_M8N8_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    "   add x3, x3, #224    \n"

    "   uKERNEL_K77_10_M8N8_BEGIN      \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
 
    "   b K77_10_M8N8_ADD_ROW           \n"
    
    "K77_10_M8N8_LOOP:            \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    "   uKERNEL_K77_10_M8N8_LAYERS     \n"
    
    "K77_10_M8N8_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_10_M8N8_STORE       \n"
    "   uKERNEL_K77_10_M8N8_LAYERS_END     \n"
    "   b       K77_10_M8N8_LOOP       \n"
    
    "K77_10_M8N8_STORE:          \n"
    "   uKERNEL_K77_10_M8N8_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}


void direct_K77_M8N8_11_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_11_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
    "   ldr q7, [x0], #16                      \n"
    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_11_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_11_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    
    "   add x3, x3, #224    \n"
    
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_11_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_11_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    "   add x3, x3, #224    \n"

    "   uKERNEL_K77_M8N8_11_BEGIN      \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"


    "   b K77_M8N8_11_ADD_ROW           \n"
    
    "K77_M8N8_11_LOOP:            \n"
    
    
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"
    "   uKERNEL_K77_M8N8_11_LAYERS     \n"

    
    "K77_M8N8_11_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_11_STORE       \n"
    "   uKERNEL_K77_M8N8_11_LAYERS_END     \n"
    "   b       K77_M8N8_11_LOOP       \n"
    
    "K77_M8N8_11_STORE:          \n"
    "   uKERNEL_K77_M8N8_11_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_K77_M8N8_12_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_12_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"

    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_12_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q5, [x0], #16                      \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_12_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"


    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    
    "   add x3, x3, #224    \n"
    
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q5, [x0], #16                      \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_12_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_12_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    "   add x3, x3, #224    \n"

    "   uKERNEL_K77_M8N8_12_BEGIN      \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"

    "   b K77_M8N8_12_ADD_ROW           \n"
    
    "K77_M8N8_12_LOOP:            \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"
    "   uKERNEL_K77_M8N8_12_LAYERS     \n"

    
    "K77_M8N8_12_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_12_STORE       \n"
    "   uKERNEL_K77_M8N8_12_LAYERS_END     \n"
    "   b       K77_M8N8_12_LOOP       \n"
    
    "K77_M8N8_12_STORE:          \n"
    "   uKERNEL_K77_M8N8_12_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_K77_M8N8_20_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_20_M8N8_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "movi v8.4s, #0            \n"
    "movi v12.4s, #0            \n"
    
    "movi v9.4s, #0            \n"
    "movi v13.4s, #0            \n"
    
    
    "   fmul    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmul    v11.4s, v0.4s, v2.s[3]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmul    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   fmul    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmul    v17.4s, v0.4s, v3.s[3]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmul    v19.4s, v0.4s, v4.s[3]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmul    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmul    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmul    v23.4s, v1.4s, v4.s[3]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    

    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_20_M8N8_LAYERS     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    //7
    
    
    
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   ldr q6, [x0], #16                      \n"

    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_20_M8N8_LAYERS_END     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    //"   add x3, x3, #224    \n"
    //7

    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"

    
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_20_M8N8_STORE     \n"
    
    
    
    
    
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"

    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"

    //7
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"

    
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"

    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"

    
   

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_20_M8N8_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    //"   add x3, x3, #224    \n"

    "   uKERNEL_K77_20_M8N8_BEGIN      \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
 
    "   b K77_20_M8N8_ADD_ROW           \n"
    
    "K77_20_M8N8_LOOP:            \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    "   uKERNEL_K77_20_M8N8_LAYERS     \n"
    
    "K77_20_M8N8_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_20_M8N8_STORE       \n"
    "   uKERNEL_K77_20_M8N8_LAYERS_END     \n"
    "   b       K77_20_M8N8_LOOP       \n"
    
    "K77_20_M8N8_STORE:          \n"
    "   uKERNEL_K77_20_M8N8_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}


void direct_K77_M8N8_21_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_21_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
    "   ldr q7, [x0], #16                      \n"
    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_21_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_21_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    
    //"   add x3, x3, #224    \n"
    
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_21_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_21_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    //"   add x3, x3, #224    \n"

    "   uKERNEL_K77_M8N8_21_BEGIN      \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"

    "   b K77_M8N8_21_ADD_ROW           \n"
    
    "K77_M8N8_21_LOOP:            \n"
    
    
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    "   uKERNEL_K77_M8N8_21_LAYERS     \n"
    
    "K77_M8N8_21_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_21_STORE       \n"
    "   uKERNEL_K77_M8N8_21_LAYERS_END     \n"
    "   b       K77_M8N8_21_LOOP       \n"
    
    "K77_M8N8_21_STORE:          \n"
    "   uKERNEL_K77_M8N8_21_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_K77_M8N8_22_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_22_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"

    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_22_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q5, [x0], #16                      \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_22_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"


    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    
    //"   add x3, x3, #224    \n"
    
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"

    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_22_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_22_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    //"   add x3, x3, #224    \n"

    "   uKERNEL_K77_M8N8_22_BEGIN      \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"

    "   b K77_M8N8_22_ADD_ROW           \n"
    
    "K77_M8N8_22_LOOP:            \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    "   uKERNEL_K77_M8N8_22_LAYERS     \n"
    
    "K77_M8N8_22_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_22_STORE       \n"
    "   uKERNEL_K77_M8N8_22_LAYERS_END     \n"
    "   b       K77_M8N8_22_LOOP       \n"
    
    "K77_M8N8_22_STORE:          \n"
    "   uKERNEL_K77_M8N8_22_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_K77_M8N8_30_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_30_M8N8_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "movi v8.4s, #0            \n"
    "movi v12.4s, #0            \n"
    
    "movi v9.4s, #0            \n"
    "movi v13.4s, #0            \n"
    
    
    "   fmul    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmul    v11.4s, v0.4s, v2.s[3]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmul    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   fmul    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmul    v17.4s, v0.4s, v3.s[3]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmul    v19.4s, v0.4s, v4.s[3]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmul    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmul    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmul    v23.4s, v1.4s, v4.s[3]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    

    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_30_M8N8_LAYERS     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    //7
    
    
    
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"

    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   ldr q6, [x0], #16                      \n"

    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_30_M8N8_LAYERS_END     \n"
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    "   add x3, x3, #448    \n"
    //7

    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q2, [x0], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    "   ldr q4, [x0], #16                      \n"
    "   ldr q5, [x0], #16                      \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_30_M8N8_STORE     \n"
    
    
    
    
    
    //1
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v10.4s, v0.4s, v2.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v2.s[3]          \n"
    
    
    "   fmla    v14.4s, v1.4s, v2.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v2.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v3.s[3]           \n"

    
    "   fmla    v18.4s, v0.4s, v4.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v4.s[3]          \n"

    
    "   fmla    v20.4s, v1.4s, v3.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v4.s[3]          \n"
    
    
    
    
    
    //2
    
    "   fmla    v9.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v2.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[0]          \n"
    

    "   fmla    v13.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v2.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[0]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[0]          \n"
    
    //3
    

    "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
    "   ldr q27, [x3], #16                      \n"
    

    "   fmla    v13.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v3.s[3]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[1]          \n"
    
    "   fmla    v20.4s, v1.4s, v3.s[3]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[2]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[2]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"

    "   fmla    v20.4s, v31.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"

    //7
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"

    
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"

    
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"

    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"

    
   

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_30_M8N8_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    //"   add x3, x3, #224    \n"

    "   uKERNEL_K77_30_M8N8_BEGIN      \n"
    "   uKERNEL_K77_30_M8N8_LAYERS     \n"
    "   uKERNEL_K77_30_M8N8_LAYERS     \n"
    "   uKERNEL_K77_30_M8N8_LAYERS     \n"

 
    "   b K77_30_M8N8_ADD_ROW           \n"
    
    "K77_30_M8N8_LOOP:            \n"
    "   uKERNEL_K77_30_M8N8_LAYERS     \n"
    "   uKERNEL_K77_30_M8N8_LAYERS     \n"
    "   uKERNEL_K77_30_M8N8_LAYERS     \n"
    "   uKERNEL_K77_30_M8N8_LAYERS     \n"

    
    "K77_30_M8N8_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_30_M8N8_STORE       \n"
    "   uKERNEL_K77_30_M8N8_LAYERS_END     \n"
    "   b       K77_30_M8N8_LOOP       \n"
    
    "K77_30_M8N8_STORE:          \n"
    "   uKERNEL_K77_30_M8N8_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_K77_M8N8_31_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_31_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
    "   ldr q7, [x0], #16                      \n"
    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_31_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_31_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #96             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    
    "   add x3, x3, #448    \n"
    
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   ldr q4, [x0], #16                      \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q7, [x0], #16                      \n"
    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_31_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[3]          \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[3]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   fmla    v19.4s, v27.4s, v7.s[0]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   fmla    v23.4s, v31.4s, v7.s[0]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_31_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    //"   add x3, x3, #224    \n"

    "   uKERNEL_K77_M8N8_31_BEGIN      \n"
    "   uKERNEL_K77_M8N8_31_LAYERS     \n"
    "   uKERNEL_K77_M8N8_31_LAYERS     \n"
    "   uKERNEL_K77_M8N8_31_LAYERS     \n"


    "   b K77_M8N8_31_ADD_ROW           \n"
    
    "K77_M8N8_31_LOOP:            \n"
    
    
    "   uKERNEL_K77_M8N8_31_LAYERS     \n"
    "   uKERNEL_K77_M8N8_31_LAYERS     \n"
    "   uKERNEL_K77_M8N8_31_LAYERS     \n"
    "   uKERNEL_K77_M8N8_31_LAYERS     \n"

    
    "K77_M8N8_31_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_31_STORE       \n"
    "   uKERNEL_K77_M8N8_31_LAYERS_END     \n"
    "   b       K77_M8N8_31_LOOP       \n"
    
    "K77_M8N8_31_STORE:          \n"
    "   uKERNEL_K77_M8N8_31_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_K77_M8N8_32_p3_s2(float *img, float *kernal, float *out, long stride_out_size, long stride_in_size, long row_in_size){
    asm volatile(
    
    ".macro uKERNEL_K77_M8N8_32_BEGIN     \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    "   add     x10, x4, x6                     \n"
    
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    "   add     x11, x10, x6                     \n"
    
    "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmul    v11.4s, v0.4s, v3.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"
    
    "   add     x12, x11, x6                     \n"
    
    "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   add     x13, x12, x6                     \n"
    
    
    "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
     "   add     x14, x13, x6                     \n"
    
    "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmul    v19.4s, v0.4s, v5.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   add     x15, x14, x6                     \n"
    
    "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmul    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmul    v23.4s, v1.4s, v5.s[2]          \n"

    "   add     x16, x15, x6                     \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x4, #64]           \n"
    "   prfm    PLDL2KEEP, [x10, #64]          \n"
    "   prfm    PLDL2KEEP, [x11, #64]          \n"
    "   prfm    PLDL2KEEP, [x12, #64]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    
    //2
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    "   ldr q31, [x3], #16                      \n"
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q1, [x3], #16                      \n"
    "   ldr q3, [x0], #16                      \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    
    "   ldr q4, [x0], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    
    "   ldr q5, [x0], #16                      \n"
    
    "   prfm    PLDL2KEEP, [x13, #64]          \n"
    "   prfm    PLDL2KEEP, [x14, #64]          \n"
    "   prfm    PLDL2KEEP, [x15, #64]          \n"
    "   prfm    PLDL2KEEP, [x16, #64]          \n"
    "   ldr q6, [x0], #16                      \n"

    ".endm                                  \n"
    
    //Layers
    ".macro uKERNEL_K77_M8N8_32_LAYERS     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x9          \n"
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"
    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    //Layers_end
    ".macro uKERNEL_K77_M8N8_32_LAYERS_END     \n"
    //1
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    "   sub x0, x0, #80             \n"
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"


    "   ldr q31, [x3], #16                      \n"
    
    //7
    "   add x0, x0, x5          \n"
    
    "   add x3, x3, #448    \n"
    
    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    "   ldr q0, [x3], #16                      \n"
    "   ldr q2, [x0], #16                      \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    "   ldr q3, [x0], #16                      \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    "   ldr q4, [x0], #16                      \n"

    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   ldr q5, [x0], #16                      \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"

    "   ldr q6, [x0], #16                      \n"
    "   ldr q1, [x3], #16                      \n"

    ".endm                                  \n"
    
    ".macro uKERNEL_K77_M8N8_32_STORE     \n"
    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
    //2
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v3.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
    "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[1]          \n"
    "   fmla    v23.4s, v31.4s, v5.s[3]          \n"
    //3
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v2.s[2]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
    
    "   fmla    v16.4s, v0.4s, v4.s[2]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
    
    "   fmla    v20.4s, v1.4s, v4.s[2]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[0]          \n"
    //4
    "   ldr q0, [x3], #16                      \n"
    
    "   fmla    v8.4s, v27.4s, v2.s[3]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v10.4s, v27.4s, v3.s[3]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[1]          \n"
    
    "   ldr q1, [x3], #16                      \n"
    
    "   fmla    v12.4s, v31.4s, v2.s[3]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v14.4s, v31.4s, v3.s[3]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[1]          \n"
    
    "   fmla    v16.4s, v27.4s, v4.s[3]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v18.4s, v27.4s, v5.s[3]          \n"
    "   fmla    v19.4s, v27.4s, v6.s[1]          \n"
    
    "   fmla    v20.4s, v31.4s, v4.s[3]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v22.4s, v31.4s, v5.s[3]          \n"
    "   fmla    v23.4s, v31.4s, v6.s[1]          \n"
    //5
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v8.4s, v0.4s, v3.s[0]           \n"
    "   fmla    v9.4s, v0.4s, v3.s[2]           \n"
    "   fmla    v10.4s, v0.4s, v4.s[0]          \n"
    "   fmla    v11.4s, v0.4s, v4.s[2]          \n"
    
    "   ldr q31, [x3], #16                      \n"
    
    "   fmla    v12.4s, v1.4s, v3.s[0]           \n"
    "   fmla    v13.4s, v1.4s, v3.s[2]           \n"
    "   fmla    v14.4s, v1.4s, v4.s[0]          \n"
    "   fmla    v15.4s, v1.4s, v4.s[2]          \n"
    
    "   fmla    v16.4s, v0.4s, v5.s[0]           \n"
    "   fmla    v17.4s, v0.4s, v5.s[2]           \n"
    "   fmla    v18.4s, v0.4s, v6.s[0]          \n"
    "   fmla    v19.4s, v0.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v1.4s, v5.s[0]           \n"
    "   fmla    v21.4s, v1.4s, v5.s[2]           \n"
    "   fmla    v22.4s, v1.4s, v6.s[0]          \n"
    "   fmla    v23.4s, v1.4s, v6.s[2]          \n"
    //6
    
    "   fmla    v8.4s, v27.4s, v3.s[1]           \n"
    "   fmla    v9.4s, v27.4s, v3.s[3]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[1]          \n"
    "   fmla    v11.4s, v27.4s, v4.s[3]          \n"
    
    "   fmla    v12.4s, v31.4s, v3.s[1]           \n"
    "   fmla    v13.4s, v31.4s, v3.s[3]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[1]          \n"
    "   fmla    v15.4s, v31.4s, v4.s[3]          \n"
    
    "   fmla    v16.4s, v27.4s, v5.s[1]           \n"
    "   fmla    v17.4s, v27.4s, v5.s[3]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[1]          \n"

    
    "   ldr q27, [x3], #16                      \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[1]           \n"
    "   fmla    v21.4s, v31.4s, v5.s[3]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[1]          \n"

    
    "   ldr q31, [x3], #16                      \n"
    
    //7

    "   fmla    v8.4s, v27.4s, v3.s[2]           \n"
    "   fmla    v9.4s, v27.4s, v4.s[0]           \n"
    "   fmla    v10.4s, v27.4s, v4.s[2]          \n"
    "   fmla    v11.4s, v27.4s, v5.s[0]          \n"
    
    
    "   fmla    v12.4s, v31.4s, v3.s[2]           \n"
    "   fmla    v13.4s, v31.4s, v4.s[0]           \n"
    "   fmla    v14.4s, v31.4s, v4.s[2]          \n"
    "   fmla    v15.4s, v31.4s, v5.s[0]          \n"
    
    
    "   fmla    v16.4s, v27.4s, v5.s[2]           \n"
    "   fmla    v17.4s, v27.4s, v6.s[0]           \n"
    "   fmla    v18.4s, v27.4s, v6.s[2]          \n"
    
    "   fmla    v20.4s, v31.4s, v5.s[2]           \n"
    "   fmla    v21.4s, v31.4s, v6.s[0]           \n"
    "   fmla    v22.4s, v31.4s, v6.s[2]          \n"

    
    "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
    "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
    
    "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
    "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    
    "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x4]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x10]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x11]    \n"
    "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x12]    \n"
    
    "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x13]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x14]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x15]    \n"
    "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x16]    \n"
    ".endm                                  \n"
    
    "K77_M8N8_32_ENTRANCE:        \n"

    "   ldr x0, %[img]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   mov x8, #3      \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   ldr x9, %[row_in_size]             \n"

    //"   add x3, x3, #224    \n"

    "   uKERNEL_K77_M8N8_32_BEGIN      \n"
    "   uKERNEL_K77_M8N8_32_LAYERS     \n"
    "   uKERNEL_K77_M8N8_32_LAYERS     \n"
    "   uKERNEL_K77_M8N8_32_LAYERS     \n"


    "   b K77_M8N8_32_ADD_ROW           \n"
    
    "K77_M8N8_32_LOOP:            \n"
    "   uKERNEL_K77_M8N8_32_LAYERS     \n"
    "   uKERNEL_K77_M8N8_32_LAYERS     \n"
    "   uKERNEL_K77_M8N8_32_LAYERS     \n"
    "   uKERNEL_K77_M8N8_32_LAYERS     \n"

    
    "K77_M8N8_32_ADD_ROW:       \n"
    "   subs    x8, x8, #1                 \n"
    "   cmp     x8, #0                          \n"
    "   beq     K77_M8N8_32_STORE       \n"
    "   uKERNEL_K77_M8N8_32_LAYERS_END     \n"
    "   b       K77_M8N8_32_LOOP       \n"
    
    "K77_M8N8_32_STORE:          \n"
    "   uKERNEL_K77_M8N8_32_STORE      \n"
    :
    
    :
        [img]                       "m"         (img),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]           "m"     (stride_in_size),
        [row_in_size]               "m"    (row_in_size)
    :
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        "x26", "x27", "x28","x29","x30",
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );

}

void direct_7x7s2(float* out_kernal, float* img, float* out, long inch, long outch, long in_h, long out_h, long k_h, long p_h, long s_h, long kc_tile){

    
    long in_w = in_h;
    
    long out_w = out_h;
    
    long stride_in = in_h * in_h;
    
    long stride_in_size = stride_in * sizeof(float);
    
    long stride_out = out_h * out_w;
    
    long stride_out_size = stride_out * sizeof(float);
    
    long row_in_size = in_h * sizeof(float);
    
    long stride_3c = stride_in_size - 3 * row_in_size;
    
    long stride_5c = stride_in_size - 5 * row_in_size;
    
    long stride_6c = stride_in_size - 6 * row_in_size;
    
    long stride_4c = stride_in_size - 4 * row_in_size;
    
    float *kernal_c, *kernal_start;
    
    float *out_img_s, *out_img_c, *out_img_p;
    
    float *img_start, *img_start_h, *img_start_s;
    
    
    long last_h = out_h - 1;
    
    long i, j, k, ic, oc, oh, ow;
    
    long kc;
    
    long kc_size = kc_tile;
    
    long last_w = out_w -8;
    
    
    for(kc = 0; kc < outch; kc += kc_tile){
        
        img_start = img;
        
        kernal_start = out_kernal + 147 * kc;
        
        for(oh=0; oh < out_h; oh++){

            if(oh==0){
                
                img_start_h = img_start;
                
                out_img_s = out;
                
                ow = 0;

                img_start_s = img_start_h;
                    
                out_img_c = out_img_s;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_00_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_3c, row_in_size);
                }
                

                for(ow=8; ow<last_w; ow+=8){
                    
                    img_start_s = img_start_h + ow * 2 - 3;
                    
                    out_img_c = out_img_s + ow;
                    
                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                        
                        out_img_p = out_img_c + oc * stride_out;
                        
                        kernal_c = kernal_start + 147 * (oc - kc);
                        
                        direct_K77_M8N8_01_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_3c, row_in_size);
                        
                        
                        
                    }

                }

                img_start_s = img_start_h + ow * 2 - 3;
                
                out_img_c = out_img_s + ow;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_02_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_3c, row_in_size);
                }
                

            }

            else if(oh == 1){

                img_start_h = img_start;
                
                out_img_s = out + out_w;
                
                ow = 0;

                img_start_s = img_start_h;
                
                out_img_c = out_img_s;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_10_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_5c, row_in_size);
                }

                for(ow=8; ow<last_w; ow+=8){
                    
                    img_start_s = img_start_h + ow * 2 - 3;
                    
                    out_img_c = out_img_s + ow;
                    
                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                        
                        out_img_p = out_img_c + oc * stride_out;
                        
                        kernal_c = kernal_start + 147 * (oc - kc);
                        
                        direct_K77_M8N8_11_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_5c, row_in_size);
                    }
                }

                img_start_s = img_start_h + ow * 2 - 3;
                
                out_img_c = out_img_s + ow;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_12_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_5c, row_in_size);
                }

            }

            else if(oh == last_h ){

                img_start_h = img_start + (oh * 2 - 3) * in_w ;
                
                out_img_s = out + oh * out_w;
                
                ow = 0;

                img_start_s = img_start_h;
                
                out_img_c = out_img_s;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_30_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_4c, row_in_size);
                }
                

                for(ow=8; ow<last_w; ow+=8){
                    
                    img_start_s = img_start_h + ow * 2 - 3;
                    
                    out_img_c = out_img_s + ow;
                    
                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                        
                        out_img_p = out_img_c + oc * stride_out;
                        
                        kernal_c = kernal_start + 147 * (oc - kc);
                        
                        direct_K77_M8N8_31_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_4c, row_in_size);
                    }
                }
                

                img_start_s = img_start_h + ow * 2 - 3;
                
                out_img_c = out_img_s + ow;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_32_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_4c, row_in_size);
                }
                

            }
                
            else{

                img_start_h = img_start + (oh * 2 - 3) * in_w ;
                
                out_img_s = out + oh * out_w;
                
                ow = 0;

                img_start_s = img_start_h;
                
                out_img_c = out_img_s;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_20_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_6c, row_in_size);
                }

                for(ow=8; ow<last_w; ow+=8){
                    
                    img_start_s = img_start_h + ow * 2 - 3;
                    
                    out_img_c = out_img_s + ow;
                    
                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                        
                        out_img_p = out_img_c + oc * stride_out;
                        
                        kernal_c = kernal_start + 147 * (oc - kc);
                        
                        direct_K77_M8N8_21_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_6c, row_in_size);
                    }
                }
                

                img_start_s = img_start_h + ow * 2 - 3;
                
                out_img_c = out_img_s + ow;
                
                for(oc = kc; oc < kc + kc_tile; oc+=8){
                    
                    out_img_p = out_img_c + oc * stride_out;
                    
                    kernal_c = kernal_start + 147 * (oc - kc);
                    
                    direct_K77_M8N8_22_p3_s2(img_start_s, kernal_c, out_img_p, stride_out_size, stride_6c, row_in_size);
                }

            }
            

            
        }
    }
    
}

