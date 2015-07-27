#include <msp430.h>
#include "delay.h"
#include "sensor.h"

static void __inline__ delay(register unsigned int n)
{
  __asm__ __volatile__ (
  "1: \n"
  " dec %[n] \n"
  " jne 1b \n"
        : [n] "+r"(n));
}


void sensor_setup(){

}

int sensor_sense(char* TxBuf){
  P1DIR &= ~(BIT7|BIT6); 
  P1OUT |= (BIT7|BIT6);
  P1REN |= (BIT7|BIT6);
  DELAY_MS(500); 


  if(P1IN & BIT6){ 
    TxBuf[1]=1;
  }else{
    TxBuf[1]=0;
  }
  if(P1IN & BIT7){ 
    TxBuf[2]=1;
  }else{
    TxBuf[2]=0;
  }
  P1OUT ^= (BIT7+BIT6);
  return 1;
}

