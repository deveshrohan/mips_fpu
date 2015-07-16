#ifndef INCORDEC_H_
#define INCORDEC_H_

#include "mips.h"

SC_MODULE(IncorDec)
{
	sc_in< sc_bv<EXPONENT> > a;
	sc_in< sc_bv<1> > IncDec;
	sc_out< sc_bv<EXPONENT> > out;

	void IncorDec_thread(void);

	SC_CTOR(IncorDec)
	{
		SC_THREAD(IncorDec_thread);
			sensitive << a;
			sensitive << IncDec;

	}
};

#endif