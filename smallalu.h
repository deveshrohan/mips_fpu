#ifndef SMALLALU_H_INCLUDED
#define SMALLALU_H_INCLUDED

#include "mips.h"
 
SC_MODULE(SMALL_ALU) {
	sc_in<  sc_bv<EXPONENT> >	a;	// First operand input 
	sc_in<  sc_bv<EXPONENT> >	b;	// Second operand input
	sc_out< sc_bv<EXPONENT> >	diff;		// Result

	void small_alu_thread(void);

	// Constructor
	SC_CTOR(SMALL_ALU) {
		SC_THREAD(small_alu_thread);
		sensitive << a;
		sensitive << b;
	}
};

#endif
