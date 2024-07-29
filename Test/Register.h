#pragma once
#pragma once
/*
 *	Author: Huan Nguyen-Duy
 *  Date: 26/07/2024
 */

#ifndef _REGISTER_H
#define _REGISTER_H
#include "../../SocPlatform/IO.h"

#define PASS 0x01
#define FAIL 0x00

 /*Define register address for DMAC*/
 /* i from 0 to 255 */
#define REG_DMADESADDR(i)	 (BASE_DMAC + 0x00 + 0x04*(i))			// 0x000	->	0x400
#define REG_DMASRCADDR(i)	 (BASE_DMAC + 0x400 + 0x04*(i))			// 0x400	->	0x800
#define REG_DMADATALENGTH(i) (BASE_DMAC + 0x800 + 0x04*(i))			// 0x800	->	0xC00
						 				  
/* i from 0 to 7 */		 				  
#define REG_DMAREQ(i)		 (BASE_DMAC + 0xC00 + 0x04*(i))			// 0xC00	->	0xC20
#define REG_DMAACK(i)		 (BASE_DMAC + 0xC20 + 0x04*(i))			// 0xC20	->	0xC40
#define REG_DMAINT(i)		 (BASE_DMAC + 0xC40 + 0x04*(i))			// 0xC40	->	0xC60
#define REG_DMACHEN(i)		 (BASE_DMAC + 0xC60 + 0x04*(i))			// 0xC60	->	0xC80 

/* To Define register address for DummySlave*/
#define REG_DUMMYRESULT		 (BASE_DUMMYSLAVE + 0x00)

/* To Define register for ADDER*/
#define REG_ADDERINPUT		 (BASE_FOUR_BIT_ADDER + 0x00)
#define REG_ADDEROUTPUT		 (BASE_FOUR_BIT_ADDER + 0x04)

/* To Define register for */
#define REG_COUNTERINPUT		 (BASE_COUNTER + 0x00)
#define REG_COUNTEROUTPUT		 (BASE_COUNTER + 0x04)

#endif