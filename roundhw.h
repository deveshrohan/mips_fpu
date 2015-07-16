#ifndef ROUNDHW_H_
#define ROUNDHW_H_

#include "mips.h"

SC_MODULE(RoundHW)
{
	sc_in< sc_bv<EXPONENT> > a;
	sc_in< sc_bv<FRACTION+2> > b;
	sc_out< sc_bv<EXPONENT> > a_out;
	sc_out< sc_bv<FRACTION+2> > b_out;
	sc_in< sc_bv<1> > ctrl;

	void roundhw_thread(void);

	SC_CTOR(RoundHW)
	{
		SC_THREAD(roundhw_thread);
			sensitive << a;
			sensitive << b;
	}
};

#endif