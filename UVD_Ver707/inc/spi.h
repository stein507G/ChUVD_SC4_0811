/*******************************************************************************
* Company: Shin Young Semicon
*
* File: spi.c
* File history:
*      Revision: 1.0 Date: July 9, 2015
*
* Description:
*
* Device driver for the on-board SPI FRAM and uSD for KoreaEnE V8K board
* Author: Antony. Han
*
*******************************************************************************/

#ifndef __SPI_H_
#define __SPI_H_

#include <stdint.h>
#include <stdlib.h>

/* Configuration for the SPI Flash */

/*******************************************************************************
 * This function initialzes the SPI pripheral and PDMA for data transfer
 ******************************************************************************/
void spi_sd_init( void);

#endif
