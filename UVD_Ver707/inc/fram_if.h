/*
 * fram_if.h
 *
 *  Created on: 2015. 4. 14.
 *      Author: chanii
 */

#ifndef FRAM_IF_H_
#define FRAM_IF_H_

#define MSS_SPI_SLAVE_FRAM		MSS_SPI_SLAVE_0

#define FRAM_PRESET1_MODE_ADDR	(0x10)
#define FRAM_PRESET2_MODE_ADDR	(0x14)
#define FRAM_EVENT_SERIAL_ADDR	(0x18)
#define FRAM_ZIGBEE_SERIAL_ADDR	(0x1C)
#define FRAM_ALARM_SERIAL_ADDR	(0x20)

#define FRAM_ALM_START_ADDR	    (0x100)
#define FRAM_PARAM_START_ADDR	(1024*1)	// size : 70*5*4 = 1400
#define FRAM_LTD_START_ADDR	    (1024*3)	// size : about 250
#define FRAM_TOTAL_START_ADDR	(0xE00)

// ============================================================
// 64K bit Ferroelectric Nonvolatile RAM
// Organized as 8,192 x 8 bits
// 0x0000 ~ 0x1fff
// ============================================================
#define FRAM_INFORM					(0)		
#define FRAM_INFORM_DATA_ADDR		(0x100)	
//#define FRAM_INFORM_DATA_SIZE		(100)		// 15 x 4
#define FRAM_INFORM_DATA_SIZE		(36)		// 32 + 4

#define FRAM_LAMP_TIME				(1)		
#define FRAM_LAMP_TIME_DATA_ADDR	(0x200)	
//#define FRAM_LAMP_TIME_DATA_SIZE	(100)		// 15 x 4
#define FRAM_LAMP_TIME_DATA_SIZE	(8)		// 2 x 4

#define FRAM_WAVE					(2)		
#define FRAM_WAVE_DATA_ADDR			(0x300)	
//#define FRAM_WAVE_DATA_SIZE			(100)		// 15 x 4
#define FRAM_WAVE_DATA_SIZE			(36)		// 9 x 4


//void FRAM_read_status( void );
/*F07.MDL001*/void FRAM_WriteEnable( void );
void FRAM_WriteDisable( void );
/*F07.MDL002*/void FRAM_WriteData( uint16_t StartAddr,uint8_t * pWrData, uint16_t Len );
/*F07.MDL003*/void FRAM_ReadData( uint16_t StartAddr,uint8_t * pRdData, uint16_t Len );
/*F07.MDL004*/void FRAM_WriteWord( uint16_t StartAddr,uint32_t pWrWord );
/*F07.MDL005*/uint32_t FRAM_ReadWord( uint16_t StartAddr );

/*F07.MDL006*/uint32_t FRAM_ReadEventLogSerial(void);
/*F07.MDL007*/uint32_t FRAM_IncEventLogSerial(void);
/*F07.MDL008*/void FRAM_WriteAlarmList(uint8_t alm);
/*F07.MDL009*/uint8_t FRAM_ReadAlarmList(uint8_t Num);
/*F07.MDL010*/void FRAM_ClearAlarmList(void);
/*F07.MDL011*/void FRAM_test1(void);

void fram_Inform_Save(void);
void fram_Inform_Load(void);
void fram_LampTime_Save(void);
void fram_LampTime_Load(void);
void fram_Wave_Reset(void);
void fram_Wave_Save(void);
void fram_Wave_Load(void);



#endif /* FRAM_IF_H_ */
