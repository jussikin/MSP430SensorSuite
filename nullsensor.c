#include <msp430.h>
#include "onewire.h"
#include "delay.h"
#include "sensor.h"
#include <signal.h>
onewire_t ow;

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
  signed char hop=0;
  signed long temp;

  ADC10CTL1 = INCH_10 + ADC10DIV_3;         // Temp Sensor ADC10CLK/4
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
  delay(1000);                     

  ADC10CTL0 |= ENC + ADC10SC;               // Sampling and conversion start
  __bis_SR_register(CPUOFF + GIE);          // LPM0 with interrupts enabled
  temp = ADC10MEM;
  temp =  (temp * 27069L - 18169625L) >> 16;
  hop = temp;

  TxBuf[1]=hop;
  TxBuf[2]=0;
  return 1;
}

interrupt(ADC10_VECTOR) ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}