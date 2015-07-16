#include "muxfpu.h"

void
MUX_EXP::
mux_exp_thread(void)
{
	sc_bv<EXPONENT> data;

	while (true) {
		#ifdef VERBOSE
		cerr << "MUX_EXP" << endl;
		#endif

		// Read correct input
		if(sel.read() == 0)
			data = in0.read();
		else
			data = in1.read();

		// Write output
		out.write(data);

		// wait for new event
		wait();
	}
}

void
MUX_FRAC::
mux_frac_thread(void)
{
	sc_bv<FRACTION> data;

	while (true) {
		#ifdef VERBOSE
		cerr << "MUX_FRAC" << endl;
		#endif

		if(sel.read() == 0)
			data = in0.read();
		else
			data = in1.read();

		out.write(data);

		// wait for new event
		wait();
	}
}

void MUX_ALU_NORMAL::mux_alu_normal_thread(void)
{
	sc_bv<FRACTION+2> data;

	while (true) {
		#ifdef VERBOSE
		cerr << "MUX_FRAC" << endl;
		#endif

		if(sel.read() == 0)
			data = in0.read();
		else
			data = in1.read();

		out.write(data);

		// wait for new event
		wait();
	}
}
