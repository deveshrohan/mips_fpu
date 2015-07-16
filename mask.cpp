#include "mask.h"

void MASK::
mask_thread (void) {
		sc_uint<28> t_in_1;
		sc_uint<DWORD> t_in_2;
		sc_uint<DWORD> t_out;
		
		while(true) {
		#ifdef VERBOSE
		cerr << "APPEND BITS" << endl;
		#endif

		// Read input
		t_in_1 = in_1.read();
		t_in_2 = in_2.read();

		//Compute Jump Address
		t_out = t_in_1;
		
		for (int i = 28 ; i < 32 ; i++){
				t_out[i] = t_in_2[i];
			}
		//Write Result
		out.write(t_out);
		
		// Wait for next event
		wait();
	}
}	

