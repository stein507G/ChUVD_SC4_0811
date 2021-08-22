/*
 * fram_if.c
 *
 *  Created on: 2015. 4. 14.
 *      Author: chanii
 */
#include "config.h"

#define READ_STATUS         0x05
#define READY_BIT_MASK      0x01
#define DONT_CARE       0

#define WREN         (0x06) // Set Write Enable Latch
#define WRDI         (0x04) // Write Disable
#define RDSR         (0x05) // Read Status Register
#define WRSR         (0x01) // Write Status Register
#define READ         (0x03) // Read Memory Data
#define WRITE        (0x02) // Write Memory Data

uint8_t fram_buf[1024*3];

/*******************************************************************************
 * Poll the FLASH device until the current operation is complete.
 */
static void wait_ready(void)
{
 //   uint8_t ready_bit;
 //   uint8_t command = READ_STATUS;
    uint32_t timeout=0xfff;
    while(timeout)
    {
    	timeout--;
    };
/*
    do {
        MSS_SPI_transfer_block(&g_mss_spi0, &command, sizeof(command), &ready_bit, sizeof(ready_bit));
        ready_bit = ready_bit & READY_BIT_MASK;
        timeout++;
        if(timeout > 0xfff) break;
    } while(ready_bit == 1);
    */
}

/*******************************************************************************
 * Read block of bytes from FLASH device into RAM.
 */
void FRAM_read
(
    uint32_t address,
    uint8_t * rx_buffer,
    size_t size_in_bytes
)
{
    uint8_t cmd_buffer[6];

    //cmd_buffer[0] = READ_ARRAY_OPCODE;
    cmd_buffer[1] = (uint8_t)((address >> 16) & 0xFF);
    cmd_buffer[2] = (uint8_t)((address >> 8) & 0xFF);;
    cmd_buffer[3] = (uint8_t)(address & 0xFF);
    //cmd_buffer[4] = DONT_CARE;
    //cmd_buffer[5] = DONT_CARE;

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
    wait_ready();
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, sizeof(cmd_buffer), rx_buffer, size_in_bytes);
    wait_ready();
    MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

}

/*
void FRAM_read_status( void )
{
    uint8_t cmd_buffer[6];
    uint8_t rx_buffer[6];

    cmd_buffer[0] = RDSR;
    cmd_buffer[1] = DONT_CARE;
    cmd_buffer[2] = DONT_CARE;

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, 1, rx_buffer, 1);

    //printf("FRAM status %02X,%02X\r\n",rx_buffer[0],rx_buffer[1]);

}
*/

void FRAM_WriteEnable( void )
{
    uint8_t cmd_buffer[6];
    uint8_t rx_buffer[6];

    cmd_buffer[0] = WREN;
    cmd_buffer[1] = DONT_CARE;

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
    wait_ready();
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, 1, rx_buffer, 0);
    wait_ready();
    MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
}

void FRAM_WriteDisable( void )
{
    uint8_t cmd_buffer[6];
    uint8_t rx_buffer[6];

    cmd_buffer[0] = WRDI;
    cmd_buffer[1] = DONT_CARE;

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_FRAM);
    wait_ready();
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, 1, rx_buffer, 0);
    wait_ready();
    MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_FRAM);
// ryu		
//wait_ready();		
}

// Len : max value - 255
void FRAM_WriteData( uint16_t StartAddr,uint8_t * pWrData, uint16_t Len )
{
    uint8_t cmd_buffer[1024*3+100];
    uint16_t i;

    cmd_buffer[0] = WRITE;
    cmd_buffer[1] = ((StartAddr>>8)&0xFF);
    cmd_buffer[2] = ((StartAddr>>0)&0xFF);
    for(i=0;i<Len;i++)
    	cmd_buffer[3+i] = pWrData[i];

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
    wait_ready();
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, Len+3 , fram_buf, 0);
    wait_ready();
    MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
}


void FRAM_ReadData( uint16_t StartAddr,uint8_t * pRdData, uint16_t Len )
{
    uint8_t cmd_buffer[6];

    cmd_buffer[0] = READ;
    cmd_buffer[1] = ((StartAddr>>8)&0xFF);
    cmd_buffer[2] = ((StartAddr>>0)&0xFF);
    cmd_buffer[3] = DONT_CARE;

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
    wait_ready();
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, 3, pRdData, Len);
    wait_ready();
    MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
}

/*
 * Write 4byte
 * StartAddr should be word aligned
 */
void FRAM_WriteWord( uint16_t StartAddr,uint32_t pWrWord )
{
    uint8_t cmd_buffer[10];

    cmd_buffer[0] = WRITE;
    cmd_buffer[1] = ((StartAddr>>8)&0xFF);
    cmd_buffer[2] = ((StartAddr>>0)&0xFF);
    cmd_buffer[3] = ((pWrWord>>0)&0xFF);
    cmd_buffer[4] = ((pWrWord>>8)&0xFF);
    cmd_buffer[5] = ((pWrWord>>16)&0xFF);
    cmd_buffer[6] = ((pWrWord>>24)&0xFF);

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
    wait_ready();
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, 7 , fram_buf, 0);
    wait_ready();
    MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
}

uint32_t FRAM_ReadWord( uint16_t StartAddr )
{
	uint32_t pRdWord=0;
    uint8_t cmd_buffer[6];
    uint8_t rx_buffer[6];
    uint8_t i;

    cmd_buffer[0] = READ;
    cmd_buffer[1] = ((StartAddr>>8)&0xFF);
    cmd_buffer[2] = ((StartAddr>>0)&0xFF);
    cmd_buffer[3] = DONT_CARE;

    MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
    wait_ready();
    MSS_SPI_transfer_block(&g_mss_spi0, cmd_buffer, 3, rx_buffer, 4);
    wait_ready();
    MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

    for(i=0;i<4;i++)
    	pRdWord = (pRdWord<<8) + rx_buffer[3-i];

    return pRdWord;
}


// FRAM reserved area (0x00 ~ 0x0F) :  for test, magic code, etc
// this function use 0x00 ~ 0x03 space, increasing value 1
void FRAM_test1(void)
{
    uint8_t rbuf[10]; // read buffer
    uint8_t wrbuf[10];// write read buffer

	FRAM_ReadData(0,rbuf,4);
	DEBUG_I(DBG_LEVEL_INFO, "() FRAM test    : %02X,%02X,%02X,%02X\r\n", rbuf[0], rbuf[1], rbuf[2], rbuf[3]);
	rbuf[0] +=1;
	rbuf[1] +=1;
	rbuf[2] +=1;
	rbuf[3] +=1;
	FRAM_WriteEnable();
	FRAM_WriteData(0,rbuf,4);
	FRAM_ReadData(0,wrbuf,4);
	DEBUG_I(DBG_LEVEL_INFO, "() FRAM increase: %02X,%02X,%02X,%02X\r\n", wrbuf[0], wrbuf[1], wrbuf[2], wrbuf[3]);
}

void fram_Inform_Save(void)
{
	uint8_t rwbuf[512];

	
	FRAM_WriteEnable();

	*rwbuf = DATA_SAVE_FLAG;

	memcpy(rwbuf + 1,&gUVD_INFORM,sizeof(gUVD_INFORM));
	FRAM_WriteData(FRAM_INFORM_DATA_ADDR, (uint8_t *) rwbuf, sizeof(gUVD_INFORM)+1 );

	FRAM_WriteDisable();
}

void fram_Inform_Load(void)
{
	YL9020_UVD_INFORM tmp;

	FRAM_ReadData(FRAM_INFORM_DATA_ADDR+1 , (uint8_t *) &tmp,sizeof(gUVD_INFORM));
	memcpy(&gUVD_INFORM, &tmp, sizeof(gUVD_INFORM));
}

void fram_LampTime_Save(void)
{
	uint8_t rwbuf[512];

	
	FRAM_WriteEnable();

	*rwbuf = DATA_SAVE_FLAG;
#if 0
	LampT.fD2LampElapse = (float)detStt.fD2LampElapse*10.;
	LampT.fWLampElapse = (float)detStt.fWLampElapse*10.;
#else
	LampT.uiD2LampElapse = (unsigned int)(detStt.fD2LampElapse*10.);
	LampT.uiWLampElapse = (unsigned int)(detStt.fWLampElapse*10.);
#endif
	memcpy(rwbuf + 1,&LampT,sizeof(LampT));
	FRAM_WriteData(FRAM_LAMP_TIME_DATA_ADDR, rwbuf, sizeof(LampT)+1 );
#ifdef Message
	DEBUG_I(DBG_LEVEL_INFO, "() -- Lamp Time Save --\r\n");
#endif

	FRAM_WriteDisable();
}

void fram_LampTime_Load(void)
{
	UVD_LampTime_Check tmp;

	FRAM_ReadData(FRAM_LAMP_TIME_DATA_ADDR+1, (uint8_t *) &tmp, sizeof(LampT));
	memcpy(&LampT, &tmp, sizeof(LampT));

	detStt.fD2LampElapse = (float)(LampT.uiD2LampElapse)/10.;
	detStt.fWLampElapse = (float)(LampT.uiWLampElapse)/10.;
	gUVD_CONFIG.nD2LampElapse = (unsigned int)detStt.fD2LampElapse;
	gUVD_CONFIG.nWLampElapse = (unsigned int)detStt.fWLampElapse;
#ifdef Message
	DEBUG_I(DBG_LEVEL_INFO, "() Lamp Time Load		D2=0x%x	Wlamp=0x%x\r\n", LampT.uiD2LampElapse, LampT.uiWLampElapse);
#endif
}

void fram_Wave_Reset(void)
{
	DEBUG_I(DBG_LEVEL_INFO, "()\n");

	dWave.wave[0] = 0;
	dWave.wave[1] = 0;
	dWave.wave[2] = 0;
	dWave.GM_wave[0] = 0;
	dWave.GM_wave[1] = 0;
	dWave.GM_wave[2] = 0;
	dWave.fGM_ZeroWaveStep = 0;
	dWave.fGM_486WaveStep = 0;
	dWave.fGM_656WaveStep = 0;

	fram_Wave_Save();

}

void fram_Wave_Save(void)
{
	uint8_t rwbuf[512];

	
	FRAM_WriteEnable();

	*rwbuf = DATA_SAVE_FLAG;
	 
	memcpy(rwbuf + 1,&dWave,sizeof(dWave));
	FRAM_WriteData(FRAM_WAVE_DATA_ADDR, rwbuf, sizeof(dWave)+1 );
#ifdef Message
	DEBUG_I(DBG_LEVEL_INFO, "() -- Wave Save --\r\n");
#endif
	FRAM_WriteDisable();
}
void fram_Wave_Load(void)
{
	UVD_Wave_Check tmp;
	FRAM_ReadData(FRAM_WAVE_DATA_ADDR+1 ,(uint8_t *) &tmp,sizeof(dWave));

	memcpy(&dWave, &tmp, sizeof(dWave));
#ifdef Message
	DEBUG_I(DBG_LEVEL_INFO, "() Wave1 Load		fGM_486WaveStep=%f	fGM_656WaveStep=%f	fGM_ZeroWaveStep=%f\r\n"
		, dWave.fGM_486WaveStep, dWave.fGM_656WaveStep, dWave.fGM_ZeroWaveStep);

	DEBUG_I(DBG_LEVEL_INFO, "() Wave2 Load		GM_wave0=%d	GM_wave1=%d	GM_wave2=%d\r\n"
		, dWave.GM_wave[0], dWave.GM_wave[1], dWave.GM_wave[2]);
	
	DEBUG_I(DBG_LEVEL_INFO, "() Wave3 Load		wave0=%d	wave1=%d	wave2=%d\r\n"
		, dWave.wave[0], dWave.wave[1], dWave.wave[2]);
#endif
}




