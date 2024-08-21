/**************************************************************
	v2sc_gate.h
	Some macros for implementing verilog-like gates in SystemC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
***************************************************************/


#ifndef V2SC_GATE_H
#define V2SC_GATE_H

//<Logic>
#ifdef SC_GATE_LOGIC
	#define TYPE sc_logic
	#define COMP ~
#else
	#define TYPE int
	#define COMP !
#endif
//</Logic>

//<SC_BUF>
	// Decleration
#define SC_BUF_DEC(name,out1,in)   GATE1_DEC(name,in)
#define SC_BUF1_DEC(name,out1,in)  GATE1_DEC(name,in)
#define SC_BUF2_DEC(name,out1,out2,in)   GATE1_DEC(name,in)
#define SC_BUF3_DEC(name,out1,out2,out3,in)   GATE1_DEC(name,in)
#define SC_BUF4_DEC(name,out1,out2,out3,out4,in)   GATE1_DEC(name,in)
#define SC_BUF5_DEC(name,out1,out2,out3,out4,out5,in)   GATE1_DEC(name,in)
#define SC_BUF6_DEC(name,out1,out2,out3,out4,out5,out6,in)   GATE1_DEC(name,in)
#define SC_BUF7_DEC(name,out1,out2,out3,out4,out5,out6,out7,in)   GATE1_DEC(name,in)
#define SC_BUF8_DEC(name,out1,out2,out3,out4,out5,out6,out7,out8,in)   GATE1_DEC(name,in)
#define SC_BUF9_DEC(name,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   GATE1_DEC(name,in)
	// Implementation
#define SC_BUF_IMP(name,out1,in)    BUF_BEGIN( ,name,in) BUF_PROP1(out1) BUF_END
#define SC_BUF1_IMP(name,out1,in)   BUF_BEGIN( ,name,in) BUF_PROP1(out1) BUF_END
#define SC_BUF2_IMP(name,out1,out2,in)   BUF_BEGIN( ,name,in) BUF_PROP2(out1,out2) BUF_END
#define SC_BUF3_IMP(name,out1,out2,out3,in)   BUF_BEGIN( ,name,in) BUF_PROP3(out1,out2,out3) BUF_END
#define SC_BUF4_IMP(name,out1,out2,out3,out4,in)   BUF_BEGIN( ,name,in) BUF_PROP4(out1,out2,out3,out4) BUF_END
#define SC_BUF5_IMP(name,out1,out2,out3,out4,out5,in)   BUF_BEGIN( ,name,in) BUF_PROP5(out1,out2,out3,out4,out5) BUF_END
#define SC_BUF6_IMP(name,out1,out2,out3,out4,out5,out6,in)   BUF_BEGIN( ,name,in) BUF_PROP6(out1,out2,out3,out4,out5,out6) BUF_END
#define SC_BUF7_IMP(name,out1,out2,out3,out4,out5,out6,out7,in)   BUF_BEGIN( ,name,in) BUF_PROP7(out1,out2,out3,out4,out5,out6,out7) BUF_END
#define SC_BUF8_IMP(name,out1,out2,out3,out4,out5,out6,out7,out8,in)   BUF_BEGIN( ,name,in) BUF_PROP8(out1,out2,out3,out4,out5,out6,out7,out8) BUF_END
#define SC_BUF9_IMP(name,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   BUF_BEGIN( ,name,in) BUF_PROP9(out1,out2,out3,out4,out5,out6,out7,out8,out9) BUF_END
//</SC_BUF>


//<SC_NOT>
	// Decleration
#define SC_NOT_DEC(name,out1,in)   GATE1_DEC(name,in)
#define SC_NOT1_DEC(name,out1,in)  GATE1_DEC(name,in)
#define SC_NOT2_DEC(name,out1,out2,in)   GATE1_DEC(name,in)
#define SC_NOT3_DEC(name,out1,out2,out3,in)   GATE1_DEC(name,in)
#define SC_NOT4_DEC(name,out1,out2,out3,out4,in)   GATE1_DEC(name,in)
#define SC_NOT5_DEC(name,out1,out2,out3,out4,out5,in)   GATE1_DEC(name,in)
#define SC_NOT6_DEC(name,out1,out2,out3,out4,out5,out6,in)   GATE1_DEC(name,in)
#define SC_NOT7_DEC(name,out1,out2,out3,out4,out5,out6,out7,in)   GATE1_DEC(name,in)
#define SC_NOT8_DEC(name,out1,out2,out3,out4,out5,out6,out7,out8,in)   GATE1_DEC(name,in)
#define SC_NOT9_DEC(name,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   GATE1_DEC(name,in)
	// Implementation
#define SC_NOT_IMP(name,out1,in)    BUF_BEGIN(COMP,name,in) BUF_PROP1(out1) BUF_END
#define SC_NOT1_IMP(name,out1,in)   BUF_BEGIN(COMP,name,in) BUF_PROP1(out1) BUF_END
#define SC_NOT2_IMP(name,out1,out2,in)   BUF_BEGIN(COMP,name,in) BUF_PROP2(out1,out2) BUF_END
#define SC_NOT3_IMP(name,out1,out2,out3,in)   BUF_BEGIN(COMP,name,in) BUF_PROP3(out1,out2,out3) BUF_END
#define SC_NOT4_IMP(name,out1,out2,out3,out4,in)   BUF_BEGIN(COMP,name,in) BUF_PROP4(out1,out2,out3,out4) BUF_END
#define SC_NOT5_IMP(name,out1,out2,out3,out4,out5,in)   BUF_BEGIN(COMP,name,in) BUF_PROP5(out1,out2,out3,out4,out5) BUF_END
#define SC_NOT6_IMP(name,out1,out2,out3,out4,out5,out6,in)   BUF_BEGIN(COMP,name,in) BUF_PROP6(out1,out2,out3,out4,out5,out6) BUF_END
#define SC_NOT7_IMP(name,out1,out2,out3,out4,out5,out6,out7,in)   BUF_BEGIN(COMP,name,in) BUF_PROP7(out1,out2,out3,out4,out5,out6,out7) BUF_END
#define SC_NOT8_IMP(name,out1,out2,out3,out4,out5,out6,out7,out8,in)   BUF_BEGIN(COMP,name,in) BUF_PROP8(out1,out2,out3,out4,out5,out6,out7,out8) BUF_END
#define SC_NOT9_IMP(name,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   BUF_BEGIN(COMP,name,in) BUF_PROP9(out1,out2,out3,out4,out5,out6,out7,out8,out9) BUF_END
//</SC_NOT>


//<SC_AND>
	// Decleration
#define SC_AND2_DEC(name,out,in1,in2)   GATE2_DEC(name,in1,in2)
#define SC_AND3_DEC(name,out,in1,in2,in3)   GATE3_DEC(name,in1,in2,in3)
#define SC_AND4_DEC(name,out,in1,in2,in3,in4)   GATE4_DEC(name,in1,in2,in3,in4)
#define SC_AND5_DEC(name,out,in1,in2,in3,in4,in5)   GATE5_DEC(name,in1,in2,in3,in4,in5)
#define SC_AND6_DEC(name,out,in1,in2,in3,in4,in5,in6)   GATE6_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_AND7_DEC(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_AND8_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_AND9_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_AND2_IMP(name,out,in1,in2)   GATE2_IMP( ,&,name,out,in1,in2)
#define SC_AND3_IMP(name,out,in1,in2,in3)   GATE3_IMP( ,&,name,out,in1,in2,in3)
#define SC_AND4_IMP(name,out,in1,in2,in3,in4)   GATE4_IMP( ,&,name,out,in1,in2,in3,in4)
#define SC_AND5_IMP(name,out,in1,in2,in3,in4,in5)   GATE5_IMP( ,&,name,out,in1,in2,in3,in4,in5)
#define SC_AND6_IMP(name,out,in1,in2,in3,in4,in5,in6)   GATE6_IMP( ,&,name,out,in1,in2,in3,in4,in5,in6)
#define SC_AND7_IMP(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_IMP( ,&,name,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_AND8_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_IMP( ,&,name,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_AND9_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_IMP( ,&,name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_AND>

//<SC_NAND>
	// Decleration
#define SC_NAND2_DEC(name,out,in1,in2)   GATE2_DEC(name,in1,in2)
#define SC_NAND3_DEC(name,out,in1,in2,in3)   GATE3_DEC(name,in1,in2,in3)
#define SC_NAND4_DEC(name,out,in1,in2,in3,in4)   GATE4_DEC(name,in1,in2,in3,in4)
#define SC_NAND5_DEC(name,out,in1,in2,in3,in4,in5)   GATE5_DEC(name,in1,in2,in3,in4,in5)
#define SC_NAND6_DEC(name,out,in1,in2,in3,in4,in5,in6)   GATE6_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_NAND7_DEC(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_NAND8_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NAND9_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_NAND2_IMP(name,out,in1,in2)   GATE2_IMP(COMP,&,name,out,in1,in2)
#define SC_NAND3_IMP(name,out,in1,in2,in3)   GATE3_IMP(COMP,&,name,out,in1,in2,in3)
#define SC_NAND4_IMP(name,out,in1,in2,in3,in4)   GATE4_IMP(COMP,&,name,out,in1,in2,in3,in4)
#define SC_NAND5_IMP(name,out,in1,in2,in3,in4,in5)   GATE5_IMP(COMP,&,name,out,in1,in2,in3,in4,in5)
#define SC_NAND6_IMP(name,out,in1,in2,in3,in4,in5,in6)   GATE6_IMP(COMP,&,name,out,in1,in2,in3,in4,in5,in6)
#define SC_NAND7_IMP(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_IMP(COMP,&,name,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_NAND8_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_IMP(COMP,&,name,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NAND9_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_IMP(COMP,&,name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_NAND>

//<SC_OR>
	// Decleration
#define SC_OR2_DEC(name,out,in1,in2)   GATE2_DEC(name,in1,in2)
#define SC_OR3_DEC(name,out,in1,in2,in3)   GATE3_DEC(name,in1,in2,in3)
#define SC_OR4_DEC(name,out,in1,in2,in3,in4)   GATE4_DEC(name,in1,in2,in3,in4)
#define SC_OR5_DEC(name,out,in1,in2,in3,in4,in5)   GATE5_DEC(name,in1,in2,in3,in4,in5)
#define SC_OR6_DEC(name,out,in1,in2,in3,in4,in5,in6)   GATE6_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_OR7_DEC(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_OR8_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_OR9_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_OR2_IMP(name,out,in1,in2)   GATE2_IMP( ,|,name,out,in1,in2)
#define SC_OR3_IMP(name,out,in1,in2,in3)   GATE3_IMP( ,|,name,out,in1,in2,in3)
#define SC_OR4_IMP(name,out,in1,in2,in3,in4)   GATE4_IMP( ,|,name,out,in1,in2,in3,in4)
#define SC_OR5_IMP(name,out,in1,in2,in3,in4,in5)   GATE5_IMP( ,|,name,out,in1,in2,in3,in4,in5)
#define SC_OR6_IMP(name,out,in1,in2,in3,in4,in5,in6)   GATE6_IMP( ,|,name,out,in1,in2,in3,in4,in5,in6)
#define SC_OR7_IMP(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_IMP( ,|,name,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_OR8_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_IMP( ,|,name,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_OR9_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_IMP( ,|,name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_OR>


//<SC_NOR>
	// Decleration
#define SC_NOR2_DEC(name,out,in1,in2)   GATE2_DEC(name,in1,in2)
#define SC_NOR3_DEC(name,out,in1,in2,in3)   GATE3_DEC(name,in1,in2,in3)
#define SC_NOR4_DEC(name,out,in1,in2,in3,in4)   GATE4_DEC(name,in1,in2,in3,in4)
#define SC_NOR5_DEC(name,out,in1,in2,in3,in4,in5)   GATE5_DEC(name,in1,in2,in3,in4,in5)
#define SC_NOR6_DEC(name,out,in1,in2,in3,in4,in5,in6)   GATE6_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_NOR7_DEC(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_NOR8_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NOR9_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_NOR2_IMP(name,out,in1,in2)   GATE2_IMP(COMP,|,name,out,in1,in2)
#define SC_NOR3_IMP(name,out,in1,in2,in3)   GATE3_IMP(COMP,|,name,out,in1,in2,in3)
#define SC_NOR4_IMP(name,out,in1,in2,in3,in4)   GATE4_IMP(COMP,|,name,out,in1,in2,in3,in4)
#define SC_NOR5_IMP(name,out,in1,in2,in3,in4,in5)   GATE5_IMP(COMP,|,name,out,in1,in2,in3,in4,in5)
#define SC_NOR6_IMP(name,out,in1,in2,in3,in4,in5,in6)   GATE6_IMP(COMP,|,name,out,in1,in2,in3,in4,in5,in6)
#define SC_NOR7_IMP(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_IMP(COMP,|,name,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_NOR8_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_IMP(COMP,|,name,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NOR9_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_IMP(COMP,|,name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_NOR>


//<SC_XOR>
	// Decleration
#define SC_XOR2_DEC(name,out,in1,in2)   GATE2_DEC(name,in1,in2)
#define SC_XOR3_DEC(name,out,in1,in2,in3)   GATE3_DEC(name,in1,in2,in3)
#define SC_XOR4_DEC(name,out,in1,in2,in3,in4)   GATE4_DEC(name,in1,in2,in3,in4)
#define SC_XOR5_DEC(name,out,in1,in2,in3,in4,in5)   GATE5_DEC(name,in1,in2,in3,in4,in5)
#define SC_XOR6_DEC(name,out,in1,in2,in3,in4,in5,in6)   GATE6_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_XOR7_DEC(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_XOR8_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XOR9_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_XOR2_IMP(name,out,in1,in2)   GATE2_IMP( ,^,name,out,in1,in2)
#define SC_XOR3_IMP(name,out,in1,in2,in3)   GATE3_IMP( ,^,name,out,in1,in2,in3)
#define SC_XOR4_IMP(name,out,in1,in2,in3,in4)   GATE4_IMP( ,^,name,out,in1,in2,in3,in4)
#define SC_XOR5_IMP(name,out,in1,in2,in3,in4,in5)   GATE5_IMP( ,^,name,out,in1,in2,in3,in4,in5)
#define SC_XOR6_IMP(name,out,in1,in2,in3,in4,in5,in6)   GATE6_IMP( ,^,name,out,in1,in2,in3,in4,in5,in6)
#define SC_XOR7_IMP(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_IMP( ,^,name,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_XOR8_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_IMP( ,^,name,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XOR9_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_IMP( ,^,name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_XOR>


//<SC_XNOR>
	// Decleration
#define SC_XNOR2_DEC(name,out,in1,in2)   GATE2_DEC(name,in1,in2)
#define SC_XNOR3_DEC(name,out,in1,in2,in3)   GATE3_DEC(name,in1,in2,in3)
#define SC_XNOR4_DEC(name,out,in1,in2,in3,in4)   GATE4_DEC(name,in1,in2,in3,in4)
#define SC_XNOR5_DEC(name,out,in1,in2,in3,in4,in5)   GATE5_DEC(name,in1,in2,in3,in4,in5)
#define SC_XNOR6_DEC(name,out,in1,in2,in3,in4,in5,in6)   GATE6_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_XNOR7_DEC(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_XNOR8_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XNOR9_DEC(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_XNOR2_IMP(name,out,in1,in2)   GATE2_IMP(COMP,^,name,out,in1,in2)
#define SC_XNOR3_IMP(name,out,in1,in2,in3)   GATE3_IMP(COMP,^,name,out,in1,in2,in3)
#define SC_XNOR4_IMP(name,out,in1,in2,in3,in4)   GATE4_IMP(COMP,^,name,out,in1,in2,in3,in4)
#define SC_XNOR5_IMP(name,out,in1,in2,in3,in4,in5)   GATE5_IMP(COMP,^,name,out,in1,in2,in3,in4,in5)
#define SC_XNOR6_IMP(name,out,in1,in2,in3,in4,in5,in6)   GATE6_IMP(COMP,^,name,out,in1,in2,in3,in4,in5,in6)
#define SC_XNOR7_IMP(name,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_IMP(COMP,^,name,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_XNOR8_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_IMP(COMP,^,name,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XNOR9_IMP(name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_IMP(COMP,^,name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_XNOR>


//<SC_GATE>
	// Decleration
#define GATE1_DEC(name,in1) \
	SC_METHOD(name); \
	sensitive << in1;
#define GATE2_DEC(name,in1,in2) \
	SC_METHOD(name); \
	sensitive << in1 << in2;
#define GATE3_DEC(name,in1,in2,in3) \
	SC_METHOD(name); \
	sensitive << in1 << in2 << in3;
#define GATE4_DEC(name,in1,in2,in3,in4) \
	SC_METHOD(name); \
	sensitive << in1 << in2 << in3 << in4;
#define GATE5_DEC(name,in1,in2,in3,in4,in5) \
	SC_METHOD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5;
#define GATE6_DEC(name,in1,in2,in3,in4,in5,in6) \
	SC_METHOD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6;
#define GATE7_DEC(name,in1,in2,in3,in4,in5,in6,in7) \
	SC_METHOD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6 << in7;
#define GATE8_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8) \
	SC_METHOD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8;
#define GATE9_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9) \
	SC_METHOD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9;
	// Implementation
#define GATE1_IMP(op1,op2,name,out,in1) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() ); \
	}
#define GATE2_IMP(op1,op2,name,out,in1,in2) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() ); \
	}
#define GATE3_IMP(op1,op2,name,out,in1,in2,in3) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() ); \
	}
#define GATE4_IMP(op1,op2,name,out,in1,in2,in3,in4) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() ); \
	}
#define GATE5_IMP(op1,op2,name,out,in1,in2,in3,in4,in5) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() ); \
	}
#define GATE6_IMP(op1,op2,name,out,in1,in2,in3,in4,in5,in6) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() ); \
	}
#define GATE7_IMP(op1,op2,name,out,in1,in2,in3,in4,in5,in6,in7) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() op2 in7.read() ); \
	}
#define GATE8_IMP(op1,op2,name,out,in1,in2,in3,in4,in5,in6,in7,in8) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() op2 in7.read() op2 in8.read() ); \
	}
#define GATE9_IMP(op1,op2,name,out,in1,in2,in3,in4,in5,in6,in7,in8,in9) \
	void name() \
	{ \
		out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() op2 in7.read() op2 in8.read() op2 in9.read() ); \
	}
//</SC_GATE>


//<BUF>
#define BUF_BEGIN(op,name,in) void name() { TYPE t = op( in.read() );
#define BUF_PROP1(out1) out1 = t;
#define BUF_PROP2(out1,out2)  BUF_PROP1(out1)  BUF_PROP1(out2)
#define BUF_PROP3(out1,out2,out3)  BUF_PROP2(out1,out2)  BUF_PROP1(out3)
#define BUF_PROP4(out1,out2,out3,out4)  BUF_PROP2(out1,out2)  BUF_PROP2(out3,out4)
#define BUF_PROP5(out1,out2,out3,out4,out5)  BUF_PROP4(out1,out2,out3,out4)  BUF_PROP1(out5)
#define BUF_PROP6(out1,out2,out3,out4,out5,out6)  BUF_PROP4(out1,out2,out3,out4)  BUF_PROP2(out5,out6)
#define BUF_PROP7(out1,out2,out3,out4,out5,out6,out7)  BUF_PROP4(out1,out2,out3,out4)  BUF_PROP2(out5,out6) BUF_PROP1(out7)
#define BUF_PROP8(out1,out2,out3,out4,out5,out6,out7,out8)  BUF_PROP4(out1,out2,out3,out4)  BUF_PROP4(out5,out6,out7,out8)
#define BUF_PROP9(out1,out2,out3,out4,out5,out6,out7,out8,out9)  BUF_PROP8(out1,out2,out3,out4,out5,out6,out7,out8) BUF_PROP1(out9)
#define BUF_END  }
//</BUF>


//<SC_BUFIF>
	// Decleration
#define SC_BUFIF0_DEC(name,out,in,ctrl)      GATE2_DEC(name,in,ctrl)
#define SC_BUFIF1_DEC(name,out,in,ctrl)      GATE2_DEC(name,in,ctrl)
#define SC_NOTIF0_DEC(name,out,in,ctrl)      GATE2_DEC(name,in,ctrl)
#define SC_NOTIF1_DEC(name,out,in,ctrl)      GATE2_DEC(name,in,ctrl)
	// Implementation
#define SC_BUFIF0_IMP(name,out,in,ctrl)      BUFIF_IMP( ,0,name,out,in,ctrl)
#define SC_BUFIF1_IMP(name,out,in,ctrl)      BUFIF_IMP( ,1,name,out,in,ctrl)
#define SC_NOTIF0_IMP(name,out,in,ctrl)      BUFIF_IMP(COMP,0,name,out,in,ctrl)
#define SC_NOTIF1_IMP(name,out,in,ctrl)      BUFIF_IMP(COMP,1,name,out,in,ctrl)

#define BUFIF_IMP(op,cond,name,out,in,ctrl) \
	void name() \
	{ \
		if( ctrl.read() == cond ) out = (sc_logic) op( in.read() ); \
		else out = sc_logic('Z'); \
	}
//</SC_BUFIF>

//<SC_DELAY_BUF>
	// Decleration
#define SC_BUF_DELAY_DEC(name,delay,unit,out1,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF1_DELAY_DEC(name,delay,unit,out1,in)  GATE1_DELAY_DEC(name,in)
#define SC_BUF2_DELAY_DEC(name,delay,unit,out1,out2,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF3_DELAY_DEC(name,delay,unit,out1,out2,out3,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF4_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF5_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF6_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF7_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF8_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,in)   GATE1_DELAY_DEC(name,in)
#define SC_BUF9_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   GATE1_DELAY_DEC(name,in)
	// Implementation
#define SC_BUF_DELAY_IMP(name,delay,unit,out1,in)    BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP1(out1) BUF_DELAY_END
#define SC_BUF1_DELAY_IMP(name,delay,unit,out1,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP1(out1) BUF_DELAY_END
#define SC_BUF2_DELAY_IMP(name,delay,unit,out1,out2,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP2(out1,out2) BUF_DELAY_END
#define SC_BUF3_DELAY_IMP(name,delay,unit,out1,out2,out3,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP3(out1,out2,out3) BUF_DELAY_END
#define SC_BUF4_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP4(out1,out2,out3,out4) BUF_DELAY_END
#define SC_BUF5_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP5(out1,out2,out3,out4,out5) BUF_DELAY_END
#define SC_BUF6_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP6(out1,out2,out3,out4,out5,out6) BUF_DELAY_END
#define SC_BUF7_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP7(out1,out2,out3,out4,out5,out6,out7) BUF_DELAY_END
#define SC_BUF8_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP8(out1,out2,out3,out4,out5,out6,out7,out8) BUF_DELAY_END
#define SC_BUF9_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   BUF_DELAY_BEGIN( ,name,delay,unit,in) BUF_DELAY_PROP9(out1,out2,out3,out4,out5,out6,out7,out8,out9) BUF_DELAY_END
//</SC_DELAY_BUF>


//<SC_DELAY_NOT>
	// Decleration
#define SC_NOT_DELAY_DEC(name,delay,unit,out1,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT1_DELAY_DEC(name,delay,unit,out1,in)  GATE1_DELAY_DEC(name,in)
#define SC_NOT2_DELAY_DEC(name,delay,unit,out1,out2,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT3_DELAY_DEC(name,delay,unit,out1,out2,out3,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT4_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT5_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT6_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT7_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT8_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,in)   GATE1_DELAY_DEC(name,in)
#define SC_NOT9_DELAY_DEC(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   GATE1_DELAY_DEC(name,in)
	// Implementation
#define SC_NOT_DELAY_IMP(name,delay,unit,out1,in)    BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP1(out1) BUF_DELAY_END
#define SC_NOT1_DELAY_IMP(name,delay,unit,out1,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP1(out1) BUF_DELAY_END
#define SC_NOT2_DELAY_IMP(name,delay,unit,out1,out2,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP2(out1,out2) BUF_DELAY_END
#define SC_NOT3_DELAY_IMP(name,delay,unit,out1,out2,out3,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP3(out1,out2,out3) BUF_DELAY_END
#define SC_NOT4_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP4(out1,out2,out3,out4) BUF_DELAY_END
#define SC_NOT5_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP5(out1,out2,out3,out4,out5) BUF_DELAY_END
#define SC_NOT6_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP6(out1,out2,out3,out4,out5,out6) BUF_DELAY_END
#define SC_NOT7_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP7(out1,out2,out3,out4,out5,out6,out7) BUF_DELAY_END
#define SC_NOT8_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP8(out1,out2,out3,out4,out5,out6,out7,out8) BUF_DELAY_END
#define SC_NOT9_DELAY_IMP(name,delay,unit,out1,out2,out3,out4,out5,out6,out7,out8,out9,in)   BUF_DELAY_BEGIN(COMP,name,delay,unit,in) BUF_DELAY_PROP9(out1,out2,out3,out4,out5,out6,out7,out8,out9) BUF_DELAY_END
//</SC_DELAY_NOT>


//<SC_DELAY_AND>
	// Decleration
#define SC_AND2_DELAY_DEC(name,delay,unit,out,in1,in2)   GATE2_DELAY_DEC(name,in1,in2)
#define SC_AND3_DELAY_DEC(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_DEC(name,in1,in2,in3)
#define SC_AND4_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_DEC(name,in1,in2,in3,in4)
#define SC_AND5_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_DEC(name,in1,in2,in3,in4,in5)
#define SC_AND6_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_AND7_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_AND8_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_AND9_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_AND2_DELAY_IMP(name,delay,unit,out,in1,in2)   GATE2_DELAY_IMP( ,&,name,delay,unit,out,in1,in2)
#define SC_AND3_DELAY_IMP(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_IMP( ,&,name,delay,unit,out,in1,in2,in3)
#define SC_AND4_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_IMP( ,&,name,delay,unit,out,in1,in2,in3,in4)
#define SC_AND5_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_IMP( ,&,name,delay,unit,out,in1,in2,in3,in4,in5)
#define SC_AND6_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_IMP( ,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6)
#define SC_AND7_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_IMP( ,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_AND8_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_IMP( ,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_AND9_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_IMP( ,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_DELAY_AND>

//<SC_DELAY_NAND>
	// Decleration
#define SC_NAND2_DELAY_DEC(name,delay,unit,out,in1,in2)   GATE2_DELAY_DEC(name,in1,in2)
#define SC_NAND3_DELAY_DEC(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_DEC(name,in1,in2,in3)
#define SC_NAND4_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_DEC(name,in1,in2,in3,in4)
#define SC_NAND5_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_DEC(name,in1,in2,in3,in4,in5)
#define SC_NAND6_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_NAND7_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_NAND8_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NAND9_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_NAND2_DELAY_IMP(name,delay,unit,out,in1,in2)   GATE2_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2)
#define SC_NAND3_DELAY_IMP(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2,in3)
#define SC_NAND4_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2,in3,in4)
#define SC_NAND5_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2,in3,in4,in5)
#define SC_NAND6_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6)
#define SC_NAND7_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_NAND8_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NAND9_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_IMP(COMP,&,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_DELAY_NAND>

//<SC_DELAY_OR>
	// Decleration
#define SC_OR2_DELAY_DEC(name,delay,unit,out,in1,in2)   GATE2_DELAY_DEC(name,in1,in2)
#define SC_OR3_DELAY_DEC(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_DEC(name,in1,in2,in3)
#define SC_OR4_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_DEC(name,in1,in2,in3,in4)
#define SC_OR5_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_DEC(name,in1,in2,in3,in4,in5)
#define SC_OR6_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_OR7_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_OR8_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_OR9_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_OR2_DELAY_IMP(name,delay,unit,out,in1,in2)   GATE2_DELAY_IMP( ,|,name,delay,unit,out,in1,in2)
#define SC_OR3_DELAY_IMP(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_IMP( ,|,name,delay,unit,out,in1,in2,in3)
#define SC_OR4_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_IMP( ,|,name,delay,unit,out,in1,in2,in3,in4)
#define SC_OR5_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_IMP( ,|,name,delay,unit,out,in1,in2,in3,in4,in5)
#define SC_OR6_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_IMP( ,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6)
#define SC_OR7_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_IMP( ,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_OR8_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_IMP( ,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_OR9_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_IMP( ,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_DELAY_OR>


//<SC_DELAY_NOR>
	// Decleration
#define SC_NOR2_DELAY_DEC(name,delay,unit,out,in1,in2)   GATE2_DELAY_DEC(name,in1,in2)
#define SC_NOR3_DELAY_DEC(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_DEC(name,in1,in2,in3)
#define SC_NOR4_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_DEC(name,in1,in2,in3,in4)
#define SC_NOR5_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_DEC(name,in1,in2,in3,in4,in5)
#define SC_NOR6_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_NOR7_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_NOR8_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NOR9_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_NOR2_DELAY_IMP(name,delay,unit,out,in1,in2)   GATE2_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2)
#define SC_NOR3_DELAY_IMP(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2,in3)
#define SC_NOR4_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2,in3,in4)
#define SC_NOR5_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2,in3,in4,in5)
#define SC_NOR6_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6)
#define SC_NOR7_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_NOR8_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_NOR9_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_IMP(COMP,|,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_DELAY_NOR>


//<SC_DELAY_XOR>
	// Decleration
#define SC_XOR2_DELAY_DEC(name,delay,unit,out,in1,in2)   GATE2_DELAY_DEC(name,in1,in2)
#define SC_XOR3_DELAY_DEC(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_DEC(name,in1,in2,in3)
#define SC_XOR4_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_DEC(name,in1,in2,in3,in4)
#define SC_XOR5_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_DEC(name,in1,in2,in3,in4,in5)
#define SC_XOR6_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_XOR7_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_XOR8_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XOR9_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_XOR2_DELAY_IMP(name,delay,unit,out,in1,in2)   GATE2_DELAY_IMP( ,^,name,delay,unit,out,in1,in2)
#define SC_XOR3_DELAY_IMP(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_IMP( ,^,name,delay,unit,out,in1,in2,in3)
#define SC_XOR4_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_IMP( ,^,name,delay,unit,out,in1,in2,in3,in4)
#define SC_XOR5_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_IMP( ,^,name,delay,unit,out,in1,in2,in3,in4,in5)
#define SC_XOR6_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_IMP( ,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6)
#define SC_XOR7_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_IMP( ,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_XOR8_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_IMP( ,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XOR9_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_IMP( ,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_DELAY_XOR>


//<SC_DELAY_XNOR>
	// Decleration
#define SC_XNOR2_DELAY_DEC(name,delay,unit,out,in1,in2)   GATE2_DELAY_DEC(name,in1,in2)
#define SC_XNOR3_DELAY_DEC(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_DEC(name,in1,in2,in3)
#define SC_XNOR4_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_DEC(name,in1,in2,in3,in4)
#define SC_XNOR5_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_DEC(name,in1,in2,in3,in4,in5)
#define SC_XNOR6_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_DEC(name,in1,in2,in3,in4,in5,in6)
#define SC_XNOR7_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7)
#define SC_XNOR8_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XNOR9_DELAY_DEC(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9)
	// Implementation
#define SC_XNOR2_DELAY_IMP(name,delay,unit,out,in1,in2)   GATE2_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2)
#define SC_XNOR3_DELAY_IMP(name,delay,unit,out,in1,in2,in3)   GATE3_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2,in3)
#define SC_XNOR4_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4)   GATE4_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2,in3,in4)
#define SC_XNOR5_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5)   GATE5_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2,in3,in4,in5)
#define SC_XNOR6_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6)   GATE6_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6)
#define SC_XNOR7_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)   GATE7_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7)
#define SC_XNOR8_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)   GATE8_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8)
#define SC_XNOR9_DELAY_IMP(name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)   GATE9_DELAY_IMP(COMP,^,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9)
//</SC_DELAY_XNOR>


//<SC_DELAY_GATE>
	// Decleration
#define GATE1_DELAY_DEC(name,in1) \
	SC_THREAD(name); \
	sensitive << in1;
#define GATE2_DELAY_DEC(name,in1,in2) \
	SC_THREAD(name); \
	sensitive << in1 << in2;
#define GATE3_DELAY_DEC(name,in1,in2,in3) \
	SC_THREAD(name); \
	sensitive << in1 << in2 << in3;
#define GATE4_DELAY_DEC(name,in1,in2,in3,in4) \
	SC_THREAD(name); \
	sensitive << in1 << in2 << in3 << in4;
#define GATE5_DELAY_DEC(name,in1,in2,in3,in4,in5) \
	SC_THREAD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5;
#define GATE6_DELAY_DEC(name,in1,in2,in3,in4,in5,in6) \
	SC_THREAD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6;
#define GATE7_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7) \
	SC_THREAD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6 << in7;
#define GATE8_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8) \
	SC_THREAD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8;
#define GATE9_DELAY_DEC(name,in1,in2,in3,in4,in5,in6,in7,in8,in9) \
	SC_THREAD(name); \
	sensitive << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9;
	// Implementation
#define GATE1_DELAY_IMP(op1,op2,name,delay,unit,out,in1) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() ); \
			wait(); \
		} \
	}
#define GATE2_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() ); \
			wait(); \
		} \
	}
#define GATE3_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2,in3) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() ); \
			wait(); \
		} \
	}
#define GATE4_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2,in3,in4) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() ); \
			wait(); \
		} \
	}
#define GATE5_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2,in3,in4,in5) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() ); \
			wait(); \
		} \
	}
#define GATE6_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2,in3,in4,in5,in6) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() ); \
			wait(); \
		} \
	}
#define GATE7_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() op2 in7.read() ); \
			wait(); \
		} \
	}
#define GATE8_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() op2 in7.read() op2 in8.read() ); \
			wait(); \
		} \
	}
#define GATE9_DELAY_IMP(op1,op2,name,delay,unit,out,in1,in2,in3,in4,in5,in6,in7,in8,in9) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			out = (TYPE) op1( in1.read() op2 in2.read() op2 in3.read() op2 in4.read() op2 in5.read() op2 in6.read() op2 in7.read() op2 in8.read() op2 in9.read() ); \
			wait(); \
		} \
	}
//</SC_DELAY_GATE>


//<DELAY_BUF>
#define BUF_DELAY_BEGIN(op,name,delay,unit,in) void name() { while(1){ wait(delay,unit); TYPE t = op( in.read() );
#define BUF_DELAY_PROP1(out1) out1 = t;
#define BUF_DELAY_PROP2(out1,out2)  BUF_DELAY_PROP1(out1)  BUF_DELAY_PROP1(out2)
#define BUF_DELAY_PROP3(out1,out2,out3)  BUF_DELAY_PROP2(out1,out2)  BUF_DELAY_PROP1(out3)
#define BUF_DELAY_PROP4(out1,out2,out3,out4)  BUF_DELAY_PROP2(out1,out2)  BUF_DELAY_PROP2(out3,out4)
#define BUF_DELAY_PROP5(out1,out2,out3,out4,out5)  BUF_DELAY_PROP4(out1,out2,out3,out4)  BUF_DELAY_PROP1(out5)
#define BUF_DELAY_PROP6(out1,out2,out3,out4,out5,out6)  BUF_DELAY_PROP4(out1,out2,out3,out4)  BUF_DELAY_PROP2(out5,out6)
#define BUF_DELAY_PROP7(out1,out2,out3,out4,out5,out6,out7)  BUF_DELAY_PROP4(out1,out2,out3,out4)  BUF_DELAY_PROP2(out5,out6) BUF_DELAY_PROP1(out7)
#define BUF_DELAY_PROP8(out1,out2,out3,out4,out5,out6,out7,out8)  BUF_DELAY_PROP4(out1,out2,out3,out4)  BUF_DELAY_PROP4(out5,out6,out7,out8)
#define BUF_DELAY_PROP9(out1,out2,out3,out4,out5,out6,out7,out8,out9)  BUF_DELAY_PROP8(out1,out2,out3,out4,out5,out6,out7,out8) BUF_DELAY_PROP1(out9)
#define BUF_DELAY_END  wait(); } }
//</DELAY_BUF>


//<SC_DELAY_BUFIF>
	// Decleration
#define SC_BUFIF0_DELAY_DEC(name,delay,unit,out,in,ctrl)      GATE2_DELAY_DEC(name,in,ctrl)
#define SC_BUFIF1_DELAY_DEC(name,delay,unit,out,in,ctrl)      GATE2_DELAY_DEC(name,in,ctrl)
#define SC_NOTIF0_DELAY_DEC(name,delay,unit,out,in,ctrl)      GATE2_DELAY_DEC(name,in,ctrl)
#define SC_NOTIF1_DELAY_DEC(name,delay,unit,out,in,ctrl)      GATE2_DELAY_DEC(name,in,ctrl)
	// Implementation
#define SC_BUFIF0_DELAY_IMP(name,delay,unit,out,in,ctrl)      BUFIF_DELAY_IMP( ,0,name,delay,unit,out,in,ctrl)
#define SC_BUFIF1_DELAY_IMP(name,delay,unit,out,in,ctrl)      BUFIF_DELAY_IMP( ,1,name,delay,unit,out,in,ctrl)
#define SC_NOTIF0_DELAY_IMP(name,delay,unit,out,in,ctrl)      BUFIF_DELAY_IMP(COMP,0,name,delay,unit,out,in,ctrl)
#define SC_NOTIF1_DELAY_IMP(name,delay,unit,out,in,ctrl)      BUFIF_DELAY_IMP(COMP,1,name,delay,unit,out,in,ctrl)

#define BUFIF_DELAY_IMP(op,cond,name,delay,unit,out,in,ctrl) \
	void name() \
	{ \
		while(1) \
		{ \
			wait(delay,unit); \
			if( ctrl.read() == cond ) out = (sc_logic) op( in.read() ); \
			else out = sc_logic('Z'); \
			wait(); \
		} \
	}

//</SC_DELAY_BUFIF>

#endif
