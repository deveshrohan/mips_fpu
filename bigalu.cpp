#include "bigalu.h"
 
void
BIG_ALU::
big_alu_thread(void)
{
	sc_bv<FRACTION+2>    result;
	sc_bv<FRACTION+2>  s;
	sc_bv<FRACTION>  t;

	sc_bv<FRACTION + 2> temp1;
	sc_bv<FRACTION + 2> temp2; 

	while (true) {
		#ifdef VERBOSE
		cerr << "BIG_ALU" << endl;
		#endif

		// Read the inputs
		s = a.read();
		t = b.read();

	//	std::cout << s << "\t" << t << endl;

		for(int i=0;i<25;i++)
		{
			temp1[i] = s[i];
		}

		for(int i=0;i<23;i++)
		{
			temp2[i] = t[i];
		}
		temp2[24]=0;
		temp2[23]=1;

		bool carry=0;

	

		for(int i=0;i<25;i++)
		{
			if(temp1[i] ==0 && temp2[i]==0)
			{
				result[i]=carry;
				carry=0;
			}
			else if(temp1[i] == 0 && temp2[i]==1 && carry==0)
			{
				result[i] = 1;
				carry = 0;
			}
			else if(temp1[i]==1 && temp2[i]==0 && carry==0)
			{
				result[i]=1;
				carry=0;
			}	
			else if(temp1[i]==0 && temp2[i]==1 && carry==1)
			{
				result[i]=0;
				carry=1;
			}
			else if(temp1[i]==1 && temp2[i]==0 && carry==1)
			{
				result[i]=0;
				carry=1;
			}
			else if(temp1[i]==1 && temp2[i]==1 && carry==0)
			{
				result[i]=0;
				carry=1;
			}
			else if(temp1[i]==1 && temp2[i]==1 && carry==1)
			{
				result[i]=1;
				carry=1;
			}
		}

		// Write results to output
		r.write(result);

		// Wait for next event
		wait();
	}
}
