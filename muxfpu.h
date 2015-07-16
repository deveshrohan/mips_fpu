#ifndef MUXFPU_H_INCLUDED
#define MUXFPU_H_INCLUDED

#include "mips.h"

SC_MODULE(MUX_EXP) {
	sc_in<  sc_bv<EXPONENT> >	in0;	// First input
	sc_in<  sc_bv<EXPONENT> > 	in1;	// Second input
	sc_in<  sc_bv<1> >	sel;	// Selection bit
	sc_out< sc_bv<EXPONENT> >	out;	// Output

	void mux_exp_thread(void);

	SC_CTOR(MUX_EXP) {
		SC_THREAD(mux_exp_thread);
		sensitive << in0;
		sensitive << in1;
		sensitive << sel;
	}
};

SC_MODULE(MUX_FRAC) {
	sc_in<  sc_bv<FRACTION> >	in0;	// First input
	sc_in<  sc_bv<FRACTION> >	in1;	// Second input
	sc_in<  sc_bv<1> >		sel;	// Selection bit
	sc_out< sc_bv<FRACTION> >	out;	// Output

	void mux_frac_thread(void);

	SC_CTOR(MUX_FRAC) {
		SC_THREAD(mux_frac_thread);
		sensitive << in0;
		sensitive << in1;
		sensitive << sel;
	}
};

SC_MODULE(MUX_ALU_NORMAL)
{
	sc_in< sc_bv<FRACTION+2> > 	in0;
	sc_in< sc_bv<FRACTION+2> >	in1;
	sc_in< sc_bv<1> >			sel;
	sc_out< sc_bv<FRACTION+2> >	out;

	void mux_alu_normal_thread(void);

	SC_CTOR(MUX_ALU_NORMAL)
	{
		SC_THREAD(mux_alu_normal_thread);
		sensitive << sel;
		sensitive << in0;
		sensitive << in1;
	}
};

#endif
