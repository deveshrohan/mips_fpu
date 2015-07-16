#ifndef MASK_H_INCLUDED
#define MASK_H_INCLUDED

#include "mips.h"

SC_MODULE (MASK) {
	sc_in< sc_bv<28> >		in_1;
	sc_in< sc_bv<DWORD> >	in_2;
	sc_out< sc_bv<DWORD> >	out;
	
	void mask_thread ( void );
	
	SC_CTOR (MASK) {
		SC_THREAD (mask_thread);
			sensitive << in_1;
			sensitive << in_2;
	}	
};

#endif
