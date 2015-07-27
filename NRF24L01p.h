#ifndef __NRF24P_H
#define __NRF24P_H


//==============================================================================
#define  RF24L01_CE_0        P1OUT &=~BIT5         
#define  RF24L01_CE_1        P1OUT |= BIT5        
//=============================RF24L01 CSN ==================================
#define  RF24L01_CSN_0       P1OUT &=~BIT4        
#define  RF24L01_CSN_1       P1OUT |= BIT4     
//=============================RF24L01 SCK======================================
#define  RF24L01_SCK_0       P1OUT &=~BIT3      
#define  RF24L01_SCK_1       P1OUT |= BIT3   
//==============================RF24L01 MISO PIN=========================================
#define  RF24L01_MISO_0      P1OUT &=~BIT1
#define  RF24L01_MISO_1      P1OUT |= BIT1
//============================RF24L01 MOSI PIN==================================
#define  RF24L01_MOSI_0      P1OUT &=~BIT2
#define  RF24L01_MOSI_1      P1OUT |= BIT2
//===========================IRQ PIN ============================================
#define  RF24L01_IRQ_0       P1OUT &=~BIT0      
#define  RF24L01_IRQ_1       P1OUT |= BIT0
//==============================================================================
#define  LED1_0              P2OUT &=~BIT7          //LED1 ON
#define  LED1_1              P2OUT |= BIT7          //LED1 OFF
#define  LED2_0              P1OUT &=~BIT7         //LED2 ON
#define  LED2_1              P1OUT |= BIT7          //LED2 OFF
//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH 2
#define RX_PLOAD_WIDTH 2
//****************************************************************// 
// SPI(nRF24L01) commands 
#define READ_REG        0x00  // Define read command to register 
#define WRITE_REG       0x20  // Define write command to register 
#define RD_RX_PLOAD     0x61  // Define RX payload register address 
#define WR_TX_PLOAD     0xA0  // Define TX payload register address 
#define FLUSH_TX        0xE1  // Define flush TX register command 
#define FLUSH_RX        0xE2  // Define flush RX register command 
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command 
#define NOP1            0xFF  // Define No Operation, might be used to read status register 

//***************************************************// 
// SPI(nRF24L01) registers(addresses) 
#define CONFIG          0x00  // 'Config' register address 
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address 
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address 
#define SETUP_AW        0x03  // 'Setup address width' register address 
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address 
#define RF_CH           0x05  // 'RF channel' register address 
#define RF_SETUP        0x06  // 'RF setup' register address 
#define STATUS          0x07  // 'Status' register address 
#define OBSERVE_TX      0x08  // 'Observe TX' register address 
#define CD              0x09  // 'Carrier Detect' register address 
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address 
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address 
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address 
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address 
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address 
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address 
#define TX_ADDR         0x10  // 'TX address' register address 
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address 
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address 
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address 
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address 
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address 
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address 
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address 

void RF24L01_IO_set(void);
char SPI_RW(char data);
char SPI_Read(char reg);
char SPI_RW_Reg(char reg, char value);
char SPI_W_Reg(char reg);
char SPI_Read_Buf(char reg, char *pBuf, char chars);
char SPI_Write_Buf(char reg, char *pBuf, char chars);
void SetRX_Mode(void);
char nRF24L01_RxPacket(char* rx_buf);
void nRF24L01_TxPacket(char * tx_buf);
void init_NRF24L01(void);


#endif