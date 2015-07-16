#include "add.h"
#include "aluctrl.h"
#include "alu.h"
#include "and.h"
#include "ctrl.h"
#include "decoder.h"
#include "mux.h"
#include "ram.h"
#include "rom.h"
#include "registerfile.h"
#include "register.h"
#include "shift.h"
#include "shiftjump.h"
#include "signextend.h"
#include "mask.h"
#include "decoderfpu.h"
#include "smallalu.h"
#include "muxfpu.h"
#include "fpuctrl.h"
#include "bigalu.h"
#include "fpushift.h"
#include "incordec.h"
#include "roundhw.h"
#include "combine.h"
#include "mips.h"

int
sc_main(int argc, char* argv[])
{
	sc_report_handler::
	set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

	char stbuf[256];

	// SIGNALS

	// Data signals
	sc_signal< sc_bv<DWORD> >		bus_mux1;
	sc_signal< sc_bv<DWORD> >		bus_mux2;
	sc_signal< sc_bv<DWORD> >		bus_mux3;
	sc_signal< sc_bv<AWORDREG> >	bus_mux4;
	sc_signal< sc_bv<DWORD> >		bus_mux5;
	sc_signal< sc_bv<DWORD> >		bus_mux6;	
	
	sc_signal< sc_bv<6> >				bus_decoder_instr_31_26;
	sc_signal< sc_bv<AWORDREG> >		bus_decoder_instr_25_21;
	sc_signal< sc_bv<AWORDREG> >		bus_decoder_instr_20_16;
	sc_signal< sc_bv<AWORDREG> >		bus_decoder_instr_15_11;
	sc_signal< sc_bv<SIGNEXTENDBIT> >	bus_decoder_instr_15_0;
	sc_signal< sc_bv<6> >				bus_decoder_instr_5_0;
	sc_signal< sc_bv<26> >				bus_decoder_instr_25_0;

	sc_signal< sc_bv<DWORD> >		bus_pc;

	sc_signal< sc_bv<DWORD> >		bus_add1;
	sc_signal< sc_bv<DWORD> >		bus_add2;

	sc_signal< sc_bv<DWORD> >		bus_shiftleft;
		
	sc_signal< sc_bv<DWORD> >		bus_signextend;

	sc_signal< sc_bv<DWORD> >		bus_imem_1;

	sc_signal< sc_bv<DWORD> >		bus_dmem_1;

	sc_signal< sc_bv<DWORD> > 		bus_fpu_result;
	sc_signal< sc_bv<DWORD> >		bus_alu_result;
	sc_signal< sc_bv<1> >			bus_alu_zero;

	sc_signal< sc_bv<DWORD> >		bus_registers_1;
	sc_signal< sc_bv<DWORD> >		bus_registers_2;
	
	sc_signal< sc_bv<28> >			bus_jump_shift;
	sc_signal< sc_bv<DWORD> >		bus_jump_addr;

	// Control signals
	sc_signal< sc_bv<1> >		bus_ctrl_regdst;
	sc_signal< sc_bv<1> >		bus_ctrl_branch;
	sc_signal< sc_bv<1> >		bus_ctrl_memread;
	sc_signal< sc_bv<1> >		bus_ctrl_memtoreg;
	sc_signal< sc_bv<2> >		bus_ctrl_aluop;
	sc_signal< sc_bv<1> >		bus_ctrl_memwrite;
	sc_signal< sc_bv<1> >		bus_ctrl_alusrc;
	sc_signal< sc_bv<1> >		bus_ctrl_regwrite;
	sc_signal< sc_bv<DWORD> >	bus_ctrl_c4;
	sc_signal< sc_bv<1> >		bus_ctrl_jump;
	sc_signal< sc_bv<1> >		bus_ctrl_int_or_float;	//ALU Result or FPU Result

	sc_signal< sc_bv<3> >		bus_aluctrl;

	sc_signal< sc_bv<1> >		bus_and1;



	// MODULES

	REGISTER	pc("pc");
                            
	ADD		add1("add1");
	ADD		add2("add2");

	AND		and1("and1");

	ROM		imem("instruction_memory");	// Instruction memory
	RAM		dmem("data_memory");		// Data memory

	REGFILE		registers("registers");		// Registerfile

	ALU		alu("alu");
	ALUCTRL		aluctrl("aluctrl");

	SIGNEXTEND	signextend("signextend");

	SHIFTLEFT	shiftleft1("shiftleft1");
	
	SHIFTLEFT2	shiftleft2("shiftleft2");

	CTRL		ctrl("ctrl");

	DECODER		decoder("decoder");

	MUX		mux1("mux1");
	MUX		mux2("mux2");
	MUX		mux3("mux3");
	MUX2_AWORDREG	mux4("mux4");
	MUX		mux5("mux5");
	MUX 	mux6("mux6");			//ALU Result or FPU Result

	MASK 	mask("mask");				// Appends Bits Of PC+4 and JUMP Address Bits
	
	sc_clock	clk("clock", 20);		// Clock

	// CONNECTIONS

	// Program counter
	pc.in(bus_mux5);
	pc.out(bus_pc);
	pc.w(clk);
	pc.clk(clk);

	// Add 1 (PC + 4)
	add1.a(bus_pc);
	add1.b(bus_ctrl_c4);
	add1.r(bus_add1);

	// Add 2 (add1 + shiftleft)
	add2.a(bus_add1);
	add2.b(bus_shiftleft);
	add2.r(bus_add2);

	// Mux 1 (add1 or add2)
	mux1.in0(bus_add1);
	mux1.in1(bus_add2);
	mux1.sel(bus_and1);
	mux1.out(bus_mux1);

	// Shift left 2 
	shiftleft1.in(bus_signextend);
	shiftleft1.out(bus_shiftleft);

	// Sign extend
	signextend.in(bus_decoder_instr_15_0);
	signextend.out(bus_signextend);

	// Decoder (Select correct part of instruction for registerfile)
	decoder.instr(bus_imem_1);
	decoder.instr_31_26(bus_decoder_instr_31_26);
	decoder.instr_25_21(bus_decoder_instr_25_21);
	decoder.instr_20_16(bus_decoder_instr_20_16);
	decoder.instr_15_11(bus_decoder_instr_15_11);
	decoder.instr_15_0(bus_decoder_instr_15_0);
	decoder.instr_5_0(bus_decoder_instr_5_0);
	decoder.instr_25_0(bus_decoder_instr_25_0);
	
	// Shift left 2 ------ 2
	shiftleft2.in(bus_decoder_instr_25_0);
	shiftleft2.out(bus_jump_shift);
	
	// Combine PC and Jump Address [27-0]
	mask.in_1(bus_jump_shift);
	mask.in_2(bus_add1);
	mask.out(bus_jump_addr);
	
	//Mux 5 (Select Jump Address or Mux1)
	mux5.in1(bus_jump_addr);
	mux5.in0(bus_mux1);
	mux5.sel(bus_ctrl_jump);
	mux5.out(bus_mux5);

	// Mux 4 (Select address for write to registerfile)
	mux4.in0(bus_decoder_instr_20_16);
	mux4.in1(bus_decoder_instr_15_11);
	mux4.sel(bus_ctrl_regdst);
	mux4.out(bus_mux4);

	// ALU
	alu.a(bus_registers_1);
	alu.b(bus_mux2);
	alu.r(bus_alu_result);
	alu.z(bus_alu_zero);
	alu.ctrl(bus_aluctrl);

	


	// MUX 6 (ALU Result or FPU Result)
	mux6.in0(bus_alu_result);
	mux6.in1(bus_fpu_result);
	mux6.sel(bus_ctrl_int_or_float);
	mux6.out(bus_mux6);	

	
	// Mux 2 (Registerfile or signextend)
	mux2.in0(bus_registers_2);
	mux2.in1(bus_signextend);
	mux2.sel(bus_ctrl_alusrc);
	mux2.out(bus_mux2);

	// ALU ctrl
	aluctrl.ALUop(bus_ctrl_aluop);
	aluctrl.functionCode(bus_decoder_instr_5_0);
	aluctrl.ALUctrl(bus_aluctrl);

	// Mux 3 (ALU result or memory result to register)
	mux3.in0(bus_mux6);
	mux3.in1(bus_dmem_1);
	mux3.sel(bus_ctrl_memtoreg);
	mux3.out(bus_mux3);

	// AND
	and1.a(bus_alu_zero);
	and1.b(bus_ctrl_branch);
	and1.r(bus_and1);

	// Registerfile
	registers.r_addr_reg1(bus_decoder_instr_25_21);
	registers.r_addr_reg2(bus_decoder_instr_20_16);
	registers.w_addr_reg(bus_mux4);
	registers.r_data_reg1(bus_registers_1);
	registers.r_data_reg2(bus_registers_2);
	registers.w_data_reg(bus_mux3);
	registers.w(bus_ctrl_regwrite);
	registers.clk(clk);

	// Data memory
	dmem.a_read(bus_mux6);
	dmem.d_read(bus_dmem_1);
	dmem.r(bus_ctrl_memread);
	dmem.a_write(bus_mux6);
	dmem.d_write(bus_registers_2);
	dmem.w(bus_ctrl_memwrite);
	dmem.clk(clk);

	// Instruction Memory 
	imem.a_read(bus_pc);
	imem.d_read(bus_imem_1);
	imem.clk(clk);

	// Controller
	ctrl.Opcode(bus_decoder_instr_31_26);
	ctrl.RegDst(bus_ctrl_regdst);
	ctrl.Branch(bus_ctrl_branch);
	ctrl.MemRead(bus_ctrl_memread);
	ctrl.MemtoReg(bus_ctrl_memtoreg);
	ctrl.ALUop(bus_ctrl_aluop);
	ctrl.MemWrite(bus_ctrl_memwrite);
	ctrl.ALUSrc(bus_ctrl_alusrc);
	ctrl.RegWrite(bus_ctrl_regwrite);
	ctrl.c4(bus_ctrl_c4);
	ctrl.Jump(bus_ctrl_jump);
	ctrl.int_or_float(bus_ctrl_int_or_float);
	
	/*FLOATING POINT UNIT SECTION*/

	// SIGNALS

	sc_signal< sc_bv<EXPONENT> > bus_op1_exp;
	sc_signal< sc_bv<EXPONENT> > bus_op2_exp;
	sc_signal< sc_bv<FRACTION> > bus_op1_frac;
	sc_signal< sc_bv<FRACTION> > bus_op2_frac;
	sc_signal< sc_bv<1> >		 bus_op1_sign;
	sc_signal< sc_bv<1> > 		 bus_op2_sign;

	sc_signal< sc_bv<EXPONENT> > bus_exp_diff;				//differnce of exponents

	sc_signal< sc_bv<EXPONENT> > bus_fpu_mux1;				//mux for large exponent
	sc_signal< sc_bv<FRACTION> > bus_fpu_mux2; 				//mux for small significand
	sc_signal< sc_bv<FRACTION> > bus_fpu_mux3;				//mux for large significand
	sc_signal< sc_bv<EXPONENT> > bus_fpu_mux4;				//mux fot increment or decrement
	sc_signal< sc_bv<FRACTION+2> > bus_fpu_mux5;			//mux for shift left or right




	sc_signal< sc_bv<FRACTION+2> > bus_frac_shift_right;		//bus of output from shift right

	sc_signal< sc_bv<FRACTION+2> > bus_big_alu;				//bus for big alu output

	sc_signal< sc_bv<FRACTION+2> > bus_shift_left_or_right;	//bus of output from shift left or right
	sc_signal< sc_bv<EXPONENT> > bus_inc_dec;				//bus of outpur if inc or dec

	sc_signal< sc_bv<FRACTION+2> > bus_round_frac;			//bus of round fraction output
	sc_signal< sc_bv<EXPONENT> > bus_round_exp;				//bus of round expoenent output

	//CONTROL SIGNALS

	sc_signal< sc_bv<1> > bus_ctrl_mux_large_exp;				//ctrl for mux1
	sc_signal< sc_bv<1> > bus_ctrl_mux_small_sig;				//ctrl for mux2
	sc_signal< sc_bv<1> > bus_ctrl_mux_large_sig;				//ctrl for mux3
	sc_signal< sc_bv<1> > bus_ctrl_mux_inc_dec;					//ctrl for inc or dec mux4
	sc_signal< sc_bv<1> > bus_ctrl_mux_shift_left_right;		//ctrl for shift left or right mux5

	sc_signal< sc_bv<1> > bus_ctrl_inc_dec;				//ctrl for inc or dec
	sc_signal< sc_bv<EXPONENT> > bus_ctrl_shift_right;			//ctr for shiifting right
	sc_signal< sc_bv<1> > 		 bus_ctrl_shift_left_right;		//ctrl for shift left or right
	sc_signal< sc_bv<1> > 		 bus_ctrl_round;				//ctrl for rounding hardware


	//HARWARE MODULES

	DECODER_FPU 			decoder_fpu("decoder_fpu");
	SMALL_ALU  				small_alu("small_alu");
	SHIFTRIGHT_FRAC			shift_right_frac("shift_right");
	SHIFT_LEFT_OR_RIGHT 	shift_left_or_right("shift_left_or_right");
	BIG_ALU 				big_alu("big_alu");
	IncorDec 				inc_or_dec("inc_or_dec");
	RoundHW 				round_hw("round_hw");
	FPU_CTRL 				fpu_ctrl("fpu_ctrl");
	COMBINE 				combine("combine");
	
	MUX_EXP 				fpu_mux1("fpu_mux1");
	MUX_EXP 				fpu_mux4("fpu_mux4");
	MUX_FRAC 				fpu_mux2("fpu_mux2");
	MUX_FRAC 				fpu_mux3("fpu_mux3");
	MUX_ALU_NORMAL 			fpu_mux5("fpu_mux5");

	//CONNECTIONS
	
	decoder_fpu.op_1(bus_registers_1);
	decoder_fpu.op_2(bus_mux2);
	decoder_fpu.op_1_sign(bus_op1_sign);
	decoder_fpu.op_2_sign(bus_op2_sign);
	decoder_fpu.op_1_frac(bus_op1_frac);
	decoder_fpu.op_2_frac(bus_op2_frac);
	decoder_fpu.op_1_exp(bus_op1_exp);
	decoder_fpu.op_2_exp(bus_op2_exp);
	
	small_alu.a(bus_op1_exp);
	small_alu.b(bus_op2_exp);
	small_alu.diff(bus_exp_diff);

	shift_right_frac.in(bus_fpu_mux2);
	shift_right_frac.count(bus_exp_diff);
	shift_right_frac.out(bus_frac_shift_right);

	shift_left_or_right.a(bus_fpu_mux5);
	shift_left_or_right.ctrl(bus_ctrl_shift_left_right);
	shift_left_or_right.out(bus_shift_left_or_right);

	big_alu.a(bus_frac_shift_right);
	big_alu.b(bus_fpu_mux3);
	big_alu.r(bus_big_alu);

	inc_or_dec.a(bus_fpu_mux4);
	inc_or_dec.IncDec(bus_ctrl_inc_dec);
	inc_or_dec.out(bus_inc_dec);

	round_hw.a(bus_inc_dec);
	round_hw.b(bus_shift_left_or_right);
	round_hw.ctrl(bus_ctrl_round);
	round_hw.a_out(bus_round_exp);
	round_hw.b_out(bus_round_frac);

	combine.a(bus_round_exp);
	combine.b(bus_round_frac);
	combine.c(bus_fpu_result);

	fpu_mux1.in0(bus_op1_exp);
	fpu_mux1.in1(bus_op2_exp);
	fpu_mux1.sel(bus_ctrl_mux_large_exp);
	fpu_mux1.out(bus_fpu_mux1);

	fpu_mux2.in0(bus_op1_frac);
	fpu_mux2.in1(bus_op2_frac);
	fpu_mux2.sel(bus_ctrl_mux_small_sig);
	fpu_mux2.out(bus_fpu_mux2);

	fpu_mux3.in0(bus_op1_frac);
	fpu_mux3.in1(bus_op2_frac);
	fpu_mux3.sel(bus_ctrl_mux_large_sig);
	fpu_mux3.out(bus_fpu_mux3);

	fpu_mux4.in0(bus_fpu_mux1);
	fpu_mux4.in1(bus_round_exp);
	fpu_mux4.sel(bus_ctrl_mux_inc_dec);
	fpu_mux4.out(bus_fpu_mux4);

	fpu_mux5.in0(bus_big_alu);
	fpu_mux5.in1(bus_round_frac);
	fpu_mux5.sel(bus_ctrl_mux_shift_left_right);
	fpu_mux5.out(bus_fpu_mux5);	

			
	fpu_ctrl.ExpDiff(bus_exp_diff);							// Exponent Difference
	fpu_ctrl.BigAluOut(bus_big_alu);						// Big ALU Result
	fpu_ctrl.RoundFraction(bus_round_frac);					// Fraction Round Off
	fpu_ctrl.LargeExp(bus_ctrl_mux_large_exp);				// Exponent MUX
	fpu_ctrl.SmallSD(bus_ctrl_mux_small_sig);				// Smaller Significand
	fpu_ctrl.LargeSD(bus_ctrl_mux_large_sig);				// Large Significand
	fpu_ctrl.ShiftR(bus_ctrl_shift_right);					// Shift Right
	fpu_ctrl.ShiftLR(bus_ctrl_shift_left_right);		// Shift Left OR Right
	fpu_ctrl.IncDec(bus_ctrl_inc_dec);						// Increment or Decrement
	fpu_ctrl.RoundHW(bus_ctrl_round);						// Rounding Hawrdware
	fpu_ctrl.alu_or_normal(bus_ctrl_mux_shift_left_right);	//	select alu or normalized
	fpu_ctrl.alu_or_normal_expo(bus_ctrl_mux_inc_dec);		//	select expo normalized or not


	// INITIALIZATION

	imem.rom_init("mips.rom"); 
	dmem.ram_init("mips.ram");

	// TRACING

	sc_trace_file* tf;
	tf = sc_create_vcd_trace_file("mips");

	// Signals
	sc_trace(tf, clk,						"clock");
	sc_trace(tf, bus_mux1,					"bus_mux1");
	sc_trace(tf, bus_mux2,					"bus_mux2");
	sc_trace(tf, bus_mux3,					"bus_mux3");
	sc_trace(tf, bus_mux4,					"bus_mux4");
	sc_trace(tf, bus_mux5,					"bus_mux5");
	sc_trace(tf, bus_mux6,					"bus_mux6");
	sc_trace(tf, bus_pc,					"bus_pc");
	sc_trace(tf, bus_add1,					"bus_add1");
	sc_trace(tf, bus_add2,					"bus_add2");
	sc_trace(tf, bus_shiftleft,				"bus_shiftleft1");
	sc_trace(tf, bus_signextend,			"bus_signextend");
	sc_trace(tf, bus_imem_1,				"bus_imem_1");
	sc_trace(tf, bus_dmem_1,				"bus_dmem_1");
	sc_trace(tf, bus_alu_result,			"bus_alu_result");
	sc_trace(tf, bus_alu_zero,				"bus_alu_zero");
	sc_trace(tf, bus_registers_1,			"bus_registers_1");
	sc_trace(tf, bus_registers_2,			"bus_registers_2");
	sc_trace(tf, bus_jump_shift,			"bus_jump_shift");
	sc_trace(tf, bus_jump_addr,				"bus_jump_addr");
	sc_trace(tf, bus_ctrl_regdst,			"bus_ctrl_regdst");
	sc_trace(tf, bus_ctrl_branch,			"bus_ctrl_branch");
	sc_trace(tf, bus_ctrl_memread,			"bus_ctrl_memread");
	sc_trace(tf, bus_ctrl_memtoreg,			"bus_ctrl_memtoreg");
	sc_trace(tf, bus_ctrl_aluop,			"bus_ctrl_aluop");
	sc_trace(tf, bus_ctrl_memwrite,			"bus_ctrl_memwrite");
	sc_trace(tf, bus_ctrl_alusrc,			"bus_ctrl_alusrc");
	sc_trace(tf, bus_ctrl_regwrite,			"bus_ctrl_regwrite");
	sc_trace(tf, bus_ctrl_c4,				"bus_ctrl_c4");
	sc_trace(tf, bus_ctrl_jump,				"bus_ctrl_jump");
	sc_trace(tf, bus_aluctrl,				"bus_aluctrl");
	sc_trace(tf, bus_and1,					"bus_and1");
	sc_trace(tf, bus_decoder_instr_31_26,	"bus_decoder_instr_31_26");
	sc_trace(tf, bus_decoder_instr_25_21,	"bus_decoder_instr_25_21");
	sc_trace(tf, bus_decoder_instr_20_16,	"bus_decoder_instr_20_16");
	sc_trace(tf, bus_decoder_instr_15_11,	"bus_decoder_instr_15_11");
	sc_trace(tf, bus_decoder_instr_15_0,	"bus_decoder_instr_15_0");
	sc_trace(tf, bus_decoder_instr_5_0,		"bus_decoder_instr_5_0");
	sc_trace(tf, bus_decoder_instr_25_0,	"bus_decoder_instr_25_0");
	sc_trace(tf, bus_ctrl_int_or_float,		"bus_ctrl_int_or_float");
	
	sc_trace(tf, bus_fpu_result,			"bus_fpu_result");
	sc_trace(tf, bus_exp_diff,				"bus_exp_diff");
	sc_trace(tf, bus_big_alu,				"bus_big_alu");
	sc_trace(tf, bus_round_frac,			"bus_round_frac");	
	sc_trace(tf, bus_ctrl_mux_large_exp,	"bus_ctrl_mux_large_exp");	
	sc_trace(tf, bus_ctrl_mux_small_sig,		"bus_ctrl_mux_small_sig");	
	sc_trace(tf, bus_ctrl_mux_large_sig,		"bus_ctrl_mux_large_sig");
	sc_trace(tf, bus_ctrl_shift_right,		"bus_ctrl_shift_right");
	sc_trace(tf, bus_ctrl_shift_left_right,		"bus_ctrl_shift_left_right");
	sc_trace(tf, bus_ctrl_inc_dec,		"bus_ctrl_inc_dec");
	sc_trace(tf, bus_ctrl_round,		"bus_ctrl_round");
	sc_trace(tf, bus_ctrl_mux_shift_left_right,		"bus_ctrl_mux_shift_left_right");
	sc_trace(tf, bus_ctrl_mux_inc_dec,		"bus_ctrl_mux_inc_dec");
	sc_trace(tf, bus_frac_shift_right, "bus_frac_shift_right");
	sc_trace(tf, bus_shift_left_or_right, "bus_shift_left_or_right");



	sc_trace(tf, bus_fpu_mux1, "bus_fpu_mux1");
	sc_trace(tf, bus_fpu_mux2, "bus_fpu_mux2");
	sc_trace(tf, bus_fpu_mux3, "bus_fpu_mux3");
	sc_trace(tf, bus_fpu_mux4, "bus_fpu_mux4");
	sc_trace(tf, bus_fpu_mux5, "bus_fpu_mux5");

	sc_trace(tf, bus_op1_exp, "bus_op1_exp");
	sc_trace(tf, bus_op2_exp, "bus_op2_exp");
	sc_trace(tf, bus_op1_frac, "bus_op1_frac");
	sc_trace(tf, bus_op2_frac, "bus_op2_frac");

	sc_trace(tf, bus_round_exp, "bus_round_exp");
	sc_trace(tf, bus_inc_dec, "bus_inc_dec");

	for (int i = 0; i < REGSIZE; i++) {
		sprintf(stbuf, "registers.reg(%d)", i);
		sc_trace(tf, registers.rfile[i], stbuf);
	}

	for (int i = 0; i < RAMSIZE; i++) {
		sprintf(stbuf, "memory.dmem(%d)", i);
		sc_trace(tf, dmem.ramfile[i], stbuf);
	}

	for (int i = 0; i < ROMSIZE; i++) {
		sprintf(stbuf, "memory.imem(%d)", i);
		sc_trace(tf, imem.romfile[i], stbuf);
	}

	// SIMULATION

	int sim_time = 500;
	if (argc == 2)
		sim_time = atoi(argv[1]);

	sc_start(sim_time, SC_NS);

	sc_close_vcd_trace_file(tf);
	dmem.ram_dump("mips_ram.dump");

	return 0;
}
