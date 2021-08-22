/*
 * UV_Define.h
 *
 *  Created on: 2020. 6. 28.
 *      Author: ksyi
 */
#ifndef __DEFINE_H_
#define __DEFINE_H_

#define FIX_SAM_REF_OFFSET
//#define Timer_1ms
#define Message


#define NO 0
#define YES 1

#define OFF 0
#define ON 1

#define LED_OFF 1
#define LED_ON 0

#define RE_ERR		0
#define RE_OK			1

#define Buzz_OFF 0
#define Buzz_ON 1


//	Common.vhd
//#define PWR_Con_5V_W_Addr	0x0001
#define Tung_OnOff_W_Addr	0x400A
#define D2_Strk_W_Addr		0x400B
#define D2_Ctrl_W_Addr		0x400C
#define LED_W_Addr			0x400D
#define Buzzer_W_Addr		0x400E
#define D2_BD_PWR_W_Addr	0x400F
#define Ready_Out_W_Addr	0x4001
#define Hold_Out_W_Addr		0x4002
#define Mark_Out_W_Addr		0x4003

#define FM_W_Addr		0x1000
#define G_Mot_W_Addr	0x1001
#define DAC_W_Addr		0x3000
#define DAC_L_W_Addr		0x3200
#define DAC_R_W_Addr		0x3400
#define DAC_Run_W_Addr		0x3800


#define AD_RST_Addr	0x5001
#define Heat_Ctrl_Addr	0x6001

// [31:24]Time(0.1ms) / [23:16]Green / [15:8] Red / [7:0] Blue
#define FLED_W_Addr		0x7000
#define FLED_Green_Color		0x05FF0000
#define FLED_Green_Color_0_4sec	0x04FF0000
#define FLED_Green_Color_0_1sec	0x01FF0000
#define FLED_Red_Color			0x0500FF00
#define FLED_Red_Color_0_4sec	0x0400FF00
#define FLED_Red_Color_0_1sec	0x0100FF00
#define FLED_Blue_Color			0x050000FF
#define FLED_Blue_Color_0_4sec	0x040000FF
#define FLED_Blue_Color_0_1sec	0x010000FF
#define FLED_Black_Color		0x05000000
#define FLED_Black_Color_0_4sec	0x04000000
#define FLED_Black_Color_0_1sec	0x01000000

#define FLED_White_Color		0x05ffffFF
#define FLED_White_Color_0_4sec	0x04ffffFF
#define FLED_White_Color_0_1sec	0x01ffffFF
#define FLED_Yellow_Color		0x05040400
#define FLED_Yellow_Color_0_4sec 0x04FFFF00
#define FLED_Yellow_Color_0_1sec 0x01FFFF00



#define FLED_Green			0
#define FLED_Green_Blink	1	// Green/Off blink
#define FLED_Red			2
#define FLED_Red_Blink		3	// Red/Off blink
#define FLED_Blue			4
#define FLED_Blue_Blink		5	// Blue/Off blink
#define FLED_BlueGreen_Blink 6

#define FLED_White			7
#define FLED_Black			8
#define FLED_YellowBlack_Blink 9
#define FLED_YellowGreen_Blink 10
#define FLED_Yellow		11


//#define PWR_CON_OnOff(X)	FPGA_WriteWord(PWR_Con_5V_W_Addr, X)
#define Tungsten_OnOff(X)	FPGA_WriteWord(Tung_OnOff_W_Addr, X)
#define D2_Strk(X)			FPGA_WriteWord(D2_Strk_W_Addr, X)	// ycm
#define D2_Ctrl(X)			FPGA_WriteWord(D2_Ctrl_W_Addr, X)	// ham
#define LED_OnOff(X)		FPGA_WriteWord(LED_W_Addr, X)
#define Buzzer_OnOff(X)		FPGA_WriteWord(Buzzer_W_Addr, X)
#define D2_BD_OnOff(X)		FPGA_WriteWord(D2_BD_PWR_W_Addr, X)
//#define FrontLED_Ctrl(X)	FPGA_WriteWord(FrontLED_W_Addr, X)
//#define ADselect_Ctrl(X)	FPGA_WriteWord(AD_Select_W_Addr, X)

#define FM_Write(X)			FPGA_WriteWord(FM_W_Addr, X)
#define Ready_Out_Ctrl(X)	FPGA_WriteWord(Ready_Out_W_Addr, ~X)
#define Hold_Out_Ctrl(X)	FPGA_WriteWord(Hold_Out_W_Addr, ~X)		// Start Out??
#define Mark_Out_Ctrl(X)	FPGA_WriteWord(Mark_Out_W_Addr, ~X)
#define Dac_L_Write(X)		FPGA_WriteWord(DAC_L_W_Addr, X)
#define Dac_R_Write(X)		FPGA_WriteWord(DAC_R_W_Addr, X)
#define Dac_Run_Write(X)	FPGA_WriteWord(DAC_Run_W_Addr, X)
#define G_Mot_Write(X)		FPGA_WriteWord(G_Mot_W_Addr, X)

#define AD_RST_Write(X)		FPGA_WriteWord(AD_RST_Addr, X)
#define Heat_Ctrl_Write(X)	FPGA_WriteWord(Heat_Ctrl_Addr, X)

#define FLED_LED1_Write(X)		FPGA_WriteWord(FLED_W_Addr+0x6, X)
#define FLED_LED2_Write(X)		FPGA_WriteWord(FLED_W_Addr+0x7, X)
#define FLED_LED3_Write(X)		FPGA_WriteWord(FLED_W_Addr+0x8, X)
#define FLED_LED_Load(X)		FPGA_WriteWord(FLED_W_Addr+0x5, X)

#define BUZZER_CON_ON Buzzer_OnOff(ON)
#define BUZZER_CON_OFF Buzzer_OnOff(OFF)

//(+)210629 CJKIM, 컴파일 시 Redeined warning , Not used
//	#define REMOTE_READY_OUT_CONTACT_CLOSE		Ready_Out_Ctrl(0x0)
//	#define REMOTE_READY_OUT_CONTACT_OPEN		Ready_Out_Ctrl(0x1)

#define REMOTE_START_OUT_CONTACT_CLOSE		Hold_Out_Ctrl(0x0)
#define REMOTE_START_OUT_CONTACT_OPEN		Hold_Out_Ctrl(0x1)

//#define PWR_Button_R_Addr	0x0001
#define PWR_Button2_R_Addr	0x7111
#define PWR_Button_R_Addr	0x7fff
#define FPGA_Ver_R_Addr		0x4000		// FPGA HW Version
#define D2_State_R_Addr		0x4002
#define Thermal_R_Addr		0x4003
#define Leak_R_Addr			0x4004
#define M1_Home_R_Addr		0x4005	// 4pin Graging Home Check	// Cut Filter Motor Home XX
#define M2_Home_R_Addr		0x4006	// Grating Motor Home

#define Ready_IN_R_Addr	0x4007
#define Start_IN_R_Addr	0x4008
#define Mark_IN_R_Addr	0x4009

#define SW1_R_Addr	0x400A
#define SW2_R_Addr	0x400B

#define PWR_BUTTON_READ FPGA_ReadWord(PWR_Button_R_Addr)
#define PWR_BUTTON_IN_READ FPGA_ReadWord(PWR_Button2_R_Addr)
#define FPGA_Ver_READ FPGA_ReadWord(FPGA_Ver_R_Addr)		// FPGA HW Version
#define D2_State_READ FPGA_ReadWord(D2_State_R_Addr)
#define Thermal_READ FPGA_ReadWord(Thermal_R_Addr)
#define Leak_READ FPGA_ReadWord(Leak_R_Addr)
#define Home1_READ FPGA_ReadWord(M1_Home_R_Addr)	// grating
#define Home2_READ FPGA_ReadWord(M2_Home_R_Addr)

#define Ready_IN_READ FPGA_ReadWord(Ready_IN_R_Addr)
#define Start_IN_READ FPGA_ReadWord(Start_IN_R_Addr)
#define Mark_IN_READ FPGA_ReadWord(Mark_IN_R_Addr)

#define SW1_READ FPGA_ReadWord(SW1_R_Addr)
#define SW2_READ FPGA_ReadWord(SW2_R_Addr)

#define AD1_READ FPGA_ReadWord(0x5001)
#define AD2_READ FPGA_ReadWord(0x5002)
#define AD3_READ FPGA_ReadWord(0x5003)

#define UV_Boot		0
#define UV_Oper		1
#define UV_Error	2
#define UV_Buz_1sec	3
#define UV_Buz_OFF	4

	
#define DATA_LOAD			0
#define DATA_SAVE			1	
#define DATA_MODIFY		2	
#define DATA_SAVE_FLAG		0xA5

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// hplc_gpio.c ////////////////////////////
 *///////////////////////////////////////////////////////////////////////

#define ACCEL_STEP_CNT	48

#define RMT_SET					0	// IOCTL_GPIO_RMT_SET
#define RMT_OUT					1	// IOCTL_GPIO_RMT_OUT
#define RMT_Check_ClrSet		2	// IOCTL_REMOTE_CHECK_CLEAR_SET
#define RMT_Read				3	// IOCTL_REMOTE_READ

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// hplc_gpio.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////


#define ENABLE			1
#define DISABLE			0

#define OK				1
#define ERROR			0
#define NOT_CHECK		0xff

#define CONTACT_CLOSE		0
#define CONTACT_OPEN		1
#define CONTACT_PULSE		2
#define CONTACT_IGNORE		3
#define SWITCH_IGNORE 		3


#define SWITCH_OFF		0
#define SWITCH_ON		1
#define SWITCH_PULSE	2


#define LEVEL_LOW		CONTACT_CLOSE
#define LEVEL_HIGH		CONTACT_OPEN

#define RISING_EDGE			1
#define FALLING_EDGE		2

#define FAIL			0

#define TRUE			1
#define FALSE			0

#define PASS			1
#define REJECT			0

#define READY			1
#define NOT_READY		0

#define CW				1
#define CCW				0

#define NONE			0

#define LED_ON			0
#define LED_OFF			1
#define LED_BLINK		2

// UVD Detector setting
#define NEGATIVE		1
#define POSITIVE		0

#define DATA_LOAD		0
#define DATA_SAVE		1

// system define
//#define RS232_RX_BUF	1024

#define COM_BUFF_SIZE		0x400
//#define COM_BUFF_SIZE1		0x400
//#define COM_BUFF_SIZE2		0x800

#define COM_BUFF_CHECK	 	0x3ff
//#define COM_BUFF_CHECK2 	0x7ff

#define MIN_COM_CMD_SIZE	0x06

#define RX_HEAD_CHECK1 	comdata1.rxhead &= COM_BUFF_CHECK
#define RX_TAIL_CHECK1 	comdata1.rxtail &= COM_BUFF_CHECK
#define TX_HEAD_CHECK1 	comdata1.txhead &= COM_BUFF_CHECK
#define TX_TAIL_CHECK1 	comdata1.txtail &= COM_BUFF_CHECK

#define RX_HEAD_CHECK2 	comdata2.rxhead &= COM_BUFF_CHECK
#define RX_TAIL_CHECK2 	comdata2.rxtail &= COM_BUFF_CHECK
#define TX_HEAD_CHECK2 	comdata2.txhead &= COM_BUFF_CHECK
#define TX_TAIL_CHECK2 	comdata2.txtail &= COM_BUFF_CHECK

/*
#define TX_FRONT_CHECK0 	uiTxFront0 &= COM_BUFF_CHECK0
#define TX_FRONT_CHECK1 	uiTxFront1 &= COM_BUFF_CHECK1

//#define RX_FRONT_CHECK0 	uiRxFront0 &= COM_BUFF_CHECK0
#define RX_FRONT_CHECK1 	uiRxFront1 &= COM_BUFF_CHECK1

#define TX_REAR_CHECK0		uiTxRear0 &= COM_BUFF_CHECK0
#define TX_REAR_CHECK1		uiTxRear1 &= COM_BUFF_CHECK1

#define RX_REAR_CHECK0		uiRxRear0 &= COM_BUFF_CHECK0
#define RX_REAR_CHECK1		uiRxRear1 &= COM_BUFF_CHECK1
*/

// customEvent() -- Event Type
#define START_CMD_EVENT			10000
#define STOP_CMD_EVENT			9999

#define THR_PUMP_FUNC_CALL			10001
#define THR_DET_FUNC_CALL			10002
#define THR_OVEN_FUNC_CALL			10003
#define THR_MENU_PAUSE_FUNC_CALL	10004
#define THR_MENU_RESUME_FUNC_CALL	10005
#define THR_MENU_STOP_FUNC_CALL		10006
#define FUNC_DATE_OUTPUT_CALL		20000

//#define FUNC_SEND_OVEN_DATA_CALL	0x3000
//#define FUNC_SEND_PUMP_DATA_CALL	0x9000
//#define FUNC_SEND_DET_DATA_CALL	0x7000

// internal ADC power check
//#define ADC_REF_VOLTAGE		2.5
#define ADC_REF_VOLTAGE		1.25
//#define ADC_FULL_COUNT		1023
//#define ADC_REF_VOLTAGE		5.0
#define ADC_FULL_COUNT		16777215

#define ADC_TEMP_AVERAGE_COUNT	4
#define ADC_TEMP_AVERAGE_SHIFT	2


// ====================================
#define INST_ID_MIN		1
#define INST_ID_MAX		10

// remote mode setting
#define REMOTE_MARK		0
#define REMOTE_START	1
#define REMOTE_READY	2

#define REMOTE_AUZERO	3
#define REMOTE_LAMP		4
#define REMOTE_CHART	5


#define REMOTE_TRIGGER_NONE				0
#define REMOTE_RISING_EDGE_TRIGGER		1
#define REMOTE_FALLING_EDGE_TRIGGER		2
#define REMOTE_LEVEL_TRIGGER_LOW		3
#define REMOTE_LEVEL_TRIGGER_HIGH		4

#define REMOTEIN_TRIGGER_TIME				100 // 100ms
#define REMOTEIN_TRIGGER_CNT				REMOTEIN_TRIGGER_TIME/2	 // 50

#define REMOTEOUT_TRIGGER_TIME				100 // 100ms
#define REMOTEOUT_TRIGGER_CNT				REMOTEOUT_TRIGGER_TIME/50	 // 2

#define MARK_OUT_DEFAULT_LOW	0
#define MARK_OUT_DEFAULT_HIGH	1

#define MARK_OUT_METHOD_LEVEL	0
#define MARK_OUT_METHOD_PULSE	1

#define EXTOUTTIME_DEFAULT	200
#define EXTOUTTIME_MIN		100
#define EXTOUTTIME_MAX		100000

#define MARK_OUT_CONTACT_OPEN			Mark_Out_Ctrl(0x1)
#define MARK_OUT_CONTACT_CLOSE			Mark_Out_Ctrl(0x0)
#define MARK_OUT_HIGH			MARK_OUT_CONTACT_OPEN
#define MARK_OUT_LOW			MARK_OUT_CONTACT_CLOSE
#define MARK_OUT_ON				MARK_OUT_CONTACT_OPEN
#define MARK_OUT_OFF			MARK_OUT_CONTACT_CLOSE

#define START_OUT_CONTACT_OPEN			Hold_Out_Ctrl(0x1)
#define START_OUT_CONTACT_CLOSE			Hold_Out_Ctrl(0x0)
#define START_OUT_HIGH			START_OUT_CONTACT_OPEN
#define START_OUT_LOW			START_OUT_CONTACT_CLOSE
#define START_OUT_ON			START_OUT_CONTACT_OPEN
#define START_OUT_OFF			START_OUT_CONTACT_CLOSE

#define READY_OUT_CONTACT_OPEN			Ready_Out_Ctrl(0x1)
#define READY_OUT_CONTACT_CLOSE			Ready_Out_Ctrl(0x0)
#define READY_OUT_HIGH			READY_OUT_CONTACT_OPEN
#define READY_OUT_LOW			READY_OUT_CONTACT_CLOSE
#define READY_OUT_ON			READY_OUT_CONTACT_OPEN
#define READY_OUT_OFF			READY_OUT_CONTACT_CLOSE

#define EVENT_TABLE_COUNT		101		// array 0번에는 setup 의 initEvent값을 저장하자 <--- software에서는 다르다.
#define EVENT_TIME_MIN		0.0
#define EVENT_TIME_MAX		9999.0

#define PROGRAM_COUNT_MAX	100

#define RUN_STOP		0
#define RUN_START		1
#define RUN_USER_BREAK	2

#define CHECKED			(YES)
#define NOT_CHECKED		(NO)

#define GM_HOME_STEP		10
#define GM_HOME_CHECK_STEP	200
#define GM_FULL_STEP		4000
#define GM_HOME_CHECK_WAIT_CNT	15
#define GM_DUAL_WAIT_CNT	20
//#define GM_DUAL_MOVE_CNT	2000	// 250msec - 다음 파장으로 이동할 시각(cnt)
#define GM_DUAL_MOVE_CNT	4000	// 500msec - 다음 파장으로 이동할 시각(cnt)
#define GM_DUAL_READ_CNT	GM_DUAL_MOVE_CNT - 400	// 50msec = photo data를 읽어야 하는 시각(cnt)

#define FM_STEP_WAIT_CNT	30
//#define FM_HOME_CHECK_WAIT_CNT	250
#define FM_HOME_CHECK_WAIT_CNT	500



/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// h_Func.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////

#define DET_DUAL_PHOTO_DATA_AVE		20

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// def.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////


/***************  Clock Parameter  **********/

#define DET_TEMP_AVE_CNT	8
#define DET_TEMP_FACTOR		3.

#define DEV_DET			0x20

#define FILE_DET_OPERATION_LOG	1
#define FILE_DET_CONFIG_LOG		2
#define FILE_DET_D2LAMP_LOG		3
#define FILE_DET_WLAMP_LOG		4
#define FILE_DET_GM_WAVE_STEP	5

// define
//------------------------------------------------------------------------

//(+)210701 CJKIM, added
// ChroZen UVD	24V / 5V / 3.3V / Tung / X / DA1 / DA2 / Temp
#define ADC_24V				0
#define ADC_5V				1
#define ADC_3_3V			2
#define ADC_TUNGSTEN		3
#define ADC_UNUSED			4
#define ADC_DA1				5
#define ADC_DA2				6
#define ADC_TEMP			7


#define GM_OFF			0
#define GM_FREE_ACT		1
#define GM_SINGLE_ACT	2
#define GM_DUAL_ACT		3
#define GM_SCAN_ACT		4
#define GM_HOME_ACT		5
#define GM_CAL_SCAN_ACT	6

#define CUTFILTER_AUTO	0
#define CUTFILTER_NONE	1
#define CUTFILTER_1ST	2
#define CUTFILTER_2ND	3
#define CUTFILTER_HOLMIUM	4
#define CUTFILTER_HOME		5

#define FM_TEST_ACT		6	// TEST position
#define FM_INSTALL_ACT	7
#define FM_OFF			8

#define FM_POS_NONE		24
#define FM_POS_1ST		48
#define FM_POS_2ND		72
#define FM_HOME_POS		0

#define UVD_DA_RUN_CMD			(0)
#define UVD_LEFT_DATA_SEL		(1)
#define UVD_RIGHT_DATA_SEL		(2)
//#define UVD_ALL_DATA_SEL		(3)
#define UVD_LEFT_DATA_WRITE		(4)
#define UVD_RIGHT_DATA_WRITE	(5)
//#define UVD_ALL_DATA_WRITE		(6)

#define DET_INTERRUPT_HZ		500
#define DET_PHOTO_DATA_AVE				10	// 50Hz로 데이터를 수집한다.

#define DET_TEMP_CONTROL_COUNT		10		// 50 / 10 = 5Hz
//#define DET_POWER_CHECK_COUNT		10		//(+)210629 CJKIM, 10 or 25 ??
//#define DET_STATE_SEND_COUNT		50
//#define SIGNAL_SEND_COUNT	25	// ChroZen
//#define DET_STATE_SEND_COUNT		125
#define DET_STATE_SEND_COUNT		121		//(+)210715 CJKIM, state = 968mSec, Acq Data Interval


#define DET_TIME_CONSTANT_SIG_COUNT	125
#define DET_TIME_CONSTANT_ACQ_ARRAY	1250 //	1250	


/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// Thr_detfunc.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////
#if 0	// 9120 50Hz
#define AUTO_ZERO_OFFSET_DATA_NUM	100	// 2초 data
#define AUTO_ZERO_OFFSET_COUNT		100	// 2초
#define AUTO_ZERO_OFFSET_DUAL_COUNT		150	// 3초
#else	// ChroZen 125Hz
	#define AUTO_ZERO_OFFSET_DATA_NUM	250	// 2초 data
	#define AUTO_ZERO_OFFSET_COUNT		250	// 2초
//	#define AUTO_ZERO_OFFSET_DUAL_COUNT		375	// 3초
#endif
#define ERROR_CHK_ALL				0
#define ERROR_CHK_D2LAMP			1
#define ERROR_CHK_LEAK				2

#define LAMP_USE_TIME_CAL			(1/3600.)	// 시간단위저장
#define LAMP_USE_SAVE_POINT			(900)		// 15분 시점
#define LAMP_USE_SAVE_TIME			(LAMP_USE_SAVE_POINT * 2) // 30분


/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// Det_initfunc.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////
// Diagnostic
#define DIAG_ALL		0x1F
#define DIAG_BKDATA	0x01
#define DIAG_POWER	0x02
#define DIAG_D2		0x04
#define DIAG_W		0x08
#define DIAG_2CUT	0x10

#define SEQ_POWER_CHECK		0
#define SEQ_GRATING_HOME	1
#define SEQ_FILTER_HOME		2
#define SEQ_D2_LAMP_ON		3
#define SEQ_D2_LAMP_CHECK	4
#define SEQ_GRATING_SCANNING	5
#define SEQ_ERROR_OCCUR		6

//#define INIT_NONE			0
#define INIT_LED_BLINK		0
#define INIT_OFFSET_VALUE_SAVE	1
#define INIT_DIAGNOSTIC		2
#define INIT_CALIBRATE		3
#define INIT_BACKLASH_TEST	4
#define INIT_W_LAMP_TEST	5
#define INIT_CUTFILTER_TEST	6
#define INIT_DA_AD_TEST		7
#define INIT_HOLMIUM_TEST	8
#define INIT_NOISE_DRIFT_TEST	9
#define INIT_READYINIT_SET		10

#define OFFSET_VALUE_SAVE_TIME_SEC		4
#define OFFSET_VALUE_COUNT	(OFFSET_VALUE_SAVE_TIME_SEC * 50)	// 9120
//#define OFFSET_VALUE_COUNT	(OFFSET_VALUE_SAVE_TIME_SEC * 125)		// ChroZen
#define OFFSET_VALUE_DELAY_TIME_SEC		5
#define OFFSET_VALUE_DELAY_COUNT	((OFFSET_VALUE_DELAY_TIME_SEC)* 500)

#define WAVE_STEP_486_MIN	1500	//1565
#define WAVE_STEP_486_MAX	1600
#define WAVE_STEP_656_MIN	2100	//2147
#define WAVE_STEP_656_MAX	2200

// zero --- 대략 542 step --> 500-600
// 486nm --- 대략 2100-2200 step	==> (zeroStep + 1560) - 100step
// 656nm --- 대략 2700-2780 step	==> (zeroStep + 2160) - 80step
#define FIND_ZERO_WAVE_MAX	1000		// step
//#define FIND_ZERO_WAVE_MIN	5
#define FIND_ZERO_WAVE_MIN	 100	// 500
#define FIND_WAVE_WAIT_TIME_CNT		 50
#define FIND_WAVE_MAX_STEP		4000

#if 0	// 9120
//#define MAX_PHOTOCNT		15728640	// 0xf0 0000	// 24bit = 16777215
//#define MAX_PHOTOCNT		0xD00000	// 0xf0 0000	// 24bit = 16777215
//#define MAX_PHOTOCNT		0xCF0000 * 10	// 0xf0 0000	// 24bit = 16777215
//#define MAX_PHOTOCNT		(0xCF0000 * DET_PHOTO_DATA_AVE)		// full scale 200nA --> 25Mohm사용시 : 2009.08.11
//#define MAX_PHOTOCNT		(0xA59999 * DET_PHOTO_DATA_AVE)		// full scale 250nA --> 20Mohm사용시 : 2009.08.11 -하지마
//#define MAX_PHOTOCNT		(0xCF0000 * DET_PHOTO_DATA_AVE)		// full scale 250nA --> 20Mohm사용시 : 원래는 0xA59999이지만  2009.08.11
#define MAX_PHOTOCNT		(0xD80000 * DET_PHOTO_DATA_AVE)		// full scale 250nA --> 20Mohm사용시 : 원래는 0xA59999이지만  2009.08.11
//#define MAX_PHOTOCNT		(0xF00000 * DET_PHOTO_DATA_AVE)		// full scale 250nA --> 20Mohm사용시 : 원래는 0xA59999이지만  2009.08.11
#else	// Chrozen Test
//#define MAX_PHOTOCNT		(0xA59999 * DET_PHOTO_DATA_AVE)		// full scale 250nA --> 20Mohm사용시 : 2009.08.11 -하지마
//#define MAX_PHOTOCNT		(0xD80000 * DET_PHOTO_DATA_AVE)		// full scale 250nA --> 20Mohm사용시 : 2021.03.15 test  //(+)210729 CJKIM, Origin
#define MAX_PHOTOCNT		(0xF7FFFF * DET_PHOTO_DATA_AVE)		// full scale 250nA --> 20Mohm사용시 : //(+)210729 CJKIM, 기준값 0x9aFFFF6


#endif

//efine FIND_486_WAVE_ZERO_DIFF		1535
#define FIND_486_WAVE_ZERO_DIFF		1545
#define FIND_486_WAVE_END			100
//#define FIND_656_WAVE_ZERO_DIFF		2126
#define FIND_656_WAVE_ZERO_DIFF		2135
#define FIND_656_WAVE_END			80

#define FIND_486_WAVE_MAX	1650
#define FIND_486_WAVE_MIN	1550
#define FIND_486_WAVE_PEAK	100

#define FIND_656_WAVE_MAX	2223
#define FIND_656_WAVE_MIN	2123
#define FIND_656_WAVE_PEAK	300

#define SEQ_D2_LAMP_HEATING_TIME	20*500	// 20*500
#define SEQ_D2_LAMP_STRIKE_ON_TIME	SEQ_D2_LAMP_HEATING_TIME + 500

#define CONST_RESULT		1.349928	// 656.2/486.1 = 1.349928
#define CONST_LIMIT_P		0.0001
#define CONST_LIMIT_M		-0.0001
#define CONSTA_MAX			9000.0
#define CONSTA_MIN			0.0
#define CONSTA_START		4500.0
#define PI_2				1.5707963268	// PI/2

#define ADC_RD_ING			0		// 모터 구동중
#define ADC_RD_OK			1		// 읽었음
#define ADC_RD_STEP_END		2		// 구간 끝까지 이동하였음.

#define CAL_NONE			0
#define CAL_CHECK_CALIB		1
#define CAL_FIND_SAM_ZERO	2
//#define CAL_FIND_REF_ZERO	2
#define CAL_FIND_W486		3
#define CAL_FIND_W656		4
#define CAL_CAL_PARA		5
#define CAL_FILE_OPEN_STEP	6
#define CAL_SAVE_GRWAVE_STEP	7
#define DEVIATION_ANGLE		17.35f	// CONCAVED GRATING
//#define DEVIATION_ANGLE		16.1602	// 구형(UV730) GRATING
//#define DEVIATION_ANGLE		19.704	// xpy
//float GM_ZeroWaveStep;
//float Wv486=0.0, Wv656=0.0;

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// Thr_detfunc.c ////////////////////////////
 *///////////////////////////////////////////////////////////////////////

//#define		CONV_AD_TO_ENERGY	5.960465e-7	// (100ns/ (float)0xffffff / 10(-->sum))	// full scale 100nA --> 50Mohm사용시
//#define		CONV_AD_TO_ENERGY	(5.960465e-6 / DET_PHOTO_DATA_AVE)						// full scale 100nA --> 50Mohm사용시
//#define	CONV_AD_TO_ENERGY	(1.192093e-5 / DET_PHOTO_DATA_AVE)							// full scale 200nA --> 25Mohm사용시
#define	CONV_AD_TO_ENERGY	(1.490116e-5 / DET_PHOTO_DATA_AVE)								// full scale 250nA --> 20Mohm사용시 : 2009.08.11

//#define SIGNAL_SEND_COUNT	10
#define SIGNAL_SEND_COUNT	25	// 125Hz용 


//#define DET_ENERGY_NANOA_DIG	83886.075	// 1nA Dig => (0xffffff / 200nA)				// full scale 200nA --> 25Mohm사용시
#define DET_ENERGY_NANOA_DIG	67108.86	// 1nA Dig => (0xffffff / 250nA)				// full scale 250nA --> 20Mohm사용시 : 2009.08.11

#ifdef FIX_SAM_REF_OFFSET
// Offset 값을 램프온오프상태에다라 다르게
// float fEnergyOffsetDummy;
#define DET_OFFSET_ENERGY_D2_W_ON		0.025
#define DET_OFFSET_ENERGY_D2_ON			0.010
#define DET_OFFSET_ENERGY_W_ON			0.005
#else
//#define DET_OFFSET_ENERGY	0.54	// nA
//#define DET_OFFSET_ENERGY	0.2		// nA		// 2.5ABS 이상에서 다른회사 장비와 다름 수정 - 직선성 모양이 다름 - offset의 영향으로 판단됨 : 2009.08.11
#define DET_OFFSET_ENERGY	0.0		// nA		// Linearity 보완 - offset의 영향으로 판단됨 :재수정 -  2010.08.30
//#define ENERGY_OFFSET_DUMMY		167772.15	// 0.2nA  = (0xffffff * 10) / 200 * 0.2
//#define ENERGY_OFFSET_DUMMY		0	// 0.54nA  = (0xffffff * 10) / 200 * 0.54
//#define ENERGY_OFFSET_DUMMY		452984.805	// 0.54nA  = (0xffffff * 10) / 200 * 0.54
#define ENERGY_OFFSET_DUMMY		DET_ENERGY_NANOA_DIG * DET_PHOTO_DATA_AVE * DET_OFFSET_ENERGY	// 0.54nA  = (0xffffff / 200) * 10 * 0.54
#endif

//#define ENERGY_DATA_OFFSET_ZERO		83.886075	// 0.0001nA  = (0xffffff * 10) / 200 * 0.0001
#define ENERGY_DATA_OFFSET_ZERO		DET_ENERGY_NANOA_DIG * DET_PHOTO_DATA_AVE / 10000		// 0.0001nA  = (0xffffff * 10) / 200 * 0.0001
#define TRANS_ZERO_VALUE			0.0000005

#define DET_DA_1V_DIG_FACTOR	5457780.09
#define DET_DA_ZERO_VOLT_DIG	0x800000
#define DET_DA_SIGNAL_MAX	1.5
#define DET_DA_SIGNAL_MIN	-1.5

#define STATE_CHK_D2_LAMP_HEATING_CNT		20
#define STATE_CHK_D2_LAMP_ON_RETRY_CNT		2
#define LEAK_DETECT_ERROR_CNT			2
#define D2_DETECT_ERROR_CNT			3

#define DIAG_SEQ_MOVE_WAVE	1
#define DIAG_SEQ_LAMP_OFF	2
#define DIAG_SEQ_LAMP_ON	3
#define DIAG_SEQ_ENERGY_CHK	4

#define DIAG_D2LAMP_CHECK_WAVE		254
#define DIAG_WLAMP_CHECK_WAVE		720
#define DIAG_LAMP_CHECK_CUTFILTER	CUTFILTER_NONE

// Cut filter
#define DIAG_SEQ_MOVE_CUTFILTER_NONE	1
#define DIAG_SEQ_MOVE_CUTFILTER_1ST		2
#define DIAG_SEQ_ENERGY_CHK_1ST			3
#define DIAG_SEQ_MOVE_CUTFILTER_2ND		4
#define DIAG_SEQ_ENERGY_CHK_2ND			5

#define DIAG_CUTFILTER_CHECK_WAVE_1ST		254
#define DIAG_CUTFILTER_CHECK_WAVE_2ND		486

// Calibration
#define UVD_DIAG_CAL_DIAG_SEQ		1
#define UVD_DIAG_CAL_CAL_SEQ		2

#define DIAG_LAMP_CHECK_WAIT_CNT		1000	// 2초

#define EVENT_TIME_COUNT_0_2SEC		4		// 0.2sec

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// ChroZen GC		Def_name.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////
//=========================================================================
// Self Message
//=========================================================================
/*
#define SELF_STATE				1
#define SELF_ERROR				2
#define SELF_NO_ERROR			0
#define SELF_ERROR_COMMAND_UNKNOWN			1
// adjustment 실행한적이 없음. - 전원을 켰을 때 초기화하기 전에 체크하여 전송한다.
// adjustment를 꼭 한번은 실행해야 한다.
#define SELF_ERROR_OPERATION						5 	// 기구물 에러
#define SELF_ERROR_DOOR_OPEN	(6) // 도어열림
#define SELF_ERROR_LEAK			(7) // Leak
#define SELF_ERROR_TEMPERATURE	(8) // 온도 이상


#define SELF_ERROR_CODE_MAX		(255)



#define SELF_METHOD_LOAD		3
#define SELF_EXTOUT				4
#define SELF_START				5
#define SELF_STOP				6
#define SELF_UPDATE				7

// lcas
#define SELF_COMMAND_REJECT		8		// 전송 명령 거부 , 전송된 명령을 실행할 수 없는 상태일 떄 전송됨.

#define SELF_START_BUTTON		0	// 외부입력 (button 입력 )
#define SELF_START_REPEAT_RUN	1	// 반복분석자동시작

#define SELF_STOT_BUTTON		0	// Stop button 입력
#define SELF_STOP_ANALYSIS_END	1	// 분석완료 - 반복분석중이면 매번 stop시 전송

#define SELF_UPDATE_VER_GC_LOW		0	// GC < Soft
#define SELF_UPDATE_VER_GC_HIGH		1	// GC > Soft
#define SELF_UPDATE_SUCCESS		2
#define SELF_UPDATE_FAIL		3
*/

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// ChroZen GC		define_GC.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////
#define MSS_SPI_SLAVE_FRAM		MSS_SPI_SLAVE_0

#endif

