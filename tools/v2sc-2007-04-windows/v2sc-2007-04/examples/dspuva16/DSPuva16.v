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
// MODULE:	DSPuva16.v
//
// DESCRIPTION:	The 'DSPuva16' is a 16-bit fixed-point DSP processor for FPGA.
//		It has 16 internal 24-bit registers, r0 to r15, but it has no more memory.
//		All registers, except r0, can be used in all operations.
//		It can operate with up to three registers (rD = rS op rT) with "uva" architecture.
//		It can execute 16x16 MAC operations (rD = rD +/- rS * rT) in one instruction cycle.
//		The ALU has eight 24-bit operations: logic, arithmetic, conditional assignment.
//		Precission is extended up to 24 bits (from <1,15> to <1,23>) in all operations.
//		The executable code is from 256 words (version 'A') up to 4K words (version 'E').
//		External accesses are made through 128 I/O ports (P0 to P127).
//
//		The "uva" architecture means that 'r0' is replaced depending on where it's used:
//		  - rD = rS op rT  Uses three registers, anyone from 'r1' to r15'.
//		  - rD = rS op K   Uses an immediate constant because 'r0' is used as rT.
//		  - rD =  0 op rT  A zero is included if the operand rS is 'r0'.
//		  - rD =  0 op K   Both, a zero and a constant, can be used simultaneously.
//		  -      rS op rT  A compare operation is done because 'r0' is used as rD.
//
//		The size of the processor core is about 250 slices in Virtex/Spartan2 FPGAs.
//		Up to eight 16-words memory blocks can be added, with only 9 CLB for each one.
//		All components can be integrated in the same FPGA: core, code memory, ports and added memory.
//
// VERSIONS:	'A' has 256x16 code; 'B' has 512x16; 'C' has 1Kx16; 'D' has 2Kx16; 'E' has 4Kx16.
//
// REVISION:	1.02	20010421	Introducing ACC (same behaviour)
//		1.00	20010416	First stable version (HDL simulation only)
//		0.25	20010411	Adding MODELs for 256-4K code.
//		0.23	20010409	Cheching for '<='
//		0.21	20010326	First public version (not simulated yet)
//		0.19	20010318	More code
//		0.17	20010209	More code
//		0.15	20001221	Successful compilation in Verilog
//		0.13	20001219	Initial version in Verilog
//		0.11	20001209	Initial version in VHDL
//
// TO DO LIST:	Beta tests.
//		Better Test Bench.
//		Test with Program Memory.
//		Generate V flag with MAC?
//		16-bit ports or 24-bit ports?
//
// OPCODES:	NOP            rD =      rS x rT   IF flag, R = T    rD = rS and rT   rD = 0
//		BREAK/MODEL    rD =      rS x K    IF flag, R = K    rD = rS and K    rD = K
//		RET (rS)       rD =      rS * rT   IF flag, R = -T   rD = rS or  rT   rD = -K
//		IN  rD,pN      rD =      rS * K    IF flag, R = -K   rD = rS or  K    rD = rT
//		OUT pN,rD      rD = rD + rS * rT   rD = rS + rT      rD = rS nor rT   rD = -rT
//		JP flag,nn     rD = rD + rS * K    rD = rS + K       rD = rS nor K    rD = not rT
//		GOTO nn        rD = rD - rS * rT   rD = rS - rT      rD = rS xor rT   rD = not K
//		CALL (rD) nn   rD = rD - rS * K    rD = rS - K       rD = rS xor K    -
//
// FLAGS:	(eq), (ne), (ov), (nv), (ge), (gt), (le), (lt) and 8 more for interfacing/control.
//
// BUGS:	Please, report bugs to "dteopenp@eis.uva.es" with reference "OpenDSP v1.02".
//


`define	LOW_POWER		// Valid LOW_POWER or LOW_LOGIC


//---------------//
// DSPuva16 Core //
//---------------//

module DSPuva16 (CLK, nRESET, PORTin, PORTout, PORTaddr, IOread, IOwrite, CODEaddr, CODEdata);

	parameter MODEL = 4;	// Model 'A' uses 0, 'B' uses 1, ..., 'E' uses 4.

	input  			CLK;		// 40 MHz Clock (=> 100ns/instruction)
	input  			nRESET;		// Active-low external Reset

	input       [15:0]	PORTin;		// Input Data Port
	output      [15:0]	PORTout;	// Output Data Port
	output       [6:0]	PORTaddr;	// Port Address
	output			IOread;		// Port Read Signal, active high
	output			IOwrite;	// Port Write Signal, active high

	output [MODEL+7:0]	CODEaddr;	// Code Memory Address
	input       [15:0]	CODEdata;	// Code Memory Data (always read)


	// Internal Registers and Buses:

	reg         [11:0]	PC;		// Program Counter (up to 4K code)
	reg         [15:0]	IR;		// Instruction Register
	reg         [23:0]	ACC;		// Accumulator for 'rT'
	wire        [23:0]	DataBus;	// Internal Data Bus
	wire        [23:0]	RegOut;		// Output from Register Bank
	reg	       		Flag;		// Selected Flag
	wire        ALUoverflow;


//----------------------------------------------------------------------------------------------------------------------//
//   State =>	Operations			Program Counter								\\
//----------------------------------------------------------------------------------------------------------------------//
//	00   =>	Read Instruction		PC = PC									\\
//	01   =>	Load IR				PC = PC + 1								//
//	11   =>	ACC = rT			PC = PC									\\
//	10   =>	Read rS - Load RegS and RegT	PC = PC/PC+1/PC+nn/nnn							//
//	00   =>	MAC1 - Writes on rD if ALU	PC = PC									\\
//	01   =>	MAC2 				PC = PC + 1								//
//	11   =>	MAC3				PC = PC									\\
//	10   =>	MAC4				...									//
//	00   =>	Last MAC cycle (for segmentation)									\\
//	01   =>	Reads, accumulates and Writes on rD if MAC								//
//----------------------------------------------------------------------------------------------------------------------\\
//															//
//	 |       Instruction 0 (MAC)      |       Instruction 1 (ALU)      |       Instruction 2 (GOTO)     |		\\
//	 | ___     ___     ___     ___    | ___     ___     ___     ___    | ___     ___     ___     ___    | ___     __//
//  CLK	_|/   \___/   \___/   \___/   \___|/   \___/   \___/   \___/   \___|/   \___/   \___/   \___/   \___|/   \___/	\\
//	 | _______ _______ _______ _______| _______ _______ _______ _______| _______ _______ _______ _______| _______ __//
//State	_|/__st0__/__st1__/__st2__/__st3__|/__st0__/__st1__/__st2__/__st3__|/__st0__/__st1__/__st2__/__st3__|/__st0__/__\\
//	_| _______________ _______________|________________ _______________|________________ _______________| __________//
//   PC	_|/_______n_______\______n+1______|_______n+1______\______n+2______|_______n+2______\______n+3______|/__(new)___\\
//	_| _______________ _______________|________________ _______________|________________ _______________|___________//
//   IR	_|/_______x_______\_______I0______|________I0______\_______I1______|________I1______\_______I2______|___________\\
//	_| _______________________________| _______________________________| _______________________________|___________//
//  S,T	_|/_______________x_______________|\_____________S0,T0_____________|\_____________S1,T1_____________|\__________\\
//	 | _______ _______ _______ _______| _______ _______ _______ _______| _______ _______ _______ _______| _______ __//
//  MAC	_|/___x___/___x___/___x___/___x___|/__m0a__/__m0b__/__m0c__/__m0d__|/__m1a__/__m1b__/__m1c__/__m1d__|/__m2a__/__\\
//	 |________________________________| _______ _______________________| _______ _______________________| _______ __//
//  ALU	_|____x___/___x___/___x___/___x___|/_(OP0)_/___x___/___x___/___x___|/__OP1__/___x___/___x___/___x___|/_(OP2)_/__\\
//	 | _______ _______ _______ _______| _______ _______ _______ _______| _______ _______ _______ _______| _______ __//
// Rdir	_|/___x___/___x___/___T0__/___S0__|/___R0__/___x___/___T1__/___S1__|/___R1__/___R0__/___T2__/___S2__|/___R2__/__\\
//	 |                                |                                | _______ _______                |		//
//  rWE	_|________________________________|________________________________|/  ALU1 \  MAC0 \_______________|___________\\
//	 |                                |                                | _______                        |		//
//  IOr	_|________________________________|________________________________|/ (IN1) \_______________________|___________\\
//	 |                                |                         _______|                                |		//
//  IOw	_|________________________________|________________________/ (OUT1)|\_______________________________|___________\\
//	_|________________________ _______|________________________ _______|________________________ _______|___________//
//Paddr	_|________________________\_______|_______AD0______________\_______|_______AD1______________\_______|_______AD2_\\
//															//
//----------------------------------------------------------------------------------------------------------------------\\


	//----------------//
	// DFF for nRESET //
	//----------------//

	reg ResetFF;	// DFF for nRESET

	always @(posedge CLK or negedge nRESET)		// External nRESET is active 'low'
	begin
		if (nRESET == 1'b0)	ResetFF <= 1'b1;	// Resets all operations
		else			ResetFF <= 1'b0;	// Release the DSP
	end


	//--------------------------------------------//
	// Processor Control Unit: State and Decoding //
	//--------------------------------------------//

	reg [1:0] State;						// Sequencer
	parameter st0 = 2'b00, st1 = 2'b01, st2 = 2'b11, st3 = 2'b10;	// States

	always @(posedge CLK or posedge ResetFF)
	begin
		if (ResetFF)	State	<= st0;
		else case (State)		// synopsys parallel_case full_case
			st0:	State	<= st1;
			st1:	State	<= st2;
			st2:	State	<= st3;
			st3:	State	<= st0;
		endcase
	end

	`define	PHASE0		(State == st0)
	`define	PHASE1		(State == st1)
	`define	PHASE2		(State == st2)
	`define	PHASE3		(State == st3)

	wire [3:0]		OpCode	= IR[15:12];		// Bits used for OpCode
	wire [3:0]		rD	= IR[11: 8];		// Bits used for the destination reg. (rD)
	wire [3:0]		rS	= IR[ 7: 4];		// Bits used for the first operand (rS)
	wire [3:0]		rT	= IR[ 3: 0];		// Bits used for the second operand (rT)
	wire [7:0]		RelAddr	= IR[ 7: 0];		// Bits used for relative addressing
	wire [7:0]		AbsAddr	= {IR[11:8],IR[3:0]};	// Bits used for absolute addressing
	parameter		r0	= 4'b0000;		// The special register 'r0' is being used

	`define	OP_NOP		(OpCode == 4'b0000)	// A NOP/BREAK/RET operation is executed
	`define	OP_IO		(OpCode == 4'b0001)	// An I/O operation is executed
	`define OP_JP		(OpCode == 4'b0010)	// A conditional jump is executed
	`define OP_GOTO		(OpCode == 4'b0011)	// An absolute jump is executed
	`define OP_CALL		(OpCode == 4'b0011)	// CALL and GOTO are identical

	`define	OP_CTRL		(OpCode[3:2] == 2'b00)	// Program Control operations
	`define	OP_MAC		(OpCode[3:2] == 2'b01)	// MAC operations
	`define	OP_ARITH	(OpCode[3:2] == 2'b10)	// Arithmetic operations
	`define	OP_LOGIC	(OpCode[3:2] == 2'b11)	// Logic operations
	`define OP_ALUMAC	(OpCode[3:2] != 2'b00)	// ALU or MAC operations
	`define	OP_ALU		(OpCode[3])		// Arithmetic or Logic operations

	`define	OUT_IN		(IR[7])				// '1' if OUT, '0' if IN
	`define	OP_IN		(`OP_IO & ~`OUT_IN)		// IN  rD,port
	`define	OP_OUT		(`OP_IO &  `OUT_IN)		// OUT port,rS
	`define	OP_RET		(`OP_NOP & (IR[11:9] == 3'h1))	// RET (or IRET)
	`define	OP_COND_ASG	(`OP_ARITH & ~IR[13])		// IF (flag) rD = rT|K



	//--------------------------//
	// Instruction Register: IR //
	//--------------------------//

	reg		OldMAC;		// Registers 'one' if last instruction was a MAC one
	reg  [3:0]	OldRegD;	// Destination Registry of a MAC instruction
	reg  [3:0]	FlagSelect;	// Flag Selected by the current instruction

	always @(posedge CLK or posedge ResetFF)
	begin
		if      (ResetFF)	OldMAC <= 1'b0;
		else if (`PHASE1)	OldMAC <= `OP_MAC;

		if      (ResetFF)	OldRegD <= r0;
		else if (`PHASE1)	OldRegD <= rD;

		if      (ResetFF)	FlagSelect <= 4'b0000;
		else if (`PHASE1)	FlagSelect <= CODEdata[11:8];

		if      (ResetFF)				IR <= 16'h0000;
		else if (`PHASE1)				IR <= CODEdata;
		else if ((`PHASE2 | `PHASE3) & `OP_COND_ASG)	IR <= {OpCode, rS, rS, rT};
	end


	//---------------------//
	// Program Counter: PC //
	//---------------------//

	reg	PCinc, PCflag;	// Auxiliary

	wire [12:0]	PCmask  = (PCflag) ? {RelAddr[7],RelAddr[7],RelAddr[7],RelAddr[7], RelAddr[7:0], 1'b1} : 13'h0001;
	wire [12:0]	PCadder = {PC, PCinc} + PCmask;

	always @(posedge CLK or posedge ResetFF)
	begin
		if (ResetFF)
		begin
			PC	<= 12'h000;
			PCinc	<= 1'b0;
			PCflag	<= 1'b0;
		end
		else
		begin
			if      (`PHASE3 & `OP_GOTO)	PC <= (AbsAddr << MODEL);	// GOTO
			else if (`PHASE3 & `OP_RET)	PC <= ACC[11:0];		// RET
			else				PC <= PCadder[12:1];		// Others

			PCinc	<= (`PHASE0) | (`PHASE2 & (rT == r0) & `OP_ALUMAC);	// OpFetch or K used

			PCflag  <= `PHASE2 & `OP_JP & Flag;			// '1' if relative jump
		end
	end

	assign CODEaddr = PC[MODEL+7:0];		// The logic not used is optimized away 


	//----------------------------------------------//
	// Bank of 16 Internal 24-bit Registers: r0-r15 //
	//----------------------------------------------//

	reg  [23:0]	RegsBank[0:15];	// Register Bank Memory
	reg   [3:0]	RegAddr;	// Register Bank Address

	wire RegWE = (`PHASE0 & `OP_ALU)	// ALU operation
		   | (`PHASE0 & `OP_IN)		// INport command
		   | (`PHASE1 &  OldMAC)	// MAC operation
		   | (`PHASE3 & `OP_CALL);	// CALL command

	always @(posedge CLK or posedge ResetFF)
	begin
		if (ResetFF)	RegAddr <= r0;
		else case (State)			// synopsys parallel_case full_case
			st0:	RegAddr <= OldRegD;		// Destination for MAC
			st1:	RegAddr <= CODEdata[3:0];	// Second Operand
			st2:	RegAddr <= rS;			// First Operand
			st3:	RegAddr <= rD;			// Destination for ALU
		endcase
	end

	always @(posedge CLK)
	begin
	   if (RegWE)	RegsBank[RegAddr] <= DataBus;	// Sync Write from DataBus
	end

	assign RegOut = RegsBank[RegAddr];		// Async Read

	always @(posedge CLK or posedge ResetFF)	// Sync Register Output
	begin
		if (ResetFF)	ACC <= 24'h000000;
		else 		ACC <= RegOut;
	end


	//--------------------------------------------------//
	// Multiplier-Accumulator and Arithmetic-Logic Unit //
	//--------------------------------------------------//

	wire [23:0]	ALUmac, ALUlogic, ALUarith;	// ALU outputs

	wire [23:0]	RegS = (rS == r0) ? 24'h000000	      : RegOut;	// rS or 0
	wire [23:0]	RegT = (rT == r0) ? {CODEdata, 8'h00} : ACC;	// rT or K

	ALUuva16 ALUandMAC
	(
		.Clk		(CLK),
		.InA		(RegS),
		.InB		(RegT),
		.InC		(RegOut),
		.Phase		(State),
		.OpCode		(OpCode),
		.FlagIn		(Flag),
		.Vflag		(ALUoverflow),
		.OutMAC		(ALUmac),
		.OutLogic	(ALUlogic),
		.OutArith	(ALUarith)
	);


	//-------------------//
	// Internal Data Bus //
	//-------------------//

	assign  DataBus = (`PHASE0 & `OP_ARITH)	? ALUarith	  : 24'bz;	// Arithmetic operation
	assign  DataBus = (`PHASE0 & `OP_LOGIC)	? ALUlogic	  : 24'bz;	// Logic operation
	assign  DataBus = (`PHASE1)		? ALUmac	  : 24'bz;	// MAC operation
	assign  DataBus = (`PHASE3 | `PHASE2)	? {12'h000,PC}	  : 24'bz;	// Return Address on CALLs
	assign  DataBus = (`PHASE0 & `OP_CTRL)	? {PORTin, 8'h00} : 24'bz;	// External input through PORT
	assign  DataBus = (`PHASE0 & `OP_MAC)	? 24'hFFFFFF	  : 24'bz;	// Otherwise, pull-up


	//-------//
	// Flags //
	//-------//

	`define	EQ	3'b000
	`define	NE	3'b001
	`define	OV	3'b010
	`define	NV	3'b011
	`define	GE	3'b100
	`define	GT	3'b101
	`define	LE	3'b110
	`define	LT	3'b111

	reg	ZFF, SFF, VFF;	// Internal flags: zero, sign and overflow.

	always @(posedge CLK or posedge ResetFF)
	begin
		if (ResetFF)
			{ZFF, SFF, VFF} <= 3'b000;
		else if (`PHASE0)			// Updates flags on ALU and INPORT ops
		begin
			ZFF <= (DataBus == 24'h000000);	// One if null result
			SFF <= DataBus[23];		// One if negative result
			VFF <= ALUoverflow;		// One if overflow on add/sub
		end
	end

	always @(ZFF or SFF or VFF or FlagSelect)
	begin
		case (FlagSelect[2:0])		// synopsys parallel_case full_case
			`EQ:	Flag =  ZFF;		// equal (to zero)
			`NE:	Flag = ~ZFF;		// not equal (to zero)
			`OV:	Flag =  VFF;		// overflow
			`NV:	Flag = ~VFF;		// not overflow
			`GE:	Flag = ~SFF |  ZFF;	// greater or equal (to zero)
			`GT:	Flag = ~SFF & ~ZFF;	// greater than (zero)
			`LE:	Flag =  SFF |  ZFF;	// less or equal (to zero)
			`LT:	Flag =  SFF & ~ZFF;	// less than (zero)
		endcase
	end


	//-----------------------------------//
	// External Interface: 128 I/O ports //
	//-----------------------------------//

	reg  [6:0]	PORTaddr;		// Port Address Register
	reg		IOread, IOwrite;

	always @(posedge CLK or posedge ResetFF)
	begin
		if (ResetFF)
			PORTaddr <= 7'h00;
		else if (`PHASE2)
		begin
`ifdef LOW_POWER
		    if (`OP_IO)					// Only changes when used
`endif
			PORTaddr <= (`OUT_IN) ? {IR[6:4],IR[11:8]} : IR[6:0];	// OUT or IN operation
		end

		if (ResetFF)	IOwrite <= 1'b0;
		else		IOwrite <= `PHASE2 & `OP_OUT;	// On the last subcycle

		if (ResetFF)	IOread  <= 1'b0;
		else		IOread  <= `PHASE3 & `OP_IN;	// On the first subcycle
	end

	assign	PORTout = ACC[23:8];

	// External I/O ports must decode PORTaddr
	// to gain control on PORTin bus (else pull-up)


endmodule	// DSPuva16


module test_dsp;

	reg	CLK;		// 40 MHz Clock (=> 100ns/instruction)
	reg	nRESET;		// Active-low external Reset
	reg	[15:0]	PORTin;		// Input Data Port
	reg	[15:0]	CODEdata;	// Code Memory Data (always read)	
	
	wire	[15:0]	PORTout;	// Output Data Port
	wire	[6:0]	PORTaddr;	// Port Address
	wire	IOread;		// Port Read Signal, active high
	wire	IOwrite;	// Port Write Signal, active high
	wire	[11:0]	CODEaddr;	// Code Memory Address
	
	DSPuva16 dut(CLK, nRESET, PORTin, PORTout, PORTaddr, IOread, IOwrite, CODEaddr, CODEdata);
	
	initial
	begin
		CLK = 1;
		forever 
		begin
			CLK = 1;
			#50 CLK = 0;
			#50;
		end
	end

	initial
	begin
		nRESET = 0;
		#15 nRESET = 1;		
	end
	
	
	initial
	begin
		PORTin = 16'h0000;
		CODEdata = 16'hFFFF;
		#100;
		
		// ------------------------------------------
		// -- Program Memory               	       --
		// ------------------------------------------
		// -- 0x00: 0x0000 NOP			           --
		// -- 0x01: 0xAE00 r14 = K			       --
		// -- 0x02: 0x53A2 K = 0x53A2		       --
		// -- 0x03: 0xA700 r7 = K			       --
		// -- 0x04: 0x6B1F K = 0x6B1F		       --
		// -- 0x05: 0xB3E7 r3 = r14 - r7	 	   --
		// -- 0x06: 0x9733 if (lt) r3 = -r3	       --
		// -- 0x07: 0x42E7 r2 = r14 * r7		   --
		// -- 0x08: 0x6273 r2 = r2 + r7 * r3	   --
		// -- 0x09: 0xAF27 r15 = r2 + r7 (*)	   --
		// -- 0x0A: 0x0000 NOP			           --
		// -- (*) r2 computed on 0x07, not on 0x08!--
		// ------------------------------------------
		
	
		// -- NOP at 0x00 --
		#15;
		CODEdata = 16'h0000;
		#200;
		CODEdata = 0;//16'hXXXX;
		#185;
			
		// -- "r14 = 0x53A2" at 0x01 and 0x02 --
		#15;
		CODEdata = 16'hAE00;
		#200;
		CODEdata = 16'h53A2;
		#185;
		
		// -- "r7 = 0x6B1F" at 0x03 and 0x04 --
		#15;
		CODEdata = 16'hA700;
		#200;
		CODEdata = 16'h6B1F;
		#185;
			
		// -- "r3 = r14 - r7" at 0x05 --
		#15;
		CODEdata = 16'hB3E7;
		#200;
		CODEdata = 0;//16'hXXXX;
		#185;
			
		// -- "if (lt) r3 = -r3" at 0x06 --
		#15;
		CODEdata = 16'h9733;
		#200;
		CODEdata = 0;//16'hXXXX;
		#185;
			
		// -- "r2 = r14 * r7" at 0x06 --
		#15;
		CODEdata = 16'h42E7;
		#200;
		CODEdata = 0;//16'hXXXX;
		#185;
			
		// -- "r2 = r2 + r7 * r3" at 0x07 --
		#15;
		CODEdata = 16'h6273;
		#200;
		CODEdata = 0;//16'hXXXX;
		#185;
			
		// -- "r15 = r2 + r7" at 0x06 --
		#15;
		CODEdata = 16'hAF27;
		#200;
		CODEdata = 0;//16'hXXXX;
		#185;
			
		// -- Ending NOPs --
		#15;
		CODEdata = 16'h0000;
		#200;
		CODEdata = 16'h0000;
		#185;
				
		$stop;
		
	end

endmodule
