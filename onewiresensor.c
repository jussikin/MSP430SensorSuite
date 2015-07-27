#include <msp430.h>
#include "onewire.h"
#include "delay.h"
#include "sensor.h"

onewire_t ow;
void sensor_setup(){
	BCSCTL1 = CALBC1_8MHZ; //Set 8 MHZ for ONE wire operation
 	DCOCTL = CALDCO_8MHZ;
 	ow.port_out = &P1OUT;
  	ow.port_in = &P1IN;
    ow.port_ren = &P1REN;
    ow.port_dir = &P1DIR;
    ow.pin = BIT7; 
    BCSCTL1 = CALBC1_1MHZ; // Set Speed back to 1MHZ 
  	DCOCTL = CALDCO_1MHZ;  
}

int sensor_sense(char* TxBuf){
	signed char hop=0;
	uint8_t scratchpad[9];
	int i;
	int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

		  BCSCTL1 = CALBC1_8MHZ; //Set 8 MHZ for ONE wire operation
 	      DCOCTL = CALDCO_8MHZ;
 	      onewire_reset(&ow);
 		  onewire_write_byte(&ow, 0xcc); // skip ROM commands
	      onewire_write_byte(&ow, 0x44); // convert T command
          onewire_line_high(&ow);
          DELAY_MS(800); // at least 750 ms for the default 12-bit resolution
          onewire_reset(&ow);
          onewire_write_byte(&ow, 0xcc); // skip ROM command
          onewire_write_byte(&ow, 0xbe); // read scratchpad command
          for (i = 0; i < 9; i++) scratchpad[i] = onewire_read_byte(&ow);
          	 LowByte = scratchpad[0];
             HighByte = scratchpad[1];
             TReading = (HighByte << 8) + LowByte;
             SignBit = TReading & 0x8000;  // test most sig bit
             if (SignBit) // negative
             {
                 TReading = (TReading ^ 0xffff) + 1; // 2's comp
                 TReading = TReading*-1;
             }
             TReading = TReading>>1;
             TReading = TReading<<1;

             Tc_100 = (6 * TReading) + (TReading / 4);
             
            Whole = Tc_100 / 100;  // separate off the whole and fractional portions
            Fract = Tc_100 % 100;
             //if(Fract<0)
              //Fract=Fract*-1;
          hop=Whole;
      	  TxBuf[1]=hop;
      	  hop=Fract;
      	  TxBuf[2]=hop;

     	  BCSCTL1 = CALBC1_1MHZ; // Set Speed back to 1MHZ 
      	  DCOCTL = CALDCO_1MHZ;
      	  return 1;
}