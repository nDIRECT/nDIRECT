### NDIRECT
----------------------
Contact: Pengyu Wang (pengyu_wang@nudt.edu.cn)

NDIRECT is a library for direct convolution on ARMv8-based processors. It preserves the conventional `NCHW` data format, which is compatible with mainstream deep learning frameworks, such as Pytorch and MXNet. 

This work continues to be optimized.
### Software Dependences
------------------------
* [GNU Compiler (GCC)](https://gcc.gnu.org/) (>=v8.2)
* [OpenMP](https://www.openmp.org/) 

### Hardware Platform
-------------------------
Phyitum 2000+, Kunpeng 920, ThunderX2, Raspberry Pi 4 Model B or other ARMv8-based processors.

### Compile and Install
----------------------
```cpp
$ cd direct_cnn/
$ make
$ make install PREFIX= specify the installation path
```
### Compile with NDIRECT
----------------------
```cpp
#include <stdio.h>
#include <stdlib.h>
#include "NDIRECT_direct.h"
```
### API
----------------------
We illustrate the convolution interface of NDIRECT.
```cpp
NDIRECT_dnn_conv_fwd_exec(int H, int W, int N, int C, float *input,
                                 int K, int R, int S, float *filter,
                                 int padh, int padw, int stride, float* output);
```
### Running Benchmark
----------------------
To run the command
```cpp
$ cd direct_cnn/test
$ make
$ ./run.sh
```
will evaluate the performance of convolution layers from ResNet-50 and VggNet-16.
### Getting Started
----------------------
The following source code provides an instance  to evaluate the convolution performance.
```cs
#include <stdio.h>
#include <stdlib.h>
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

int main()
{
	int H = 224, W = 224, N = 64, C = 3;
	int K = 64, R = 7, S = 7;
	int padh = 3, padw = 3, stride = 2;

	int out_H = (H - R + 2 * pad_h)/stride + 1; 
    int out_W = (W - S + 2 * pad_w)/stride + 1;

    filter = ( float * ) malloc( C * K * R *S *sizeof( float ));
    input = ( float * ) malloc(N * C * H * W * sizeof( float ));
    output = ( float * ) malloc(N * K * (out_H * out_W) * sizeof( float ));
    random_matrix(K, C * R * S, filter);
    random_matrix(C, N * H * W, input);
	// warm up
    NDIRECT_dnn_conv_fwd_exec(H, W, N , C, input, K, R, S, filter,
    padh, padw, stride, output);
    
    // evaluate
    start = dclock();
    for ( i = 0; i < loop ; i++)
    	NDIRECT_dnn_conv_fwd_exec(H, W, N, C, input, K, R, S, filter, padh, padw, stride, output);
    cost = (dclock() - start) / loop;

    free(filter);
    free(input);
    free(output);
	return 0;
}
```
The corresponding makefile for this program is:
```makefile
NDIRECT_PREFIX = path to install NDIRECT
NDIRECT_INC    = $(NDIRECT_PREFIX)/ND/include
NDIRECT_LIB    = $(NDIRECT_PREFIX)/ND/lib

OTHER_LIBS  = -fopenmp

CC          = g++
CFLAGS      = -g -fopenmp -O3 -I$(NDIRECT_INC) -L$(NDIRECT_LIB) -lnd
LINKER      = $(CC)

OBJS        = test.o

%.o: %.c
	 $(CC) $(CFLAGS) -c -fopenmp $< -o $@

all: $(OBJS)
	$(LINKER) $(OBJS) $(CFLAGS) $(OTHER_LIBS) -o a.out
	
.PHONY:clean
clean:
	rm -f *.o *.a *.so
```
### Note
----------------------
NDIRECT adopts the traditional `NCHW` and `KCRS` data formats to store input and filter tensors. Therefore, you can integrate NDIRECT with DL frameworks for CNNs training and inference directly.
