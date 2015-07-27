#include <msp430.h>
#include "delay.h"
#include "sensor.h"
#include <signal.h>

volatile int countterPos = 1;

void sensor_setup(){
   P1IE=0;
   P1IE |= BIT6;
   P1IE |= BIT7; 
   P1DIR &= ~(BIT7|BIT6);                
   P1REN |= BIT7; 
   P1REN |= BIT6;                       
   P1IFG &= ~(BIT7|BIT6);
   eint();
}

int sensor_sense(char* TxBuf){
       TxBuf[2]=countterPos;
       TxBuf[1]=countterPos >> 8;
       return 0;
}

#pragma vector=PORT1_VECTOR       // Port 1 interrupt service routine
  __interrupt void Port_1(void) {
    countterPos++;
    P1IFG = 0; 
}


