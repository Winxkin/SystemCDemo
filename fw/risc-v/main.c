// #include "nvic.h"


void main(void)
{
    // nvic_enable_irq(0);


    *(volatile unsigned int *)0xE002E000 = 0x01;


    
}
