#ifndef FPUCTRL_H_INCLUDED
#define FPUCTRL_H_INCLUDED

#include "mips.h"

SC_MODULE(FPU_CTRL) {
	sc_in<  sc_bv<EXPONENT> >	ExpDiff;	// Exponent Difference
	sc_in<  sc_bv<FRACTION+2> >	BigAluOut;	// Big ALU Result
	sc_in<  sc_bv<FRACTION+2> >	RoundFraction;	// Fraction Round Off
	sc_out< sc_bv<1> >		LargeExp;		// Exponent MUX
	sc_out< sc_bv<1> >		SmallSD;	// Smaller Significand
	sc_out< sc_bv<1> >		LargeSD;		// Large Significand
	sc_out< sc_bv<8> >		ShiftR;		// Shift Right
	sc_out< sc_bv<1> >		ShiftLR;	// Shift Left OR Right
	sc_out< sc_bv<1> >		IncDec;		// Increment or Decrement
	sc_out< sc_bv<1> >		RoundHW;	// Rounding Hawrdware

	sc_out< sc_bv<1> >		alu_or_normal;	//select alu or normalized
	sc_out< sc_bv<1> >		alu_or_normal_expo;

				
	void fpu_ctrl_thread(void);

	// Constructor
	SC_CTOR(FPU_CTRL) {
		SC_THREAD(fpu_ctrl_thread);
		sensitive << ExpDiff;
		sensitive << BigAluOut;
		sensitive << RoundFraction;
	}
};

#endif
