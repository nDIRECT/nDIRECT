NDIRECT_PREFIX = /thfs1/home/test652/wpy/SC23/github/NDIRECT/direct_cnn
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
	rm -f *.out *.o *.a *.so
