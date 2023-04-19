/*
Authors: Weiling Yang
Date: 2023.1.27
*/

void SMM_S2_KERNEL8x1(float *C, float *A, float *B, long	M, long N, long K,
         long LN, long LK, long out_LN,float *SB, long k_tag)
{

	asm volatile(

			".macro	PACK_KERNEL8x1_BEGIN_K							\n"


	    "	ldp		q0,	q1, [x11], #32									\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldr		s2, [x12]														\n"
	    "	fmul 	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	fmul 	v17.4s, v1.4s, v2.s[0] 							\n"

	    "	ldr		q3, [x11], #16 											\n"
	    "	st1 	{v2.s}[0], [x24], #4 								\n"
	    "	add 	x12, x12, x9, lsl #3 								\n"
	    "	ldr		s5, [x13]														\n"
	    "	ldr		q4, [x11], #16 											\n"
			"	add 	x13, x13, x9, lsl #3 								\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x1_K0										\n"

	    "	ldr		s5, [x13] 													\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	ldp		q3, q4, [x11], #32 									\n"
	    "	fmla	v17.4s, v1.4s, v2.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	add 	x13, x13, x9, lsl #3 								\n"
	    "	st1 	{v2.s}[0], [x24], #4 								\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x1_K1										\n"

	    "	ldr		s2, [x12] 													\n"
	    "	fmla	v16.4s, v3.4s, v5.s[0]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v17.4s, v4.4s, v5.s[0]							\n"

	    "	ldr		q1, [x11], #16 											\n"
	    "	add 	x12, x12, x9, lsl #3 								\n"
	    "	st1 	{v5.s}[0], [x24], #4 								\n"

	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x1_END_K								\n"


	    "	fmla	v16.4s, v3.4s, v5.s[0]							\n"
	    "	fmla	v17.4s, v4.4s, v5.s[0]							\n"
	    "	st1 	{v5.s}[0], [x24], #4 								\n"

	    ".endm 																			\n"


	    ".macro	Conv_s2_M8N1_ADD_C 									\n"

	    "	prfm	PLDL1KEEP, [x25, #8]								\n"
	    "	prfm	PLDL1KEEP, [x26, #8]								\n"
	    "	prfm	PLDL1KEEP, [x27, #8]								\n"
	    "	prfm	PLDL1KEEP, [x28, #8]								\n"

	    "	ld1		{v0.s}[0], [x25]										\n"
	    "	ld1		{v0.s}[1], [x26]										\n"
	    "	ld1		{v0.s}[2], [x27]										\n"
	    "	ld1		{v0.s}[3], [x28]										\n"
	    "	fadd 	v16.4s, v16.4s, v0.4s 							\n"
	    

	   	"	prfm	PLDL1KEEP, [x15, #8]								\n"
	    "	prfm	PLDL1KEEP, [x16, #8]								\n"
	    "	prfm	PLDL1KEEP, [x17, #8]								\n"
	    "	prfm	PLDL1KEEP, [x18, #8]								\n"
	    "	ld1		{v1.s}[0], [x15]										\n"
	    "	ld1		{v1.s}[1], [x16]										\n"
	    "	ld1		{v1.s}[2], [x17]										\n"
	    "	ld1		{v1.s}[3], [x18]										\n"
			"	fadd 	v17.4s, v17.4s, v1.4s 							\n"

	    ".endm 																			\n"



			".macro	Conv_s2_SAVE8x1												\n"

			"	subs	x21, x21, #1 													\n"

			"	st1		{v16.s}[0], [x25]											\n"
			"	add		x25, x25, x7, lsl #5 			 						\n"   
			"	st1		{v16.s}[1], [x26] 										\n"
			"	add 	x26, x26, x7, lsl #5 									\n"

			"	st1		{v16.s}[2], [x27] 										\n"
			"	st1		{v16.s}[3], [x28] 										\n"
			"	add 	x27, x27, x7, lsl #5 									\n"
			"	add 	x28, x28, x7, lsl #5 									\n"

			"	st1		{v17.s}[0], [x15] 										\n"
			"	st1		{v17.s}[1], [x16] 										\n"
			"	add 	x15, x15, x7, lsl #5 									\n"
			"	add 	x16, x16, x7, lsl #5 									\n"

			"	st1		{v17.s}[2], [x17] 										\n"
			"	st1		{v17.s}[3], [x18] 										\n"
			"	add 	x17, x17, x7, lsl #5 									\n"
			"	add 	x18, x18, x7, lsl #5 									\n"

			".endm 																				\n"




			".macro	KERNEL8x1_BEGIN_K										\n"


	    "	ldr		q0, [x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	ldr		q2, [x24], #16											\n"

	    "	fmul	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	ldr		q1, [x11], #16											\n"
	    "	fmul	v17.4s, v1.4s, v2.s[0] 							\n"
	    "	ldp		q3, q4, [x11], #32 									\n"
	    
	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x1_K0												\n"

	    "	prfm	PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmla	v17.4s, v1.4s, v2.s[0]							\n"
	   	"	ldr		q4, [x11], #16 											\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x1_K1												\n"

	    "	prfm	PLDL1KEEP, [x11, #2560] 						\n"

	    "	fmla	v16.4s, v3.4s, v2.s[1]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v17.4s, v4.4s, v2.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x1_K2												\n"

	    "	fmla	v16.4s, v0.4s, v2.s[2]							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmla	v17.4s, v1.4s, v2.s[2]							\n"
	   	"	ldr		q4, [x11], #16 											\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x1_K3												\n"

	    "	prfm	PLDL1KEEP, [x11, #2560] 						\n"

	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v16.4s, v3.4s, v2.s[3]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v17.4s, v4.4s, v2.s[3]							\n"
	    "	ldr		q2, [x24], #16 											\n"
	    

	    ".endm 																			\n"


	    ".macro	KERNEL8x1_END_K											\n"

	    "	fmla	v16.4s, v3.4s, v2.s[3]							\n"
	    "	fmla	v17.4s, v4.4s, v2.s[3]							\n"

	    ".endm 																			\n"



			//--------------------------------------------------------------

	    "SMM_S2_KERNEL8x1:														\n"

	    "	ldr		x0, %[C]						 									\n"  //output
	    "	ldr		x1, %[A]						 									\n"	 //filter
	    "	ldr		x2, %[B]						 									\n"  //input

	    "	ldr		x3, %[M]						 									\n"
	    "	ldr		x4, %[N]						 									\n"  // (H * W) / 2
	    "	ldr		x5, %[K]						 									\n"
	    "	ldr		x9, %[LN] 						 								\n"   // H * W
	    "	ldr		x6, %[LK]						 									\n"
	    "	ldr		x7, %[out_LN] 												\n"  // (H / stride) * (W / stride)

	    "	prfm	PLDL1KEEP, [x1, #512]        					\n"
	    "	prfm	PLDL1KEEP, [x2, #8]         			 		\n"

	    "	ldr		x10, %[SB]						 								\n"
	    "	ldr		x8, %[k_tag] 					 								\n"


	    "Conv_1x1s2_BEGIN_N1:								 					\n"

	    "	mov 	x25, x0   						 								\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #16]									\n"
	    "	add		x26, x25, x7, lsl #2 			 						\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #16]									\n"
	    "	add 	x27, x25, x7, lsl #3 			 						\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #16]									\n"
	    "	add 	x28, x26, x7, lsl #3 			 						\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #16]									\n"

	    "	add 	x15, x27, x7, lsl #3 									\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #16]									\n"
	    "	add 	x16, x28, x7, lsl #3 									\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #16]									\n"
	    "	add 	x17, x15, x7, lsl #3 									\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #16]									\n"
	    "	add 	x18, x16, x7, lsl #3 									\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #16]									\n"

	    "	mov		x11, x1   						 								\n"   // address_A
	    "	lsr		x21, x3, #3						 								\n"   // M / 8


//-------------------------------------------------------PACK

	    "Conv_1x1s2_N1_BEGIN_PACKB:										\n"

	    "	mov		x24, x10						 									\n"   //还原SB的地址

	    "	mov		x12, x2						 	 									\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 						\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #8]			 					\n"
	    "	prfm    PLDL1KEEP, [x13, #8]			 					\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 				\n"


	    "PACK_N1_Body_K_S2:														\n"

	    "	lsr		x22, x5, #2 					 								\n"     // K / 4

	    "	PACK_KERNEL8x1_BEGIN_K				 	 						\n"

	    "	subs	x22, x22, #1		     		 							\n"
	    "	b 		PACK_N1_K1_3_conv_1x1s2								\n"

	    "PACK_K_N1_S2:									 							\n"

	    "	PACK_KERNEL8x1_K0						 								\n"
	    "PACK_N1_K1_3_conv_1x1s2:											\n"

	    "	PACK_KERNEL8x1_K1						 								\n"
	    "	PACK_KERNEL8x1_K0						 								\n"

	    "	beq		PACK_N1_Edge_K_S2											\n"

	    "	PACK_KERNEL8x1_K1						 								\n"

	    "	subs	x22, x22, #1			 		 								\n"
	    "	b 		PACK_K_N1_S2			 										\n"

	    "PACK_N1_Edge_K_S2:							    					\n"

	    "	PACK_KERNEL8x1_END_K			         					\n"


	    "	cmp		x8, #0							 									\n"
	    "	beq		PACK_M8N1_SAVE_S2 					 	 				\n"
	    "	Conv_s2_M8N1_ADD_C 							 						\n"
	    "PACK_M8N1_SAVE_S2: 							     				\n"
	    "	Conv_s2_SAVE8x1								 	 						\n"
	   
	    "	beq		M8N1_END_S2							 							\n"



	    //-----------------------------------------------------------------
	    "BEGIN_M8N1_Conv_1x1_S2:                      \n"

	    " mov     x24, x10                    				\n"   //  address_B, x11 is address_A + offset
	    " prfm    PLDL1KEEP, [x24, #128]    		 			\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 			\n"

	    "Body_N1_K_S2:                            		\n"

	    " lsr   x22, x5, #2                 		 			\n"     // K / 4
	    " KERNEL8x1_BEGIN_K                		 				\n"
	    " subs  x22, x22, #1                		 			\n"
	    " b 	K1_3_N1_S2                            	\n"

	    "Main_N1_K_S2:                            		\n"

	    " KERNEL8x1_K0                     						\n"
	    "K1_3_N1_S2:                              		\n"

	    " KERNEL8x1_K1                     		 				\n"
	    " KERNEL8x1_K2                     		 				\n"

	    " beq   Edge_N1_K_S2                      		\n"

	    " KERNEL8x1_K3                     		 				\n"

	    " subs  x22, x22, #1                		 			\n"
	    " b     Main_N1_K_S2                      		\n"

	    "Edge_N1_K_S2:                            		\n"

	    " KERNEL8x1_END_K                 			 			\n"

	    "	cmp		x8, #0							 									\n"
	    "	beq		M8N1_SAVE_S2 					 	 							\n"
	    "	Conv_s2_M8N1_ADD_C 							 						\n"
	    "M8N1_SAVE_S2: 							     							\n"
	    "	Conv_s2_SAVE8x1								 	 						\n"

	    " bgt  BEGIN_M8N1_Conv_1x1_S2              		\n"

	    "M8N1_END_S2:									 								\n"

	    
			:
	    :
	    [C] "m" (C),
	    [A] "m" (A),
	    [B] "m" (B),
	    [M] "m" (M),
	    [N] "m" (N),
	    [K] "m" (K),
	    [LN] "m" (LN),
	    [LK] "m" (LK),
	    [out_LN] "m" (out_LN),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag)
	    : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	    "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
	    "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
	    "x26", "x27", "x28", "x29", "x30",
	    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
	);

}

void SMM_S2_KERNEL8x2(float *C, float *A, float *B, long	M, long N, long K,
         long LN, long LK, long out_LN,float *SB, long k_tag)
{

	asm volatile(

			".macro	PACK_KERNEL8x2_BEGIN_K							\n"


	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldr		q2, [x12]														\n"
	    "	fmul 	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	fmul 	v17.4s, v0.4s, v2.s[2] 							\n"
	    "	ldr		q1, [x11], #16											\n"

	    "	ldr		q3, [x11], #16 											\n"
	    "	st1 	{v2.s}[0], [x24], #4 								\n"
	    "	add 	x12, x12, x9, lsl #3 								\n"

	    "	fmul 	v18.4s, v1.4s, v2.s[0] 							\n"
	    "	ldr		q5, [x13]														\n"
	    "	fmul 	v19.4s, v1.4s, v2.s[2] 							\n"
	    "	ldr		q4, [x11], #16 											\n"

			"	add 	x13, x13, x9, lsl #3 								\n"
	    "	st1 	{v2.s}[2], [x24], #4 								\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x2_K0										\n"

	    "	ldr		q5, [x13] 													\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmla	v17.4s, v0.4s, v2.s[2]							\n"
	 
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	ldr		q4, [x11], #16 											\n"
	    "	add 	x13, x13, x9, lsl #3 								\n"

	    "	fmla	v18.4s, v1.4s, v2.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x13, #8] 								\n"
	    "	st1 	{v2.s}[0], [x24], #4 								\n"
	    "	fmla	v19.4s, v1.4s, v2.s[2]							\n"
	    "	st1 	{v2.s}[2], [x24], #4 								\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x2_K1										\n"

	    "	ldr		q2, [x12] 													\n"
	    "	fmla	v16.4s, v3.4s, v5.s[0]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v17.4s, v3.4s, v5.s[2]							\n"

	    "	ldr		q1, [x11], #16 											\n"
	    "	add 	x12, x12, x9, lsl #3 								\n"
	    "	fmla	v18.4s, v4.4s, v5.s[0]							\n"
	    "	st1 	{v5.s}[0], [x24], #4 								\n"
	    "	prfm	PLDL1KEEP, [x12, #8] 								\n"
	    "	fmla	v19.4s, v4.4s, v5.s[2]							\n"
	    "	st1 	{v5.s}[2], [x24], #4 								\n"

	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x2_END_K								\n"

	    "	fmla	v16.4s, v3.4s, v5.s[0]							\n"
	    "	st1 	{v5.s}[0], [x24], #4 								\n"
	    "	fmla	v17.4s, v3.4s, v5.s[2]							\n"
	    "	st1 	{v5.s}[2], [x24], #4 								\n"
	    "	fmla	v18.4s, v4.4s, v5.s[0]							\n"
	    "	fmla	v19.4s, v4.4s, v5.s[2]							\n"

	    ".endm 																			\n"


	    ".macro	Conv_s2_M8N2_ADD_C 									\n"

	    "	prfm	PLDL1KEEP, [x25, #8]								\n"
	    "	prfm	PLDL1KEEP, [x26, #8]								\n"
	    "	prfm	PLDL1KEEP, [x27, #8]								\n"
	    "	prfm	PLDL1KEEP, [x28, #8]								\n"

	    "	ld2		{v0.s, v1.s}[0], [x25]							\n"
	    "	ld2		{v0.s, v1.s}[1], [x26]							\n"
	    "	ld2		{v0.s, v1.s}[2], [x27]							\n"
	    "	ld2		{v0.s, v1.s}[3], [x28]							\n"
	    "	fadd 	v16.4s, v16.4s, v0.4s 							\n"
	    "	fadd 	v17.4s, v17.4s, v1.4s 							\n"

	   	"	prfm	PLDL1KEEP, [x15, #8]								\n"
	    "	prfm	PLDL1KEEP, [x16, #8]								\n"
	    "	prfm	PLDL1KEEP, [x17, #8]								\n"
	    "	prfm	PLDL1KEEP, [x18, #8]								\n"
	    "	ld2		{v2.s, v3.s}[0], [x15]							\n"
	    "	ld2		{v2.s, v3.s}[1], [x16]							\n"
	    "	ld2		{v2.s, v3.s}[2], [x17]							\n"
	    "	ld2		{v2.s, v3.s}[3], [x18]							\n"
	    "	fadd 	v18.4s, v18.4s, v2.4s 							\n"
	    "	fadd 	v19.4s, v19.4s, v3.4s 							\n"

	    ".endm 																			\n"



			".macro	Conv_s2_SAVE8x2												\n"

			"	subs	x21, x21, #1 													\n"

			"	st2		{v16.s, v17.s}[0], [x25]							\n"
			"	add		x25, x25, x7, lsl #5 			 						\n"   
			"	st2		{v16.s, v17.s}[1], [x26] 							\n"
			"	add 	x26, x26, x7, lsl #5 									\n"

			"	st2		{v16.s, v17.s}[2], [x27] 							\n"
			"	st2		{v16.s, v17.s}[3], [x28] 							\n"
			"	add 	x27, x27, x7, lsl #5 									\n"
			"	add 	x28, x28, x7, lsl #5 									\n"

			"	st2		{v18.s, v19.s}[0], [x15] 							\n"
			"	st2		{v18.s, v19.s}[1], [x16] 							\n"
			"	add 	x15, x15, x7, lsl #5 									\n"
			"	add 	x16, x16, x7, lsl #5 									\n"

			"	st2		{v18.s, v19.s}[2], [x17] 							\n"
			"	st2		{v18.s, v19.s}[3], [x18] 							\n"
			"	add 	x17, x17, x7, lsl #5 									\n"
			"	add 	x18, x18, x7, lsl #5 									\n"

			".endm 																				\n"



			".macro	KERNEL8x2_BEGIN_K										\n"


	    "	ldr		q0, [x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	ldr		q2, [x24],#16												\n"

	    "	fmul	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	ldr		q1, [x11], #16											\n"
	    "	fmul	v17.4s, v0.4s, v2.s[1] 							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmul	v18.4s, v1.4s, v2.s[0] 							\n"
	    "	fmul	v19.4s, v1.4s, v2.s[1] 							\n"

	    "	ldr 	q4, [x11], #16 											\n"
	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x2_K0												\n"

	    "	prfm	PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmla	v17.4s, v0.4s, v2.s[1]							\n"
	   	"	ldr		q4, [x11], #16 											\n"
	    "	fmla	v18.4s, v1.4s, v2.s[0]							\n"
	    "	fmla	v19.4s, v1.4s, v2.s[1]							\n"


	    ".endm 																			\n"



	    ".macro	KERNEL8x2_K1												\n"

	    "	prfm	PLDL1KEEP, [x11, #2560] 						\n"

	    "	fmla	v16.4s, v3.4s, v2.s[2]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v17.4s, v3.4s, v2.s[3]							\n"
	    "	ldr		q5, [x24], #16 											\n"
	    "	fmla	v18.4s, v4.4s, v2.s[2]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v19.4s, v4.4s, v2.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x2_K2												\n"

	    "	fmla	v16.4s, v0.4s, v5.s[0]							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmla	v17.4s, v0.4s, v5.s[1]							\n"
	   	"	ldr		q4, [x11], #16 											\n"
	    "	fmla	v18.4s, v1.4s, v5.s[0]							\n"
	    "	fmla	v19.4s, v1.4s, v5.s[1]							\n"


	    ".endm 																			\n"


	    ".macro	KERNEL8x2_K3												\n"

	    "	prfm	PLDL1KEEP, [x11, #2560] 						\n"

	    "	fmla	v16.4s, v3.4s, v5.s[2]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v17.4s, v3.4s, v5.s[3]							\n"
	    "	ldr		q2, [x24], #16 											\n"
	    "	fmla	v18.4s, v4.4s, v5.s[2]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v19.4s, v4.4s, v5.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x2_END_K											\n"

	    "	fmla	v16.4s, v3.4s, v5.s[2]							\n"
	    "	fmla	v17.4s, v3.4s, v5.s[3]							\n"
	    "	fmla	v18.4s, v4.4s, v5.s[2]							\n"
	    "	fmla	v19.4s, v4.4s, v5.s[3]							\n"

	    ".endm 																			\n"



			//--------------------------------------------------------------

	    "SMM_S2_KERNEL8x2:														\n"

	    "	ldr		x0, %[C]						 									\n"  //output
	    "	ldr		x1, %[A]						 									\n"	 //filter
	    "	ldr		x2, %[B]						 									\n"  //input

	    "	ldr		x3, %[M]						 									\n"
	    "	ldr		x4, %[N]						 									\n"  // (H * W) / 2
	    "	ldr		x5, %[K]						 									\n"
	    "	ldr		x9, %[LN] 						 								\n"   // H * W
	    "	ldr		x6, %[LK]						 									\n"
	    "	ldr		x7, %[out_LN] 												\n"  // (H / stride) * (W / stride)

	    "	prfm	PLDL1KEEP, [x1, #512]        					\n"
	    "	prfm	PLDL1KEEP, [x2, #8]         			 		\n"

	    "	ldr		x10, %[SB]						 								\n"
	    "	ldr		x8, %[k_tag] 					 								\n"


	    "Conv_1x1s2_BEGIN_N2:								 					\n"

	    "	mov 	x25, x0   						 								\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #16]									\n"
	    "	add		x26, x25, x7, lsl #2 			 						\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #16]									\n"
	    "	add 	x27, x25, x7, lsl #3 			 						\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #16]									\n"
	    "	add 	x28, x26, x7, lsl #3 			 						\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #16]									\n"

	    "	add 	x15, x27, x7, lsl #3 									\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #16]									\n"
	    "	add 	x16, x28, x7, lsl #3 									\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #16]									\n"
	    "	add 	x17, x15, x7, lsl #3 									\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #16]									\n"
	    "	add 	x18, x16, x7, lsl #3 									\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #16]									\n"

	    "	mov		x11, x1   						 								\n"   // address_A
	    "	lsr		x21, x3, #3						 								\n"   // M / 8


//-------------------------------------------------------PACK

	    "Conv_1x1s2_N2_BEGIN_PACKB:										\n"

	    "	mov		x24, x10						 									\n"   //还原SB的地址

	    "	mov		x12, x2						 	 									\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 						\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #8]			 					\n"
	    "	prfm    PLDL1KEEP, [x13, #8]			 					\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 				\n"


	    "PACK_N2_Body_K_S2:														\n"

	    "	lsr		x22, x5, #2 					 								\n"     // K / 4

	    "	PACK_KERNEL8x2_BEGIN_K				 	 						\n"

	    "	subs	x22, x22, #1		     		 							\n"
	    "	b 		PACK_N2_K1_3_conv_1x1s2								\n"

	    "PACK_K_N2_S2:									 							\n"

	    "	PACK_KERNEL8x2_K0						 								\n"
	    "PACK_N2_K1_3_conv_1x1s2:											\n"

	    "	PACK_KERNEL8x2_K1						 								\n"
	    "	PACK_KERNEL8x2_K0						 								\n"

	    "	beq		PACK_N2_Edge_K_S2											\n"

	    "	PACK_KERNEL8x2_K1						 								\n"

	    "	subs	x22, x22, #1			 		 								\n"
	    "	b 		PACK_K_N2_S2			 										\n"

	    "PACK_N2_Edge_K_S2:							    					\n"

	    "	PACK_KERNEL8x2_END_K			         					\n"


	    "	cmp		x8, #0							 									\n"
	    "	beq		PACK_M8N2_SAVE_S2 					 	 				\n"
	    "	Conv_s2_M8N2_ADD_C 							 						\n"
	    "PACK_M8N2_SAVE_S2: 							     				\n"
	    "	Conv_s2_SAVE8x2								 	 						\n"
	   
	    "	beq		M8N2_END_S2							 							\n"



	    //-----------------------------------------------------------------
	    "BEGIN_M8N2_Conv_1x1_S2:                      \n"

	    " mov     x24, x10                    				\n"   //  address_B, x11 is address_A + offset
	    " prfm    PLDL1KEEP, [x24, #128]    		 			\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 			\n"

	    "Body_N2_K_S2:                            		\n"

	    " lsr   x22, x5, #2                 		 			\n"     // K / 4
	    " KERNEL8x2_BEGIN_K                		 				\n"
	    " subs  x22, x22, #1                		 			\n"
	    " b 	K1_3_N2_S2                            	\n"

	    "Main_N2_K_S2:                            		\n"

	    " KERNEL8x2_K0                     						\n"
	    "K1_3_N2_S2:                              		\n"

	    " KERNEL8x2_K1                     		 				\n"
	    " KERNEL8x2_K2                     		 				\n"

	    " beq   Edge_N2_K_S2                      		\n"

	    " KERNEL8x2_K3                     		 				\n"

	    " subs  x22, x22, #1                		 			\n"
	    " b     Main_N2_K_S2                      		\n"

	    "Edge_N2_K_S2:                            		\n"

	    " KERNEL8x2_END_K                 			 			\n"

	    "	cmp		x8, #0							 									\n"
	    "	beq		M8N2_SAVE_S2 					 	 							\n"
	    "	Conv_s2_M8N2_ADD_C 							 						\n"
	    "M8N2_SAVE_S2: 							     							\n"
	    "	Conv_s2_SAVE8x2								 	 						\n"

	    " bgt  BEGIN_M8N2_Conv_1x1_S2              		\n"

	    "M8N2_END_S2:									 								\n"

	    
			:
	    :
	    [C] "m" (C),
	    [A] "m" (A),
	    [B] "m" (B),
	    [M] "m" (M),
	    [N] "m" (N),
	    [K] "m" (K),
	    [LN] "m" (LN),
	    [LK] "m" (LK),
	    [out_LN] "m" (out_LN),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag)
	    : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	    "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
	    "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
	    "x26", "x27", "x28", "x29", "x30",
	    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
	);

}


void SMM_S2_KERNEL8x4(float *C, float *A, float *B, long	M, long N, long K,
         long LN, long LK, long out_LN,float *SB, long k_tag)
{

	asm volatile(

			".macro	PACK_KERNEL8x4_BEGIN_K							\n"


	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldp		q2, q3, [x12]												\n"
	    "	fmul 	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	fmul 	v17.4s, v0.4s, v2.s[2] 							\n"
	    "	ldr		q1, [x11], #16											\n"
	    "	fmul 	v18.4s, v0.4s, v3.s[0] 							\n"
	    "	fmul 	v19.4s, v0.4s, v3.s[2] 							\n"
	
	    "	ldr		q4, [x11], #16 											\n"
	    "	st2 	{v2.s, v3.s}[0], [x24], #8 					\n"
	    "	add 	x12, x12, x9, lsl #3 								\n"

	    "	fmul 	v20.4s, v1.4s, v2.s[0] 							\n"
	    "	ldp		q6, q7, [x13]												\n"
	    "	fmul 	v21.4s, v1.4s, v2.s[2] 							\n"
	    "	fmul 	v22.4s, v1.4s, v3.s[0] 							\n"
	    "	ldr		q5, [x11], #16 											\n"
	    "	fmul 	v23.4s, v1.4s, v3.s[2] 							\n"

			"	add 	x13, x13, x9, lsl #3 								\n"
	    "	st2 	{v2.s, v3.s}[2], [x24], #8 					\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x4_K0										\n"

	    "	ldp		q6, q7, [x13] 											\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	fmla	v17.4s, v0.4s, v2.s[2]							\n"
	    "	ldr		q4, [x11], #16 											\n"
	    "	fmla	v18.4s, v0.4s, v3.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	fmla	v19.4s, v0.4s, v3.s[2]							\n"

	    "	ldr		q5, [x11], #16 											\n"
	    "	add 	x13, x13, x9, lsl #3 								\n"

	    "	fmla	v20.4s, v1.4s, v2.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x13, #16] 							\n"
	    "	fmla	v21.4s, v1.4s, v2.s[2]							\n"
	    "	st2 	{v2.s, v3.s}[0], [x24], #8 					\n"
	    "	fmla	v22.4s, v1.4s, v3.s[0]							\n"
	    "	st2 	{v2.s, v3.s}[2], [x24], #8 					\n"
	    "	fmla	v23.4s, v1.4s, v3.s[2]							\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x4_K1										\n"

	    "	ldp		q2, q3, [x12] 											\n"
	    "	fmla	v16.4s, v4.4s, v6.s[0]							\n"
	    "	fmla	v17.4s, v4.4s, v6.s[2]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v18.4s, v4.4s, v7.s[0]							\n"
	    "	fmla	v19.4s, v4.4s, v7.s[2]							\n"

	    "	ldr		q1, [x11], #16 											\n"
	    "	add 	x12, x12, x9, lsl #3 								\n"
	    "	fmla	v20.4s, v5.4s, v6.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x12, #16] 							\n"
	    "	fmla	v21.4s, v5.4s, v6.s[2]							\n"
	    "	st2 	{v6.s, v7.s}[0], [x24], #8 					\n"
	    "	fmla	v22.4s, v5.4s, v7.s[0]							\n"
	    "	st2 	{v6.s, v7.s}[2], [x24], #8 					\n"
	    "	fmla	v23.4s, v5.4s, v7.s[2]							\n"

	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x4_END_K								\n"

	    "	fmla	v16.4s, v4.4s, v6.s[0]							\n"
	    "	fmla	v17.4s, v4.4s, v6.s[2]							\n"
	    "	fmla	v18.4s, v4.4s, v7.s[0]							\n"
	    "	fmla	v19.4s, v4.4s, v7.s[2]							\n"

	    "	fmla	v20.4s, v5.4s, v6.s[0]							\n"
	    "	fmla	v21.4s, v5.4s, v6.s[2]							\n"
	    "	st2 	{v6.s, v7.s}[0], [x24], #8 					\n"
	    "	fmla	v22.4s, v5.4s, v7.s[0]							\n"
	    "	st2 	{v6.s, v7.s}[2], [x24], #8 					\n"
	    "	fmla	v23.4s, v5.4s, v7.s[2]							\n"

	    ".endm 																			\n"


	    ".macro	Conv_s2_M8N4_ADD_C 									\n"

	    "	prfm	PLDL1KEEP, [x25, #16]								\n"
	    "	prfm	PLDL1KEEP, [x26, #16]								\n"

	    "	ldr		q0, [x25]														\n"
	    "	fadd 	v16.4s, v16.4s, v0.4s 							\n"
	    "	prfm	PLDL1KEEP, [x27, #16]								\n"
	    "	prfm	PLDL1KEEP, [x28, #16]								\n"
	    "	ldr		q1, [x26]														\n"
	    "	fadd 	v17.4s, v17.4s, v1.4s 							\n"
	    "	ldr		q2, [x27]														\n"
	   	"	prfm	PLDL1KEEP, [x15, #16]								\n"
	    "	prfm	PLDL1KEEP, [x16, #16]								\n"
	    "	fadd 	v18.4s, v18.4s, v2.4s 							\n"
	    "	ldr		q3, [x28]														\n"
	    "	fadd 	v19.4s, v19.4s, v3.4s 							\n"

	   	"	prfm	PLDL1KEEP, [x17, #16]								\n"
	    "	prfm	PLDL1KEEP, [x18, #16]								\n"
	    "	ldr		q4, [x15]														\n"
	    "	fadd 	v20.4s, v20.4s, v4.4s 							\n"
	    "	ldr		q5, [x16]														\n"
	    "	fadd 	v21.4s, v21.4s, v5.4s 							\n"
	    "	ldr		q6, [x17]														\n"
	    "	fadd 	v22.4s, v22.4s, v6.4s 							\n"
	    "	ldr		q7, [x18]														\n"
	    "	fadd 	v23.4s, v23.4s, v7.4s 							\n"

	    ".endm 																			\n"



		  ".macro	Conv_s2_SAVE8x4												\n"

			"	subs	x21, x21, #1 													\n"

			"	str		q16, [x25] 														\n"
			"	add 	x25, x25, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x25, #16]									\n"
			"	str		q17, [x26] 														\n"
			"	add 	x26, x26, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x26, #16]									\n"
			"	str		q18, [x27] 														\n"
			"	add 	x27, x27, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x27, #16]									\n"
			"	str		q19, [x28] 														\n"
			"	add 	x28, x28, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x28, #16]									\n"
			"	str		q20, [x15] 														\n"
			"	add 	x15, x15, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x15, #16]									\n"
			"	str		q21, [x16] 														\n"
			"	add 	x16, x16, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x16, #16]									\n"
			"	str		q22, [x17] 														\n"
			"	add 	x17, x17, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x17, #16]									\n"
			"	str		q23, [x18] 														\n"
			"	add 	x18, x18, x7, lsl #5 									\n"
			"	prfm	PLDL2KEEP, [x18, #16]									\n"

			".endm 																				\n"


			".macro	Conv_s2_SAVE8x4_ENDK									\n"

			"	subs	x21, x21, #1 													\n"

			"	st4		{v16.s, v17.s, v18.s, v19.s}[0], [x25]				\n"
			"	add		x25, x25, x7, lsl #5 			 										\n"   //C2*
			"	st4		{v16.s, v17.s, v18.s, v19.s}[1], [x26] 				\n"
			"	add 	x26, x26, x7, lsl #5 													\n"

			"	st4		{v16.s, v17.s, v18.s, v19.s}[2], [x27] 				\n"
			"	st4		{v16.s, v17.s, v18.s, v19.s}[3], [x28] 				\n"

			"	add 	x27, x27, x7, lsl #5 													\n"
			"	add 	x28, x28, x7, lsl #5 													\n"

			"	st4		{v20.s, v21.s, v22.s, v23.s}[0], [x15] 				\n"
			"	st4		{v20.s, v21.s, v22.s, v23.s}[1], [x16] 				\n"

			"	add 	x15, x15, x7, lsl #5 													\n"
			"	add 	x16, x16, x7, lsl #5 													\n"

			"	st4		{v20.s, v21.s, v22.s, v23.s}[2], [x17] 				\n"
			"	st4		{v20.s, v21.s, v22.s, v23.s}[3], [x18] 				\n"

			"	add 	x17, x17, x7, lsl #5 													\n"
			"	add 	x18, x18, x7, lsl #5 													\n"

			".endm 																								\n"



			".macro	KERNEL8x4_BEGIN_K										\n"


	    "	ldr		q0, [x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	ldr		q2, [x24], #16											\n"

	    "	fmul	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	ldr		q1, [x11], #16											\n"
	    "	fmul	v17.4s, v0.4s, v2.s[2] 							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmul	v18.4s, v0.4s, v2.s[1] 							\n"
	    "	ldr 	q5, [x24], #16 											\n"
	    "	fmul	v19.4s, v0.4s, v2.s[3] 							\n"

	    "	fmul	v20.4s, v1.4s, v2.s[0] 							\n"
	    "	fmul	v21.4s, v1.4s, v2.s[2] 							\n"
	    "	ldr 	q4, [x11], #16 											\n"
	    "	fmul	v22.4s, v1.4s, v2.s[1] 							\n"
	    "	prfm	PLDL1KEEP, [x24, #256]							\n"
	    "	fmul	v23.4s, v1.4s, v2.s[3] 							\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x4_K0												\n"

	    "	prfm	PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	ldr		q3, [x11], #16 											\n"
	    "	fmla	v17.4s, v0.4s, v2.s[2]							\n"
	    "	ldr		q5, [x24], #16 											\n"
	    "	fmla	v18.4s, v0.4s, v2.s[1]							\n"
	    "	ldr		q4, [x11], #16 											\n"
	    "	fmla	v19.4s, v0.4s, v2.s[3]							\n"

	    "	fmla	v20.4s, v1.4s, v2.s[0]							\n"
	    "	fmla	v21.4s, v1.4s, v2.s[2]							\n"
	    "	fmla	v22.4s, v1.4s, v2.s[1]							\n"
	    "	fmla	v23.4s, v1.4s, v2.s[3]							\n"


	    ".endm 																			\n"



	    ".macro	KERNEL8x4_K1												\n"

	    "	prfm	PLDL1KEEP, [x11, #2560] 						\n"

	    "	fmla	v16.4s, v3.4s, v5.s[0]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v17.4s, v3.4s, v5.s[2]							\n"
	    "	ldr		q2, [x24], #16 											\n"
	    "	fmla	v18.4s, v3.4s, v5.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v19.4s, v3.4s, v5.s[3]							\n"

	    "	fmla	v20.4s, v4.4s, v5.s[0]							\n"
	    "	fmla	v21.4s, v4.4s, v5.s[2]							\n"
	    "	fmla	v22.4s, v4.4s, v5.s[1]							\n"
	    "	fmla	v23.4s, v4.4s, v5.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x4_END_K											\n"

	    "	fmla	v16.4s, v3.4s, v5.s[0]							\n"
	    "	fmla	v17.4s, v3.4s, v5.s[2]							\n"
	    "	fmla	v18.4s, v3.4s, v5.s[1]							\n"
	    "	fmla	v19.4s, v3.4s, v5.s[3]							\n"

	    "	fmla	v20.4s, v4.4s, v5.s[0]							\n"
	    "	fmla	v21.4s, v4.4s, v5.s[2]							\n"
	    "	fmla	v22.4s, v4.4s, v5.s[1]							\n"
	    "	fmla	v23.4s, v4.4s, v5.s[3]							\n"

	    ".endm 																			\n"



			//--------------------------------------------------------------

	    "SMM_S2_KERNEL8x4:														\n"

	    "	ldr		x0, %[C]						 									\n"  //output
	    "	ldr		x1, %[A]						 									\n"	 //filter
	    "	ldr		x2, %[B]						 									\n"  //input

	    "	ldr		x3, %[M]						 									\n"
	    "	ldr		x4, %[N]						 									\n"  // (H * W) / 2
	    "	ldr		x5, %[K]						 									\n"
	    "	ldr		x9, %[LN] 						 								\n"   // H * W
	    "	ldr		x6, %[LK]						 									\n"
	    "	ldr		x7, %[out_LN] 												\n"  // (H / stride) * (W / stride)

	    "	prfm	PLDL1KEEP, [x1, #512]        					\n"
	    "	prfm	PLDL1KEEP, [x2, #16]         			 		\n"

	    "	ldr		x10, %[SB]						 								\n"
	    "	ldr		x8, %[k_tag] 					 								\n"


	    "Conv_1x1s2_BEGIN_N4:								 					\n"

	    "	mov 	x25, x0   						 								\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #16]									\n"
	    "	add		x26, x25, x7, lsl #2 			 						\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #16]									\n"
	    "	add 	x27, x25, x7, lsl #3 			 						\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #16]									\n"
	    "	add 	x28, x26, x7, lsl #3 			 						\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #16]									\n"

	    "	add 	x15, x27, x7, lsl #3 									\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #16]									\n"
	    "	add 	x16, x28, x7, lsl #3 									\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #16]									\n"
	    "	add 	x17, x15, x7, lsl #3 									\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #16]									\n"
	    "	add 	x18, x16, x7, lsl #3 									\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #16]									\n"

	    "	mov		x11, x1   						 								\n"   // address_A
	    "	lsr		x21, x3, #3						 								\n"   // M / 8


//-------------------------------------------------------PACK

	    "Conv_1x1s2_N4_BEGIN_PACKB:										\n"

	    "	mov		x24, x10						 									\n"   //还原SB的地址

	    "	mov		x12, x2						 	 									\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 						\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #16]			 					\n"
	    "	prfm    PLDL1KEEP, [x13, #16]			 					\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 				\n"


	    "PACK_N4_Body_K_S2:														\n"

	    "	lsr		x22, x5, #2 					 								\n"     // K / 4

	    "	PACK_KERNEL8x4_BEGIN_K				 	 						\n"

	    "	subs	x22, x22, #1		     		 							\n"
	    "	b 		PACK_N4_K1_3_conv_1x1s2								\n"

	    "PACK_K_N4_S2:									 							\n"

	    "	PACK_KERNEL8x4_K0						 								\n"
	    "PACK_N4_K1_3_conv_1x1s2:											\n"

	    "	PACK_KERNEL8x4_K1						 								\n"
	    "	PACK_KERNEL8x4_K0						 								\n"

	    "	beq		PACK_N4_Edge_K_S2											\n"

	    "	PACK_KERNEL8x4_K1						 								\n"

	    "	subs	x22, x22, #1			 		 								\n"
	    "	b 		PACK_K_N4_S2			 										\n"

	    "PACK_N4_Edge_K_S2:							    					\n"

	    "	PACK_KERNEL8x4_END_K			         					\n"


	    "	cmp		x8, #2							 									\n"
	    "	beq		PACK_SAVE8x4_ENDK_S2 									\n"
	    "	cmp		x8, #0							 									\n"
	    "	beq		M8N4_S2_PACK_SAVE 					 	 				\n"
	    "	Conv_s2_M8N4_ADD_C 							 						\n"
	    "	blt		PACK_SAVE8x4_ENDK_S2									\n"
	    "M8N4_S2_PACK_SAVE: 							     				\n"
	    "	Conv_s2_SAVE8x4								 	 						\n"
	    " b     PACK_END_SAVE_S2                			\n"
	    "PACK_SAVE8x4_ENDK_S2:                    		\n"
	    "	Conv_s2_SAVE8x4_ENDK												\n"
	    

	    "PACK_END_SAVE_S2: 														\n"

	    "	beq		M8N4_END_S2							 							\n"




	    //-----------------------------------------------------------------
	    "BEGIN_M8N4_Conv_1x1_S2:                      \n"

	    " mov     x24, x10                    				\n"   //  address_B, x11 is address_A + offset
	    " prfm    PLDL1KEEP, [x24, #128]    		 			\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 			\n"

	    "Body_N4_K_S2:                            		\n"

	    " lsr   x22, x5, #2                 		 			\n"     // K / 4
	    " KERNEL8x4_BEGIN_K                		 				\n"
	    " subs  x22, x22, #1                		 			\n"
	    " b 	K1_3_N4_S2                            	\n"

	    "Main_N4_K_S2:                            		\n"

	    " KERNEL8x4_K0                     						\n"
	    "K1_3_N4_S2:                              		\n"

	    " KERNEL8x4_K1                     		 				\n"
	    " KERNEL8x4_K0                     		 				\n"

	    " beq   Edge_N4_K_S2                      		\n"

	    " KERNEL8x4_K1                     		 				\n"

	    " subs  x22, x22, #1                		 			\n"
	    " b     Main_N4_K_S2                      		\n"

	    "Edge_N4_K_S2:                            		\n"

	    " KERNEL8x4_END_K                 			 			\n"

	    "	cmp		x8, #2							 									\n"
	    "	beq		SAVE8x4_ENDK_S2 											\n"
	    "	cmp		x8, #0							 									\n"
	    "	beq		M8N4_SAVE_S2 					 	 							\n"
	    "	Conv_s2_M8N4_ADD_C 							 						\n"
	    "	blt		SAVE8x4_ENDK_S2												\n"
	    "M8N4_SAVE_S2: 							     							\n"
	    "	Conv_s2_SAVE8x4								 	 						\n"
	    " b    	SAVE_N4_ENDK_S2          							\n"
	    "SAVE8x4_ENDK_S2:                             \n"
	    "	Conv_s2_SAVE8x4_ENDK												\n"

	    "SAVE_N4_ENDK_S2: 														\n"
	    " bgt  BEGIN_M8N4_Conv_1x1_S2              		\n"

	    "M8N4_END_S2:									 								\n"

	    
			:
	    :
	    [C] "m" (C),
	    [A] "m" (A),
	    [B] "m" (B),
	    [M] "m" (M),
	    [N] "m" (N),
	    [K] "m" (K),
	    [LN] "m" (LN),
	    [LK] "m" (LK),
	    [out_LN] "m" (out_LN),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag)
	    : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	    "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
	    "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
	    "x26", "x27", "x28", "x29", "x30",
	    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
	);

}


void SMM_S2(float *C, float *A, float *B, long	M, long N, long K,
         long LN, long LK, long out_LN,float *SB, long k_tag)
{


	asm volatile(

	    ".macro	PACK_KERNEL8x8_BEGIN_K							\n"


	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldp		q2, q3, [x12]												\n"
	    "	fmul 	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	fmul 	v17.4s, v0.4s, v2.s[2] 							\n"
	    "	ldp		q4, q5, [x12, #32]									\n"
	    "	fmul 	v18.4s, v0.4s, v3.s[0] 							\n"
	    "	fmul 	v19.4s, v0.4s, v3.s[2] 							\n"

	    "	ldr		q1, [x11], #16											\n"
	    "	fmul 	v20.4s, v0.4s, v4.s[0] 							\n"
	    "	ldr		q6, [x11], #16 											\n"
	    "	fmul 	v21.4s, v0.4s, v4.s[2] 							\n"
	    "	st4 	{v2.s, v3.s, v4.s, v5.s}[0], [x24], #16 		\n"
	    "	fmul 	v22.4s, v0.4s, v5.s[0] 							\n"
	    "	fmul 	v23.4s, v0.4s, v5.s[2] 							\n"

	    "	add 	x12, x12, x9, lsl #3 								\n"

	    "	st4 	{v2.s, v3.s, v4.s, v5.s}[2], [x24], #16 		\n"
	    "	fmul 	v24.4s, v1.4s, v2.s[0] 							\n"
	    "	fmul 	v25.4s, v1.4s, v2.s[2] 							\n"
	    "	ldp		q8, q9, [x13]												\n"
	    "	fmul 	v26.4s, v1.4s, v3.s[0] 							\n"
	    "	fmul 	v27.4s, v1.4s, v3.s[2] 							\n"

	    "	ldp		q10, q11, [x13, #32]								\n"
	    "	fmul 	v28.4s, v1.4s, v4.s[0] 							\n"
	    "	ldr		q7, [x11], #16 											\n"
	    "	fmul 	v29.4s, v1.4s, v4.s[2] 							\n"
	    "	fmul 	v30.4s, v1.4s, v5.s[0] 							\n"
	    "	fmul 	v31.4s, v1.4s, v5.s[2] 							\n"
	    "	add 	x13, x13, x9, lsl #3 								\n"


	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x8_K0										\n"

	    "	ldp		q8, q9, [x13] 											\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	fmla	v17.4s, v0.4s, v2.s[2]							\n"
	    "	ldr		q6, [x11], #16 											\n"
	    "	fmla	v18.4s, v0.4s, v3.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	fmla	v19.4s, v0.4s, v3.s[2]							\n"

	    "	ldp		q10, q11, [x13, #32] 								\n"
	   	"	fmla	v20.4s, v0.4s, v4.s[0]							\n"
	    "	fmla	v21.4s, v0.4s, v4.s[2]							\n"
	    "	ldr		q7, [x11], #16 											\n"
	    "	fmla	v22.4s, v0.4s, v5.s[0]							\n"
	    "	fmla	v23.4s, v0.4s, v5.s[2]							\n"

	    "	add 	x13, x13, x9, lsl #3 								\n"
	    "	st4 	{v2.s, v3.s, v4.s, v5.s}[0], [x24], #16 		\n"
	    "	fmla	v24.4s, v1.4s, v2.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x13, #64] 							\n"
	    "	fmla	v25.4s, v1.4s, v2.s[2]							\n"
	    "	st4 	{v2.s, v3.s, v4.s, v5.s}[2], [x24], #16 		\n"
	    "	fmla	v26.4s, v1.4s, v3.s[0]							\n"
	    "	fmla	v27.4s, v1.4s, v3.s[2]							\n"

	   	"	fmla	v28.4s, v1.4s, v4.s[0]							\n"
	    "	fmla	v29.4s, v1.4s, v4.s[2]							\n"
	    "	fmla	v30.4s, v1.4s, v5.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v5.s[2]							\n"

	    ".endm 																			\n"




	    ".macro	PACK_KERNEL8x8_K1										\n"

	    "	ldp		q2, q3, [x12] 											\n"
	    "	fmla	v16.4s, v6.4s, v8.s[0]							\n"
	    "	fmla	v17.4s, v6.4s, v8.s[2]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v18.4s, v6.4s, v9.s[0]							\n"
	    "	fmla	v19.4s, v6.4s, v9.s[2]							\n"

	    "	ldp		q4, q5, [x12, #32] 									\n"
	   	"	fmla	v20.4s, v6.4s, v10.s[0]							\n"
	    "	fmla	v21.4s, v6.4s, v10.s[2]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v22.4s, v6.4s, v11.s[0]							\n"
	    "	fmla	v23.4s, v6.4s, v11.s[2]							\n"

	    "	add 	x12, x12, x9, lsl #3 								\n"
	    "	st4 	{v8.s, v9.s, v10.s, v11.s}[0], [x24], #16 		\n"
	    "	fmla	v24.4s, v7.4s, v8.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x12, #64] 							\n"
	    "	fmla	v25.4s, v7.4s, v8.s[2]							\n"
	    "	st4 	{v8.s, v9.s, v10.s, v11.s}[2], [x24], #16 		\n"
	    "	fmla	v26.4s, v7.4s, v9.s[0]							\n"
	    "	fmla	v27.4s, v7.4s, v9.s[2]							\n"

	   	"	fmla	v28.4s, v7.4s, v10.s[0]							\n"
	    "	fmla	v29.4s, v7.4s, v10.s[2]							\n"
	    "	fmla	v30.4s, v7.4s, v11.s[0]							\n"
	    "	fmla	v31.4s, v7.4s, v11.s[2]							\n"

	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x8_END_K								\n"

	    "	fmla	v16.4s, v6.4s, v8.s[0]							\n"
	    "	fmla	v17.4s, v6.4s, v8.s[2]							\n"
	    "	fmla	v18.4s, v6.4s, v9.s[0]							\n"
	    "	fmla	v19.4s, v6.4s, v9.s[2]							\n"

	    "	st4 	{v8.s, v9.s, v10.s, v11.s}[0], [x24], #16 		\n"

	   	"	fmla	v20.4s, v6.4s, v10.s[0]							\n"
	    "	fmla	v21.4s, v6.4s, v10.s[2]							\n"
	    "	fmla	v22.4s, v6.4s, v11.s[0]							\n"
	    "	fmla	v23.4s, v6.4s, v11.s[2]							\n"

	    "	st4 	{v8.s, v9.s, v10.s, v11.s}[2], [x24], #16 		\n"

	    "	fmla	v24.4s, v7.4s, v8.s[0]							\n"
	    "	fmla	v25.4s, v7.4s, v8.s[2]							\n"
	    "	fmla	v26.4s, v7.4s, v9.s[0]							\n"
	    "	fmla	v27.4s, v7.4s, v9.s[2]							\n"

	   	"	fmla	v28.4s, v7.4s, v10.s[0]							\n"
	    "	fmla	v29.4s, v7.4s, v10.s[2]							\n"
	    "	fmla	v30.4s, v7.4s, v11.s[0]							\n"
	    "	fmla	v31.4s, v7.4s, v11.s[2]							\n"

	    ".endm 																			\n"



	    ".macro	Conv_s2_M8N8_ADD_C									\n"

	    "	prfm	PLDL1KEEP, [x25, #32]								\n"
	    "	prfm	PLDL1KEEP, [x26, #32]								\n"

	    "	ldp		q0, q1, [x25]												\n"
	    "	fadd 	v16.4s, v16.4s, v0.4s 							\n"
	    "	prfm	PLDL1KEEP, [x27, #32]								\n"
	    "	prfm	PLDL1KEEP, [x28, #32]								\n"
	    "	ldp		q2, q3, [x26]												\n"
	    "	fadd 	v17.4s, v17.4s, v1.4s 							\n"
	    "	ldp		q4, q5, [x27]												\n"
	   	"	prfm	PLDL1KEEP, [x15, #32]								\n"
	    "	prfm	PLDL1KEEP, [x16, #32]								\n"
	    "	fadd 	v18.4s, v18.4s, v2.4s 							\n"
	    "	ldp		q6, q7, [x28]												\n"
	    "	fadd 	v19.4s, v19.4s, v3.4s 							\n"

	   	"	prfm	PLDL1KEEP, [x17, #32]								\n"
	    "	prfm	PLDL1KEEP, [x18, #32]								\n"
	    "	ldp		q8, q9, [x15]												\n"
	    "	fadd 	v20.4s, v20.4s, v4.4s 							\n"
	    "	ldp		q10, q11, [x16]											\n"
	    "	fadd 	v21.4s, v21.4s, v5.4s 							\n"
	    "	ldp		q12, q13, [x17]											\n"
	    "	fadd 	v22.4s, v22.4s, v6.4s 							\n"
	    "	ldp		q14, q15, [x18]											\n"
	    "	fadd 	v23.4s, v23.4s, v7.4s 							\n"

	    "	fadd 	v24.4s, v24.4s, v8.4s 							\n"
	    "	fadd 	v25.4s, v25.4s, v9.4s 							\n"
	    "	fadd 	v26.4s, v26.4s, v10.4s 							\n"
	    "	fadd 	v27.4s, v27.4s, v11.4s 							\n"

	    "	fadd 	v28.4s, v28.4s, v12.4s 							\n"
	    "	fadd 	v29.4s, v29.4s, v13.4s 							\n"
	    "	fadd 	v30.4s, v30.4s, v14.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v15.4s 							\n"

	    ".endm	 																		\n"


		".macro	Conv_s2_SAVE8x8												\n"

		"	subs	x21, x21, #1 													\n"

		"	stp		q16, q17, [x25] 											\n"
		"	add 	x25, x25, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x25, #32]									\n"
		"	stp		q18, q19, [x26] 											\n"
		"	add 	x26, x26, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x26, #32]									\n"
		"	stp		q20, q21, [x27] 											\n"
		"	add 	x27, x27, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x27, #32]									\n"
		"	stp		q22, q23, [x28] 											\n"
		"	add 	x28, x28, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x28, #32]									\n"
		"	stp		q24, q25, [x15] 											\n"
		"	add 	x15, x15, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x15, #32]									\n"
		"	stp		q26, q27, [x16] 											\n"
		"	add 	x16, x16, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x16, #32]									\n"
		"	stp		q28, q29, [x17] 											\n"
		"	add 	x17, x17, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x17, #32]									\n"
		"	stp		q30, q31, [x18] 											\n"
		"	add 	x18, x18, x7, lsl #5 									\n"
		"	prfm	PLDL2KEEP, [x18, #32]									\n"

		".endm 																				\n"


		".macro	Conv_s2_SAVE8x8_ENDK									\n"

		"	subs	x21, x21, #1 													\n"

		"	st4		{v16.s, v17.s, v18.s, v19.s}[0], [x25], #16 	\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[0], [x25] 				\n"
		"	st4		{v16.s, v17.s, v18.s, v19.s}[1], [x26], #16 	\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[1], [x26] 				\n"

		"	sub 	x25, x25, #16 																\n"
		"	add 	x25, x25, x7, lsl #5 													\n"
		"	sub 	x26, x26, #16 																\n"
		"	add 	x26, x26, x7, lsl #5 													\n"

		"	st4		{v16.s, v17.s, v18.s, v19.s}[2], [x27], #16 	\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[2], [x27] 				\n"
		"	st4		{v16.s, v17.s, v18.s, v19.s}[3], [x28], #16 	\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[3], [x28] 				\n"

		"	sub 	x27, x27, #16 																\n"
		"	add 	x27, x27, x7, lsl #5 													\n"
		"	sub 	x28, x28, #16 																\n"
		"	add 	x28, x28, x7, lsl #5 													\n"

		"	st4		{v24.s, v25.s, v26.s, v27.s}[0], [x15], #16 	\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[0], [x15] 				\n"
		"	st4		{v24.s, v25.s, v26.s, v27.s}[1], [x16], #16 	\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[1], [x16] 				\n"

		"	sub 	x15, x15, #16 																\n"
		"	add 	x15, x15, x7, lsl #5 													\n"
		"	sub 	x16, x16, #16 																\n"
		"	add 	x16, x16, x7, lsl #5 													\n"

		"	st4		{v24.s, v25.s, v26.s, v27.s}[2], [x17], #16 	\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[2], [x17] 				\n"
		"	st4		{v24.s, v25.s, v26.s, v27.s}[3], [x18], #16 	\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[3], [x18] 				\n"

		"	sub 	x17, x17, #16 																\n"
		"	add 	x17, x17, x7, lsl #5 													\n"
		"	sub 	x18, x18, #16 																\n"
		"	add 	x18, x18, x7, lsl #5 													\n"

		".endm 																								\n"



	    ".macro	KERNEL8x8_BEGIN_K										\n"


	    "	ldr		q0, [x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	ldp		q2, q3, [x24], #32									\n"

	    "	fmul	v16.4s, v0.4s, v2.s[0] 							\n"
	    "	fmul	v17.4s, v0.4s, v3.s[0] 							\n"
	    "	ldr		q1, [x11], #16											\n"
	    "	fmul	v18.4s, v0.4s, v2.s[1] 							\n"
	    "	fmul	v19.4s, v0.4s, v3.s[1] 							\n"
	    "	ldr		q4, [x11], #16 											\n"
	    "	fmul	v20.4s, v0.4s, v2.s[2] 							\n"
	    "	fmul	v21.4s, v0.4s, v3.s[2] 							\n"
	    "	ldr 	q6, [x24], #16 											\n"
	    "	fmul	v22.4s, v0.4s, v2.s[3] 							\n"
	    "	fmul	v23.4s, v0.4s, v3.s[3] 							\n"

			"	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	fmul	v24.4s, v1.4s, v2.s[0] 							\n"
	    "	ldr 	q7, [x24], #16 											\n"
	    "	fmul	v25.4s, v1.4s, v3.s[0] 							\n"
	    "	fmul	v26.4s, v1.4s, v2.s[1] 							\n"
	    "	ldr 	q5, [x11], #16 											\n"
	    "	fmul	v27.4s, v1.4s, v3.s[1] 							\n"
	    "	fmul	v28.4s, v1.4s, v2.s[2] 							\n"
	    "	prfm	PLDL1KEEP, [x24, #256]							\n"
	    "	fmul	v29.4s, v1.4s, v3.s[2] 							\n"
	    "	fmul	v30.4s, v1.4s, v2.s[3] 							\n"
	    "	fmul	v31.4s, v1.4s, v3.s[3] 							\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x8_K0												\n"

	    "	prfm	PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v16.4s, v0.4s, v2.s[0]							\n"
	    "	fmla	v17.4s, v0.4s, v3.s[0]							\n"
	    "	ldr		q4, [x11], #16 											\n"
	    "	fmla	v18.4s, v0.4s, v2.s[1]							\n"
	    "	fmla	v19.4s, v0.4s, v3.s[1]							\n"
	    "	ldr		q6, [x24], #16 											\n"
	    "	fmla	v20.4s, v0.4s, v2.s[2]							\n"
	    "	fmla	v21.4s, v0.4s, v3.s[2]							\n"
	    "	ldr		q7, [x24], #16 											\n"
	    "	fmla	v22.4s, v0.4s, v2.s[3]							\n"
	    "	fmla	v23.4s, v0.4s, v3.s[3]							\n"

	    "	ldr		q5, [x11], #16 											\n"
	    "	fmla	v24.4s, v1.4s, v2.s[0]							\n"
	    "	fmla	v25.4s, v1.4s, v3.s[0]							\n"
	    "	fmla	v26.4s, v1.4s, v2.s[1]							\n"
	    "	fmla	v27.4s, v1.4s, v3.s[1]							\n"
	    "	fmla	v28.4s, v1.4s, v2.s[2]							\n"
	    "	fmla	v29.4s, v1.4s, v3.s[2]							\n"
	    "	fmla	v30.4s, v1.4s, v2.s[3]							\n"
	    "	fmla	v31.4s, v1.4s, v3.s[3]							\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x8_K1												\n"

	    "	prfm	PLDL1KEEP, [x11, #2560] 						\n"

	    "	fmla	v16.4s, v4.4s, v6.s[0]							\n"
	    "	fmla	v17.4s, v4.4s, v7.s[0]							\n"
	    "	ldr		q0, [x11], #16 											\n"
	    "	fmla	v18.4s, v4.4s, v6.s[1]							\n"
	    "	fmla	v19.4s, v4.4s, v7.s[1]							\n"
	    "	ldr		q2, [x24], #16 											\n"
	    "	fmla	v20.4s, v4.4s, v6.s[2]							\n"
	    "	fmla	v21.4s, v4.4s, v7.s[2]							\n"
	    "	ldr		q3, [x24], #16 											\n"
	    "	fmla	v22.4s, v4.4s, v6.s[3]							\n"
	    "	fmla	v23.4s, v4.4s, v7.s[3]							\n"

	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v24.4s, v5.4s, v6.s[0]							\n"
	    "	fmla	v25.4s, v5.4s, v7.s[0]							\n"
	    "	fmla	v26.4s, v5.4s, v6.s[1]							\n"
	    "	fmla	v27.4s, v5.4s, v7.s[1]							\n"
	    "	fmla	v28.4s, v5.4s, v6.s[2]							\n"
	    "	fmla	v29.4s, v5.4s, v7.s[2]							\n"
	    "	fmla	v30.4s, v5.4s, v6.s[3]							\n"
	    "	fmla	v31.4s, v5.4s, v7.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x8_END_K											\n"

	    "	fmla	v16.4s, v4.4s, v6.s[0]							\n"
	    "	fmla	v17.4s, v4.4s, v7.s[0]							\n"
	    "	fmla	v18.4s, v4.4s, v6.s[1]							\n"
	    "	fmla	v19.4s, v4.4s, v7.s[1]							\n"
	    "	fmla	v20.4s, v4.4s, v6.s[2]							\n"
	    "	fmla	v21.4s, v4.4s, v7.s[2]							\n"
	    "	fmla	v22.4s, v4.4s, v6.s[3]							\n"
	    "	fmla	v23.4s, v4.4s, v7.s[3]							\n"

	    "	fmla	v24.4s, v5.4s, v6.s[0]							\n"
	    "	fmla	v25.4s, v5.4s, v7.s[0]							\n"
	    "	fmla	v26.4s, v5.4s, v6.s[1]							\n"
	    "	fmla	v27.4s, v5.4s, v7.s[1]							\n"
	    "	fmla	v28.4s, v5.4s, v6.s[2]							\n"
	    "	fmla	v29.4s, v5.4s, v7.s[2]							\n"
	    "	fmla	v30.4s, v5.4s, v6.s[3]							\n"
	    "	fmla	v31.4s, v5.4s, v7.s[3]							\n"

	    ".endm 																			\n"

	    //--------------------------------------------------------------

	    "NDIRECT_conv_1x1s2_kernel:									\n"

	    "	ldr		x0, %[C]						 									\n"  //output
	    "	ldr		x1, %[A]						 									\n"	 //filter
	    "	ldr		x2, %[B]						 									\n"  //input

	    "	ldr		x3, %[M]						 									\n"
	    "	ldr		x4, %[N]						 									\n"  // (H * W) / 2
	    "	ldr		x5, %[K]						 									\n"
	    "	ldr		x9, %[LN] 						 								\n"   // H * W
	    "	ldr		x6, %[LK]						 									\n"
	    "	ldr		x7, %[out_LN] 												\n"  // (H / stride) * (W / stride)

	    "	prfm	PLDL1KEEP, [x1, #512]        					\n"
	    "	prfm	PLDL1KEEP, [x2, #32]         			 		\n"

	    "	ldr		x10, %[SB]						 								\n"
	    "	ldr		x8, %[k_tag] 					 								\n"

	    "	lsr  	x20, x4, #3 													\n"			// N / 8


	    //---------------------------------------------------------LOOP

	    "Conv_1x1s2_BEGIN_N8:								 					\n"

	    "	mov 	x25, x0   						 								\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #64]									\n"
	    "	add		x26, x25, x7, lsl #2 			 						\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #64]									\n"
	    "	add 	x27, x25, x7, lsl #3 			 						\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #64]									\n"
	    "	add 	x28, x26, x7, lsl #3 			 						\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #64]									\n"

	    "	add 	x15, x27, x7, lsl #3 									\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #64]									\n"
	    "	add 	x16, x28, x7, lsl #3 									\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #64]									\n"
	    "	add 	x17, x15, x7, lsl #3 									\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #64]									\n"
	    "	add 	x18, x16, x7, lsl #3 									\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #64]									\n"

	    "	mov		x11, x1   						 								\n"   // address_A
	    "	lsr		x21, x3, #3						 								\n"   // M / 8

	    //-----------------------------------------------------PACK

	    "Conv_1x1s2_BEGIN_PACKB:											\n"

	    "	mov		x24, x10						 									\n"   //还原SB的地址

	    "	mov		x12, x2						 	 									\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 						\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #64]			 					\n"
	    "	prfm    PLDL1KEEP, [x13, #64]			 					\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 				\n"


	    "PACK_Body_K_S2:															\n"

	    "	lsr		x22, x5, #2 					 								\n"     // K / 4

	    "	PACK_KERNEL8x8_BEGIN_K				 	 						\n"

	    "	subs	x22, x22, #1		     		 							\n"
	    "	b 		PACK_K1_7_conv_1x1s2									\n"

	    "PACK_K_S2:									 									\n"

	    "	PACK_KERNEL8x8_K0						 								\n"
	    "PACK_K1_7_conv_1x1s2:												\n"

	    "	PACK_KERNEL8x8_K1						 								\n"
	    "	PACK_KERNEL8x8_K0						 								\n"

	    "	beq		PACK_Edge_K_S2												\n"

	    "	PACK_KERNEL8x8_K1						 								\n"

	    "	subs	x22, x22, #1			 		 								\n"
	    "	b 		PACK_K_S2			 												\n"

	    "PACK_Edge_K_S2:							    						\n"

	    "	PACK_KERNEL8x8_END_K			         					\n"


	    "	cmp		x8, #2							 									\n"
	    "	beq		PACK_SAVE8x8_ENDK_S2 									\n"
	    "	cmp		x8, #0							 									\n"
	    "	beq		M8N8_S2_PACK_SAVE 					 	 				\n"
	    "	Conv_s2_M8N8_ADD_C 							 						\n"
	    "	blt		PACK_SAVE8x8_ENDK_S2									\n"
	    "M8N8_S2_PACK_SAVE: 							     				\n"
	    "	Conv_s2_SAVE8x8								 	 						\n"
	    " b     PACK_SAVE_ENDK_S2                			\n"
	    "PACK_SAVE8x8_ENDK_S2:                    		\n"
	    "	Conv_s2_SAVE8x8_ENDK												\n"
	    

	    "PACK_SAVE_ENDK_S2: 													\n"

	    "	beq		M8_END_S2							 								\n"

//-----------------------------------------------------------------
	    "BEGIN_M8_Conv_1x1_S2:                        \n"

	    " mov     x24, x10                    				\n"   //  address_B, x11 is address_A + offset
	    " prfm    PLDL1KEEP, [x24, #128]    		 			\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 			\n"

	    "Body_K_S2:                            		 		\n"

	    " lsr   x22, x5, #2                 		 			\n"     // K / 4
	    " KERNEL8x8_BEGIN_K                		 				\n"
	    " subs  x22, x22, #1                		 			\n"
	    " b 	K1_3_S2                            	 		\n"

	    "Main_K_S2:                            		 		\n"

	    " KERNEL8x8_K0                     						\n"
	    "K1_3_S2:                              				\n"

	    " KERNEL8x8_K1                     		 				\n"
	    " KERNEL8x8_K0                     		 				\n"

	    " beq   Edge_K_S2                      		 		\n"

	    " KERNEL8x8_K1                     		 				\n"

	    " subs  x22, x22, #1                		 			\n"
	    " b     Main_K_S2                      		 		\n"

	    "Edge_K_S2:                            		 		\n"

	    " KERNEL8x8_END_K                 			 			\n"

	    "BEGIN_SAVE8x8_S2:														\n"

	    "	cmp		x8, #2							 									\n"
	    "	beq		SAVE8x8_ENDK_S2 											\n"
	    "	cmp		x8, #0							 									\n"
	    "	beq		M8N8_SAVE_S2 					 	 							\n"
	    "	Conv_s2_M8N8_ADD_C 							 						\n"
	    "	blt		SAVE8x8_ENDK_S2												\n"
	    "M8N8_SAVE_S2: 							     							\n"
	    "	Conv_s2_SAVE8x8								 	 						\n"
	    " b    	SAVE_ENDK_S2          								\n"
	    "SAVE8x8_ENDK_S2:                             \n"
	    "	Conv_s2_SAVE8x8_ENDK												\n"

	    "SAVE_ENDK_S2: 																\n"
	    " bgt     BEGIN_M8_Conv_1x1_S2                \n"

	    "M8_END_S2:									 									\n"

	    "	subs	x20, x20, #1					 								\n"
	    "	add		x0, x0, #32						 								\n"
	    "	add   x2, x2, #64  													\n"  // address_B + offset
	    "	bgt		Conv_1x1s2_BEGIN_N8						 				\n"

	    :
	    :
	    [C] "m" (C),
	    [A] "m" (A),
	    [B] "m" (B),
	    [M] "m" (M),
	    [N] "m" (N),
	    [K] "m" (K),
	    [LN] "m" (LN),
	    [LK] "m" (LK),
	    [out_LN] "m" (out_LN),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag)
	    : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	    "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
	    "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
	    "x26", "x27", "x28", "x29", "x30",
	    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
	);

}