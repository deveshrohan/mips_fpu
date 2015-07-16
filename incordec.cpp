#include "incordec.h"

void IncorDec::IncorDec_thread(void)
{
	sc_bv<EXPONENT> a_temp;
	sc_bv<1> ctrl;
	sc_bv<EXPONENT> result;


	while(true)
	{
		a_temp = a.read();
		ctrl = IncDec.read();

		bool carry=1;
	//	std::cout << "\t" << a_temp << "\t" << ctrl << endl;;

		if(ctrl==1)
		{
			for(int i=0;i<EXPONENT;i++)
			{
				if(a_temp[i]==0)
				{
					result[i]=carry;
					carry=0;
				}
				else if(a_temp[i]==1 && carry==0)
				{
					result[i]=1;
					carry=0;
				}
				else if(a_temp[i]==1 && carry==1)
				{
					result[i]=0;
					carry=1;
				}
			}
		}
		else
		{
			result = a_temp;
		}

 
	/*	if(ctrl == 1)
		{
			for(int i=0;i<EXPONENT;i++)
			{
					switch(carry)
					{
						case 0: switch(a_temp[i])
								{
									case 0: a_temp[i]=0;
											carry=0;
											break;
									case 1: a_temp[i]=1;
											carry=0;
											break;
								}
						case 1: switch(a_temp[i])
								{
									case 0: a_temp[i]=1;
											carry=0;
											break;
									case 1: a_temp[i]=0;
											carry=1;
											break;
								}
					}
					result[i] = a_temp[i];
			}
				

		}
		*/
	
		
		
	//	std::cout << a_temp << endl;
		out.write(result);

		wait();

	}

}
