#ifndef FPUSHIFT_H_INCLUDED
#define FPUSHIFT_H_INCLUDED

#include "mips.h"

SC_MODULE(SHIFTRIGHT_FRAC) {
	sc_in< sc_bv<FRACTION> >	in;
	sc_out< sc_bv<FRACTION+2> >	out;
	sc_in<sc_bv<8> >			count;

	void shiftright_frac_thread(void);

	SC_CTOR(SHIFTRIGHT_FRAC) {
		SC_THREAD(shiftright_frac_thread);
		sensitive << in;
		sensitive << count;
	}
};



SC_MODULE(SHIFT_LEFT_OR_RIGHT)
{
	sc_in< sc_bv<FRACTION+2> > a;
	sc_in< sc_bv<1> > ctrl;
	sc_out< sc_bv<FRACTION+2> > out;

	void shift_left_or_right_thread(void);

	SC_CTOR(SHIFT_LEFT_OR_RIGHT)
	{
		SC_THREAD(shift_left_or_right_thread);
			sensitive << a;
	}
};

#endif
