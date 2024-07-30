#pragma once
/*
 *	Author: Huan Nguyen-Duy
 *  Date: 26/07/2024
 */

#ifndef _IO_H
#define _IO_H

/*To define base address for all models*/
	
#define BASE_RAM1			0x0000
#define BASE_DUMMYSLAVE		0x3000
#define BASE_DMAC			0x4000
#define BASE_TARGET1		0x5000
#define BASE_TARGET2		0x6000
#define BASE_TARGET3		0x7000
#define BASE_TARGET4		0x8000
#define BASE_FOUR_BIT_ADDER 0x9000
#define BASE_COUNTER		0x10000
#define BASE_UART			0x11000

/*To define size*/
#define SIZE_RAM1			0x3000
#define SIZE_DUMMYSLAVE		0x1000
#define SIZE_DMAC			0x1000
#define SIZE_TARGET1		0x1000
#define SIZE_TARGET2		0x1000
#define SIZE_TARGET3		0x1000
#define SIZE_TARGET4		0x1000
#define SIZE_FOUR_BIT_ADDER 0x1000
#define SIZE_COUNTER		0x1000
#define SIZE_UART			0x1000


#endif