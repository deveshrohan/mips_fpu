#include "fpushift.h"

void
SHIFTRIGHT_FRAC::
shiftright_frac_thread(void)
{

	sc_bv<FRACTION> a;
	sc_int<8> b;
	sc_bv<FRACTION + 2> temp; 
	while(true)
	{	
		a = in.read();
		b = count.read();
		if(b<0)
		{
			b=-b;
		}

		for(int i=0;i<23;i++)
		{
			temp[i] = a[i];
		}
		temp[23] = 1;
		temp[24] = 0;
 
		if(b >0)
		{
			for(int i=0;i<b;i++)
			{
				temp = temp >> 1;
			}
		}
	//	std::cout << b << "\t" << temp << endl;;

		
		out.write(temp);

		wait();
	}


}


void SHIFT_LEFT_OR_RIGHT::shift_left_or_right_thread(void)
{
	sc_uint<FRACTION+2> a_temp;
	sc_uint<1> ctrl_temp;
	sc_uint<FRACTION+2> out_temp;


	while(true)
	{
		ctrl_temp = ctrl.read();
		a_temp = a.read();



		if(ctrl_temp == 0)
		{
			out.write(a_temp);
		}
		else
		{
			out_temp = a_temp >> 1;
			out.write(out_temp);
		}
		wait();
	}

	
}
