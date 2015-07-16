#ifndef BIGALU_H_
#define BIGALU_H_

#include "mips.h"
 
SC_MODULE(BIG_ALU) {
	sc_in<  sc_bv<FRACTION+2> >	a;	// First operand input 
	sc_in<  sc_bv<FRACTION> >	b;	// Second operand input
	sc_out< sc_bv<FRACTION+2> >	r;	// Result

	void big_alu_thread(void);

	// Constructor
	SC_CTOR(BIG_ALU) {
		SC_THREAD(big_alu_thread);
		sensitive << a;
		sensitive << b;
	}
};

#endif
