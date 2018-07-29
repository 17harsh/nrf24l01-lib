/*
   * nrf24l01.h
   *
 - * Copyright (C) 2017 by Harsh Singhal.
 - * Modification or use of this software in source or binary form
 - * is permitted as long as files maintain this copyright.
 - * This is completely for educational purposes and not for commercial use.
 - *
   * Created: 07-11-2017 14:49:09
 - * Author: Harsh Singhal
 - */


#ifndef NRF24L01_H_
#define NRF24L01_H_

/*ATmega8*/

#include <avr/io.h>
#include <util/delay.h>
#include "nrf_mnemonics.h"
#include "SPI.h"

/**************Control Lines*****************/

#define CE				PINB1
#define CSN				PINB2
//#define IRQ				PINB0	//(use same port as CE and CSN)(comment it out if you want to use IRQ)
									//(also modify nrf_transmit(), nrf_receive() and nrf_init()) 
#define Cont_DDR		DDRB
#define Cont_Pull		PORTB
#define Cont_read		PINB
/*********************************************/

/***REFER DATASHEET FOR CHANGING THESE VALUES***/

/*NRF CONFIG REG*/
#define MASK_RX_DR			0			//Mask interrupt caused by RX_DR (1=interrupt NOT refelected on pin ; 0=interrupt reflected on pin)
#define MASK_TX_DS			0			//Mask interrupt caused by TX_DS (1=interrupt NOT refelected on pin ; 0=interrupt reflected on pin)
#define MASK_MAX_RT			0			//Mask interrupt caused by MAX_RT (1=interrupt NOT refelected on pin ; 0=interrupt reflected on pin)
#define EN_CRC				1			//Enable CRC (Forced 1 if Auto ACK is enabled)
#define CRCO				0			//CRC encoding scheme 


/*NRF AUTO Acknowledgment (Enhanced ShockBurst)*/
#define ENAA_Px				1			// 1: enable auto ack on all data pipes
										// 0: disables auto ack on all data pipes
										
/*NRF Enable data pipe (Enable Rx addresses)*/
#define ERX_P5				0			// Enable data pipe 5
#define ERX_P4				0			// Enable data pipe 4
#define ERX_P3				0			// Enable data pipe 3
#define ERX_P2				0			// Enable data pipe 2
#define ERX_P1				0			// Enable data pipe 1
#define ERX_P0				1			// Enable data pipe 0

/*Setup Address Width of TX/RX (common for all data pipes)*/
#define AW					3			// 0 = 00; 1 = 01; 2 = 10; 3 = 11;			

/*Setup of Automatic Retransmission*/
#define ARD					2			// Auto retransmit delay (refer data sheet for value)
#define ARC					3			// Auto retransmit Count (refer data sheet for value)

/*Setup RF channel Frequency*/
#define Frequency			2402ul		//Write in MHz(for eg. 2402MHz or 2.402Ghz)(Can operate form 2.400GHz to 2.525GHz)(Resolution is 1MHz) 

/*RF Setup Register*/
#define CONT_WAVE			0			//Enables continuous carrier transmit when high
#define RF_DR_LOW			0			//Set RF data rate to 250kbps. Used in combination with RD_DR_HIGH
#define PLL_LOCK			0			//Force PLL lock signal. Only used in test
#define RF_DR_HIGH			0			//2Mbps and 1Mbps setting. Used in combination with RD_DR_LOW
#define RF_PWR				3			//Set RF Output Power in TX mode

/*Receive address of data pipe 0-5*/
#define Data_Pipe0			0xE7E7E7E7E7ull		//Receive Add for data pipe0. LSBytes are used if less than 5 bytes in Address width
#define Data_Pipe1			0xC2C2C2C2C2ull		//Receive Add for data pipe1. LSBytes are used if less than 5 bytes in Address width
#define Data_Pipe2			0xC3				//Receive Add for data pipe2. MSBytes equal to Data_Pipe1
#define Data_Pipe3			0xC4				//Receive Add for data pipe3. MSBytes equal to Data_Pipe1 
#define Data_Pipe4			0xC5				//Receive Add for data pipe4. MSBytes equal to Data_Pipe1
#define Data_Pipe5			0xC6				//Receive Add for data pipe5. MSBytes equal to Data_Pipe1
 
/*Transmitter address*/
#define tx_address			0xE7E7E7E7E7ull		//used for PTX device only. LSByte written first.Should be same as data pipe address of the receiver sending to

/*Setup RX Payload Size (for data pipe 0-5) (Can take value form 0x00-0x20 i.e form 1byte to 32byte (0 is pipe not used))*/
#define RX_Payload_P0		1			//RX Payload size of data pipe 0
#define RX_Payload_P1		0			//RX Payload size of data pipe 1
#define RX_Payload_P2		0			//RX Payload size of data pipe 2
#define RX_Payload_P3		0			//RX Payload size of data pipe 3
#define RX_Payload_P4		0			//RX Payload size of data pipe 4
#define RX_Payload_P5		0			//RX Payload size of data pipe 5

/*Enable dynamic Payload length*/
#define DPL_P5				0			//Enable dynamic payload length on pipe5
#define DPL_P4				0			//Enable dynamic payload length on pipe4
#define DPL_P3				0			//Enable dynamic payload length on pipe3
#define DPL_P2				0			//Enable dynamic payload length on pipe2
#define DPL_P1				0			//Enable dynamic payload length on pipe1
#define DPL_P0				0			//Enable dynamic payload length on pipe0

/*Setup Feature register*/
#define EN_DPL				0			//Enables Dynamic Payload length
#define	EN_ACK_PAY			0			//Enables Payload with ACK
#define	EN_DYN_ACK			0			//Enables the W_TX_PAYLOAD_NOACK command 

/**********IMPORTANT FUNCTIONS*************/

/**************************************************************************************************
* Description : initialize nrf24l01+ module with important commands and settings such as auto acknowledgment,
*				Data pipe, Address width, Re-transmission setting, RF channel setting, RF setup(speed),
*				RX and TX addresses, RX payload size for each data pipe, Dynamic payload and other features
*				like auto acknowledgment payload etc.
*				REFER DATASHEET AND MAKE CHANGES ABOVE
**************************************************************************************************/
void nrf24l01_init(void);

/*************************************************************************************************
* Description : Configure nrf24l01+ as PRIMARY TX or PRIMARY RX as well as powers module.
*				use this after initializing
* Parameters  : PWR_UP = power up module (1 = ON) (0 = OFF)
*				PRIM_RX = Primary RX or mode (1 = PRX) (0 = PTX)
**************************************************************************************************/
void nrf_config(unsigned char PWR_UP, unsigned char PRIM_RX);

/*************************************************************************************************
* Description : Transmits the array of data in TX FIFO. Supports simple transmission WITHOUT
*				ACK, transmission with ACK and transmission with ACK PAYLOAD.
* Parameters  : unsigned char *data = array of data to be transmitted in TX FIFO
*				unsigned char Byte_size = size of array of data (max 5 bytes)
* Returns	  : unsigned char *nrf_transmit = returns array of data that is ACK Payload
**************************************************************************************************/
unsigned char *nrf_transmit(unsigned char *data, unsigned char Byte_size);

/*************************************************************************************************
* Description : Returns the Received data stored in RX FIFO. Supports ACK and noACK
* Parameters  : unsigned char Rec_Byte_Size = size of array of received data in RX FIFO
* Returns     : unsigned char *nrf_receive = array of data that is present in RX FIFO
**************************************************************************************************/
unsigned char *nrf_receive(unsigned char Rec_Byte_size);

/*************************************************************************************************
* Description : Returns the Received data stored in RX FIFO. Supports ACK with Payload
* Parameters  : unsigned char *data = Array of data in ACK Payload
*				unsigned char Ack_Byte_Size = size of array of ACK Payload(max 5 bytes)
*				unsigned char Rec_Byte_Size = size of array of received data in RX FIFO
* Returns     : unsigned char *nrf_receive = array of data that is present in RX FIFO
**************************************************************************************************/
unsigned char *nrf_receive_ackpayload(unsigned char *data, unsigned char Ack_Byte_size, unsigned char Rec_Byte_size);

/*************************************************************************************************
* Description : Returns array of data read from particular register (eg. STATUS, RX FIFO)
* Parameters  : unsigned char Register = register address from which data is to be read (use mnemonics)
*				unsigned char Byte_size = size of data that is being read (max 5 bytes)
* Returns     : static unsigned char *read_nrf =  array of data read from register
**************************************************************************************************/
static unsigned char *read_nrf(unsigned char Register, unsigned char Byte_size);

/*************************************************************************************************
* Description : writes array of data to the following registers (eg. TX FIFO).
* Parameters  : unsigned char Register = Register address to which data is to written (use mnemonics)
*				unsigned char *data = array data that is to be written
*				unsigned char Byte_size = size of data that is to be written (max 5 bytes)
**************************************************************************************************/
void write_nrf(unsigned char Register,unsigned char *data, unsigned char Byte_size);


/******************OTHER FUNCTIONS****************************/

/*************************************************************************************************
* Description : enables auto ack ON or OFF on all data pipes
**************************************************************************************************/
void autoack(void);

/*************************************************************************************************
* Description : enables or disables data pipe/s (from 0 to 5)
**************************************************************************************************/
void enable_pipe(void);

/*************************************************************************************************
* Description : Sets TX and RX address width of the module
**************************************************************************************************/
void address_width(void);

/*************************************************************************************************
* Description : Sets the Re-Transmission register of module
**************************************************************************************************/
void re_trans(void);

/*************************************************************************************************
* Description : Sets RF channel frequency (from 2.400 GHz to 2.525 GHz)
**************************************************************************************************/
void rf_ch(void);

/*************************************************************************************************
* Description : Sets RF speed (250kbps, 1Mbps or 2Mbps) and RF power
**************************************************************************************************/
void rf_setup(void);

/*************************************************************************************************
* Description : Sets RX address of all data pipes from 0 to 5
**************************************************************************************************/
void rx_add(void);

/*************************************************************************************************
* Description : Sets TX address of module
**************************************************************************************************/
void tx_add(void);

/*************************************************************************************************
* Description : Sets RX Payload size of all data pipes
**************************************************************************************************/
void rx_payload(void);

/*************************************************************************************************
* Description : Sets Dynamic Payload size in data pipes
**************************************************************************************************/
void dynamic_payload(void);

/*************************************************************************************************
* Description : Enables features like dynamic payload, auto ack with payload etc.
**************************************************************************************************/
void feature(void);

/************************FUNCTION DEFINATIONS*********************************/

/********MODIFY FIRST THREE FUNCTION TO USE IRQ***********/

void nrf24l01_init(){
	SPI_init();				//initialize SPI
	DDR_high;				//CE and CSN as output
	//DDR_low;				//IRQ as input
	//IRQ_low;				//pull down IRQ
	CE_low;
	CSN_high;
	_delay_ms(110);			//power on reset
	autoack();
	enable_pipe();
	address_width();
	re_trans();
	rf_ch();
	rf_setup();
	rx_add();
	tx_add();
	rx_payload();
	dynamic_payload();
	feature();
}
unsigned char *nrf_transmit(unsigned char *data, unsigned char Byte_size){
	char tries = 0;
	jump: write_nrf(FLUSH_TX,data,0);
	write_nrf(FLUSH_RX,data,0);
	unsigned char temp[Byte_size];
	for (unsigned char i = 0; i < Byte_size; i++){
		temp[i] = *data;
		data++;
	}
	write_nrf(W_TX_PAYLOAD,temp,Byte_size);
	unsigned char temp1[1];
	
	if(ENAA_Px == 0){
		CE_high;
		_delay_us(20);								//minimum 10us pulse
		//use this :
		temp1[0] = *read_nrf(STATUS,0);
		while(!(temp1[0] & (1<<5))){				//checking status register for change in nrf
			temp1[0] = *read_nrf(STATUS,0);
		}
		//or this :
		//while (!(Cont_read & (1<<IRQ)));
		CE_low;
		unsigned char data1[1];
		if(temp1[0] & (1<<4)){
			data1[0] = 	0x1e;
			write_nrf(STATUS,data1,1);
		}
		if(temp1[0] & (1<<5)){
			data1[0] = 0x2e;
			write_nrf(STATUS,data1,1);
		}
		if(temp1[0] & (1<<6)){
			data1[0] = 0x4e;
			write_nrf(STATUS,data1,1);
		}
		return 0;
	}
	if(ENAA_Px == 1){
		CE_high;
		_delay_us(20);								//minimum 10us pulse
		//use this :
		temp1[0] = *read_nrf(STATUS,0);
		while(!((temp1[0] & (1<<5)) || (temp1[0] & (1<<4)))){	//checking status register for change in nrf
			temp1[0] = *read_nrf(STATUS,0);
		}
		//or this :
		//while (!(Cont_read & (1<<IRQ)));
		CE_low;
		unsigned char data1[1];
		if(temp1[0] & (1<<4)){
			if(tries > 5){
				return 0;
			}
			else{
				tries++;
				data1[0] = 	0x1e;
				write_nrf(STATUS,data1,1);
				goto jump;
			}
		}
		//ACK with payload
		if(temp1[0] & (1<<6)){
			if(temp1[0] & (1<<4)){
				data1[0] = 	0x1e;
				write_nrf(STATUS,data1,1);
			}
			if(temp1[0] & (1<<5)){
				data1[0] = 0x2e;
				write_nrf(STATUS,data1,1);
			}
			if(temp1[0] & (1<<6)){
				data1[0] = 0x4e;
				write_nrf(STATUS,data1,1);
			}
			return read_nrf(R_RX_PAYLOAD,1);
		}
		//ACK without payload
		else{
			if(temp1[0] & (1<<4)){
				data1[0] = 	0x1e;
				write_nrf(STATUS,data1,1);
			}
			if(temp1[0] & (1<<5)){
				data1[0] = 0x2e;
				write_nrf(STATUS,data1,1);
			}
			if(temp1[0] & (1<<6)){
				data1[0] = 0x4e;
				write_nrf(STATUS,data1,1);
			}
			return 0;
		}
	}
}

unsigned char *nrf_receive(unsigned char Rec_Byte_size){
	unsigned char *data;
	write_nrf(FLUSH_RX,data,0);
	write_nrf(FLUSH_TX,data,0);
	
	unsigned char temp1[1];
	CE_high;
	_delay_us(140);									//minimum 130us delay
	//use this :
	temp1[0] = *read_nrf(STATUS,0);
	while (!(temp1[0] & (1<<6))){					//checking status register for change in nrf
		temp1[0] = *read_nrf(STATUS,0);
	}
	//or this :
	//while (!(Cont_read & (1<<IRQ)));
	CE_low;
	unsigned char data1[1];
	if(temp1[0] & (1<<4)){
		data1[0] = 	0x1e;
		write_nrf(STATUS,data1,1);
	}
	if(temp1[0] & (1<<5)){
		data1[0] = 0x2e;
		write_nrf(STATUS,data1,1);
	}
	if(temp1[0] & (1<<6)){
		data1[0] = 0x4e;
		write_nrf(STATUS,data1,1);
	}
	return read_nrf(R_RX_PAYLOAD,Rec_Byte_size);
}

unsigned char *nrf_receive_ackpayload(unsigned char *data, unsigned char Ack_Byte_size, unsigned char Rec_Byte_size){
	write_nrf(FLUSH_RX,data,0);
	write_nrf(FLUSH_TX,data,0);
	unsigned char temp[Ack_Byte_size];
	for (unsigned char i = 0; i < Ack_Byte_size; i++){
		temp[i] = *data;
		data++;
	}
	write_nrf(W_ACK_PAYLOAD,temp,Ack_Byte_size);
	unsigned char temp1[1];
	CE_high;
	_delay_us(140);									//minimum 130us delay
	//use this :
	temp1[0] = *read_nrf(STATUS,0);
	while (!(temp1[0] & (1<<6))){					//checking status register for change in nrf
		temp1[0] = *read_nrf(STATUS,0);
	}
	//or this :
	//while (!(Cont_read & (1<<IRQ)));
	CE_low;
	unsigned char data1[1];
	if(temp1[0] & (1<<4)){
		data1[0] = 	0x1e;
		write_nrf(STATUS,data1,1);
	}
	if(temp1[0] & (1<<5)){
		data1[0] = 0x2e;
		write_nrf(STATUS,data1,1);
	}
	if(temp1[0] & (1<<6)){
		data1[0] = 0x4e;
		write_nrf(STATUS,data1,1);
	}
	return read_nrf(R_RX_PAYLOAD,Rec_Byte_size);
}
void nrf_config(unsigned char PWR_UP, unsigned char PRIM_RX){
	unsigned char config_reg[1];
	config_reg[0] = ((MASK_RX_DR<<6)|(MASK_TX_DS<<5)|(MASK_MAX_RT<<4)|(EN_CRC<<3)|(CRCO<<2)|(PWR_UP<<1)|(PRIM_RX));
	write_nrf(CONFIG,config_reg,1);
	_delay_ms(5);												//Start up delay
}
void autoack(){
	unsigned char ENAA_P[1];
	if(ENAA_Px == 0) ENAA_P[0] = 0x00;
	if(ENAA_Px == 1) ENAA_P[0] = 0x3F;
	write_nrf(EN_AA,ENAA_P,1);
}
void enable_pipe(){
	unsigned char ERX_Px[1];
	ERX_Px[0] = ((ERX_P5<<5)|(ERX_P4<<4)|(ERX_P3<<3)|(ERX_P2<<2)|(ERX_P1<<1)|(ERX_P0));
	write_nrf(EN_RXADDR,ERX_Px,1);
}
void address_width(){
	unsigned char aw[1];
	aw[0] = AW;
	write_nrf(SETUP_AW,aw,1);
}
void re_trans(){
	unsigned char RETR_reg[1];
	RETR_reg[0] = ((ARD<<4)|(ARC));
	write_nrf(SETUP_RETR,RETR_reg,1);
}
void rf_ch(){
	unsigned char RFCH[1];
	RFCH[0] = Frequency - 2400;
	write_nrf(RF_CH,RFCH,1);
}
void rf_setup(){
	unsigned char RF_reg[1];
	RF_reg[0] = ((CONT_WAVE<<7)|(RF_DR_LOW<<5)|(PLL_LOCK<<4)|(RF_DR_HIGH<<3)|(RF_PWR<<1));
	write_nrf(RF_SETUP,RF_reg,1);
}
void rx_add(){					
	unsigned char Byte_size;
	if (AW == 1) Byte_size = 3;
	if (AW == 2) Byte_size = 4;
	if (AW == 3) Byte_size = 5;
	unsigned char Address[5];
	Address[0] = (unsigned char) (0x00000000FF & Data_Pipe0);
	Address[1] = (unsigned char) ((0x000000FF00 & Data_Pipe0)>>8);
	Address[2] = (unsigned char) ((0x0000FF0000 & Data_Pipe0)>>16);
	Address[3] = (unsigned char) ((0x00FF000000 & Data_Pipe0)>>24);
	Address[4] = (unsigned char) ((0xFF00000000 & Data_Pipe0)>>32);
	write_nrf(RX_ADDR_P0,Address,Byte_size);
	
	Address[0] = (unsigned char) (0x00000000FF & Data_Pipe1);
	Address[1] = (unsigned char) ((0x000000FF00 & Data_Pipe1)>>8);
	Address[2] = (unsigned char) ((0x0000FF0000 & Data_Pipe1)>>16);
	Address[3] = (unsigned char) ((0x00FF000000 & Data_Pipe1)>>24);
	Address[4] = (unsigned char) ((0xFF00000000 & Data_Pipe1)>>32);
	write_nrf(RX_ADDR_P1,Address,Byte_size);
	
	Address[0] = Data_Pipe2;
	write_nrf(RX_ADDR_P2,Address,1);
	
	Address[0] = Data_Pipe3;
	write_nrf(RX_ADDR_P3,Address,1);
	
	Address[0] = Data_Pipe4;
	write_nrf(RX_ADDR_P4,Address,1);
	
	Address[0] = Data_Pipe5;
	write_nrf(RX_ADDR_P5,Address,1);
}
void tx_add(){
	unsigned char Byte_size;
	if (AW == 1) Byte_size = 3;
	if (AW == 2) Byte_size = 4;
	if (AW == 3) Byte_size = 5;
	unsigned char Address[5];
	Address[0] = (unsigned char) (0x00000000FF & tx_address);
	Address[1] = (unsigned char) ((0x000000FF00 & tx_address)>>8);
	Address[2] = (unsigned char) ((0x0000FF0000 & tx_address)>>16);
	Address[3] = (unsigned char) ((0x00FF000000 & tx_address)>>24);
	Address[4] = (unsigned char) ((0xFF00000000 & tx_address)>>32);
	write_nrf(TX_ADDR,Address,Byte_size);
}
void rx_payload(){
	unsigned char rx_payload_p[1];
	rx_payload_p[0] = RX_Payload_P0;
	write_nrf(RX_PW_P0,rx_payload_p,1);
	
	rx_payload_p[0] = RX_Payload_P1;
	write_nrf(RX_PW_P1,rx_payload_p,1);
	
	rx_payload_p[0] = RX_Payload_P2;
	write_nrf(RX_PW_P2,rx_payload_p,1);
	
	rx_payload_p[0] = RX_Payload_P3;
	write_nrf(RX_PW_P3,rx_payload_p,1);
	
	rx_payload_p[0] = RX_Payload_P4;
	write_nrf(RX_PW_P4,rx_payload_p,1);
	
	rx_payload_p[0] = RX_Payload_P5;
	write_nrf(RX_PW_P5,rx_payload_p,1);
}
void dynamic_payload(){
	unsigned char dpl_px[1];
	dpl_px[0] = ((DPL_P5<<5)|(DPL_P4<<4)|(DPL_P3<<3)|(DPL_P2<<2)|(DPL_P1<<1)|(DPL_P0));
	write_nrf(DYNPD,dpl_px,1);
}
void feature(){
	unsigned char feat[1];
	feat[0] = ((EN_DPL<<2)|(EN_ACK_PAY<<1)|(EN_DYN_ACK));
	write_nrf(FEATURE,feat,1);
}
static unsigned char *read_nrf(unsigned char Register, unsigned char Byte_size){
	//_delay_us(1);
	static unsigned char ret[32];
	
	if(Register == STATUS){
		CSN_low;
		ret[0] = SPI_Read_Write(Register);
		CSN_high;
	}
	else{
		CSN_low;
		SPI_Read_Write(Register);
		for(unsigned char i=0; i<Byte_size; i++){
			ret[i] = SPI_Read_Write(NOP);
		}
		CSN_high;
	}
	return ret;
}
void write_nrf(unsigned char Register,unsigned char *data, unsigned char Byte_size){
	//_delay_us(1);
	if(Register <= 0x1D){
		Register = Register + W_REGISTER;
	}
	CSN_low;
	SPI_Read_Write(Register);
	
	for (unsigned char i=0; i< Byte_size; i++){
		SPI_Read_Write(*data);
		data++;
	}
	CSN_high;
}

#endif /* NRF24L01_H_ */