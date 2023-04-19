#                                                C  K  H  W  R  S  stride  padh  padw
export GOMP_CPU_AFFINITY="0-63"
rm -rf NDIRECT_CONV_FT.txt
#Resnet-50
yhrun -p thcp1 numactl -i 0-7 ./a.out 3 64 224 224 7 7 2 3 3
yhrun -p thcp1 numactl -i 0-7 ./a.out 128 128 56 56 3 3 2 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 64 64 56 56 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 256 512 56 56 1 1 2 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 64 64 56 56 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 64 256 56 56 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 256 64 56 56 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 256 128 56 56 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 256 256 28 28 3 3 2 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 128 128 28 28 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 1024 28 28 1 1 2 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 256 28 28 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 128 28 28 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 128 512 28 28 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 512 14 14 3 3 2 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 256 256 14 14 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 1024 2048 14 14 1 1 2 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 256 1024 14 14 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 1024 512 14 14 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 1024 256 14 14 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 512 7 7 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 2048 7 7 1 1 1 0 0
yhrun -p thcp1 numactl -i 0-7 ./a.out 2048 512 7 7 1 1 1 0 0
#VggNet-16
yhrun -p thcp1 numactl -i 0-7 ./a.out 64 64 224 224 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 128 128 112 112 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 256 256 56 56 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 512 28 28 3 3 1 1 1
yhrun -p thcp1 numactl -i 0-7 ./a.out 512 512 14 14 3 3 1 1 1

