#include <msp430.h>
#include <signal.h>
#include <string.h>
#include "sensor.h"
#include "NRF24L01p.h"

//Amount of skips between resend
//#define SKIPRESENDS 3
//define if sensor is resender
//#define RESENDER
 
char  tf;
signed long temp;
volatile int SensorRunning = 0;
volatile int waitedSteps = 0;
char TxBuf[5] = {IDCODE ,100,112,113,TYPE};
char RxBuf[5] = {125,100,112,113,114};
int resent = 0;
int received =0;
int havingAnap=0;

//===========================MiniDsp Port IO setup ==========================================
void RF24L01_IO_set(void)
{
      P1DIR = 0xbc;  P1SEL=0x00;   P1IE=0x00;
      P2DIR = 0x80;  P2SEL=0x00;   P2IE=0x00;
}
//========================delay for about 5ms=============================================
void ms_delay(void)
{ 
   unsigned int i=40000;
    while (i != 0)
    {
        i--;
    }
}
//=======================delay for about 2*s ops ================================
void Delay(int s)
{
	unsigned int i,j;
	for(i=0; i<s; i++);
	for(j=0; j<s; j++);
}

static void __inline__ delay(register unsigned int n)
{
  __asm__ __volatile__ (
  "1: \n"
  " dec %[n] \n"
  " jne 1b \n"
        : [n] "+r"(n));
}


void inerDelay_us(int n)
{
	for(;n>0;n--);
}

void ConfigureTimers(void){
	  BCSCTL3 |= LFXT1S_2;        //Set up timer for sleeping
      TACTL |= TASSEL_1 | MC_1;  //  This is used to avoid excess heat
      TACCTL0 = CCIE;	        //    and to preserve battery
      TACCR0 = 12000;          //
}

void littleSleep(void){
havingAnap=1;
 ConfigureTimers();
  __bis_SR_register(LPM3_bits + GIE);
havingAnap=0;
}

//=============================================================================
main()
{     
      WDTCTL = WDTPW + WDTHOLD; //disabel timer 
      BCSCTL1 = CALBC1_1MHZ; // Set range
      DCOCTL = CALDCO_1MHZ;  // Set DCO step and modulation
		
      RF24L01_IO_set();
      init_NRF24L01();
	  inerDelay_us(150);

	  sensor_setup();
	  
	  int skippendResends=0;
	  while(1){
	  	  SensorRunning=1;
		  sensor_sense(TxBuf);
		  SensorRunning=0;
		  RF24L01_IO_set();
          init_NRF24L01();
		  inerDelay_us(50);
		  nRF24L01_TxPacket(TxBuf);	// send the buffer packet
		  littleSleep();
		  #ifdef RESENDER
		     if(skippendResends >= SKIPRESENDS){SetRX_Mode();
			 }else {
			 skippendResends++;
			 SPI_RW_Reg(0x20, 0x00);
			 }
		  #else
		     SPI_RW_Reg(0x20, 0x00);
		  #endif
		  ConfigureTimers();
		  eint();
		  __bis_SR_register(LPM3_bits + GIE);  
	    
		    if(nRF24L01_RxPacket(RxBuf)){
		    if(RxBuf[0]<TxBuf[0]){
			    skippendResends=0;			  
			    SPI_W_Reg(FLUSH_RX);
				nRF24L01_TxPacket(RxBuf);				
			    resent++;
				littleSleep();
				}
		  }
		   		  
		  }
}


interrupt (TIMER0_A0_VECTOR) Timer_A(void)
{
   if(havingAnap){
    __bic_SR_register_on_exit(LPM3_bits + GIE);
   }else waitedSteps++;
   
   if(waitedSteps>ICOUNTMAX){
		if(!SensorRunning){
			waitedSteps=0;
			__bic_SR_register_on_exit(LPM3_bits+ GIE);
		}
   }
}
