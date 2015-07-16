#ifndef COMBINE_H_
#define COMBINE_H_

#include "mips.h"

SC_MODULE(COMBINE)
{
	sc_in< sc_bv<EXPONENT> > a;
	sc_in< sc_bv<FRACTION+2> > b;
	sc_out< sc_bv<32> > c;

	void combine_thread(void);

	SC_CTOR(COMBINE)
	{
		SC_THREAD(combine_thread);
		sensitive << a;
		sensitive << b;
	}
};

#endif