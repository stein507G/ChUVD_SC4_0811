/*
 * fpga_if.c
 *
 *  Created on: 2015. 4. 8.
 *      Author: chanii
 */

#include "config.h"


// FPGA 영역에 1Byte를 쓴다.
void FPGA_Write(uint32_t addr,unsigned char data)
{
	 *(volatile unsigned char*) (FPGA_BASE_ADDR + (addr<<2)) = data;
}

// FPGA 영역에 1word를 쓴다.
void FPGA_WriteWord(uint32_t addr,uint32_t data)
{
	 *(volatile uint32_t *) (FPGA_BASE_ADDR + (addr<<2)) = data;
}

// FPGA 영역에서 1Byte를 읽어온다.
unsigned char FPGA_Read(uint32_t addr)
{
	return *(volatile unsigned char*) (FPGA_BASE_ADDR + addr);
}

uint32_t FPGA_ReadWord(uint32_t addr)
{
	return *(volatile uint32_t *) (FPGA_BASE_ADDR + (addr<<2));
}

uint8_t Read_SWITCH(void)
{
	uint8_t dsw;
	dsw = (~(uint8_t)FPGA_READ_WORD(0x01))&0xF;

	return dsw;
}

void SetTestPinSel(uint8_t sel)
{
/*
// FPGA Source
  case TP_SEL is
      when X"0" =>  TP_OUT <= TP_SPI;
      when X"1" =>  TP_OUT <= TXTP ;
      when X"2" =>  TP_OUT <= RXTP ;
      when X"3" =>  TP_OUT <= RXTP(3 downto 0) & TXTP(3 downto 0) ;
      when X"4" =>  TP_OUT <= ADC_DAC_TP ;
      when X"F" =>  TP_OUT <= Valve; -- deault value
      when Others => TP_OUT <= TP_SPI;
  end case;
*/

	FPGA_WRITE_WORD(4,sel);
}


void SetValve(uint8_t val)
{
	FPGA_WRITE_WORD(0x10,val);
}




