#include "fpumult.h"

void FPU_MULT::fpu_mult_thread(void)
{
	sc_bv<FRACTION> a_temp;
	sc_bv<FRACTION> b_temp;
	sc_bv<FRACTION+1> result_temp;

	sc_bv<FRACTION + 2> temp1;
	sc_bv<FRACTION + 2> temp2;

	while(true)
	{
		a_temp = a.read();
		b_temp = b.read();
		

		for(int i=0;i<FRACTION;i++)
		{
			temp1[i] = a_temp[i];
			temp2[i] = b_temp2[i];
		}
		temp1[23] = 1;
		temp2[23] = 1;

		temp1[24] = 0;
		temp2[24] = 0;

		for(int i=0;i<25;i++)
		{
			result_temp[i] += a_temp[i]*b_temp << i;
		}
		result.write(result_temp);


	} 
}