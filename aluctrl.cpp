#include "aluctrl.h"

void
ALUCTRL::
aluctrl_thread(void)
{
	sc_uint<3> aluCtrl;
	sc_uint<2> aluop;
	sc_uint<6> functioncode;
	sc_uint<3> aluiCtrl;

	while (true) {
		#ifdef VERBOSE
		cerr << "ALUCTRL" << endl;
		#endif

		// Determine output
		aluop = ALUop.read();

		switch (aluop) {
			case 0x0:	// Add
				aluCtrl = ALU_ADD;
				break;
			case 0x1:	// Subtract
				aluCtrl = ALU_SUB;
				break;
			case 0x2:	// R-type instruction, look to functionCode
				functioncode = functionCode.read();
				functioncode = functioncode.range(5,0); 
				switch (functioncode) {
					case FUNC_ADD:	// Add
						aluCtrl = ALU_ADD;
						break;
					case FUNC_SUB:	// Subtract
						aluCtrl = ALU_SUB;
						break;
					case FUNC_AND:	// And
						aluCtrl = ALU_AND;
						break;
					case FUNC_OR:	// Or
						aluCtrl = ALU_OR;
						break;
					case FUNC_SLT:	// Set-on-less-than
						aluCtrl = ALU_SLT;
						break;
				}
				break;
			case 0x3:
				aluCtrl = ALU_SUI;
				break;
		}

		// Set result on output
		ALUctrl.write(aluCtrl);

		// Wait for new event
		wait();
	}
}
