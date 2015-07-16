#include "roundhw.h"

void RoundHW::roundhw_thread(void)
{
	sc_int<EXPONENT> a_temp;
	sc_int<FRACTION+2> b_temp;
	sc_int<FRACTION+2> result;
	sc_int<1> ctrl_temp;

	while(true)
	{
		a_temp = a.read();
		b_temp = b.read();

		bool temp = b_temp[0];
	//	b_temp = b_temp >> 1;
		result = b_temp + temp;
		
	//	std::cout << a_temp << endl;

		a_out.write(a_temp);
		b_out.write(result);

		wait();
	}
}
