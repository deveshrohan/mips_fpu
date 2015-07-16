#include "decoderfpu.h"

void
DECODER_FPU::
decoder_fpu_thread(void)
{
	sc_uint<DWORD>  	 t_op_1;
	sc_uint<DWORD>  	 t_op_2;
    sc_uint<SIGNBIT>     t_op_1_sign;
    sc_uint<EXPONENT>      t_op_1_exp;
    sc_uint<FRACTION>	 t_op_1_frac;
    sc_uint<SIGNBIT>	 t_op_2_sign;
    sc_uint<EXPONENT>      t_op_2_exp;
    sc_uint<FRACTION>     t_op_2_frac;


	while (true) {
		

		// Read input
		t_op_1 = op_1.read();
		t_op_2 = op_2.read();

		// Decode instruction
		t_op_1_sign = t_op_1[31];
       	t_op_1_exp  = t_op_1.range(30,23);
       	t_op_1_frac = t_op_1.range(22,0); 
   		t_op_2_sign = t_op_2[31];
       	t_op_2_exp  = t_op_2.range(30,23); 
       	t_op_2_frac = t_op_2.range(22,0);

		// Produce output
        op_1_sign.write(t_op_1_sign);
        op_1_exp.write(t_op_1_exp);
        op_1_frac.write(t_op_1_frac);
        op_2_sign.write(t_op_2_sign);
     	op_2_exp.write(t_op_2_exp);
        op_2_frac.write(t_op_2_frac);

        // Wait for next event
        wait();

	}
}
