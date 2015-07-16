#ifndef DECODERFPU_H_INCLUDED
#define DECODERFPU_H_INCLUDED

#include "mips.h"

SC_MODULE(DECODER_FPU) {
	sc_in<  sc_bv<DWORD> >		op_1;
	sc_in<	sc_bv<DWORD> >		op_2;
	sc_out< sc_bv<SIGNBIT> >	op_1_sign;
	sc_out< sc_bv<EXPONENT> >		op_1_exp;    
	sc_out< sc_bv<FRACTION> >	op_1_frac;    
	sc_out< sc_bv<SIGNBIT> >	op_2_sign;    
	sc_out< sc_bv<EXPONENT> >		op_2_exp;
	sc_out< sc_bv<FRACTION> >	op_2_frac;

	void decoder_fpu_thread(void);

	SC_CTOR(DECODER_FPU) {
		SC_THREAD(decoder_fpu_thread);
		sensitive << op_1;
		sensitive << op_2;
	}
};

#endif
