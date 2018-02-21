/*
 * nrf_mnemonics.h
 *
 * Copyright (C) 2017 by Harsh Singhal.
 * Modification or use of this software in source or binary form
 * is permitted as long as files maintain this copyright.
 * This is completely for educational purposes and not for commercial use.
 *
 * Created: 07-11-2017 14:49:09
 * Author: Harsh Singhal
 */

#ifndef NRF_MNEMONICS_H_
#define NRF_MNEMONICS_H_

#include "nrf24l01.h"

//COMMANDS
#define R_REGISTER			0x00		//Read command (0x00 + mnemonics)
#define W_REGISTER			0x20		//Write command (0x20 + mnemonics)

#define R_RX_PAYLOAD			0x61		//Read from RX FIFO
#define W_TX_PAYLOAD			0xA0		//Write in TX FIFO

#define FLUSH_TX			0xE1		//Flush or remove data from TX FIFO
#define FLUSH_RX			0xE2		//FLush or remove data from RX FIFO

#define REUSE_TX_PL			0xE3		//Reuse data from last transmission in TX FIFO

#define R_RX_PL_WID			0x60		//Calculate data width in RX FIFO

#define	W_ACK_PAYLOAD			0xA8		//TX FIFO for ACK payload	(can be from 0xA8 to 0xAD (data pipe 0 to data pipe 5)) //default at data pipe 0
#define W_TX_PAYLOAD_NOACK		0xB0		//Disables AUTOACK on specific packet

#define NOP				0xFF		//No Operation. Used to read status or multi bytes

/*Mnemonics*/
#define	CONFIG					0x00
#define EN_AA					0x01
#define EN_RXADDR				0x02
#define SETUP_AW				0x03
#define SETUP_RETR				0x04
#define RF_CH					0x05
#define RF_SETUP				0x06
#define STATUS					0x07
#define OBSERVE_TX				0X08
#define RPD						0x09
#define RX_ADDR_P0				0x0A
#define RX_ADDR_P1				0x0B
#define RX_ADDR_P2				0x0C
#define RX_ADDR_P3				0x0D
#define RX_ADDR_P4				0x0E
#define RX_ADDR_P5				0x0F
#define TX_ADDR					0x10
#define RX_PW_P0				0x11
#define RX_PW_P1				0x12
#define RX_PW_P2				0x13
#define RX_PW_P3				0x14
#define RX_PW_P4				0x15
#define RX_PW_P5				0x16
#define FIFO_STATUS				0x17
#define DYNPD					0x1C
#define FEATURE					0x1D

/***************************DO NOT MODIFY THESE***************************************/

#define DDR_high				Cont_DDR |= ((1<<CE) | (1<<CSN))	//CE and CSN as output
#define DDR_low					Cont_DDR &= ~(1<<IRQ)			//IRQ as Input (comment it out if you want to use IRQ)

#define CE_low					Cont_Pull &= ~(1<<CE)				//disables transmission
#define CE_high					Cont_Pull |= (1<<CE)				//enables transmission

#define CSN_low					Cont_Pull &= ~(1<<CSN)				//enables communication with nrf
#define CSN_high				Cont_Pull |= (1<<CSN)				//disables communication with nrf

#define IRQ_low					Cont_Pull &= ~(1<<IRQ)
/************************************************************************************/

#endif /* NRF_MNEMONICS_H_ */