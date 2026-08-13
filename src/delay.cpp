#include "xc.h"
#include "delay.hpp"


void __delay_us(unsigned int  d) 
{
    unsigned int delayCount, startTime;       

    startTime = _CP0_GET_COUNT();     
    delayCount = d * CCLK_US; 
    while((_CP0_GET_COUNT() - startTime) < delayCount);  
 }

void __delay_ms(unsigned int d){
    unsigned int delayCount, startTime;       

    startTime = _CP0_GET_COUNT();
    delayCount = d * CCLK_MS;   
    while((_CP0_GET_COUNT() - startTime) < delayCount);   
}