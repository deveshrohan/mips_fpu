#include "smallalu.h"
 
void
SMALL_ALU::
small_alu_thread(void)
{
	sc_int<EXPONENT>    t_diff;
	sc_uint<EXPONENT>  s;
	sc_uint<EXPONENT>  t;

	while (true) {
		#ifdef VERBOSE
		cerr << "SMALL ALU" << endl;
		#endif

		// Read the inputs
		s = a.read();
		t = b.read();

		t_diff = s-t;
	//	std::cout << s << "\t" << t << "\t" << t_diff << endl;

		// Write results to output
		diff.write(t_diff);

		// Wait for next event
		wait();
	}
}
