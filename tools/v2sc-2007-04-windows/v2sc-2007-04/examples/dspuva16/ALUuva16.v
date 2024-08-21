//
// PROJECT:	OpenDSP - The 'DSPuva16' 16-bit fixed-point DSP for FPGA
//		http://www.DTE.eis.uva.es/OpenProjects/OpenDSP/index.htm
//
// RIGHTS:	Santiago de Pablo
//		Copyright (c) 2001. All Rights Reserved.
//
// GPL:		You may freely copy, change, and distribute it,
//		but you may not impose restrictions on further distribution,
//		and you must make the source code available.
//
//		This code is supplied "as is", without any warranty.
//		Please, tell us how many devices have you implemented.
//
// AUTHOR:	Santiago de Pablo (sanpab@eis.uva.es)
//		Department of Electronics Technology (DTE)
//		University of Valladolid (Spain)
//
// MODULE:	ALUuva16.v
//
// DESCRIPTION:	Computation Core for the DSPuva16.
//		The 24-bit ALU has eight operations (+, -, AND, OR, ...).
//		The multiplier-acumulator operates in one cycle (four subcycles).
//
// REVISION:	1.00	20010416a	First stable version
//		0.06	20010319a	Ok
//
// TO DO LIST:	Full Test Bench
//
// BUGS:	Please, report bugs to "dteopenp@eis.uva.es"
//		with reference "OpenDSP ALUuva16 v1.00".
//


module ALUuva16 (Clk, Phase, InA, InB, InC, OpCode, FlagIn, Vflag, OutMAC, OutLogic, OutArith);

	parameter ph0 = 2'b00, ph1 = 2'b01, ph2 = 2'b11, ph3 = 2'b10;

	input		Clk;		// DSP Clock (40 MHz)
	input   [1:0]	Phase;		// Subcycle Identification
	input  [23:0]	InA;		// First operand  (RegS or 0)
	input  [23:0]	InB;		// Second operand (RegT or K)
	input  [23:0]	InC;		// Third operand (RegD for MAC)
	input   [3:0]	OpCode;		// Instruction Operation Code (2:0?)
	input 		FlagIn;		// Input of the active flag
	output		Vflag;		// Output for the Overflow flag
	output [23:0]	OutMAC;		// Output from the Multiplier-Accumulator
	output [23:0]	OutLogic;	// Logic ALU output
	output [23:0]	OutArith;	// Arithmetic ALU output


	//------------------------------------------------------//
	// Input registers: receive two 24-bit operands S and T //
	//------------------------------------------------------//

	reg  [23:0]	OpA, OpB;

	always @(posedge Clk)
	begin
		OpA <= (Phase == ph3) ? InA : {OpA[3:0],OpA[23:4]};	// Loads S and shifts it
		OpB <= (Phase == ph3) ? InB : OpB;			// Loads T and keeps it
	end


	//----------------------------------------------------------------------//
	// Multiplier and Accumulator (16-bit fixed point, extended to 24 bits) //
	//----------------------------------------------------------------------//

	reg   [1:0]	OldCode;
	wire [31:0]	OutMULT;
	wire		x;

	always @(posedge Clk)
	begin
		OldCode <= (Phase == ph1) ? OpCode[1:0] : OldCode;	// Keeps old OpCode
	end

	MULuva16 Multiplier16x16
	(
		.Clk	(Clk),
		.InA	(OpA[11:8]),
		.InB	(OpB[23:8]),
		.Phase	(Phase),
		.AxB	(OutMULT)
	);

	wire [23:0] AccA = (OldCode[1]) ? InC : 24'h000000;		// Acumulates for 011x, but not for 010x

	wire [23:0] AccB = (OldCode[1:0] == 2'b01) ? OutMULT[23:0]	// Multiply in 8,8 format               for 0101
			 : (~OldCode[0])           ? OutMULT[30:7]	// or in 1,15 format with addition      for 01x0
			 :                          ~OutMULT[30:7];	// or in 1,15 formtat with substraction for 0111
        
	/*
	wire [23:0] AccB =   (OldCode[1:0] == 2'b01) ? OutMULT[23:0]	// Multiply in 8,8 format               for 0101
			  :  (~OldCode[0])           ? OutMULT[30:7]	// or in 1,15 format with addition      for 01x0
			  :   ~OutMULT[30:7]         ? OutMULT[30:7] 	// or in 1,15 formtat with substraction for 0111
			  :                            OutMULT[30:7]; 
			  */
	assign {OutMAC,x} = {AccA, 1'b1} + {AccB, OldCode[0]};		// The MAC does not modify any flag


	//-----------//
	// Logic ALU //
	//-----------//

	`define	OP_AND	2'b00
	`define	OP_OR 	2'b01
	`define	OP_NOR	2'b10
	`define	OP_XOR	2'b11

	reg  [23:0]	OutLogic;

	always @(OpA or OpB or OpCode)
	begin
		case (OpCode[1:0])	// synopsys parallel_case full_case
			`OP_AND:	OutLogic =   OpA & OpB;
			`OP_OR: 	OutLogic =   OpA | OpB;
			`OP_NOR:	OutLogic = ~(OpA | OpB);
			`OP_XOR:	OutLogic =   OpA ^ OpB;
		endcase
	end


	//----------------//
	// Arithmetic ALU //
	//----------------//

	wire [23:0]	AddA, AddB;
	wire		Carry;

	assign AddA = ((OpCode[1] | ~FlagIn) ? OpA : 24'h000000);
	assign AddB = ((OpCode[1] |  FlagIn) ? OpB : 24'h000000);

	assign {Carry, OutArith} = (OpCode[0]) ? (AddA - AddB) : (AddA + AddB);

	assign Vflag = (Carry ^ OutArith[23] ^ AddA[23] ^ AddB[23]);	// Overflow Flag
									// Thanks Jan Gray

endmodule	// ALUuva16


module test_alu;

	reg		Clk;		    // DSP Clock (40 MHz)
	reg   [1:0]	Phase;		// Subcycle Identification
	reg  [23:0]	InA;		// First operand  (RegS or 0)
	reg  [23:0]	InB;		// Second operand (RegT or K)
	reg  [23:0]	InC;		// Third operand (RegD for MAC)
	reg   [3:0]	OpCode;		// Instruction Operation Code (2:0?)
	reg		FlagIn;		    // Input of the active flag
	
	wire		Vflag;		// Output for the Overflow flag
	wire [23:0]	OutMAC;		// Output from the Multiplier-Accumulator
	wire [23:0]	OutLogic;	// Logic ALU output
	wire [23:0]	OutArith;	// Arithmetic ALU output	

	ALUuva16 dut(Clk, Phase, InA, InB, InC, OpCode, FlagIn, Vflag, OutMAC, OutLogic, OutArith);
	
	initial
	begin
		Clk = 1;
		forever 
		begin
			Clk = 1;
			#50 Clk = 0;
			#50;
		end
	end

	initial
	begin
		Phase = 0;
		forever
		begin
			#15  Phase = 0;
			#100 Phase = 1;
			#100 Phase = 3;
			#100 Phase = 2;
			#85;
		end
	end
	
	
	initial
	begin
		FlagIn = 0;
		OpCode = 4'hF;
		InA	= 24'hFFFF00;
		InB	= 24'hFFFF00;
		#300;
		
		// A series Test
		
		// Tests "rS and rT" 
		#15;
		OpCode = 4'hF;
		InA	= 24'h00FF00;
		InB	= 24'h3A3A00;
		#385;
		#15;
		InA	= 24'hA3A300;
		InB	= 24'hFF0000;
		#385;

		// Tests "rS or rT" 
		#15;
		OpCode = 4'hD;
		InA	= 24'h00FF00;
		InB	= 24'h3A3A00;
		#385;
		#15;
		InA	= 24'hA3A300;
		InB	= 24'hFF0000;
		#385;
		
		// Tests "rS nor rT" 
		#15;
		OpCode = 4'hE;
		InA	= 24'h00FF00;
		InB	= 24'h3A3A00;
		#385;
		#15;
		InA	= 24'hA3A300;
		InB	= 24'hFF0000;
		#385;

		// Tests "rS xor rT" 
		#15;
		OpCode = 4'hF;
		InA	= 24'h00FF00;
		InB	= 24'h3A3A00;
		#385;
		#15;
		InA	= 24'hA3A300;
		InB	= 24'hFF0000;
		#385;
		
		// B series Test
		
		// -- Tests "if (flag) rT else rD" --
		#15;
		OpCode = 4'h8;
		InA	= 24'h53A200;
		InB	= 24'h6B1F00;
		FlagIn = 1;
		#385;
		#15;
		FlagIn = 0;
		#385;

		// -- Tests "if (flag) -rT else rD" --
		#15;
		OpCode = 4'h9;
		InA	= 24'h6B1F00;
		InB	= 24'h53A200;
		FlagIn = 1;
		#385;
		#15;
		FlagIn = 0;
		#385;

		// -- Tests "rS + rT" --
		#15;
		OpCode = 4'hA;
		InA	= 24'h53A200;
		InB	= 24'h6B1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'h53A200;
		#385;
		#15;
		InA	= 24'h53A200;
		InB	= 24'hDB1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'hB3A200;
		#385;
		
		// -- Tests "rS - rT" --
		#15;
		OpCode = 4'hB;
		InA	= 24'h53A200;
		InB	= 24'h6B1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'h53A200;
		#385;
		#15;
		InA	= 24'h53A200;
		InB	= 24'hDB1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'hB3A200;
		#385;
		
		// C series Test
		
		// -- Tests "rS * rT" --
		#15;
		OpCode = 4'h4;
		InA	= 24'h53A200;
		InB	= 24'h6B1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'h53A200;
		#385;
		#15;
		InA	= 24'h53A200;
		InB	= 24'hDB1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'hB3A200;
		#385;
		
		// -- Tests "rS x rT" --
		#15;
		OpCode = 4'h5;
		InA	= 24'h00A200;
		InB	= 24'h6B1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'h00A200;
		#385;
		#15;
		InA	= 24'h00A200;
		InB	= 24'hDB1F00;
		#385;
		#15;
		InA	= 24'hDB1F00;
		InB	= 24'hFCA200;
		#385;

		// -- Tests "rD + rS * rT" --
		#15;
		OpCode = 4'h6;
		InA	= 24'h53A200;
		InB	= 24'h6B1F00;
		#600;
		InC	= 24'hDB1F00;
		#100;
		InC	= 24'hFFFF00;
		#885;
		
		
		// -- Tests "rD - rS * rT" --
		#15;
		OpCode = 4'h7;
		InA	= 24'h53A200;
		InB	= 24'h6B1F00;
		#600;
		InC	= 24'hDB1F00;
		#100;
		InC	= 24'hFFFF00;
		#885;
		
		$stop;
		
	end

endmodule