#include "NRF24L01p.h"
#include <msp430.h>

char  TX_ADDRESS[TX_ADR_WIDTH]= {'a','a','a','a','a'};	//TX address
char  RX_ADDRESS[RX_ADR_WIDTH]= {'a','a','a','a','a'};	//RX address
char  sta;

char SPI_RW(char data)
{
	char i,temp=0;
   	for(i=0;i<8;i++) // output 8-bit
   	{
	if((data & 0x80)==0x80)
	{
		RF24L01_MOSI_1;         // output 'uchar', MSB to MOSI
	}
	else
	{
	 	RF24L01_MOSI_0; 
	}	
//==============================================================================
		data = (data << 1);            // shift next bit into MSB..
		temp<<=1;
		inerDelay_us(2);
		RF24L01_SCK_1;                // Set SCK high..
		if((P1IN&0x02))temp++;		// capture current MISO bit
		inerDelay_us(2);
		RF24L01_SCK_0;              // ..then set SCK low again
   	}
    return(temp);           		  // return read uchar
}
/************************************************** 
Function: SPI_Read(); 
 
Description: 
  Read one byte from nRF24L01 register, 'reg'  */ 
/**************************************************/ 
char SPI_Read(char reg)
{
	char reg_val;
	RF24L01_CSN_0;           // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	RF24L01_CSN_1;         // CSN high, terminate SPI communication
	return(reg_val);       //  return register value
}

char SPI_RW_Reg(char reg, char value)
{
	char status1;
	RF24L01_CSN_0;                   // CSN low, init SPI transaction
	status1 = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	RF24L01_CSN_1;                   // CSN high again
	return(status1);            // return nRF24L01 status uchar
}

char SPI_W_Reg(char reg){
	char status1;
	RF24L01_CSN_0;                   // CSN low, init SPI transaction
	status1 = SPI_RW(reg);      // select register
	RF24L01_CSN_1;                   // CSN high again
	return(status1);   
}

char SPI_Read_Buf(char reg, char *pBuf, char chars)
{
	char status2,uchar_ctr;
	RF24L01_CSN_0;                    		// Set CSN low, init SPI tranaction
	status2 = SPI_RW(reg);       		// Select register to write to and read status uchar
	for(uchar_ctr=0;uchar_ctr<chars;uchar_ctr++)
        {
	pBuf[uchar_ctr] = SPI_RW(0);    // 
        }
	RF24L01_CSN_1;                     // Set CSN high       
	return(status2);                    // return nRF24L01 status uchar
}

char SPI_Write_Buf(char reg, char *pBuf, char chars)
{
	char status1,uchar_ctr;
	RF24L01_CSN_0;             // Set CSN low, init SPI tranaction     
	status1 = SPI_RW(reg);    // Select register to write to and read status byte
	for(uchar_ctr=0; uchar_ctr<chars; uchar_ctr++) // then write all byte in buffer(*pBuf)
        {
	SPI_RW(*pBuf++);
        }
	RF24L01_CSN_1;           // Set CSN high
	return(status1);    		  // 
}

void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x3B);  // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
	RF24L01_CE_1; // Set CE pin high to enable RX device
	inerDelay_us(1000);//delay for about 1 second
}

char nRF24L01_RxPacket(char* rx_buf)
{
    char revale=0;
	sta=SPI_Read(STATUS);	     // read register STATUS's value	
	if(sta&0x40)                 // success!
	{
	    RF24L01_CE_0 ; 			//SPI enable
	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
	    revale =1;			//set flag to 1
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);  
	return revale;
}

void nRF24L01_TxPacket(char * tx_buf)
{
	RF24L01_CE_0 ;			//StandBy
	SPI_Write_Buf(0x2A, TX_ADDRESS, TX_ADR_WIDTH);   // RX_Addr0 same as TX_Adr for Auto.Ack
	SPI_Write_Buf(0x30, TX_ADDRESS, TX_ADR_WIDTH);
	SPI_RW_Reg(0x20,0x0A);
	SPI_Write_Buf(0xA0, tx_buf, 0x05); 		 // Writes data to TX payload
	RF24L01_CE_1;		 // finish
	inerDelay_us(1000);
}
//****************************************************************************************
//NRF24L01 init
//***************************************************************************************/
void init_NRF24L01(void)
{
    inerDelay_us(1000);
 	RF24L01_CE_0 ;    // chip enable
 	RF24L01_CSN_1;   // Spi disable 
 	RF24L01_SCK_0;   // Spi clock line init high
	inerDelay_us(15);
	SPI_Write_Buf(0x2B, TX_ADDRESS, TX_ADR_WIDTH);
	inerDelay_us(15);
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x00);
	inerDelay_us(20);
	//SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x00);
	inerDelay_us(20);
	SPI_RW_Reg(0x25,0x00);
	SPI_RW_Reg(0x31, 0x05);
	inerDelay_us(20);
	SPI_RW_Reg(0x32, 0x05);
	inerDelay_us(20);
	
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0b00100110);
	inerDelay_us(20);
	SPI_RW_Reg(0x31, 0x05);
	inerDelay_us(20);
	SPI_RW_Reg(0x32, 0x05);
	inerDelay_us(20);
    SPI_RW_Reg(0x21, 0x00); //disable acks!!
    inerDelay_us(20);
	SPI_RW_Reg(0x20, 0x00);
	inerDelay_us(20);
	SPI_RW_Reg(0x27, 0x30);
	inerDelay_us(20);	
	SPI_W_Reg(FLUSH_RX);
	inerDelay_us(20);	
}