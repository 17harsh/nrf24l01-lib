/*
 * nrf24l01.h
 *
 * Copyright (C) 2017 by Harsh Singhal.
 * Modification or use of this software in source or binary form
 * is permitted as long as files maintain this copyright.
 * This is completely for educational purposes and not for commercial use.
 *
 * Created: 07-11-2017 14:49:09
 * Author: Harsh Singhal
 */

Version V1

* This is standard library for nrf24l01+ modulefor ATmel AVRs.
* Use three files that is nrf24l01.h, SPI.h and nrf_mnemonics.h in root directory of project (where there is main.c) to use this library
* Supports all features but tested only for 1 Byte data only
* Don't forget to add #include "nrf24l01.h" in your main.c in order to use this library.

Version V1.01 (10-12-2017)

* Minor Bug fixes and code optimization.

Version V1.02 (21-2-2018)

* Minor Bug fixes and code optimization.

Version V1.10 (30-7-2018)

* Minor Bug Fixes
* Separated nrf_receive function (with ACK and without ACK) from nrf_receive_ackpayload function (ACK with payload)