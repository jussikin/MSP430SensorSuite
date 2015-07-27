#include <msp430.h>
#include "delay.h"
#include "sensor.h"
#include <signal.h>

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
  unsigned long result;

  ADC10CTL1 = INCH_7 + ADC10DIV_3;
  ADC10AE0 |= BIT7; //choose bit 7 analog input
  ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;
  delay(1000);                     

  ADC10CTL0 |= ENC + ADC10SC;               // Sampling and conversion start
  __bis_SR_register(CPUOFF + GIE);          // LPM0 with interrupts enabled
  result = ADC10MEM;

  TxBuf[1]=result;
  TxBuf[2]=result >>8;
  return 1;
}

interrupt(ADC10_VECTOR) ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}