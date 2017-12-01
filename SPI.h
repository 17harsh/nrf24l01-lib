/*
 * SPI.h
 *
 * Copyright (C) 2017 by Harsh Singhal.
 * Modification or use of this software in source or binary form
 * is permitted as long as files maintain this copyright.
 * This is completely for educational purposes and not for commercial use.
 *
 * Created: 06-11-2017 20:34:50
 * Author: Harsh Singhal
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

/** ATmega8**/
#define DDR_SPI		DDRB
#define SPI_Pull	PORTB
#define SS			PINB2
#define MOSI		PINB3
#define MISO		PINB4
#define SCK			PINB5

//Operation_Mode = 0 (Slave); Operation_Mode = 1 (Master)
#define Operation_Mode		1

/* SPI_MODE
* CPOL = 0 (SCK is low when idle); CPOL = 1 (SCK is high when idle)
* CPHA = 0 (Data is sampled on leading edge(first edge)); CPHA = 1 (Data is sampled on trailing edge(last edge))
*
* SPI Mode		Leading Edge		Trailing Edge
*	0			Sample (Rising)		Setup (Falling)		CPOL = 0, CPHA = 0
*	1			Setup (Rising)		Sample (Falling)	CPOL = 0, CPHA = 1
*	2			Sample (Falling)	Setup (Rising)		CPOL = 1, CPHA = 0
*	3			Setup (Falling)		Sample (Rising)		CPOL = 1, CPHA = 1
*/
#define SPI_Mode	0

/*SPI_Speed
* SPI_Speed		SCK Frequency
*	0			fosc/4
*	1			fosc/16
*	2			fosc/64
*	3			fosc/128
*	4			fosc/2
*	5			fosc/8
*	6			fosc/32
*	7			fosc/64
*/
#define SPI_Speed		1

//SPIE = 0 (Disables SPI interrupt); SPIE = 1 (Enables SPI interrupt)
#define SPI_interrupt	0

//DORD = 0 (MSB transmitted first); DORD = 1 (LSB transmitted first)
#define Data_Format		0


/*****************************************************************
					FUNCTION DECLERATIONS
******************************************************************/

/*******************************************************************
* Description : Initialize all the above setting and enables SPI
*******************************************************************/
void SPI_init(void);

/*******************************************************************
* Description : Pulls slave select line to high (1)
*******************************************************************/
void SS_high(void);

/*******************************************************************
* Description : Pulls slave select line to low (0)
*******************************************************************/
void SS_low(void);

/****************************************************************************
* Description : Transmits the data in SPDR register via SPI and reads
*				received data from SPDR after end of transmission
*
* Returns : Received data from SPDR after end of transmission (SPI_Read_Write)
****************************************************************************/
unsigned char SPI_Read_Write(unsigned char data);


/**************************************************
			FUNCTION DEFINATIONS
**************************************************/

void SPI_init(){
	//Master Mode
	if(Operation_Mode == 1){
		DDR_SPI |= ((1<<MOSI) | (1<<SCK) | (1<<SS));
		DDR_SPI &= ~(1<<MISO);
		SPCR |= (1<<MSTR);
	}
	
	//Slave Mode
	if (Operation_Mode == 0){
		DDR_SPI |= (1<<MISO);
		DDR_SPI &= ~((1<<MOSI) | (1<<SCK) | (1<<SS));
		SPCR &= ~(1<<MSTR);
	}
	
	//SPI Speed
	char temp;
	temp = (0x04 & SPI_Speed);			// masking SPI2X bit
	if(temp == 0x04) SPSR |= (1<<SPI2X);
	if(temp == 0x00) SPSR &= ~(1<<SPI2X);
	temp = (0x03 & SPI_Speed);			// masking SPR1 and SPR0 bits
	//Updating value of SPR1 and SPR0 in SPCR reg
	if(temp == 0x00) SPCR |= temp;
	if(temp == 0x01) SPCR |= temp;
	if(temp == 0x02) SPCR |= temp;
	if(temp == 0x03) SPCR |= temp;
		
	//Data Modes
	if (SPI_Mode == 0) SPCR &= ~((1<<CPOL) | (1<<CPHA));
	if (SPI_Mode == 1){
		SPCR &= ~(1<<CPOL);
		SPCR |= (1<<CPHA);
	}
	if (SPI_Mode == 2){
		SPCR |= (1<<CPOL);
		SPCR &= ~(1<<CPHA);
	}
	if (SPI_Mode == 3) SPCR |= ((1<<CPOL) | (1<<CPHA));
		
	//SPI interrupt
	if(SPI_interrupt == 0) SPCR &= ~(1<<SPIE);
	if(SPI_interrupt == 1) SPCR |= (1<<SPIE);
	
	//Data Format
	if(Data_Format == 0) SPCR &= ~(1<<DORD);
	if(Data_Format == 1) SPCR |= (1<<DORD);
	
	//Enable SPI
	SPCR |= (1<<SPE);
}

void SS_high(){
	SPI_Pull |= (1<<SS);
}

void SS_low(){
	SPI_Pull &= ~(1<<SS);
}

unsigned char SPI_Read_Write(unsigned char data){
	//Transmission starts as soon as data is put in SPDR
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

#endif /* SPI_H_ */