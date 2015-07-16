#ifndef FPUMULT_H_
#define FPUMULT_H_

#include "mips.h"

SC_MODULE(FPU_MULT)
{
	sc_in< sc_bv<FRACTION> > a;
	sc_in< sc_bv<FRACTION> > b;
	sc_out< sc_bv<FRACTION+2> > result;

	void fpu_mult_thread(void);

	SC_CTOR(FPU_MULT)
	{
		SC_THREAD(FPU_MULT);
		sensitive << a;
		sensitive << b;
	}
};
#endif