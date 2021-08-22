/*
 * fpga_if.h
 *
 *  Created on: 2017. 2. 2.
 *      Author: Han
 */

#ifndef FPGA_IF_H_
#define FPGA_IF_H_

#define FPGA_BASE_ADDR   		(0x50000000U)

#define FPGA_WRITE_WORD(X,Y)		 *(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2)) = (Y)

#define FPGA_READ_WORD(X)				*(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2))

//======================================================================================================
// MASTER - MAIN Control SoC
//======================================================================================================
#define FPGA_LCD_POWER_CON(Y)		 *(volatile uint32_t *) (FPGA_BASE_ADDR + ((0xB)<<2)) = (Y)
#define FPGA_RASP_POWER_CON(Y)		 *(volatile uint32_t *) (FPGA_BASE_ADDR + ((0xC)<<2)) = (Y)
#define FPGA_TRANS_POW_CON(Y)		 *(volatile uint32_t *) (FPGA_BASE_ADDR + ((0xD)<<2)) = (Y)

#define FPGA_START_ENABLE(X)			FPGA_WRITE_WORD(0x0007, (X))	// 키보드 입력을 받을 수 있도록

#define FPGA_KEY_READ						FPGA_READ_WORD(0x02)

//(+)210629 CJKIM, 컴파일 시 Redefined warning , Not used
//	#define REMOTE_READY_OUT_CONTACT_OPEN		FPGA_WRITE_WORD(0x0005, 1)
//	#define REMOTE_READY_OUT_CONTACT_CLOSE		FPGA_WRITE_WORD(0x0005, 0)

#define REMOTE_START_OUT_TIME_MS_DATA(X)	FPGA_WRITE_WORD(0x0011, (X))	// 시간 설정 msec
#define REMOTE_START_OUT_CS					FPGA_WRITE_WORD(0x0010, 1)		// Start out

#define REMOTE_START_OUT_TIME_MS(X)			FPGA_WRITE_WORD(0x0011, (X)); FPGA_WRITE_WORD(0x0010, 1)		// 시간 설정 및 out

#define LED_READY_ON		FPGA_WRITE_WORD(0x12,1)
#define LED_READY_OFF		FPGA_WRITE_WORD(0x12,0)
#define LED_READY_ON_0_5HZ		FPGA_WRITE_WORD(0x12,0x02)		// (010 : 0_5hz)  (011: 1Hz) (100 : 2_5Hz) (101 : 2_5Hz)
#define LED_READY_ON_1HZ		FPGA_WRITE_WORD(0x12,0x03)
#define LED_READY_ON_2_5HZ		FPGA_WRITE_WORD(0x12,0x04)
#define LED_READY_ON_5HZ		FPGA_WRITE_WORD(0x12,0x05)

#define LED_RUN_ON			FPGA_WRITE_WORD(0x13,1)
#define LED_RUN_OFF			FPGA_WRITE_WORD(0x13,0)
#define LED_RUN_ON_0_5HZ		FPGA_WRITE_WORD(0x13,0x02)
#define LED_RUN_ON_1HZ		FPGA_WRITE_WORD(0x13,0x03)
#define LED_RUN_ON_2_5HZ		FPGA_WRITE_WORD(0x13,0x04)
#define LED_RUN_ON_5HZ		FPGA_WRITE_WORD(0x13,0x05)

#define LED_ERROR_ON		FPGA_WRITE_WORD(0x14,1)
#define LED_ERROR_OFF		FPGA_WRITE_WORD(0x14,0)
#define LED_ERROR_ON_0_5HZ		FPGA_WRITE_WORD(0x14,0x02)
#define LED_ERROR_ON_1HZ		FPGA_WRITE_WORD(0x14,0x03)
#define LED_ERROR_ON_2_5HZ		FPGA_WRITE_WORD(0x14,0x04)
#define LED_ERROR_ON_5HZ		FPGA_WRITE_WORD(0x14,0x05)

#define LED_DUMMY_ON		FPGA_WRITE_WORD(0x15,1)
#define LED_DUMMY_OFF		FPGA_WRITE_WORD(0x15,0)
#define LED_DUMMY_ON_0_5HZ		FPGA_WRITE_WORD(0x15,0x02)
#define LED_DUMMY_ON_1HZ		FPGA_WRITE_WORD(0x15,0x03)
#define LED_DUMMY_ON_2_5HZ		FPGA_WRITE_WORD(0x15,0x04)
#define LED_DUMMY_ON_5HZ		FPGA_WRITE_WORD(0x15,0x05)

//======================================================================================================
// Detector Signal write
#define DET1_COMMAND_WRITE_DATA(Y)			FPGA_WRITE_WORD(0x1000,(Y)) // <=========== Detector CMD
#define DET2_COMMAND_WRITE_DATA(Y)			FPGA_WRITE_WORD(0x1001,(Y)) // <=========== Detector CMD
#define DET3_COMMAND_WRITE_DATA(Y)			FPGA_WRITE_WORD(0x1002,(Y)) // <=========== Detector CMD

// Detector Signal read
//#define DET_SIGNAL_READ_DATA(X,Y)				FPGA_READ_WORD(0x1040 + ((X)<<1)) // Det1=0 ~ Det3=2  <=========== Frame Data
#define DET1_SIGNAL_READ_DATA(X)				FPGA_READ_WORD(0x1040 + ((X)<<1)) // <=========== Frame Data
#define DET1_SIGNAL_READ_CHECKSUM(X)		FPGA_READ_WORD(0x1041 + ((X)<<1)) // <=========== Frame checksum

#define DET2_SIGNAL_READ_DATA(X)				FPGA_READ_WORD(0x1080 + ((X)<<1)) // <=========== Frame Data
#define DET2_SIGNAL_READ_CHECKSUM(X)		FPGA_READ_WORD(0x1081 + ((X)<<1)) // <=========== Frame checksum

#define DET3_SIGNAL_READ_DATA(X)				FPGA_READ_WORD(0x10C0 + ((X)<<1)) // <=========== Frame Data
#define DET3_SIGNAL_READ_CHECKSUM(X)		FPGA_READ_WORD(0x10C1 + ((X)<<1)) // <=========== Frame checksum


//======================================================================================================
// SLAVE - HEATER Control SoC
//======================================================================================================
#define OVEN_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4000, (X));	// pwm	oven  (0~6)

/*
#define INJ1_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4001, (X));	// pwm	Ini1  (0~255)
#define INJ2_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4002, (X));	// pwm	Ini2  (0~255)
#define INJ3_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4003, (X));	// pwm	Ini3  (0~255)
*/
#define INJ_HEATER_POWER(WHO,X)		FPGA_WRITE_WORD(0x4000 + (WHO), (X));	// pwm	Inj  (0~255)

/*
#define DET1_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4004, (X));	// pwm	 (0~255)
#define DET2_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4005, (X));	// pwm	  (0~255)
#define DET3_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4006, (X));	// pwm	  (0~255)
*/
#define DET_HEATER_POWER(WHO,X)		FPGA_WRITE_WORD(0x4000 + (WHO), (X));	// pwm	 (0~255)

/*
#define AUX1_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4007, (X));	// pwm	 (0~255)
#define AUX2_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4008, (X));	// pwm	  (0~255)
#define AUX3_HEATER_POWER(X)		FPGA_WRITE_WORD(0x4009, (X));	// pwm	  (0~255)
#define AUX4_HEATER_POWER(X)		FPGA_WRITE_WORD(0x400A, (X));	// pwm	  (0~255)
#define AUX5_HEATER_POWER(X)		FPGA_WRITE_WORD(0x400B, (X));	// pwm	 (0~255)
#define AUX6_HEATER_POWER(X)		FPGA_WRITE_WORD(0x400C, (X));	// pwm	  (0~255)
#define AUX7_HEATER_POWER(X)		FPGA_WRITE_WORD(0x400D, (X));	// pwm	  (0~255)
#define AUX8_HEATER_POWER(X)		FPGA_WRITE_WORD(0x400E, (X));	// pwm	  (0~255)
*/
#define AUX_HEATER_POWER(WHO,X)		FPGA_WRITE_WORD(0x4000 + (WHO), (X));	// pwm	 (0~255)

// FPGA_WRITE_WORD(0x4023, 0x0003);	// Motor Con  --> bit3 : Heater Rel Con , bit2 : Oven rel , bit1 : BLDC Dir  bit0 : Flap Motor En
#define HEATER_REL_MASK	0xfff7	
#define HEATER_REL_FLAG_ON 0x0008
#define HEATER_REL_FLAG_OFF 0x0000

#define OVEN_REL_MASK	0xfffb	
#define OVEN_REL_FLAG_ON 0x0004
#define OVEN_REL_FLAG_OFF 0x0000

#define BLDC_DIR_MASK 	0xfffd
#define BLDC_DIR_FLAG_CW 	0x0002
#define BLDC_DIR_FLAG_CCW 	0x0000

#define FLAP_MOTOR_CS_MASK 0xfffe
#define FLAP_MOTOR_CS_FLAG_HIGH 0x0001
#define FLAP_MOTOR_CS_FLAG_LOW 0x0000

#define OVEN_HEATER_REL_CONTROL		*(volatile uint32_t *) (FPGA_BASE_ADDR + ((0x4023)<<2)) = heatDevStt.HeaterRelConState
//#define HEATER_REL_CONTROL	FPGA_WRITE_WORD(0x4023, heatDevStt.HeaterRelConState)

#define OVEN_REL_ON	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & OVEN_REL_MASK) | OVEN_REL_FLAG_ON; OVEN_HEATER_REL_CONTROL
#define OVEN_REL_OFF	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & OVEN_REL_MASK) | OVEN_REL_FLAG_OFF; OVEN_HEATER_REL_CONTROL

#define OVEN_ENABLE_CS 		FPGA_WRITE_WORD(0x4025, 0xffff)	// Oven enable cs

#define HEATER_REL_ON	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & HEATER_REL_MASK) | HEATER_REL_FLAG_ON; OVEN_HEATER_REL_CONTROL
#define HEATER_REL_OFF	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & HEATER_REL_MASK) | HEATER_REL_FLAG_OFF; OVEN_HEATER_REL_CONTROL

#define BLDC_DIR_CW	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & BLDC_DIR_MASK) | BLDC_DIR_FLAG_CW; OVEN_HEATER_REL_CONTROL
#define BLDC_DIR_CCW	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & BLDC_DIR_MASK) | BLDC_DIR_FLAG_CCW; OVEN_HEATER_REL_CONTROL

//#define FLAP_MOTOR_CS_HIGH	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & FLAP_MOTOR_CS_MASK) | FLAP_MOTOR_CS_FLAG_HIGH; OVEN_HEATER_REL_CONTROL
//#define FLAP_MOTOR_CS_LOW	heatDevStt.HeaterRelConState = (heatDevStt.HeaterRelConState & FLAP_MOTOR_CS_MASK) | FLAP_MOTOR_CS_FLAG_LOW; OVEN_HEATER_REL_CONTROL
#define FLAP_MOTOR_nCS 		FPGA_WRITE_WORD(0x4027, 0x0)	// 

#define BLDC_POWER(X)		FPGA_WRITE_WORD(0x400F, (X))	// pwm	 (0~255)
#define BLDC_POWER_OFF		BLDC_DIR_CW
#define BLDC_POWER_ON			BLDC_DIR_CCW

#define FPGA_FLAP_POWER(X)			heaterAct.flapPos = (X); FPGA_WRITE_WORD(0x4022,(X)); FLAP_MOTOR_nCS
//	FPGA_WRITE_WORD(0x4023, 0x0002);	// Motor Con  --> bit3 : Heater Rel Con , bit2 : Oven rel , bit1 : BLDC Dir  bit0 : Flap Motor En
//	FPGA_WRITE_WORD(0x4023, 0x0003);	// Motor Con  --> bit3 : Heater Rel Con , bit2 : Oven rel , bit1 : BLDC Dir  bit0 : Flap Motor En

//===================================================
// VALVE Control
//===================================================
// VALVE_POS_A <= VALVE_ON
//  AIR_ACT_VAL1 - Low
//  AIR_ACT_VAL1_B - High
// VALVE_POS_B <= VALVE_OFF
//  AIR_ACT_VAL1 - High
//  AIR_ACT_VAL1_B - Low

#define VALVE1_FLAG_MASK	0xFEFE
#define VALVE1_FLAG_ON		0x0100
#define VALVE1_FLAG_OFF		0x0001
#define VALVE1_FLAG_POSA	VALVE1_FLAG_ON
#define VALVE1_FLAG_POSB	VALVE1_FLAG_OFF

#define VALVE2_FLAG_MASK	0xFDFD
#define VALVE2_FLAG_ON		0x0200
#define VALVE2_FLAG_OFF		0x0002
#define VALVE2_FLAG_POSA	VALVE2_FLAG_ON
#define VALVE2_FLAG_POSB	VALVE2_FLAG_OFF

#define VALVE3_FLAG_MASK	0xFBFB
#define VALVE3_FLAG_ON		0x0400
#define VALVE3_FLAG_OFF		0x0004

#define VALVE4_FLAG_MASK	0xF7F7
#define VALVE4_FLAG_ON		0x0800
#define VALVE4_FLAG_OFF		0x0008

#define VALVE5_FLAG_MASK	0xEFEF
#define VALVE5_FLAG_ON		0x1000
#define VALVE5_FLAG_OFF		0x0010

#define VALVE6_FLAG_MASK	0xDFDF
#define VALVE6_FLAG_ON		0x2000
#define VALVE6_FLAG_OFF		0x0020

#define VALVE7_FLAG_MASK	0xBFBF
#define VALVE7_FLAG_ON		0x4000
#define VALVE7_FLAG_OFF		0x0040

#define VALVE8_FLAG_MASK	0x7F7F
#define VALVE8_FLAG_ON		0x8000
#define VALVE8_FLAG_OFF		0x0080

#define VALVE_ON_OFF_CONTROL	*(volatile uint32_t *) (FPGA_BASE_ADDR + ((0x10)<<2)) = heatDevStt.ValveStateData

#define VALVE1_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE1_FLAG_MASK) | VALVE1_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE1_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE1_FLAG_MASK) | VALVE1_FLAG_OFF;	VALVE_ON_OFF_CONTROL

#define VALVE2_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE2_FLAG_MASK) | VALVE2_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE2_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE2_FLAG_MASK) | VALVE2_FLAG_OFF;	VALVE_ON_OFF_CONTROL

#define VALVE3_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE3_FLAG_MASK) | VALVE3_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE3_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE3_FLAG_MASK) | VALVE3_FLAG_OFF;	VALVE_ON_OFF_CONTROL

#define VALVE4_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE4_FLAG_MASK) | VALVE4_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE4_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE4_FLAG_MASK) | VALVE4_FLAG_OFF;	VALVE_ON_OFF_CONTROL

#define VALVE5_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE5_FLAG_MASK) | VALVE5_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE5_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE5_FLAG_MASK) | VALVE5_FLAG_OFF;	VALVE_ON_OFF_CONTROL

#define VALVE6_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE6_FLAG_MASK) | VALVE6_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE6_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE6_FLAG_MASK) | VALVE6_FLAG_OFF;	VALVE_ON_OFF_CONTROL

#define VALVE7_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE7_FLAG_MASK) | VALVE7_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE7_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE7_FLAG_MASK) | VALVE7_FLAG_OFF;	VALVE_ON_OFF_CONTROL

#define VALVE8_ON heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE8_FLAG_MASK) | VALVE8_FLAG_ON;	VALVE_ON_OFF_CONTROL
#define VALVE8_OFF  heatDevStt.ValveStateData = (heatDevStt.ValveStateData & VALVE8_FLAG_MASK) | VALVE8_FLAG_OFF;	VALVE_ON_OFF_CONTROL

//#define FPGA_VALVE_CON_ON(PORT_NO) 		ValveConData = ValveConData | (0x0001 << PORT_NO); FPGA_VALVE_CON(ValveConData)
//#define FPGA_VALVE_CON(Y)		 *(volatile uint32_t *) (FPGA_BASE_ADDR + ((0x10)<<2)) = (Y)
//#define FPGA_VALVE_CON1(Y)		 *(volatile uint32_t *) (FPGA_BASE_ADDR + ((0x10)<<2)) = (Y)

//#define FPGA_VALVE_CON_ON(PORT_NO) 		ValveConData = ValveConData | (0x0001 << PORT_NO); FPGA_VALVE_CON(ValveConData)
//#define FPGA_VALVE_CON_OFF(PORT_NO) 	ValveConData = ValveConData | (0x0001 << PORT_NO);

void FPGA_Write(uint32_t addr,unsigned char data);
void FPGA_WriteWord(uint32_t addr,uint32_t data);
unsigned char FPGA_Read(uint32_t addr);
uint32_t FPGA_ReadWord(uint32_t addr);
uint8_t Read_SWITCH(void);
void SetTestPinSel(uint8_t sel);
void SetValve(uint8_t val);


#endif /* FPGA_IF_H_ */
