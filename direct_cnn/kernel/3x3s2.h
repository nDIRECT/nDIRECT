#include <malloc.h>
#include <sys/time.h>
#include <stdio.h>
//#include <cblas.h>
#include <stdlib.h>



void trans_ass_core_s2(float *out_start, float *start, long out_num_size, long loop_count){
    
    asm volatile(
        ".macro UKERNEL_LOOP            \n"
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
        
        "ldr    x0, %[out_start]       \n"
        "ldr    x1, %[start]            \n"
        "ldr    x2, %[out_num_size]          \n"
        "ldr    x3, %[loop_count]       \n"
        
        
        "add        x4, x1, x2              \n"
        "add        x5, x4, x2              \n"
        "add        x6, x5, x2              \n"
        "add        x7, x6, x2              \n"
        "add        x8, x7, x2              \n"
        "add        x9, x8, x2              \n"
        "add        x10, x9, x2             \n"
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
        "ldr        q3, [x1], #16                       \n"
        "ldr        q4, [x4], #16                       \n"
        "ldr        q5, [x5], #16                       \n"
        "ldr        q6, [x6], #16                       \n"
        "ldr        q7, [x7], #16                       \n"
        "ldr        q8, [x8], #16                       \n"
        "ldr        q9, [x9], #16                         \n"
        "ldr        q10, [x10], #16                        \n"
        
        "PARAL_LOOP:                    \n"
        "   UKERNEL_LOOP                \n"
        "   subs    x3, x3, #1          \n"
        "   beq     END_LOOP            \n"
        "   b       PARAL_LOOP          \n"
        
        "END_LOOP:                      \n"
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

void transform_filter_paral_ass_s2(long outch, long inch, long k_h, 
                long k_w, float* kernel, float* out_kernel, long kc_tile, long ic_tile, long ic_out_tile, long kc_out_tile, long trans_threads)
{
    long i, j, k, ii, jj, kk, h, w;
    
    long kc_out, ic_out, kc, ic;
    
    long st_0, st_1, st;

    long thread_used = trans_threads;

    long out_num = inch * k_h * k_w;
    
    long out_num_size = out_num * 4;
    
    long i_off_1;
    long i_off_2;
    long i_off_3;
    long i_off_4;
    long i_off_5;
    long i_off_6;
    long i_off_7;
    long i_off_8;
    long h_off;
    long j_off;
    
    float *start, *out_start;
    
    long loop_count;
    
#pragma omp parallel for num_threads(thread_used) default(none) shared(kernel, out_kernel, out_num, outch, inch, k_w, k_h, kc_out_tile, kc_tile, ic_out_tile, ic_tile, loop_count, out_num_size) private(kc_out, ic_out, kc, ic, i, jj, h, w, i_off_1, i_off_2, i_off_3, i_off_4, i_off_5, i_off_6, i_off_7, i_off_8, j_off, h_off, st_0, st_1, st, start, out_start)
    for(kc_out = 0; kc_out < outch; kc_out += kc_out_tile){
        
        st_0 = 9 * inch * kc_out;
        
        for(ic_out = 0; ic_out < inch; ic_out += ic_out_tile){
            
            st_1 =  st_0 + 9 * kc_out_tile * ic_out;

            for(kc=kc_out; kc < kc_out + kc_out_tile; kc += kc_tile){
            
                st =  st_1 + 9 * (kc - kc_out) * ic_out_tile;
                
                for(ic=ic_out; ic < ic_out + ic_out_tile; ic += ic_tile)
                {
                    
                    for(i=kc; i < kc + kc_tile; i += 8)
                    {
                        
                        start = kernel + i * out_num;
                        
                        i_off_1 = i * out_num;
                        
                        j_off = ic * k_h *k_w;
                        
                        start = kernel + i_off_1 + j_off;
                        
                        out_start = out_kernel + st;
                        
                        loop_count = ic_tile * 9 / 4;
                        
                        trans_ass_core_s2(out_start, start, out_num_size, loop_count);
                        
                        st += ic_tile * 9 * 8;
                        
                    }
                }
            }

        }
    }
}

void direct_3x3_M8N7_00_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N7_S2_00_BEGIN                             \n"
            //update kernel
            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"

            "   ldr     q4, [x1], #16                      \n"

            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"

            
            "   add     x11, x10, x6          \n"




            "   add     x12, x11, x6           \n"


            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"
            
            "   add     x13, x12, x6           \n"


            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
                

                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"




                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"





    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N7_S2_00_K4                              \n"
                "   ld1     {v5.2s}, [x1], #8               \n"
                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N7_S2_00_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"


                "   ldr     q3, [x1], #16                   \n"

                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"

                "   ldr     q0, [x3], #16                   \n"


                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"



    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_K7                              \n"
                "   ld1     {v5.2s}, [x1], #8                   \n"
                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"


                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
 
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   ldr     q2, [x1], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"

                "   ldr     q3, [x1], #16                   \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"


                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N7_S2_00_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st3     {v16.s, v17.s, v18.s}[0], [x4]    \n"
                "   st3     {v16.s, v17.s, v18.s}[1], [x10]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   st3     {v16.s, v17.s, v18.s}[2], [x11]    \n"
                "   st3     {v16.s, v17.s, v18.s}[3], [x12]   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"

                "   st3     {v20.s, v21.s, v22.s}[0], [x13]   \n"
                "   st3     {v20.s, v21.s, v22.s}[1], [x14]    \n"
                "   st3     {v20.s, v21.s, v22.s}[2], [x15]    \n"
                "   st3     {v20.s, v21.s, v22.s}[3], [x16]    \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"
                
                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16 \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8     \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   st2     {v16.s, v17.s}[2], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[3], [x28], #8     \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8     \n"
                
                "   str     q18, [x28], #16         \n"
                "   str     q22, [x28], #16         \n"
                
                
                //4
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                //2
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4], #8                        \n"
                "   st1     {v9.2s}, [x10], #8                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11], #8                      \n"
                "   st1     {v11.2s}, [x12], #8                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13], #8                       \n"
                "   st1     {v9.2s}, [x14], #8                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15], #8                     \n"
                "   st1     {v11.2s}, [x16], #8                     \n"
                
                //1
                
                "   ld1     {v0.s}[0], [x4]                   \n"
                "   ld1     {v0.s}[1], [x10]                   \n"
                "   ld1     {v0.s}[2], [x11]                   \n"
                "   ld1     {v0.s}[3], [x12]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"

                
                "   st1     {v8.s}[0], [x4]                  \n"
                "   st1     {v8.s}[1], [x10]                  \n"
                "   st1     {v8.s}[2], [x11]                  \n"
                "   st1     {v8.s}[3], [x12]                  \n"

                //1
                
                "   ld1     {v0.s}[0], [x13]                   \n"
                "   ld1     {v0.s}[1], [x14]                   \n"
                "   ld1     {v0.s}[2], [x15]                   \n"
                "   ld1     {v0.s}[3], [x16]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   st1     {v8.s}[0], [x13]                  \n"
                "   st1     {v8.s}[1], [x14]                  \n"
                "   st1     {v8.s}[2], [x15]                  \n"
                "   st1     {v8.s}[3], [x16]                  \n"

    
    ".endm                                                  \n"
    
    "M8N7_S2_00_ENTRANCE:                                                       \n"
    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #224                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N7_S2_00_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N7_S2_00_LOOP                               \n"
    
    "M8N7_S2_00_MAIN_K:                                  \n"
        "   uKERNEL_M8N7_S2_00_K3                     \n"

    "M8N7_S2_00_LOOP:                                  \n"
    "   uKERNEL_M8N7_S2_00_K4                                 \n"
    "   uKERNEL_M8N7_S2_00_K5                                 \n"
    "   uKERNEL_M8N7_S2_00_K6                                 \n"
    "   uKERNEL_M8N7_S2_00_K7                                 \n"

    "   beq     M8N7_S2_00_END_K                        \n"

    "   uKERNEL_M8N7_S2_00_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N7_S2_00_MAIN_K                       \n" 




    "M8N7_S2_00_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N7_S2_00                     \n"

    //ic != 0
    "   uKERNEL_M8N7_S2_00_END                    \n"

    "   b    M8N7_S2_00_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N7_S2_00:                               \n"
    "   M8N7_S2_00_ADD_C                                   \n"
    
    
    "M8N7_S2_00_END:                                       \n"
    :

    :
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3_M8N7_00_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N7_S2_00_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"

            "   ldr     q4, [x0], #16                      \n"
            "   str     q2, [x1], #16                   \n"
            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"
            "   str     q3, [x1], #16                   \n"
            
            "   add     x11, x10, x6          \n"




            "   add     x12, x11, x6           \n"


            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"
            
            "   add     x13, x12, x6           \n"
            
            "   str     q4, [x1], #16                   \n"

            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_PACK_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                //update kernel 
                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"


                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"




                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N7_S2_00_PACK_K4                              \n"
                "   ld1     {v5.2s}, [x0], #8               \n"
                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"
                "   st1     {v5.2s}, [x1], #8                   \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N7_S2_00_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
                
                "   sub     x0, x0, #56                     \n"

                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q2, [x22], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"


                "   str     q2, [x1], #16                   \n"

                //update img row 1 col 1
                "   ldr     q3, [x22], #16                   \n"

                "   str     q3, [x1], #16                   \n"


                
                //update img row 1 col 2
                "   ldr     q4, [x22], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_PACK_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                "   str     q4, [x1], #16                   \n"

                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"



    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_PACK_K7                              \n"
                "   ld1     {v5.2s}, [x22], #8                   \n"
                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q6, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"

                "   sub     x22, x22, #56                   \n"             

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"
                
                //correspongding add

                "   st1     {v5.2s}, [x1], #8                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"

                "   add     x22, x22, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_PACK_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x0], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"


                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"


                //prepare: row 2 col 1
                "   ldr     q3, [x0], #16                   \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"



                //prepare: row 2 col 2
                "   ldr     q4, [x0], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N7_S2_00_PACK_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st3     {v16.s, v17.s, v18.s}[0], [x4]    \n"
                "   st3     {v16.s, v17.s, v18.s}[1], [x10]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   st3     {v16.s, v17.s, v18.s}[2], [x11]    \n"
                "   st3     {v16.s, v17.s, v18.s}[3], [x12]   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"

                "   st3     {v20.s, v21.s, v22.s}[0], [x13]   \n"
                "   st3     {v20.s, v21.s, v22.s}[1], [x14]    \n"
                "   st3     {v20.s, v21.s, v22.s}[2], [x15]    \n"
                "   st3     {v20.s, v21.s, v22.s}[3], [x16]    \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_00_PACK_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"
                
                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16 \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8     \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   st2     {v16.s, v17.s}[2], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[3], [x28], #8     \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8     \n"
                
                "   str     q18, [x28], #16         \n"
                "   str     q22, [x28], #16         \n"
                
                
                //4
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                //2
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4], #8                        \n"
                "   st1     {v9.2s}, [x10], #8                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11], #8                      \n"
                "   st1     {v11.2s}, [x12], #8                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13], #8                       \n"
                "   st1     {v9.2s}, [x14], #8                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15], #8                     \n"
                "   st1     {v11.2s}, [x16], #8                     \n"
                
                //1
                
                "   ld1     {v0.s}[0], [x4]                   \n"
                "   ld1     {v0.s}[1], [x10]                   \n"
                "   ld1     {v0.s}[2], [x11]                   \n"
                "   ld1     {v0.s}[3], [x12]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"

                
                "   st1     {v8.s}[0], [x4]                  \n"
                "   st1     {v8.s}[1], [x10]                  \n"
                "   st1     {v8.s}[2], [x11]                  \n"
                "   st1     {v8.s}[3], [x12]                  \n"

                //1
                
                "   ld1     {v0.s}[0], [x13]                   \n"
                "   ld1     {v0.s}[1], [x14]                   \n"
                "   ld1     {v0.s}[2], [x15]                   \n"
                "   ld1     {v0.s}[3], [x16]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   st1     {v8.s}[0], [x13]                  \n"
                "   st1     {v8.s}[1], [x14]                  \n"
                "   st1     {v8.s}[2], [x15]                  \n"
                "   st1     {v8.s}[3], [x16]                  \n"

    
    ".endm                                                  \n"
    
    "M8N7_S2_00_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #224                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N7_S2_00_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N7_S2_00_PACK_LOOP                               \n"
    
    "M8N7_S2_00_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N7_S2_00_PACK_K3                     \n"

    "M8N7_S2_00_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N7_S2_00_PACK_K4                                 \n"
    "   uKERNEL_M8N7_S2_00_PACK_K5                                 \n"
    "   uKERNEL_M8N7_S2_00_PACK_K6                                 \n"
    "   uKERNEL_M8N7_S2_00_PACK_K7                                 \n"

    "   beq     M8N7_S2_00_PACK_END_K                        \n"

    "   uKERNEL_M8N7_S2_00_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N7_S2_00_PACK_MAIN_K                       \n" 




    "M8N7_S2_00_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N7_S2_00_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N7_S2_00_PACK_END                    \n"

    "   b    M8N7_S2_00_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N7_S2_00_PACK:                               \n"
    "   M8N7_S2_00_PACK_ADD_C                                   \n"
    
    
    "M8N7_S2_00_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N7_01_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N7_S2_01_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"

            "   ldr     q4, [x1], #16                      \n"

            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"

            
            "   add     x11, x10, x6          \n"
            "   add     x12, x11, x6           \n"


            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"
            
            "   add     x13, x12, x6           \n"


            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N7_S2_01_K0                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"

                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"



                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N7_S2_01_K1                              \n"
                "   ld1     {v5.2s}, [x1], #8               \n"
                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q6, [x3], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N7_S2_01_K2                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   ldr     q3, [x1], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"


                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N7_S2_01_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"


                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N7_S2_01_K4                              \n"
                "   ld1     {v5.2s}, [x1], #8               \n"
                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"


                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N7_S2_01_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"

                "   ldr     q3, [x1], #16                   \n"

                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"


                "   ldr     q0, [x3], #16                   \n"
                


                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"



    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_K7                              \n"
                "   ld1     {v5.2s}, [x1], #8                   \n"
                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q6, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"

                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_K8                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   ldr     q2, [x1], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"


                "   ldr     q3, [x1], #16                   \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"


                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N7_S2_01_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st3     {v16.s, v17.s, v18.s}[0], [x4]    \n"
                "   st3     {v16.s, v17.s, v18.s}[1], [x10]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   st3     {v16.s, v17.s, v18.s}[2], [x11]    \n"
                "   st3     {v16.s, v17.s, v18.s}[3], [x12]   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"

                "   st3     {v20.s, v21.s, v22.s}[0], [x13]   \n"
                "   st3     {v20.s, v21.s, v22.s}[1], [x14]    \n"
                "   st3     {v20.s, v21.s, v22.s}[2], [x15]    \n"
                "   st3     {v20.s, v21.s, v22.s}[3], [x16]    \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"
                
                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16 \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8     \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   st2     {v16.s, v17.s}[2], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[3], [x28], #8     \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8     \n"
                
                "   str     q18, [x28], #16         \n"
                "   str     q22, [x28], #16         \n"
                
                
                //4
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                //2
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4], #8                        \n"
                "   st1     {v9.2s}, [x10], #8                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11], #8                      \n"
                "   st1     {v11.2s}, [x12], #8                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13], #8                       \n"
                "   st1     {v9.2s}, [x14], #8                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15], #8                     \n"
                "   st1     {v11.2s}, [x16], #8                     \n"
                
                //1
                
                "   ld1     {v0.s}[0], [x4]                   \n"
                "   ld1     {v0.s}[1], [x10]                   \n"
                "   ld1     {v0.s}[2], [x11]                   \n"
                "   ld1     {v0.s}[3], [x12]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"

                
                "   st1     {v8.s}[0], [x4]                  \n"
                "   st1     {v8.s}[1], [x10]                  \n"
                "   st1     {v8.s}[2], [x11]                  \n"
                "   st1     {v8.s}[3], [x12]                  \n"

                //1
                
                "   ld1     {v0.s}[0], [x13]                   \n"
                "   ld1     {v0.s}[1], [x14]                   \n"
                "   ld1     {v0.s}[2], [x15]                   \n"
                "   ld1     {v0.s}[3], [x16]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   st1     {v8.s}[0], [x13]                  \n"
                "   st1     {v8.s}[1], [x14]                  \n"
                "   st1     {v8.s}[2], [x15]                  \n"
                "   st1     {v8.s}[3], [x16]                  \n"

    
    ".endm                                                  \n"
    
    "M8N7_S2_01_ENTRANCE:                                                       \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #224                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N7_S2_01_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N7_S2_01_LOOP                               \n"
    
    "M8N7_S2_01_MAIN_K:                                  \n"
        "   uKERNEL_M8N7_S2_01_K0                     \n"

    "M8N7_S2_01_LOOP:                                  \n"
    "   uKERNEL_M8N7_S2_01_K1                     \n"
    "   uKERNEL_M8N7_S2_01_K2                     \n"
    "   uKERNEL_M8N7_S2_01_K3                     \n"
    "   uKERNEL_M8N7_S2_01_K4                                 \n"
    "   uKERNEL_M8N7_S2_01_K5                                 \n"
    "   uKERNEL_M8N7_S2_01_K6                                 \n"
    "   uKERNEL_M8N7_S2_01_K7                                 \n"

    "   beq     M8N7_S2_01_END_K                        \n"

    "   uKERNEL_M8N7_S2_01_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N7_S2_01_MAIN_K                       \n" 




    "M8N7_S2_01_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N7_S2_01                     \n"

    //ic != 0
    "   uKERNEL_M8N7_S2_01_END                    \n"

    "   b    M8N7_S2_01_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N7_S2_01:                               \n"
    "   M8N7_S2_01_ADD_C                                   \n"
    
    
    "M8N7_S2_01_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3_M8N7_01_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_start_2, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N7_S2_01_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"

            "   ldr     q4, [x0], #16                      \n"
            "   str     q2, [x1], #16                   \n"
            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"
            "   str     q3, [x1], #16                   \n"
            
            "   add     x11, x10, x6          \n"




            "   add     x12, x11, x6           \n"


            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"
            
            "   add     x13, x12, x6           \n"
            
            "   str     q4, [x1], #16                   \n"

            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N7_S2_01_PACK_K0                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                //update kernel 
                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"


                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"




                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N7_S2_01_PACK_K1                              \n"
                "   ld1     {v5.2s}, [x0], #8               \n"
                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"
                "   st1     {v5.2s}, [x1], #8                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q6, [x3], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N7_S2_01_PACK_K2                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"
                
                "   sub     x0, x0, #56                     \n"

                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q2, [x22], #16                        \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   ldr     q3, [x22], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"


                "   ldr     q4, [x22], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N7_S2_01_PACK_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
                
                "   str     q2, [x1], #16                   \n"

                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"


                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"


                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N7_S2_01_PACK_K4                              \n"
                "   ld1     {v5.2s}, [x22], #8               \n"
                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"
                "   st1     {v5.2s}, [x1], #8                   \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N7_S2_01_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
                
                "   sub     x22, x22, #56                     \n"

                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
                
                "   add     x22, x22, x5                    \n"

                "   ldr     q2, [x23], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"


                "   str     q2, [x1], #16                   \n"

                //update img row 1 col 1
                "   ldr     q3, [x23], #16                   \n"

                


                
                //update img row 1 col 2
                "   ldr     q4, [x23], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_PACK_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"
                "   str     q3, [x1], #16                   \n"

                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"
                "   str     q4, [x1], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"



    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_PACK_K7                              \n"
                "   ld1     {v5.2s}, [x23], #8                   \n"
                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q6, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"

                "   sub     x23, x23, #56                   \n"             

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"
                
                //correspongding add

                "   st1     {v5.2s}, [x1], #8                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"

                "   add     x23, x23, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_PACK_K8                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x0], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"


                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"


                //prepare: row 2 col 1
                "   ldr     q3, [x0], #16                   \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"



                //prepare: row 2 col 2
                "   ldr     q4, [x0], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N7_S2_01_PACK_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st3     {v16.s, v17.s, v18.s}[0], [x4]    \n"
                "   st3     {v16.s, v17.s, v18.s}[1], [x10]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   st3     {v16.s, v17.s, v18.s}[2], [x11]    \n"
                "   st3     {v16.s, v17.s, v18.s}[3], [x12]   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"

                "   st3     {v20.s, v21.s, v22.s}[0], [x13]   \n"
                "   st3     {v20.s, v21.s, v22.s}[1], [x14]    \n"
                "   st3     {v20.s, v21.s, v22.s}[2], [x15]    \n"
                "   st3     {v20.s, v21.s, v22.s}[3], [x16]    \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N7_S2_01_PACK_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"
                
                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16 \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8     \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   st2     {v16.s, v17.s}[2], [x28], #8     \n"
                "   st2     {v16.s, v17.s}[3], [x28], #8     \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8     \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8     \n"
                
                "   str     q18, [x28], #16         \n"
                "   str     q22, [x28], #16         \n"
                
                
                //4
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                //2
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4], #8                        \n"
                "   st1     {v9.2s}, [x10], #8                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11], #8                      \n"
                "   st1     {v11.2s}, [x12], #8                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13], #8                       \n"
                "   st1     {v9.2s}, [x14], #8                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15], #8                     \n"
                "   st1     {v11.2s}, [x16], #8                     \n"
                
                //1
                
                "   ld1     {v0.s}[0], [x4]                   \n"
                "   ld1     {v0.s}[1], [x10]                   \n"
                "   ld1     {v0.s}[2], [x11]                   \n"
                "   ld1     {v0.s}[3], [x12]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"

                
                "   st1     {v8.s}[0], [x4]                  \n"
                "   st1     {v8.s}[1], [x10]                  \n"
                "   st1     {v8.s}[2], [x11]                  \n"
                "   st1     {v8.s}[3], [x12]                  \n"

                //1
                
                "   ld1     {v0.s}[0], [x13]                   \n"
                "   ld1     {v0.s}[1], [x14]                   \n"
                "   ld1     {v0.s}[2], [x15]                   \n"
                "   ld1     {v0.s}[3], [x16]                   \n"
                
                "   ldr     q8, [x29], #16                  \n"
                
                
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   st1     {v8.s}[0], [x13]                  \n"
                "   st1     {v8.s}[1], [x14]                  \n"
                "   st1     {v8.s}[2], [x15]                  \n"
                "   st1     {v8.s}[3], [x16]                  \n"

    
    ".endm                                                  \n"
    
    "M8N7_S2_01_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"
    "   ldr x23, %[img_start_2]                      \n"
    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #224                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N7_S2_01_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N7_S2_01_PACK_LOOP                               \n"
    
    "M8N7_S2_01_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N7_S2_01_PACK_K0                     \n"

    "M8N7_S2_01_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N7_S2_01_PACK_K1                     \n"
    "   uKERNEL_M8N7_S2_01_PACK_K2                     \n"
    "   uKERNEL_M8N7_S2_01_PACK_K3                     \n"
    "   uKERNEL_M8N7_S2_01_PACK_K4                                 \n"
    "   uKERNEL_M8N7_S2_01_PACK_K5                                 \n"
    "   uKERNEL_M8N7_S2_01_PACK_K6                                 \n"
    "   uKERNEL_M8N7_S2_01_PACK_K7                                 \n"

    "   beq     M8N7_S2_01_PACK_END_K                        \n"

    "   uKERNEL_M8N7_S2_01_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N7_S2_01_PACK_MAIN_K                       \n" 




    "M8N7_S2_01_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N7_S2_01_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N7_S2_01_PACK_END                    \n"

    "   b    M8N7_S2_01_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N7_S2_01_PACK:                               \n"
    "   M8N7_S2_01_PACK_ADD_C                                   \n"
    
    
    "M8N7_S2_01_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_start_2]                 "m"     (img_start_2),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N8_00_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_00_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            
            "   ldr     q4, [x0], #16                      \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"
            "   str     q2, [x1], #16                   \n"
            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            "   ldr     q5, [x0], #16               \n"

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[1]          \n"
            "   str     q3, [x1], #16                   \n"
            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"
            
            "   add     x12, x11, x6           \n"
            
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"
            
            "   str     q4, [x1], #16                   \n"
            
            "   fmul    v23.4s, v1.4s, v5.s[1]          \n"
            "   add     x13, x12, x6           \n"
            
            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_PACK_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n" 
                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[1]          \n"

                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_00_PACK_K4                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                
                "   str     q5, [x1], #16                   \n"
                
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[2]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_00_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
                
                "   sub     x0, x0, #64                     \n"

                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q2, [x22], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                
                "   ldr     q3, [x22], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"

                "   ldr     q4, [x22], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_PACK_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"
                
                "   str     q3, [x1], #16                   \n"
                
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                
                "   ldr     q5, [x22], #16                   \n"
                
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"
                
                "   str     q4, [x1], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[1]          \n"
                "   str     q5, [x1], #16                   \n"
                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[1]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q6, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
                "   sub     x22, x22, #64                   \n"             

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
                
                "   add     x22, x22, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_PACK_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x0], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   ldr     q3, [x0], #16                   \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   ldr     q4, [x0], #16                   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"
                "   ldr     q5, [x0], #16                   \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_00_PACK_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_PACK_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"
                
                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"
                
                
                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                
                // 8
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"
                
    
    ".endm                                                  \n"
    
    
    "M8N8_S2_00_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_00_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_00_PACK_LOOP                               \n"
    
    "M8N8_S2_00_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_00_PACK_K3                     \n"

    "M8N8_S2_00_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_00_PACK_K4                                 \n"
    "   uKERNEL_M8N8_S2_00_PACK_K5                                 \n"
    "   uKERNEL_M8N8_S2_00_PACK_K6                                 \n"
    "   uKERNEL_M8N8_S2_00_PACK_K7                                 \n"

    "   beq     M8N8_S2_00_PACK_END_K                        \n"

    "   uKERNEL_M8N8_S2_00_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_00_PACK_MAIN_K                       \n" 




    "M8N8_S2_00_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_00_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_00_PACK_END                    \n"

    "   b    M8N8_S2_00_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_00_PACK:                               \n"
    "   M8N8_S2_00_PACK_ADD_C                                   \n"
    
    
    "M8N8_S2_00_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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




void direct_3x3_M8N8_01_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_start_2, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_01_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"

            "   ldr     q4, [x0], #16                      \n"
            "   str     q2, [x1], #16                   \n"
            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            "   ldr     q5, [x0], #16                      \n"

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[1]          \n"
            "   str     q3, [x1], #16                   \n"
            
            "   add     x11, x10, x6          \n"
            "   add     x12, x11, x6           \n"


            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"
            
            "   str     q4, [x1], #16                   \n"
            
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"
            "   fmul    v23.4s, v1.4s, v5.s[1]          \n"
            
            "   add     x13, x12, x6           \n"

            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N8_S2_01_PACK_K0                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                //update kernel 
                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[1]          \n"

                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[1]          \n"
                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_01_PACK_K1                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"
                "   str     q5, [x1], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[2]          \n"
                "   ldr     q6, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_01_PACK_K2                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"
                
                "   sub     x0, x0, #64                     \n"

                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q2, [x22], #16                        \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                "   ldr     q3, [x22], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   ldr     q4, [x22], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N8_S2_01_PACK_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"
                
                "   str     q2, [x1], #16                   \n"

                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
                "   ldr     q5, [x22], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[1]          \n"

                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[1]          \n"

                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_01_PACK_K4                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   str     q5, [x1], #16                   \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"


                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[2]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_01_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
                
                "   sub     x22, x22, #64                     \n"

                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"
                
                "   add     x22, x22, x5                    \n"

                "   ldr     q2, [x23], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                
                "   ldr     q3, [x23], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"

                "   str     q2, [x1], #16                   \n"
                "   ldr     q4, [x23], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_PACK_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                "   ldr     q5, [x23], #16                   \n"
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"
                "   str     q3, [x1], #16                   \n"

                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"
                "   str     q4, [x1], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[1]          \n"
                
                "   str     q5, [x1], #16                   \n"
                
                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[1]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q6, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
                "   sub     x23, x23, #64                   \n"             

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
                "   add     x23, x23, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_PACK_K8                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x0], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                "   ldr     q3, [x0], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   ldr     q4, [x0], #16                   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"


                //prepare: row 2 col 2
                "   ldr     q5, [x0], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_01_PACK_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_PACK_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"
                
                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"
                
                
                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                
                // 8
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"

    
    ".endm                                                  \n"
    
    "M8N8_S2_01_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"
    "   ldr x23, %[img_start_2]                      \n"
    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_01_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_01_PACK_LOOP                               \n"
    
    "M8N8_S2_01_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_01_PACK_K0                     \n"

    "M8N8_S2_01_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_01_PACK_K1                     \n"
    "   uKERNEL_M8N8_S2_01_PACK_K2                     \n"
    "   uKERNEL_M8N8_S2_01_PACK_K3                     \n"
    "   uKERNEL_M8N8_S2_01_PACK_K4                                 \n"
    "   uKERNEL_M8N8_S2_01_PACK_K5                                 \n"
    "   uKERNEL_M8N8_S2_01_PACK_K6                                 \n"
    "   uKERNEL_M8N8_S2_01_PACK_K7                                 \n"

    "   beq     M8N8_S2_01_PACK_END_K                        \n"

    "   uKERNEL_M8N8_S2_01_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_01_PACK_MAIN_K                       \n" 




    "M8N8_S2_01_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_01_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_01_PACK_END                    \n"

    "   b    M8N8_S2_01_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_01_PACK:                               \n"
    "   M8N8_S2_01_PACK_ADD_C                                   \n"
    
    
    "M8N8_S2_01_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_start_2]                 "m"     (img_start_2),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N8_10_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_10_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x0], #16                      \n"
            "   str     q2, [x1], #16                   \n"
            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   ldr     q5, [x0], #16               \n"
            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
            
            "   str     q3, [x1], #16                   \n"

            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   str     q4, [x1], #16                   \n"
            
            "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[2]          \n"

            "   str     q5, [x1], #16                   \n"

            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"
            "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_PACK_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   str     q2, [x1], #16                   \n"
            
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q3, [x1], #16                   \n"

                "   ldr     q27, [x3], #16                   \n"

            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   str     q4, [x1], #16                   \n"
            
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"


                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
            "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmla    v19.4s, v0.4s, v5.s[2]          \n"

                "   str     q5, [x1], #16                   \n"

            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
            "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmla    v23.4s, v1.4s, v5.s[2]          \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_10_PACK_K4                              \n"
                "   ld1     {v6.2s}, [x0]              \n"
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   st1     {v6.2s}, [x1], #8              \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[3]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_10_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
                
                "   sub     x0, x0, #64                     \n"

                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q2, [x22], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v0.4s, v6.s[0]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"
                
                "   ldr     q3, [x22], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v1.4s, v6.s[0]          \n"

                "   str     q3, [x1], #16                   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_PACK_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
                
                "   ldr     q4, [x22], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
                
                "   ldr     q5, [x22], #16                   \n"

                "   ldr     q0, [x3], #16                   \n"
                
                "   str     q4, [x1], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                
                "   str     q5, [x1], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[2]          \n"

                "   ld1     {v6.2s}, [x22]              \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   st1     {v6.2s}, [x1], #8              \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q27, [x3], #16                   \n"


                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
                "   sub     x22, x22, #64                   \n"             


                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
                
                "   add     x22, x22, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_PACK_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"


                "   ldr     q2, [x0], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"

                "   ldr     q3, [x0], #16                   \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"

                "   ldr     q4, [x0], #16                   \n"
                
                "   ldr     q5, [x0], #16                   \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_10_PACK_ADD_C                                   \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_PACK_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                

                // 8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"
    
    ".endm                                                  \n"
    
    "M8N8_S2_10_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_10_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_10_PACK_LOOP                               \n"
    
    "M8N8_S2_10_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_10_PACK_K3                     \n"

    "M8N8_S2_10_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_10_PACK_K4                                 \n"
    "   uKERNEL_M8N8_S2_10_PACK_K5                                 \n"
    "   uKERNEL_M8N8_S2_10_PACK_K6                                 \n"
    "   uKERNEL_M8N8_S2_10_PACK_K7                                 \n"

    "   beq     M8N8_S2_10_PACK_END_K                        \n"

    "   uKERNEL_M8N8_S2_10_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_10_PACK_MAIN_K                       \n" 




    "M8N8_S2_10_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_10_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_10_PACK_END                    \n"

    "   b    M8N8_S2_10_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_10_PACK:                               \n"
    "   M8N8_S2_10_PACK_ADD_C                                   \n"
    
    
    "M8N8_S2_10_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N6_10_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N6_S2_10_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x0], #16                      \n"
            "   str     q2, [x1], #16                   \n"
            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
            
            "   str     q3, [x1], #16                   \n"

            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   str     q4, [x1], #16                   \n"

            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"

            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_PACK_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   str     q2, [x1], #16                   \n"
            
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q3, [x1], #16                   \n"

                "   ldr     q27, [x3], #16                   \n"

            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   str     q4, [x1], #16                   \n"
            
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"


                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"


            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N6_S2_10_PACK_K4                              \n"
                "   ld1     {v5.2s}, [x0]              \n"
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   st1     {v5.2s}, [x1], #8              \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N6_S2_10_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
                
                "   sub     x0, x0, #48                     \n"

                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x22], #16                        \n"
                
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
                "   str     q2, [x1], #16                   \n"
                "   add     x0, x0, x5                    \n"


                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"
                "   ldr     q3, [x22], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"
                "   str     q3, [x1], #16                   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_PACK_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
                
                "   ldr     q4, [x22], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   str     q4, [x1], #16                   \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"

                "   ld1     {v5.2s}, [x22]              \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"

                "   st1     {v5.2s}, [x1], #8              \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q27, [x3], #16                   \n"


                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"

                "   sub     x22, x22, #48                   \n"             


                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   add     x22, x22, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_PACK_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                
                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x0], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                "   ldr     q3, [x0], #16                   \n"
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   ldr     q4, [x0], #16                   \n"
                "   prfm PLDL1KEEP, [x3, #256]              \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N6_S2_10_PACK_ADD_C                                   \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"

                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x10]    \n"
                "   st2     {v16.s, v17.s}[2], [x11]    \n"
                "   st2     {v16.s, v17.s}[3], [x12]    \n"

                "   st2     {v20.s, v21.s}[0], [x13]   \n"
                "   st2     {v20.s, v21.s}[1], [x14]   \n"
                "   st2     {v20.s, v21.s}[2], [x15]   \n"
                "   st2     {v20.s, v21.s}[3], [x16]   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_PACK_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[2], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[3], [x28], #8 \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8  \n"
                

                // 8
                
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4]                        \n"
                "   st1     {v9.2s}, [x10]                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11]                      \n"
                "   st1     {v11.2s}, [x12]                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13]                       \n"
                "   st1     {v9.2s}, [x14]                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15]                     \n"
                "   st1     {v11.2s}, [x16]                     \n"
    
    ".endm                                                  \n"
    
    "M8N6_S2_10_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #192                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N6_S2_10_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N6_S2_10_PACK_LOOP                               \n"
    
    "M8N6_S2_10_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N6_S2_10_PACK_K3                     \n"

    "M8N6_S2_10_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N6_S2_10_PACK_K4                                 \n"
    "   uKERNEL_M8N6_S2_10_PACK_K5                                 \n"
    "   uKERNEL_M8N6_S2_10_PACK_K6                                 \n"
    "   uKERNEL_M8N6_S2_10_PACK_K7                                 \n"

    "   beq     M8N6_S2_10_PACK_END_K                        \n"

    "   uKERNEL_M8N6_S2_10_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N6_S2_10_PACK_MAIN_K                       \n" 




    "M8N6_S2_10_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N6_S2_10_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N6_S2_10_PACK_END                    \n"

    "   b    M8N6_S2_10_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N6_S2_10_PACK:                               \n"
    "   M8N6_S2_10_PACK_ADD_C                                   \n"
    
    
    "M8N6_S2_10_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N4_10_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N4_S2_10_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   str     q2, [x1], #16                   \n"
            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
            
            "   str     q3, [x1], #16                   \n"

            
            "   add     x11, x10, x6          \n"

            "   add     x12, x11, x6           \n"
            "   add     x13, x12, x6           \n"
            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"
            "   add     x15, x14, x6          \n"
            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_PACK_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   str     q2, [x1], #16                   \n"
            
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q3, [x1], #16                   \n"

                "   ldr     q27, [x3], #16                   \n"

            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   ldr     q7, [x3], #16                   \n"




    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N4_S2_10_PACK_K4                              \n"
                "   ld1     {v4.2s}, [x0]              \n"
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   st1     {v4.2s}, [x1], #8              \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   ldr     q1, [x3], #16                   \n"


                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N4_S2_10_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                
                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
                
                "   sub     x0, x0, #32                     \n"

                "   ldr     q7, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x22], #16                        \n"
                
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
                "   str     q2, [x1], #16                   \n"
                "   add     x0, x0, x5                    \n"

                "   ldr     q3, [x22], #16                   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_PACK_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"

                "   str     q3, [x1], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   ld1     {v4.2s}, [x22]              \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   st1     {v4.2s}, [x1], #8              \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"

                "   sub     x22, x22, #32                   \n" 
                "   ldr     q27, [x3], #16                   \n"


                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"


                "   ldr     q7, [x3], #16                   \n"

                "   add     x22, x22, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_PACK_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                
                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x0], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                "   ldr     q3, [x0], #16                   \n"


                "   prfm PLDL1KEEP, [x3, #256]              \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N4_S2_10_PACK_ADD_C                                   \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"





    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_PACK_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"


                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                
    
    ".endm                                                  \n"
    
    "M8N4_S2_10_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #128                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N4_S2_10_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N4_S2_10_PACK_LOOP                               \n"
    
    "M8N4_S2_10_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N4_S2_10_PACK_K3                     \n"

    "M8N4_S2_10_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N4_S2_10_PACK_K4                                 \n"
    "   uKERNEL_M8N4_S2_10_PACK_K5                                 \n"
    "   uKERNEL_M8N4_S2_10_PACK_K6                                 \n"
    "   uKERNEL_M8N4_S2_10_PACK_K7                                 \n"

    "   beq     M8N4_S2_10_PACK_END_K                        \n"

    "   uKERNEL_M8N4_S2_10_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N4_S2_10_PACK_MAIN_K                       \n" 




    "M8N4_S2_10_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N4_S2_10_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N4_S2_10_PACK_END                    \n"

    "   b    M8N4_S2_10_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N4_S2_10_PACK:                               \n"
    "   M8N4_S2_10_PACK_ADD_C                                   \n"
    
    
    "M8N4_S2_10_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3_M8N8_11_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_start_2, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_11_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"

            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x0], #16                      \n"
            "   str     q2, [x1], #16                   \n"
            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   ldr     q5, [x0], #16               \n"
            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
            
            "   str     q3, [x1], #16                   \n"

            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   str     q4, [x1], #16                   \n"
            
            "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[2]          \n"


            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"
            "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N8_S2_11_PACK_K0                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                //update kernel 
                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[2]          \n"

                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[2]          \n"
                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_11_PACK_K1                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                
                "   ld1     {v6.2s}, [x0]              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
                "   str     q5, [x1], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   st1     {v6.2s}, [x1], #8               \n"
                
                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_11_PACK_K2                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
                
                "   sub     x0, x0, #64                     \n"

                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q2, [x22], #16                        \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   ldr     q3, [x22], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"

                "   ldr     q4, [x22], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N8_S2_11_PACK_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q2, [x1], #16                   \n"

                "   ldr     q27, [x3], #16                   \n"


            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
                "   ldr     q5, [x22], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   ld1     {v6.2s}, [x22]              \n"
            
            "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
            
                "   str     q3, [x1], #16                   \n"

            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
            "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmla    v23.4s, v1.4s, v5.s[2]          \n"

                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_11_PACK_K4                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   str     q5, [x1], #16                   \n"
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   st1     {v6.2s}, [x1], #8              \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[3]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_11_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
                
                "   sub     x22, x22, #64                     \n"

                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
                
                "   add     x22, x22, x5                    \n"

                "   ldr     q2, [x23], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"
                
                "   ldr     q3, [x23], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v1.4s, v6.s[0]          \n"

                "   str     q2, [x1], #16                   \n"
                "   ldr     q4, [x23], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_PACK_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
                
                "   ldr     q5, [x23], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
                "   str     q3, [x1], #16                   \n"

                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                
                "   ld1     {v6.2s}, [x23]              \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"
                "   str     q4, [x1], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[2]          \n"
                
                "   str     q5, [x1], #16                   \n"
                
                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   st1     {v6.2s}, [x1], #8              \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"

                //update kernel 
                "   ldr     q27, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
                "   sub     x23, x23, #64                   \n"             

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"
                "   add     x23, x23, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_PACK_K8                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x0], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   ldr     q3, [x0], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                
                "   ldr     q4, [x0], #16                   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"

                "   ldr     q5, [x0], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_11_PACK_ADD_C                                   \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_PACK_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                

                // 8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"

    
    ".endm                                                  \n"
    
    "M8N8_S2_11_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"
    "   ldr x23, %[img_start_2]                      \n"
    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_11_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_11_PACK_LOOP                               \n"
    
    "M8N8_S2_11_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_11_PACK_K0                     \n"

    "M8N8_S2_11_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_11_PACK_K1                     \n"
    "   uKERNEL_M8N8_S2_11_PACK_K2                     \n"
    "   uKERNEL_M8N8_S2_11_PACK_K3                     \n"
    "   uKERNEL_M8N8_S2_11_PACK_K4                                 \n"
    "   uKERNEL_M8N8_S2_11_PACK_K5                                 \n"
    "   uKERNEL_M8N8_S2_11_PACK_K6                                 \n"
    "   uKERNEL_M8N8_S2_11_PACK_K7                                 \n"

    "   beq     M8N8_S2_11_PACK_END_K                        \n"

    "   uKERNEL_M8N8_S2_11_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_11_PACK_MAIN_K                       \n" 




    "M8N8_S2_11_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_11_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_11_PACK_END                    \n"

    "   b    M8N8_S2_11_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_11_PACK:                               \n"
    "   M8N8_S2_11_PACK_ADD_C                                   \n"
    
    
    "M8N8_S2_11_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_start_2]                 "m"     (img_start_2),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3_M8N6_11_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_start_2, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N6_S2_11_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"

            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x0], #16                      \n"
            "   str     q2, [x1], #16                   \n"
            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
            
            "   str     q3, [x1], #16                   \n"

            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   str     q4, [x1], #16                   \n"


            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"

            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N6_S2_11_PACK_K0                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                //update kernel 
                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"


                "   str     q3, [x1], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"

                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N6_S2_11_PACK_K1                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                
                "   ld1     {v5.2s}, [x0]              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   st1     {v5.2s}, [x1], #8               \n"
                
                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"

                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N6_S2_11_PACK_K2                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
                
                "   sub     x0, x0, #48                     \n"

                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q2, [x22], #16                        \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"

                "   ldr     q3, [x22], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"


                "   ldr     q4, [x22], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N6_S2_11_PACK_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q2, [x1], #16                   \n"

                "   ldr     q27, [x3], #16                   \n"


            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   ld1     {v5.2s}, [x22]              \n"
            

            
                "   str     q3, [x1], #16                   \n"

            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"


                "   str     q4, [x1], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N6_S2_11_PACK_K4                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"

                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   st1     {v5.2s}, [x1], #8              \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"


                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N6_S2_11_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
                
                "   sub     x22, x22, #48                     \n"

                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
                
                "   add     x22, x22, x5                    \n"

                "   ldr     q2, [x23], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"
                
                "   ldr     q3, [x23], #16                   \n"


                "   str     q2, [x1], #16                   \n"
                "   ldr     q4, [x23], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_PACK_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"

                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
                "   str     q3, [x1], #16                   \n"

                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                
                "   ld1     {v5.2s}, [x23]              \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"
                "   str     q4, [x1], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"

                
                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"

                "   st1     {v5.2s}, [x1], #8              \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"

                //update kernel 
                "   ldr     q27, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"

                "   sub     x23, x23, #48                   \n"             

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"

                "   add     x23, x23, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_PACK_K8                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x0], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"

                "   ldr     q3, [x0], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                
                "   ldr     q4, [x0], #16                   \n"

    ".endm                                                  \n"
    

    
    ".macro M8N6_S2_11_PACK_ADD_C                                   \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"

                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x10]    \n"
                "   st2     {v16.s, v17.s}[2], [x11]    \n"
                "   st2     {v16.s, v17.s}[3], [x12]    \n"

                "   st2     {v20.s, v21.s}[0], [x13]   \n"
                "   st2     {v20.s, v21.s}[1], [x14]   \n"
                "   st2     {v20.s, v21.s}[2], [x15]   \n"
                "   st2     {v20.s, v21.s}[3], [x16]   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_PACK_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[2], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[3], [x28], #8 \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8  \n"
                

                // 8
                
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4]                        \n"
                "   st1     {v9.2s}, [x10]                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11]                      \n"
                "   st1     {v11.2s}, [x12]                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13]                       \n"
                "   st1     {v9.2s}, [x14]                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15]                     \n"
                "   st1     {v11.2s}, [x16]                     \n"

    
    ".endm                                                  \n"
    
    "M8N6_S2_11_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"
    "   ldr x23, %[img_start_2]                      \n"
    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #192                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N6_S2_11_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N6_S2_11_PACK_LOOP                               \n"
    
    "M8N6_S2_11_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N6_S2_11_PACK_K0                     \n"

    "M8N6_S2_11_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N6_S2_11_PACK_K1                     \n"
    "   uKERNEL_M8N6_S2_11_PACK_K2                     \n"
    "   uKERNEL_M8N6_S2_11_PACK_K3                     \n"
    "   uKERNEL_M8N6_S2_11_PACK_K4                                 \n"
    "   uKERNEL_M8N6_S2_11_PACK_K5                                 \n"
    "   uKERNEL_M8N6_S2_11_PACK_K6                                 \n"
    "   uKERNEL_M8N6_S2_11_PACK_K7                                 \n"

    "   beq     M8N6_S2_11_PACK_END_K                        \n"

    "   uKERNEL_M8N6_S2_11_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N6_S2_11_PACK_MAIN_K                       \n" 




    "M8N6_S2_11_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N6_S2_11_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N6_S2_11_PACK_END                    \n"

    "   b    M8N6_S2_11_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N6_S2_11_PACK:                               \n"
    "   M8N6_S2_11_PACK_ADD_C                                   \n"
    
    
    "M8N6_S2_11_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_start_2]                 "m"     (img_start_2),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3_M8N4_11_p1_s2_pack(float *img_start_0, float *img_start_1, float *img_start_2, float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N4_S2_11_PACK_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x0], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x0], #16                      \n"
            "   add     x10, x4, x6                     \n"

            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            "   str     q2, [x1], #16                   \n"
            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"
            
            "   str     q3, [x1], #16                   \n"
            
            "   add     x11, x10, x6          \n"


            
            "   add     x12, x11, x6           \n"

            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N4_S2_11_PACK_K0                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"
                
                "   str     q2, [x1], #16                   \n"
                
                //update kernel 
                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   ldr     q7, [x3], #16                   \n"




                "   str     q3, [x1], #16                   \n"




    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N4_S2_11_PACK_K1                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                
                "   ld1     {v4.2s}, [x0]              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   st1     {v4.2s}, [x1], #8               \n"


                "   ldr     q27, [x3], #16                   \n"



                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N4_S2_11_PACK_K2                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"
                
                "   sub     x0, x0, #32                     \n"

                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x22], #16                        \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   add     x0, x0, x5                    \n"

                "   ldr     q3, [x22], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N4_S2_11_PACK_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   str     q2, [x1], #16                   \n"
            
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

            "   ldr     q27, [x3], #16                   \n"


            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   str     q3, [x1], #16                   \n"
            
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

            "   ldr     q7, [x3], #16                   \n"




    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N4_S2_11_PACK_K4                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   ld1     {v4.2s}, [x22]              \n" 
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st1     {v4.2s}, [x1], #8              \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"


                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N4_S2_11_PACK_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                
                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"
                
                "   sub     x22, x22, #32                     \n"

                "   ldr     q7, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x23], #16                        \n"
                
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"
                "   add     x22, x22, x5                    \n"
                
                "   ldr     q3, [x23], #16                   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_PACK_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
                "   str     q2, [x1], #16                   \n"
                
                "   ld1     {v4.2s}, [x23]              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
                "   str     q3, [x1], #16                   \n"

                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                
                "   ldr     q1, [x3], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"
                "   st1     {v4.2s}, [x1], #8              \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_PACK_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   sub     x23, x23, #32                   \n"             
                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   add     x23, x23, x5                    \n"             
                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_PACK_K8                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                
                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x0], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"

                "   ldr     q3, [x0], #16                   \n"

    ".endm                                                  \n"
    

    
    ".macro M8N4_S2_11_PACK_ADD_C                                   \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_PACK_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"


                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

    
    ".endm                                                  \n"
    
    "M8N4_S2_11_PACK_ENTRANCE:                                                       \n"
    "   ldr x0, %[img_start_0]                    \n"
    "   ldr x22, %[img_start_1]                      \n"
    "   ldr x23, %[img_start_2]                      \n"
    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x0, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #128                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N4_S2_11_PACK_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N4_S2_11_PACK_LOOP                               \n"
    
    "M8N4_S2_11_PACK_MAIN_K:                                  \n"
        "   uKERNEL_M8N4_S2_11_PACK_K0                     \n"

    "M8N4_S2_11_PACK_LOOP:                                  \n"
    "   uKERNEL_M8N4_S2_11_PACK_K1                     \n"
    "   uKERNEL_M8N4_S2_11_PACK_K2                     \n"
    "   uKERNEL_M8N4_S2_11_PACK_K3                     \n"
    "   uKERNEL_M8N4_S2_11_PACK_K4                                 \n"
    "   uKERNEL_M8N4_S2_11_PACK_K5                                 \n"
    "   uKERNEL_M8N4_S2_11_PACK_K6                                 \n"
    "   uKERNEL_M8N4_S2_11_PACK_K7                                 \n"

    "   beq     M8N4_S2_11_PACK_END_K                        \n"

    "   uKERNEL_M8N4_S2_11_PACK_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N4_S2_11_PACK_MAIN_K                       \n" 




    "M8N4_S2_11_PACK_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N4_S2_11_PACK                     \n"

    //ic != 0
    "   uKERNEL_M8N4_S2_11_PACK_END                    \n"

    "   b    M8N4_S2_11_PACK_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N4_S2_11_PACK:                               \n"
    "   M8N4_S2_11_PACK_ADD_C                                   \n"
    
    
    "M8N4_S2_11_PACK_END:                                       \n"
    :

    :
        [img_start_0]                 "m"     (img_start_0),
        [img_start_1]                 "m"     (img_start_1),
        [img_start_2]                 "m"     (img_start_2),
        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


//
void direct_3x3_M8N8_00_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_00_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            
            "   ldr     q4, [x1], #16                      \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"

            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            "   ldr     q5, [x1], #16               \n"

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[1]          \n"

            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"
            
            "   add     x12, x11, x6           \n"
            
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"

            
            "   fmul    v23.4s, v1.4s, v5.s[1]          \n"
            "   add     x13, x12, x6           \n"
            
            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"

                
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n" 
                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[1]          \n"



                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[1]          \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_00_K4                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                

                
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[2]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_00_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"
                


                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"

                
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                
                "   ldr     q3, [x1], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"

                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"

                
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                
                "   ldr     q5, [x1], #16                   \n"
                
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"

                
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[1]          \n"

                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[1]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"


                "   ldr     q6, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[2]          \n"
 

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[2]          \n"

                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x1], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   ldr     q3, [x1], #16                   \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   ldr     q4, [x1], #16                   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"
                "   ldr     q5, [x1], #16                   \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_00_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_00_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"
                
                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"
                
                
                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                
                // 8
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"
                
    
    ".endm                                                  \n"
    
    
    "M8N8_S2_00_ENTRANCE:                                                       \n"


    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_00_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_00_LOOP                               \n"
    
    "M8N8_S2_00_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_00_K3                     \n"

    "M8N8_S2_00_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_00_K4                                 \n"
    "   uKERNEL_M8N8_S2_00_K5                                 \n"
    "   uKERNEL_M8N8_S2_00_K6                                 \n"
    "   uKERNEL_M8N8_S2_00_K7                                 \n"

    "   beq     M8N8_S2_00_END_K                        \n"

    "   uKERNEL_M8N8_S2_00_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_00_MAIN_K                       \n" 




    "M8N8_S2_00_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_00                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_00_END                    \n"

    "   b    M8N8_S2_00_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_00:                               \n"
    "   M8N8_S2_00_ADD_C                                   \n"
    
    
    "M8N8_S2_00_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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




void direct_3x3_M8N8_01_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_01_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   movi v8.4s, #0            \n"
            "   movi v12.4s, #0            \n"

            
            "   fmul    v9.4s, v0.4s, v2.s[1]           \n"
            "   fmul    v10.4s, v0.4s, v2.s[3]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[1]          \n"

            "   ldr     q4, [x1], #16                      \n"

            
            
            
            "   fmul    v13.4s, v1.4s, v2.s[1]          \n"
            "   fmul    v14.4s, v1.4s, v2.s[3]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[1]          \n"
            "   ldr     q5, [x1], #16                      \n"

            "   fmul    v16.4s, v0.4s, v3.s[3]          \n"
            "   fmul    v17.4s, v0.4s, v4.s[1]          \n"
            "   fmul    v18.4s, v0.4s, v4.s[3]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[1]          \n"

            
            "   add     x11, x10, x6          \n"
            "   add     x12, x11, x6           \n"


            "   fmul    v20.4s, v1.4s, v3.s[3]          \n"
            "   fmul    v21.4s, v1.4s, v4.s[1]          \n"

            
            "   fmul    v22.4s, v1.4s, v4.s[3]          \n"
            "   fmul    v23.4s, v1.4s, v5.s[1]          \n"
            
            "   add     x13, x12, x6           \n"

            "   ldr     q6, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N8_S2_01_K0                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"

                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[1]          \n"


                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[1]          \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_01_K1                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[2]          \n"
                "   ldr     q6, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_01_K2                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                "   ldr     q3, [x1], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N8_S2_01_K3                              \n"

                "   fmla    v9.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v0.4s, v2.s[3]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[1]          \n"


                "   ldr     q6, [x3], #16                   \n"


                "   fmla    v13.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[1]          \n"
                "   ldr     q5, [x1], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[1]          \n"



                "   fmla    v20.4s, v1.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[1]          \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_01_K4                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[2]           \n"

                "   fmla    v10.4s, v6.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[2]          \n"


                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[2]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_01_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q6, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                
                "   ldr     q3, [x1], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"

                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_K6                              \n"

                "   fmla    v9.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v10.4s, v6.4s, v2.s[3]          \n"
                "   ldr     q5, [x1], #16                   \n"
                "   fmla    v11.4s, v6.4s, v3.s[1]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v13.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v14.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[1]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v6.4s, v3.s[3]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v18.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[1]          \n"

                
                "   fmla    v20.4s, v7.4s, v3.s[3]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v22.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[1]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q6, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[0]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[2]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[2]          \n"

                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_K8                              \n"

                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x1], #16                   \n"

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                "   ldr     q3, [x1], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   ldr     q4, [x1], #16                   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"


                //prepare: row 2 col 2
                "   ldr     q5, [x1], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_01_ADD_C                                   \n"
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"


                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_01_END                             \n"
                
                "   fmla    v8.4s, v6.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v6.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v6.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v6.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //

                "   fmla    v16.4s, v6.4s, v4.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                
                "   fmla    v17.4s, v6.4s, v4.s[3]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                
                "   fmla    v18.4s, v6.4s, v5.s[1]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                
                "   fmla    v19.4s, v6.4s, v5.s[3]          \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"
                
                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"
                
                
                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                
                // 8
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"

    
    ".endm                                                  \n"
    
    "M8N8_S2_01_ENTRANCE:                                                       \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_01_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_01_LOOP                               \n"
    
    "M8N8_S2_01_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_01_K0                     \n"

    "M8N8_S2_01_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_01_K1                     \n"
    "   uKERNEL_M8N8_S2_01_K2                     \n"
    "   uKERNEL_M8N8_S2_01_K3                     \n"
    "   uKERNEL_M8N8_S2_01_K4                                 \n"
    "   uKERNEL_M8N8_S2_01_K5                                 \n"
    "   uKERNEL_M8N8_S2_01_K6                                 \n"
    "   uKERNEL_M8N8_S2_01_K7                                 \n"

    "   beq     M8N8_S2_01_END_K                        \n"

    "   uKERNEL_M8N8_S2_01_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_01_MAIN_K                       \n" 




    "M8N8_S2_01_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_01                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_01_END                    \n"

    "   b    M8N8_S2_01_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_01:                               \n"
    "   M8N8_S2_01_ADD_C                                   \n"
    
    
    "M8N8_S2_01_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N8_10_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_10_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x1], #16                      \n"

            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   ldr     q5, [x1], #16               \n"
            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"


            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"

            
            "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[2]          \n"


            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"
            "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            

            
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                "   ldr     q27, [x3], #16                   \n"

            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"

            
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"


                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
            "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmla    v19.4s, v0.4s, v5.s[2]          \n"

            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
            "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmla    v23.4s, v1.4s, v5.s[2]          \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_10_K4                              \n"
                "   ld1     {v6.2s}, [x1], #8              \n"
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[3]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_10_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"


                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"

                
                "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v0.4s, v6.s[0]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"
                
                "   ldr     q3, [x1], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v1.4s, v6.s[0]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
                
                "   ldr     q4, [x1], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"
                
                "   ldr     q5, [x1], #16                   \n"

                "   ldr     q0, [x3], #16                   \n"


                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"

                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[2]          \n"

                "   ld1     {v6.2s}, [x1], #8              \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q27, [x3], #16                   \n"


                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"



                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"

                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"


                "   ldr     q2, [x1], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"

                "   ldr     q3, [x1], #16                   \n"

                "   prfm PLDL1KEEP, [x3, #256]              \n"

                "   ldr     q4, [x1], #16                   \n"
                
                "   ldr     q5, [x1], #16                   \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_10_ADD_C                                   \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_10_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                

                // 8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"
    
    ".endm                                                  \n"
    
    "M8N8_S2_10_ENTRANCE:                                                       \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_10_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_10_LOOP                               \n"
    
    "M8N8_S2_10_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_10_K3                     \n"

    "M8N8_S2_10_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_10_K4                                 \n"
    "   uKERNEL_M8N8_S2_10_K5                                 \n"
    "   uKERNEL_M8N8_S2_10_K6                                 \n"
    "   uKERNEL_M8N8_S2_10_K7                                 \n"

    "   beq     M8N8_S2_10_END_K                        \n"

    "   uKERNEL_M8N8_S2_10_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_10_MAIN_K                       \n" 




    "M8N8_S2_10_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_10                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_10_END                    \n"

    "   b    M8N8_S2_10_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_10:                               \n"
    "   M8N8_S2_10_ADD_C                                   \n"
    
    
    "M8N8_S2_10_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N6_10_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N6_S2_10_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x1], #16                      \n"

            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"


            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"


            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"

            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"

            
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"


                "   ldr     q27, [x3], #16                   \n"

            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"

            
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"


                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"


            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N6_S2_10_K4                              \n"
                "   ld1     {v5.2s}, [x1], #8              \n"
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                //update kernel
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N6_S2_10_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"

                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x1], #16                        \n"
                
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"



                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"
                "   ldr     q3, [x1], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
                
                "   ldr     q4, [x1], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"

                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"

                "   ld1     {v5.2s}, [x1], #8              \n"

                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q27, [x3], #16                   \n"


                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
    


                "   ldr     q7, [x3], #16                   \n"


                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"

                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                
                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x1], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                "   ldr     q3, [x1], #16                   \n"
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   ldr     q4, [x1], #16                   \n"
                "   prfm PLDL1KEEP, [x3, #256]              \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N6_S2_10_ADD_C                                   \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"

                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x10]    \n"
                "   st2     {v16.s, v17.s}[2], [x11]    \n"
                "   st2     {v16.s, v17.s}[3], [x12]    \n"

                "   st2     {v20.s, v21.s}[0], [x13]   \n"
                "   st2     {v20.s, v21.s}[1], [x14]   \n"
                "   st2     {v20.s, v21.s}[2], [x15]   \n"
                "   st2     {v20.s, v21.s}[3], [x16]   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_10_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[2], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[3], [x28], #8 \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8  \n"
                

                // 8
                
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4]                        \n"
                "   st1     {v9.2s}, [x10]                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11]                      \n"
                "   st1     {v11.2s}, [x12]                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13]                       \n"
                "   st1     {v9.2s}, [x14]                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15]                     \n"
                "   st1     {v11.2s}, [x16]                     \n"
    
    ".endm                                                  \n"
    
    "M8N6_S2_10_ENTRANCE:                                                       \n"


    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #192                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N6_S2_10_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N6_S2_10_LOOP                               \n"
    
    "M8N6_S2_10_MAIN_K:                                  \n"
        "   uKERNEL_M8N6_S2_10_K3                     \n"

    "M8N6_S2_10_LOOP:                                  \n"
    "   uKERNEL_M8N6_S2_10_K4                                 \n"
    "   uKERNEL_M8N6_S2_10_K5                                 \n"
    "   uKERNEL_M8N6_S2_10_K6                                 \n"
    "   uKERNEL_M8N6_S2_10_K7                                 \n"

    "   beq     M8N6_S2_10_END_K                        \n"

    "   uKERNEL_M8N6_S2_10_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N6_S2_10_MAIN_K                       \n" 




    "M8N6_S2_10_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N6_S2_10                     \n"

    //ic != 0
    "   uKERNEL_M8N6_S2_10_END                    \n"

    "   b    M8N6_S2_10_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N6_S2_10:                               \n"
    "   M8N6_S2_10_ADD_C                                   \n"
    
    
    "M8N6_S2_10_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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


void direct_3x3_M8N4_10_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N4_S2_10_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"
            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"

            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"


            
            "   add     x11, x10, x6          \n"

            "   add     x12, x11, x6           \n"
            "   add     x13, x12, x6           \n"
            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"
            "   add     x15, x14, x6          \n"
            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"

            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"


                "   ldr     q27, [x3], #16                   \n"

            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   ldr     q7, [x3], #16                   \n"




    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N4_S2_10_K4                              \n"
                "   ld1     {v4.2s}, [x1], #8              \n"
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                "   ldr     q1, [x3], #16                   \n"


                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N4_S2_10_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                
                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"


                "   ldr     q7, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x1], #16                        \n"
                
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"



                "   ldr     q3, [x1], #16                   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"

                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                "   ld1     {v4.2s}, [x1], #8              \n"
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"

                "   ldr     q1, [x3], #16                   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q27, [x3], #16                   \n"


                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"


                "   ldr     q7, [x3], #16                   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_K8                              \n"
                "   add     x3, x3, #96         \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                
                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x1], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                "   ldr     q3, [x1], #16                   \n"


                "   prfm PLDL1KEEP, [x3, #256]              \n"
                
    ".endm                                                  \n"
    

    
    ".macro M8N4_S2_10_ADD_C                                   \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"





    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_10_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"


                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                
    
    ".endm                                                  \n"
    
    "M8N4_S2_10_ENTRANCE:                                                       \n"


    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    "   add  x3, x3, #96    \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #128                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N4_S2_10_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N4_S2_10_LOOP                               \n"
    
    "M8N4_S2_10_MAIN_K:                                  \n"
        "   uKERNEL_M8N4_S2_10_K3                     \n"

    "M8N4_S2_10_LOOP:                                  \n"
    "   uKERNEL_M8N4_S2_10_K4                                 \n"
    "   uKERNEL_M8N4_S2_10_K5                                 \n"
    "   uKERNEL_M8N4_S2_10_K6                                 \n"
    "   uKERNEL_M8N4_S2_10_K7                                 \n"

    "   beq     M8N4_S2_10_END_K                        \n"

    "   uKERNEL_M8N4_S2_10_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N4_S2_10_MAIN_K                       \n" 




    "M8N4_S2_10_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N4_S2_10                     \n"

    //ic != 0
    "   uKERNEL_M8N4_S2_10_END                    \n"

    "   b    M8N4_S2_10_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N4_S2_10:                               \n"
    "   M8N4_S2_10_ADD_C                                   \n"
    
    
    "M8N4_S2_10_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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



void direct_3x3_M8N8_11_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N8_S2_11_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"

            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x1], #16                      \n"

            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            
            "   ldr     q5, [x1], #16               \n"
            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"


            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"

            
            "   fmul    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmul    v19.4s, v0.4s, v5.s[2]          \n"


            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"
            "   fmul    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmul    v23.4s, v1.4s, v5.s[2]          \n"
            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N8_S2_11_K0                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"


                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[2]          \n"


                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[2]          \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_11_K1                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                
                "   ld1     {v6.2s}, [x1], #8              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"

                
                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[3]          \n"
                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"
                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_11_K2                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   ldr     q3, [x1], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"

                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N8_S2_11_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"


                "   ldr     q27, [x3], #16                   \n"


            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"
                "   ldr     q5, [x1], #16                   \n"
                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   ld1     {v6.2s}, [x1], #8              \n"
            
            "   fmla    v18.4s, v0.4s, v5.s[0]          \n"
            "   fmla    v19.4s, v0.4s, v5.s[2]          \n"


            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"
            "   fmla    v22.4s, v1.4s, v5.s[0]          \n"
            "   fmla    v23.4s, v1.4s, v5.s[2]          \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N8_S2_11_K4                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"

                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[3]          \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[3]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N8_S2_11_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"


                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v0.4s, v6.s[0]          \n"
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"
                
                "   ldr     q3, [x1], #16                   \n"
                
                "   fmla    v22.4s, v1.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v1.4s, v6.s[0]          \n"

                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"
                
                "   ldr     q5, [x1], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                
                "   ld1     {v6.2s}, [x1], #8              \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v19.4s, v27.4s, v5.s[2]          \n"

                
                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v22.4s, v7.4s, v5.s[0]          \n"
                "   fmla    v23.4s, v7.4s, v5.s[2]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"

                //update kernel 
                "   ldr     q27, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"
                "   fmla    v18.4s, v0.4s, v5.s[1]          \n"
                "   fmla    v19.4s, v0.4s, v5.s[3]          \n"
     

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"
                "   fmla    v22.4s, v1.4s, v5.s[1]          \n"
                "   fmla    v23.4s, v1.4s, v5.s[3]          \n"

                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_K8                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x1], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   ldr     q3, [x1], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                
                "   ldr     q4, [x1], #16                   \n"
                
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"

                "   ldr     q5, [x1], #16                   \n"
    ".endm                                                  \n"
    

    
    ".macro M8N8_S2_11_ADD_C                                   \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[1], [x10]    \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[2], [x11]    \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s,v19.s}[3], [x12]    \n"

                "   st4     {v20.s, v21.s, v22.s,v23.s}[0], [x13]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[1], [x14]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[2], [x15]   \n"
                "   st4     {v20.s, v21.s, v22.s,v23.s}[3], [x16]   \n"

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N8_S2_11_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   fmla    v18.4s, v27.4s, v5.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   fmla    v19.4s, v27.4s, v6.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[0], [x28], #16  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[1], [x28], #16  \n"
                "   fmla    v22.4s, v7.4s, v5.s[2]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[2], [x28], #16  \n"
                "   fmla    v23.4s, v7.4s, v6.s[0]          \n"
                "   st4     {v16.s, v17.s, v18.s, v19.s}[3], [x28], #16  \n"
                
                "   st4     {v20.s, v21.s, v22.s, v23.s}[0], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[1], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[2], [x28], #16  \n"
                "   st4     {v20.s, v21.s, v22.s, v23.s}[3], [x28], #16  \n"
                

                // 8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4]                        \n"
                "   str     q9, [x10]                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11]                      \n"
                "   str     q11, [x12]                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13]                       \n"
                "   str     q9, [x14]                      \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15]                     \n"
                "   str     q11, [x16]                     \n"

    
    ".endm                                                  \n"
    
    "M8N8_S2_11_ENTRANCE:                                                       \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #256                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N8_S2_11_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N8_S2_11_LOOP                               \n"
    
    "M8N8_S2_11_MAIN_K:                                  \n"
        "   uKERNEL_M8N8_S2_11_K0                     \n"

    "M8N8_S2_11_LOOP:                                  \n"
    "   uKERNEL_M8N8_S2_11_K1                     \n"
    "   uKERNEL_M8N8_S2_11_K2                     \n"
    "   uKERNEL_M8N8_S2_11_K3                     \n"
    "   uKERNEL_M8N8_S2_11_K4                                 \n"
    "   uKERNEL_M8N8_S2_11_K5                                 \n"
    "   uKERNEL_M8N8_S2_11_K6                                 \n"
    "   uKERNEL_M8N8_S2_11_K7                                 \n"

    "   beq     M8N8_S2_11_END_K                        \n"

    "   uKERNEL_M8N8_S2_11_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N8_S2_11_MAIN_K                       \n" 




    "M8N8_S2_11_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N8_S2_11                     \n"

    //ic != 0
    "   uKERNEL_M8N8_S2_11_END                    \n"

    "   b    M8N8_S2_11_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N8_S2_11:                               \n"
    "   M8N8_S2_11_ADD_C                                   \n"
    
    
    "M8N8_S2_11_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3_M8N6_11_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N6_S2_11_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"

            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"
            
            "   ldr     q4, [x1], #16                      \n"

            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            
            
            
            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"


            "   fmul    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmul    v17.4s, v0.4s, v4.s[2]           \n"



            
            "   add     x11, x10, x6          \n"
            "   fmul    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmul    v21.4s, v1.4s, v4.s[2]           \n"

            
            "   add     x12, x11, x6           \n"

            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N6_S2_11_K0                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v16.4s, v0.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[2]          \n"


                "   fmla    v20.4s, v1.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[2]          \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N6_S2_11_K1                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                
                "   ld1     {v5.2s}, [x1], #8              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"
                
                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"

                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"

                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N6_S2_11_K2                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"

                "   ldr     q3, [x1], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"


                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N6_S2_11_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"


                "   ldr     q27, [x3], #16                   \n"


            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"
            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   ldr     q7, [x3], #16                   \n"

            "   fmla    v16.4s, v0.4s, v4.s[0]           \n"
            "   fmla    v17.4s, v0.4s, v4.s[2]           \n"
            
            "   ld1     {v5.2s}, [x1], #8              \n"


            "   fmla    v20.4s, v1.4s, v4.s[0]           \n"
            "   fmla    v21.4s, v1.4s, v4.s[2]           \n"



    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N6_S2_11_K4                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"

                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"

                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"


                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N6_S2_11_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"


                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"


                "   ldr     q2, [x1], #16                        \n"

                "   fmla    v16.4s, v0.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v0.4s, v5.s[0]          \n"

                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v1.4s, v5.s[0]          \n"
                
                "   ldr     q3, [x1], #16                   \n"


                "   ldr     q4, [x1], #16                   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"

                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                
                "   ld1     {v5.2s}, [x1], #8              \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[0]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[2]          \n"

                
                "   fmla    v20.4s, v7.4s, v4.s[0]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[2]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"

                //update kernel 
                "   ldr     q27, [x3], #16                   \n"
                
                //edge :row 1 col 2

                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                "   fmla    v16.4s, v0.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v0.4s, v4.s[3]          \n"


                //update kernel 
                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v20.4s, v1.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v1.4s, v4.s[3]          \n"

                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_K8                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                //update kernel
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"

                //prepare: row 2 col 0
                "   ldr     q2, [x1], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"

                "   ldr     q3, [x1], #16                   \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                
                "   ldr     q4, [x1], #16                   \n"

    ".endm                                                  \n"
    

    
    ".macro M8N6_S2_11_ADD_C                                   \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"
                


                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"

                
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"


                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x4]    \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x10]    \n"
                "   st2     {v16.s, v17.s}[2], [x11]    \n"
                "   st2     {v16.s, v17.s}[3], [x12]    \n"

                "   st2     {v20.s, v21.s}[0], [x13]   \n"
                "   st2     {v20.s, v21.s}[1], [x14]   \n"
                "   st2     {v20.s, v21.s}[2], [x15]   \n"
                "   st2     {v20.s, v21.s}[3], [x16]   \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N6_S2_11_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"

                //
                "   fmla    v16.4s, v27.4s, v4.s[2]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   fmla    v17.4s, v27.4s, v5.s[0]          \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

                "   fmla    v20.4s, v7.4s, v4.s[2]          \n"
                "   st2     {v16.s, v17.s}[0], [x28], #8  \n"
                "   fmla    v21.4s, v7.4s, v5.s[0]          \n"
                "   st2     {v16.s, v17.s}[1], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[2], [x28], #8  \n"

                "   st2     {v16.s, v17.s}[3], [x28], #8 \n"
                
                "   st2     {v20.s, v21.s}[0], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[1], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[2], [x28], #8  \n"
                "   st2     {v20.s, v21.s}[3], [x28], #8  \n"
                

                // 8
                
                "   ld1     {v0.2s}, [x4]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x10]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                
                "   ld1     {v2.2s}, [x11]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x12]                       \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                "   st1     {v8.2s}, [x4]                        \n"
                "   st1     {v9.2s}, [x10]                       \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x11]                      \n"
                "   st1     {v11.2s}, [x12]                      \n"
                
                //
                "   ld1     {v0.2s}, [x13]                        \n"
                "   ld1     {v8.2s}, [x29], #8                  \n"
                
                "   ld1     {v1.2s}, [x14]                       \n"
                "   ld1     {v9.2s}, [x29], #8                  \n"
                "   fadd    v8.2s, v8.2s, v0.2s             \n"
                
                "   ld1     {v2.2s}, [x15]                       \n"
                "   ld1     {v10.2s}, [x29], #8                 \n"
                "   fadd    v9.2s, v9.2s, v1.2s             \n"
                
                "   ld1     {v3.2s}, [x16]                 \n"
                "   ld1     {v11.2s}, [x29], #8                 \n"
                "   fadd    v10.2s, v10.2s, v2.2s             \n"
                
                "   st1     {v8.2s}, [x13]                       \n"
                "   st1     {v9.2s}, [x14]                      \n"
                
                "   fadd    v11.2s, v11.2s, v3.2s             \n"
                "   st1     {v10.2s}, [x15]                     \n"
                "   st1     {v11.2s}, [x16]                     \n"

    
    ".endm                                                  \n"
    
    "M8N6_S2_11_ENTRANCE:                                                       \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #192                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N6_S2_11_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N6_S2_11_LOOP                               \n"
    
    "M8N6_S2_11_MAIN_K:                                  \n"
        "   uKERNEL_M8N6_S2_11_K0                     \n"

    "M8N6_S2_11_LOOP:                                  \n"
    "   uKERNEL_M8N6_S2_11_K1                     \n"
    "   uKERNEL_M8N6_S2_11_K2                     \n"
    "   uKERNEL_M8N6_S2_11_K3                     \n"
    "   uKERNEL_M8N6_S2_11_K4                                 \n"
    "   uKERNEL_M8N6_S2_11_K5                                 \n"
    "   uKERNEL_M8N6_S2_11_K6                                 \n"
    "   uKERNEL_M8N6_S2_11_K7                                 \n"

    "   beq     M8N6_S2_11_END_K                        \n"

    "   uKERNEL_M8N6_S2_11_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N6_S2_11_MAIN_K                       \n" 




    "M8N6_S2_11_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N6_S2_11                     \n"

    //ic != 0
    "   uKERNEL_M8N6_S2_11_END                    \n"

    "   b    M8N6_S2_11_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N6_S2_11:                               \n"
    "   M8N6_S2_11_ADD_C                                   \n"
    
    
    "M8N6_S2_11_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3_M8N4_11_p1_s2(float *img_pack, float *kernal, float *out, long ic_count, long ic, long stride_out_size, long stride_in_size){
    
    asm volatile(
    ".macro uKERNEL_M8N4_S2_11_BEGIN                             \n"

            "   ldr     q0, [x3], #16                      \n"
            "   ldr     q2, [x1], #16                      \n"
            "   ldr     q1, [x3], #16                      \n"
            "   ldr     q3, [x1], #16                      \n"
            "   add     x10, x4, x6                     \n"

            
            "   fmul    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmul    v9.4s, v0.4s, v2.s[2]           \n"

            
            "   fmul    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmul    v11.4s, v0.4s, v3.s[2]          \n"

            "   fmul    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmul    v13.4s, v1.4s, v2.s[2]           \n"
            

            "   fmul    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmul    v15.4s, v1.4s, v3.s[2]          \n"

            
            "   add     x11, x10, x6          \n"


            
            "   add     x12, x11, x6           \n"

            "   add     x13, x12, x6           \n"

            "   ldr     q27, [x3], #16                   \n"
            "   add     x14, x13, x6           \n"


            "   add     x15, x14, x6          \n"

            "   ldr     q7, [x3], #16                   \n"
            "   add     x16, x15, x6          \n"

            "   prfm    PLDL2KEEP, [x4, #64]           \n"
            "   prfm    PLDL2KEEP, [x10, #64]          \n"
            "   prfm    PLDL2KEEP, [x11, #64]          \n"
            "   prfm    PLDL2KEEP, [x12, #64]          \n"


            "   prfm    PLDL2KEEP, [x13, #64]          \n"
            "   prfm    PLDL2KEEP, [x14, #64]          \n"
            "   prfm    PLDL2KEEP, [x15, #64]          \n"
            "   prfm    PLDL2KEEP, [x16, #64]          \n"

    ".endm                                                  \n"
    ".macro uKERNEL_M8N4_S2_11_K0                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

                "   ldr     q27, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

                "   ldr     q7, [x3], #16                   \n"


    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N4_S2_11_K1                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                
                "   ld1     {v4.2s}, [x1], #8              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"



                "   ldr     q27, [x3], #16                   \n"



                "   prfm    PLDL1KEEP, [x3, #256]           \n"
                "   ldr     q7, [x3], #16                   \n"

    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N4_S2_11_K2                              \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x1], #16                        \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"


                "   ldr     q3, [x1], #16                   \n"
                "   ldr     q1, [x3], #16                   \n"

    ".endm                                                  \n"
    
    //3
    ".macro uKERNEL_M8N4_S2_11_K3                              \n"

            "   fmla    v8.4s, v0.4s, v2.s[0]           \n"
            "   fmla    v9.4s, v0.4s, v2.s[2]           \n"

            "   fmla    v10.4s, v0.4s, v3.s[0]          \n"
            "   fmla    v11.4s, v0.4s, v3.s[2]          \n"

            "   ldr     q27, [x3], #16                   \n"


            "   fmla    v12.4s, v1.4s, v2.s[0]           \n"
            "   fmla    v13.4s, v1.4s, v2.s[2]           \n"

            "   fmla    v14.4s, v1.4s, v3.s[0]          \n"
            "   fmla    v15.4s, v1.4s, v3.s[2]          \n"

            "   ldr     q7, [x3], #16                   \n"




    ".endm                                                  \n"
    
    
    
    ".macro uKERNEL_M8N4_S2_11_K4                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[3]           \n"
                "   ld1     {v4.2s}, [x1], #8              \n" 
                "   fmla    v10.4s, v27.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[3]          \n"
                "   ldr     q0, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v7.4s, v2.s[3]          \n"

                
                "   fmla    v14.4s, v7.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[3]          \n"


                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v16.4s, v27.4s, v4.s[1]          \n"
                "   fmla    v17.4s, v27.4s, v4.s[3]          \n"


                "   fmla    v20.4s, v7.4s, v4.s[1]          \n"
                "   fmla    v21.4s, v7.4s, v4.s[3]          \n"


                "   prfm    PLDL1KEEP, [x3, #256]           \n"


    ".endm                                                  \n"
    
    
    ".macro uKERNEL_M8N4_S2_11_K5                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v0.4s, v3.s[0]           \n"
                
                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v10.4s, v0.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v0.4s, v4.s[0]          \n"

                "   ldr     q7, [x3], #16                   \n"
                
                "   fmla    v12.4s, v1.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v1.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x1], #16                        \n"
                
                "   fmla    v14.4s, v1.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v1.4s, v4.s[0]          \n"

                
                "   ldr     q3, [x1], #16                   \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_K6                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[0]           \n"
                "   fmla    v9.4s, v27.4s, v2.s[2]           \n"

                "   ld1     {v4.2s}, [x1], #8              \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[0]          \n"
                "   fmla    v11.4s, v27.4s, v3.s[2]          \n"


                "   ldr     q0, [x3], #16                   \n"
                
                

                "   fmla    v12.4s, v7.4s, v2.s[0]           \n"
                "   fmla    v13.4s, v7.4s, v2.s[2]          \n"
                
                "   ldr     q1, [x3], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[0]          \n"
                "   fmla    v15.4s, v7.4s, v3.s[2]          \n"


    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_K7                              \n"

                "   fmla    v8.4s, v0.4s, v2.s[1]           \n"
                "   fmla    v9.4s, v0.4s, v2.s[3]           \n"

                "   ldr     q27, [x3], #16                   \n"
                
                "   fmla    v10.4s, v0.4s, v3.s[1]          \n"
                "   fmla    v11.4s, v0.4s, v3.s[3]          \n"


                "   ldr     q7, [x3], #16                   \n"

                "   fmla    v12.4s, v1.4s, v2.s[1]          \n"
                "   fmla    v13.4s, v1.4s, v2.s[3]          \n"
                "   fmla    v14.4s, v1.4s, v3.s[1]          \n"
                "   fmla    v15.4s, v1.4s, v3.s[3]          \n"

                

    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_K8                              \n"

                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                
                "   ldr     q0, [x3], #16                   \n"
                
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"

                "   ldr     q1, [x3], #16                   \n"

                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   ldr     q2, [x1], #16                   \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"

                "   ldr     q3, [x1], #16                   \n"

    ".endm                                                  \n"
    

    
    ".macro M8N4_S2_11_ADD_C                                   \n"
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x4], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x10], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x11], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x12], #16    \n"

                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x13], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x14], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x15], #16    \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x16], #16    \n"
    ".endm                                                  \n"
    
    ".macro uKERNEL_M8N4_S2_11_END                             \n"
                
                "   fmla    v8.4s, v27.4s, v2.s[2]           \n"
                "   fmla    v9.4s, v27.4s, v3.s[0]           \n"
                "   fmla    v10.4s, v27.4s, v3.s[2]          \n"
                "   fmla    v11.4s, v27.4s, v4.s[0]          \n"


                "   fmla    v12.4s, v7.4s, v2.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[0], [x28], #16    \n"
                
                "   fmla    v13.4s, v7.4s, v3.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[1], [x28], #16    \n"
                
                "   fmla    v14.4s, v7.4s, v3.s[2]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[2], [x28], #16    \n"
                
                "   fmla    v15.4s, v7.4s, v4.s[0]          \n"
                
                "   st4     {v8.s, v9.s, v10.s, v11.s}[3], [x28], #16    \n"


                "   st4     {v12.s, v13.s, v14.s, v15.s}[0], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[1], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[2], [x28], #16  \n"
                "   st4     {v12.s, v13.s, v14.s, v15.s}[3], [x28], #16  \n"


                // 4*8
                
                "   ldr     q0, [x4]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x10]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                
                "   ldr     q2, [x11]                       \n"
                "   ldr     q10, [x29], #16                 \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q3, [x12]                       \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                "   str     q8, [x4], #16                        \n"
                "   str     q9, [x10], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x11], #16                      \n"
                "   str     q11, [x12], #16                      \n"
    
                "   ldr     q0, [x13]                        \n"
                "   ldr     q8, [x29], #16                  \n"
                
                "   ldr     q1, [x14]                       \n"
                "   ldr     q9, [x29], #16                  \n"
                "   fadd    v8.4s, v8.4s, v0.4s             \n"
                
                "   ldr     q2, [x15]                       \n"
                "   ldr     q3, [x16]                       \n"
                "   fadd    v9.4s, v9.4s, v1.4s             \n"
                
                "   ldr     q10, [x29], #16                 \n"
                "   ldr     q11, [x29], #16                 \n"
                "   fadd    v10.4s, v10.4s, v2.4s             \n"
                
                "   str     q8, [x13], #16                        \n"
                "   str     q9, [x14], #16                       \n"
                
                "   fadd    v11.4s, v11.4s, v3.4s             \n"
                "   str     q10, [x15], #16                      \n"
                "   str     q11, [x16], #16                      \n"

    
    ".endm                                                  \n"
    
    "M8N4_S2_11_ENTRANCE:                                                       \n"

    "   ldr x1, %[img_pack]                     \n"
    "   ldr x3, %[kernal]                       \n"
    "   ldr x4, %[out]                          \n"
    "   ldr x7, %[ic_count]                     \n"
    "   ldr x6, %[stride_out_size]             \n"
    "   ldr x5, %[stride_in_size]             \n"
    "   mov x29, sp                            \n"
    
    "   prfm PLDL1KEEP, [x1, #128]              \n"
    "   prfm PLDL1KEEP, [x3, #256]              \n"
    
    "   ldr x8, %[ic]                          \n"
    
    "   sub x29, x29, #128                        \n"
    
    "   subs x7, x7, #1                         \n"
    
    "   uKERNEL_M8N4_S2_11_BEGIN                     \n"
    "   mov x28, x29    \n"
    
    
    "   b M8N4_S2_11_LOOP                               \n"
    
    "M8N4_S2_11_MAIN_K:                                  \n"
        "   uKERNEL_M8N4_S2_11_K0                     \n"

    "M8N4_S2_11_LOOP:                                  \n"
    "   uKERNEL_M8N4_S2_11_K1                     \n"
    "   uKERNEL_M8N4_S2_11_K2                     \n"
    "   uKERNEL_M8N4_S2_11_K3                     \n"
    "   uKERNEL_M8N4_S2_11_K4                                 \n"
    "   uKERNEL_M8N4_S2_11_K5                                 \n"
    "   uKERNEL_M8N4_S2_11_K6                                 \n"
    "   uKERNEL_M8N4_S2_11_K7                                 \n"

    "   beq     M8N4_S2_11_END_K                        \n"

    "   uKERNEL_M8N4_S2_11_K8                                 \n"

    "   subs    x7, x7, #1                 \n"
    "   b       M8N4_S2_11_MAIN_K                       \n" 




    "M8N4_S2_11_END_K:                                     \n"
    //ic = 0, store
    "   cmp     x8, #0                          \n"
    "   beq     END_uKERNEL_M8N4_S2_11                     \n"

    //ic != 0
    "   uKERNEL_M8N4_S2_11_END                    \n"

    "   b    M8N4_S2_11_END                             \n"
    
    //ic = 0, add
    "END_uKERNEL_M8N4_S2_11:                               \n"
    "   M8N4_S2_11_ADD_C                                   \n"
    
    
    "M8N4_S2_11_END:                                       \n"
    :

    :

        [img_pack]                    "m"     (img_pack),
        [kernal]                    "m"     (kernal),
        [out]                       "m"     (out),
        [ic_count]                  "m"     (ic_count),
        [ic]                        "m"     (ic),
        [stride_out_size]           "m"     (stride_out_size),
        [stride_in_size]            "m"     (stride_in_size)
        
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

void direct_3x3s2(float* out_kernal, float* img, float* out, long inch, long outch, long in_h, long out_h, long k_h, long p_h, long s_h, long kc_tile, long ic_tile, long ic_out_tile, long kc_out_tile){
    
    long in_w = in_h;

    long i_s_h = 18; //8*2+2=18
    
    long i_s_n = 3 * i_s_h;
    
    long i_s_n_ic_tile = i_s_n * ic_tile;
    
    long out_w = out_h;
    
    long out_w_c = out_h / 8 * 8;
    
    long out_w_r = out_h % 8;
    
    long i_t_size = in_w * in_h;
    
    long stride_in = in_h * in_h;
    
    long stride_in_size = stride_in * sizeof(float);
    
    
    long stride_out = out_h * out_w;
    
    long stride_out_size = stride_out * sizeof(float);
    
    
    long stride_k = k_h * k_h;
    
    float *img_pack = (float*)malloc(i_s_n_ic_tile * sizeof(float));
    
    float *kernal_k, *kernal_s, *kernal_c, *kernal_start, *kernal_1;
    
    float *out_img_s, *out_img_c, *out_img_p, *out_img_p_1;
    
    float *img_start, *img_start_h, *img_start_s_0;
    
    float *img_start_s_1, *img_start_s_2, *img_start_s_3;
    
    float *r0;
    
    long i, j, k, ic, oc, oh, ow;
    
    long kc;
    
    long kc_out, ic_out;
    
    long kc_size = kc_tile;
    
    long kc_count = outch / kc_size;
    
    
    for(kc_out = 0; kc_out < outch; kc_out += kc_out_tile){

        kernal_start = out_kernal + 9 * inch * kc_out;
        
        for(ic_out = 0; ic_out<inch; ic_out += ic_out_tile){
            

            kernal_1 = kernal_start + 9 * kc_out_tile * ic_out;
            
            for(kc = kc_out; kc < kc_out + kc_out_tile; kc += kc_tile){

                kernal_k = kernal_1 + 9 * (kc - kc_out) * ic_out_tile;
                
                for(ic=ic_out; ic < ic_out + ic_out_tile; ic += ic_tile){

                    kernal_s = kernal_k + 9 * (ic - ic_out) * kc_tile;
                    
                    img_start = img + ic * i_t_size;
                    
                    if(out_w_c ==0){
                        
                        img_start_h = img_start;
                        
                        out_img_s = out;

                        img_start_s_0 = img_start_h;
                        
                        img_start_s_1 = img_start_s_0 + in_w;
                        
                        out_img_c = out_img_s;
                        
                        for(oc = kc; oc < kc + kc_tile; oc+=8){
                            
                            out_img_p = out_img_c + oc * stride_out;
                            
                            kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;

                            if(oc==kc){
                                

                                direct_3x3_M8N7_00_p1_s2_pack(img_start_s_0, img_start_s_1, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                

                            }
                            else{

                                direct_3x3_M8N7_00_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);

                                }
                                    
                        }
                        
                        for(oh=1; oh < out_h; oh++){
                            
                            img_start_h = img_start + oh * 2 * in_w;
                            
                            out_img_s = out + oh * out_w;
                                
                            img_start_s_0 = img_start_h - in_w;
                            
                            img_start_s_1 = img_start_h;
                            
                            img_start_s_2 = img_start_h + in_w;
                            
                            out_img_c = out_img_s;
                            
                            for(oc = kc; oc < kc + kc_tile; oc+=8){
                                
                                out_img_p = out_img_c + oc * stride_out;
                                
                                kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;
                                
                                if(oc==kc){
                                    

                                    direct_3x3_M8N7_01_p1_s2_pack(img_start_s_0, img_start_s_1, img_start_s_2, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                    

                                }
                                else{

                                    direct_3x3_M8N7_01_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);


                                    }
                                
                            }


                        }
                    }
                    else{
                        
                        for(oh=0; oh < out_h; oh++){
                            

                            if(oh == 0){
                            
                                img_start_h = img_start;
                                
                                out_img_s = out;
                                
                                for(ow=0; ow<out_w_c; ow+=8){
                                    
                                    out_img_c = out_img_s + ow;
                                    
                                    if(ow == 0){
                                        
                                        img_start_s_0 = img_start_h;
                                        
                                        img_start_s_1 = img_start_s_0 + in_w;
                                        
                                        
                                        for(oc = kc; oc < kc + kc_tile; oc+=8){
                                            
                                            out_img_p = out_img_c + oc * stride_out;
                                            
                                            kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;
                                            if(oc==kc){
                                                

                                                direct_3x3_M8N8_00_p1_s2_pack(img_start_s_0, img_start_s_1, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                            }
                                            else{


                                                direct_3x3_M8N8_00_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);

                                            }
                                            
                                        }
                                    }
                                    else{
                                        
                                        img_start_s_0 = img_start_h + 2*ow -1;
                                        
                                        img_start_s_1 = img_start_s_0 + in_w;
                                        
                                        
                                        for(oc = kc; oc < kc + kc_tile; oc+=8){
                                            
                                            out_img_p = out_img_c + oc * stride_out;
                                            
                                            kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;
                                            

                                            if(oc==kc){
                                                

                                                direct_3x3_M8N8_10_p1_s2_pack(img_start_s_0, img_start_s_1, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                            }
                                            else{


                                                direct_3x3_M8N8_10_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);

                                            }
                                            
                                        }
                                        
                                        
                                    }

                                }
                            
                                if(out_w_r == 6){
                                    
                                    out_img_c = out_img_s + ow;
                                    
                                    img_start_s_0 = img_start_h + 2*ow -1;
                                    
                                    img_start_s_1 = img_start_s_0 + in_w;
                                    
                                    
                                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                                        
                                        out_img_p = out_img_c + oc * stride_out;
                                        
                                        kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;
                                        

                                        if(oc==kc){

                                            direct_3x3_M8N6_10_p1_s2_pack(img_start_s_0, img_start_s_1, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        else{


                                            direct_3x3_M8N6_10_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        
                                    }

                                }
                            
                                else if(out_w_r == 4){
                                    out_img_c = out_img_s + ow;
                                    
                                    img_start_s_0 = img_start_h + 2*ow -1;
                                    
                                    img_start_s_1 = img_start_s_0 + in_w;
                                    
                                    
                                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                                        
                                        out_img_p = out_img_c + oc * stride_out;
                                        
                                        kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;
                                        

                                        if(oc==kc){

                                            direct_3x3_M8N4_10_p1_s2_pack(img_start_s_0, img_start_s_1, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        else{

                                            direct_3x3_M8N4_10_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        
                                    }
                                }
                            }
                            
                            else{
                                img_start_h = img_start + oh * 2 * in_w;
                                
                                out_img_s = out + oh * out_w;
                                
                                for(ow=0; ow<out_w_c; ow+=8){
                                    
                                    out_img_c = out_img_s + ow;
                                    
                                    if(ow == 0){
                                        
                                        img_start_s_0 = img_start_h - in_w;
                                        
                                        img_start_s_1 = img_start_h;
                                        
                                        img_start_s_2 = img_start_h + in_w;
                                        
                                        
                                        for(oc = kc; oc < kc + kc_tile; oc+=8){
                                            
                                            out_img_p = out_img_c + oc * stride_out;
                                            
                                            kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;

                                            if(oc==kc){
                                                

                                                direct_3x3_M8N8_01_p1_s2_pack(img_start_s_0, img_start_s_1, img_start_s_2, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                            }
                                            else{


                                                direct_3x3_M8N8_01_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);

                                            }
                                            
                                        }
                                    }
                                    else{
                                        
                                        img_start_s_1 = img_start_h + 2*ow- 1;
                                        
                                        img_start_s_0 = img_start_s_1 - in_w;
                                        
                                        
                                        img_start_s_2 = img_start_s_1 + in_w;
                                        
                                        
                                        for(oc = kc; oc < kc + kc_tile; oc+=8){
                                            
                                            out_img_p = out_img_c + oc * stride_out;
                                            
                                            kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;

                                            if(oc==kc){
                                                

                                                direct_3x3_M8N8_11_p1_s2_pack(img_start_s_0, img_start_s_1, img_start_s_2, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                            }
                                            else{


                                                direct_3x3_M8N8_11_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);

                                            }
                                            
                                        }
                                        
                                        
                                    }

                                }
                            
                                if(out_w_r == 6){

                                    out_img_c = out_img_s + ow;
                                    
                                    img_start_s_1 = img_start_h + 2*ow- 1;
                                    
                                    img_start_s_0 = img_start_s_1 - in_w;
                                    
                                    
                                    img_start_s_2 = img_start_s_1 + in_w;
                                    
                                    
                                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                                        
                                        out_img_p = out_img_c + oc * stride_out;
                                        
                                        kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;

                                        if(oc==kc){

                                            direct_3x3_M8N6_11_p1_s2_pack(img_start_s_0, img_start_s_1, img_start_s_2, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        else{

                                            direct_3x3_M8N6_11_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        
                                    }
                                }
                            
                                else if(out_w_r == 4){
                                    out_img_c = out_img_s + ow;
                                    
                                    img_start_s_1 = img_start_h + 2*ow- 1;
                                    
                                    img_start_s_0 = img_start_s_1 - in_w;
                                    
                                    
                                    img_start_s_2 = img_start_s_1 + in_w;
                                    
                                    
                                    for(oc = kc; oc < kc + kc_tile; oc+=8){
                                        
                                        out_img_p = out_img_c + oc * stride_out;
                                        
                                        kernal_c = kernal_s + 9 * (oc - kc) * ic_tile;
                                        

                                        if(oc==kc){
                                            

                                            direct_3x3_M8N4_11_p1_s2_pack(img_start_s_0, img_start_s_1, img_start_s_2, img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        else{

                                            direct_3x3_M8N4_11_p1_s2(img_pack, kernal_c, out_img_p, ic_tile, ic, stride_out_size, stride_in_size);
                                        }
                                        
                                    }
                                }
                            
                            
                            
                            }
                            
                        }
                    
                        
                        
                    }

                }
        
            }
        

        }
    
    }
    free(img_pack);
}



