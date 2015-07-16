#include "fpuctrl.h"

void
FPU_CTRL::
fpu_ctrl_thread(void)
{
	sc_int<EXPONENT>	t_ExpDiff;
	sc_bv<FRACTION+2>	t_BigAluOut;	
	sc_bv<FRACTION+2>	t_RoundFraction;

	while (true) {
		#ifdef VERBOSE
		cerr << "FPU_CTRL" << endl;
		#endif

		// Read the input
		t_ExpDiff = ExpDiff.read();
		t_BigAluOut = BigAluOut.read();
		t_RoundFraction = RoundFraction.read();

//		alu_or_normal.write(0);


		bool flag=true;

		if(!flag)
		{
			if(t_RoundFraction[24]==1)
			{
				alu_or_normal.write(1);
				alu_or_normal_expo.write(1);
				ShiftLR.write(1);
				IncDec.write(1);
			}
			else
			{
				alu_or_normal.write(0);
				alu_or_normal_expo.write(0);
				ShiftLR.write(0);
				IncDec.write(0);
			}

		}

		if(flag)
		{
			flag = false;
	// Determine Large Exponent, Large Significand and Small Significand

		if (t_ExpDiff > 0 || t_ExpDiff == 0)
		{
				LargeExp.write(0);
				LargeSD.write(0);
				SmallSD.write(1);
		}
		else
		{
			t_ExpDiff = -t_ExpDiff;
				LargeExp.write(1);
				LargeSD.write(1);
				SmallSD.write(0);
		}

			if(t_BigAluOut[24] == 1)
			{
				ShiftLR.write(1);
				IncDec.write(1);
				alu_or_normal.write(0);
				alu_or_normal_expo.write(0);
			}
			else
			{
				alu_or_normal.write(0);
				alu_or_normal_expo.write(0);
				ShiftLR.write(0);
				IncDec.write(0);
			}
		}
		
	//	std::cout<<t_BigAluOut<<"\t"<<t_RoundFraction<<endl;
		RoundHW.write(0);
		ShiftR.write(t_ExpDiff);


		wait(); 
	}
}
