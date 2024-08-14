
void main(void)
{
    *(volatile unsigned int *)0xE002E000 = 0x01;
    
}
