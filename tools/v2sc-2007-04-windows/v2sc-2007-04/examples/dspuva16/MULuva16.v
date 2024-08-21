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
// MODULE:	MULuva16.v
//
// DESCRIPTION:	Multiplies in four stages two 16-bit values using fixed point.
//		The 16-bit inputs must be valid during the forth subcycle (ph3).
//		One cycle (four subcycles) latency is introduced for operation.
//		The 32-bit output is valid only during the second subcycle (ph1).
//		It can multiply two new values each cycle (100ns at 40 MHz).
//
// REVISION:	1.00	20010416a	First stable version
//		0.08	20010405a	Ok, with '<=' operator!
//		0.06	20010326a	Ok
//
// SIMULATION:	0x53A2 (+0'6533) * 0x6B1f (+0'8368) = 0x22FED69E => 0x45FDAD (+0'5468) ok
// 		0xDB1f (-0'2881) * 0x53A2 (+0'6533) = 0xF3F3B69E => 0xE7E76D (-0,1882) ok
// 		0x53A2 (+0'6533) * 0xDB1f (-0'2881) = 0xF3F3B69E => 0xE7E76D (-0,1882) ok
// 		0xDB1f (-0'2881) * 0xB3A2 (-0,5966) = 0x0B00569E => 0x1600AD (+0,1719) ok
//
// TO DO LIST:	Full Test Bench
//
// BUGS:	Please, report bugs to "dteopenp@eis.uva.es"
//		with reference "OpenDSP MULuva16 v1.00".
//


module MULuva16 (Clk, InA, InB, Phase, AxB);

	parameter ph0 = 2'b00, ph1 = 2'b01, ph2 = 2'b11, ph3 = 2'b10;

	input		Clk;	// Clock (40 MHz => 100 ns/MAC)
	input   [3:0]	InA;	// First operand input (four rotating bits of rS, or 0)
	input  [15:0]	InB;	// Second operand input (rT or K)
	input   [1:0]	Phase;	// Subcycle Identification
	output [31:0]	AxB;	// Valid only on second subcycle (ph1)

	wire   [17:0]	Op1, Op2, Op3;	// First adder
	wire   [18:0]	Op4, Op5, Op6;	// Second adder
	reg    [19:0]	Op7;		// Third adder (registered)
	wire   [31:0]	Op8;		// To acumulate or not
	reg    [31:0]	AxB;		// Final result (valid only during ph1)

	assign Op1 = (InA[0] ? {InB[15], InB[15], InB} : 18'h00000);
	assign Op2 = (InA[1] ? {InB[15], InB, 1'b0   } : 18'h00000);
	assign Op3 = Op1 + Op2;

	assign Op4 = (InA[2] ? {InB[15], InB[15], InB, 1'b1} : 19'h00001);
	assign Op5 = (Phase != ph3)
		   ? (InA[3] ? { InB[15],  InB, 2'b00} : 19'h00000)
		   : (InA[3] ? {~InB[15], ~InB, 2'b11} : 19'h00000);	
	assign Op6 = Op4 + Op5;

	always @(posedge Clk)
	begin
		Op7 <= {Op3[17], Op3[17], Op3} + {Op6[18:1], 2'b00};
	end

	assign Op8 = (Phase == ph1) ? 32'h00000000			// Null on new op
		   : {AxB[31], AxB[31], AxB[31], AxB[31], AxB[31:4]};	// Acumulates and shifts

	always @(posedge Clk)
	begin
		AxB <= Op8 + {Op7, 12'h000};
	end

endmodule	// MULuva16

module test_mul;

	reg	  Clk;	          // Clock (40 MHz => 100 ns/MAC)
	reg   [3:0]	InA;	  // First operand input (four rotating bits of rS, or 0)
	reg   [15:0]	InB;  // Second operand input (rT or K)
	reg   [1:0]	Phase;	  // Subcycle Identification
	
	wire  [31:0]	AxB;  // Valid only on second subcycle (ph1)

	MULuva16 dut(Clk, InA, InB, Phase, AxB);	
	
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
        InA = 0;
        InB = 0;
        #400;

		// -- 0x53A2 (+0'6533) * 0x6B1F (+0'8368) = 0x22FED69E => 0x45FDAD (+0'5468) ok
		#15;
		InB = 16'h6B1F;
		InA = 16'h2;
		#100;
		InA = 16'hA;
		#100;
		InA = 16'h3;
		#100;
		InA = 16'h5;
		#85;

		// -- 0xDB1F (-0'2881) * 0x53A2 (+0'6533) = 0xF3F3B69E => 0xE7E76D (-0,1882) ok
		#15;
		InB = 16'h53A2;
		InA = 16'hF;
		#100;
		InA = 16'h1;
		#100;
		InA = 16'hB;
		#100;
		InA = 16'hD;
		#85;

		// -- 0x53A2 (+0'6533) * 0xDB1F (-0'2881) = 0xF3F3B69E => 0xE7E76D (-0,1882) ok
		#15;
		InB = 16'hDB1F;
		InA = 16'h2;
		#100;
		InA = 16'hA;
		#100;
		InA = 16'h3;
		#100;
		InA = 16'h5;
		#85;

		// -- 0xDB1F (-0'2881) * 0xB3A2 (-0,5966) = 0x0B00569E => 0x1600AD (+0,1719) ok
		#15;
		InB = 16'hB3A2;
		InA = 16'hF;
		#100;
		InA = 16'h1;
		#100;
		InA = 16'hB;
		#100;
		InA = 16'hD;
		#85;
		
		#400;
		$stop;
	end

endmodule
