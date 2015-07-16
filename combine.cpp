#include "combine.h"

void COMBINE::combine_thread(void)
{
	sc_int<EXPONENT> 	a_temp;
	sc_int<FRACTION+2> 	b_temp;
	sc_int<FRACTION> 	fin_exponent;
	sc_int<32>			ans;

	while(true)
	{
		a_temp = a.read();
		b_temp = b.read();

		if(b_temp[24]!=1 && b_temp[23]!=1)
		{
			fin_exponent = b_temp.range(22,0);
		}
		ans[31]=0;
		for(int i=23;i<31;i++)
		{
			ans[i] = a_temp[i-23];
		}
		for(int i=0;i<23;i++)
		{
			ans[i] = b_temp[i];
		}
		c.write(ans);

		wait();	

	}
}
