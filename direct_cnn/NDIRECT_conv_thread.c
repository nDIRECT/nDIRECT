#include "NDIRECT_direct.h"

vector<int>vec{1,2,4,8,16,32,64};
int Tm=0, Tn=0, T;

void Dete_grad_N_threads_nums(int N, int K, int H, int W, int stride)
{
	int i, temp;
	T = NUM;

	if(Tm==0)
    {
		// Determines the number of threads to parallelize the N-dimension

		Tn= ceil(sqrt(4 * T * (float) (H / stride) * (W / stride) * N / (float)K));
		for(i = 0;  i < vec.size(); i++)
		{
			if(Tn <= vec[i])
			{
				temp = vec[i];
				if(i > 0 && (Tn - vec[i-1] <= vec[i] - Tn))
				{	
					Tn = vec[i-1];			
				}
				else
				{
					Tn = temp;
				}
				break;
			}
		}
		if(Tn >= T)
			Tn = T;
		// Determines the number of threads to parallelize the M-dimension
		Tm = T / Tn;
	}
}