/*******************************************************************************
* Company: Shin Young Semicon
*
* File: spi_sd.c
* File history:
*      Revision: 1.0 Date: July 9, 2015
*
* Description:
*
* Device driver for the on-board SPI FRAM and uSD for KoreaEnE V8K board
* Author: Antony. Han
*
*******************************************************************************/

#include "stdio.h"
#include "mss_spi.h"
#include "spi.h"

#define SPI_INSTANCE    &g_mss_spi0
/* Configuration for the SPI Flash */



/******************************************************************************
 *For more details please refer the spi_sd.h file
 ******************************************************************************/
void spi_sd_init( void )
{
    /*--------------------------------------------------------------------------
     * Configure MSS_SPI.
     */
    MSS_SPI_init( SPI_INSTANCE );
    MSS_SPI_configure_master_mode
    (
        SPI_INSTANCE,
        MSS_SPI_SLAVE_0,
        MSS_SPI_MODE3,
		8u,		// 2u,
        MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
    );
/*
    MSS_SPI_configure_master_mode
	(
		SPI_INSTANCE,
		MSS_SPI_SLAVE_1,
		MSS_SPI_MODE3,
		2u,
		MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
	);

    MSS_SPI_configure_master_mode
	(
		SPI_INSTANCE,
		MSS_SPI_SLAVE_2,
		MSS_SPI_MODE3,
		2u,
		MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
	);
    
    MSS_SPI_configure_master_mode
	(
		SPI_INSTANCE,
		MSS_SPI_SLAVE_3,
		MSS_SPI_MODE3,
		2u,
		MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
	);
*/
    MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

}


