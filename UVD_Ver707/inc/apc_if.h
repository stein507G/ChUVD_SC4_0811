/*
 * apc_if.h
 *
 *  Created on: 2017. 9. 12.
 *      Author: Han
 */

#ifndef APC_H_
#define APC_H_

#define APC_SLAVE_NUMBER    		5	//9
//#define APC_CONFIG_STASUS_SIZE    	32 //16	// 64 bytes
#define APC_CONFIG_STASUS_SIZE    	64 // 256 bytes
#define APC_CONFIG_STASUS_SIZE_SHIFT    	6

#define APC_RUN_CPUMODE    		0
#define APC_RUN_AUTOMODE    	1
#define APC_STATE_IDLE   0
#define APC_STATE_RUN    1

//=================================================================
// RS422 (APC)
// ----------------------------------
	#define RS422_ID_MASTER			0x0E
	#define RS422_ID_GLOBAL			0x0A

	#define RS422_ID_INLET1			0x01
	#define RS422_ID_INLET2			0x02
	#define RS422_ID_INLET3			0x03
	#define RS422_ID_DET				0x04
//	#define RS422_ID_DET1				0x04
//	#define RS422_ID_DET2				0x04
//	#define RS422_ID_DET3				0x04
	#define RS422_ID_AUX				0x05
//	#define RS422_ID_AUX1				0x05
//	#define RS422_ID_AUX2				0x05
//	#define RS422_ID_AUX3				0x05
//-----------------------------------------------------------------

#define APC_RS422_TX_STATE_ADDR		0x2200
#define APC_RS422_MY_ID_ADDR			0x220F
#define APC_RS422_SEND_TRIG_ADDR 	0x2210
#define APC_RS422_TX_LENGTH_ADDR 	0x2211
#define APC_RS422_TX_CRC_ADDR 			0x2212
#define APC_RS422_MASTER_MODE_ADDR			0x2213			// 0:cpuode,1:Auto mode
#define APC_RS422_TX_FLAG_RESET_ADDR		0x2220			

#define APC_RS422_RX_STATE_ADDR		0x2600
#define APC_RS422_RCV_GALLOW_ADDR		0x2602				// 0:Off(자신의 ID만 응답), 1:ON(ID에 상관없이 응답)
#define APC_RS422_RX_INFO_ADDR			0x2610	
#define APC_RS422_RX_AUTO_RESULT_ADDR		0x2613	
#define APC_RS422_RX_FLAG_RESET_ADDR		0x2620	

#define FPGA_WR_APC_SEND_TRIG(X)		*(volatile uint32_t *) (FPGA_BASE_ADDR + (APC_RS422_SEND_TRIG_ADDR<<2)) = (X)
#define FPGA_WR_APC_TX_LENGTH(X)		*(volatile uint32_t *) (FPGA_BASE_ADDR + (APC_RS422_TX_LENGTH_ADDR<<2)) = (X)

#define FPGA_WR_APC_MODE_AUTO				FPGA_WRITE_WORD(APC_RS422_MASTER_MODE_ADDR,1)
#define FPGA_WR_APC_MODE_CPU				FPGA_WRITE_WORD(APC_RS422_MASTER_MODE_ADDR,0)
#define FPGA_WR_APC_TX_FLAG_CLEAR				FPGA_WRITE_WORD(APC_RS422_TX_FLAG_RESET_ADDR,1)
//#define APC_RS422_CMD_RX_FLAG_CLEAR		FPGA_WriteWord(APC_RS422_RX_FLAG_RESET_ADDR,1)	
//#define FPGA_WR_APC_RX_FLAG_CLEAR		*(volatile uint32_t *) (FPGA_BASE_ADDR + (APC_RS422_RX_FLAG_RESET_ADDR<<2)) = 1
#define FPGA_WR_APC_RX_FLAG_CLEAR			FPGA_WRITE_WORD(APC_RS422_RX_FLAG_RESET_ADDR,1)



#define AUTO_MODE_CMD    	0x000000C0		// Auto Mode SOC -> APC
#define CPU_MODE_SOC_CMD    	0x000000A0		// Auto Mode SOC -> APC
//#define CPU_MODE_APC_CMD    	0x000000B0		// Auto Mode APC -> SOC
#define STATE_REQUEST_CMD    	0x000000B0		// Auto Mode APC -> SOC (자동응답 버퍼에 쌓인것을 읽는다.)
#define STATE_SEND_RET_LEN   	0x00000072		// 112 + 2
#define APC_STATE_SEND_CMD    	0x000000B0		// Auto Mode APC -> SOC 

#define APC_CMD_INLET_SET    	0xD0
#define APC_CMD_DET_SET    		0xD1
#define APC_CMD_AUX_SET    		0xD2
#define APC_CMD_CALIB_SET    	0xD3





#define rs422_APC_CMD_CONNECT_CHECK   	0x00000003

#define rs422_APC_CMD_START							0x05
#define rs422_APC_CMD_STOP							0x07

#define rs422_APC_CMD_REQUEST   			0x09

#define rs422_APC_CMD_INLET_CONFIG		0x10
//#define APC_CMD_INLET_CONFIG_1		0x10
//#define APC_CMD_INLET_CONFIG_2		0x11
//#define APC_CMD_INLET_CONFIG_3		0x12

#define rs422_APC_CMD_INLET_SETTING		0x11
//#define APC_CMD_INLET_SETTING_1		0x13
//#define APC_CMD_INLET_SETTING_2		0x14
//#define APC_CMD_INLET_SETTING_3		0x15

#define rs422_APC_CMD_INLET_FLOW_PRGM		0x12
//#define APC_CMD_INLET_FLOW_PRGM_1		0x16
//#define APC_CMD_INLET_FLOW_PRGM_2		0x17
//#define APC_CMD_INLET_FLOW_PRGM_3		0x18

#define rs422_APC_CMD_INLET_PRESS_PRGM		0x13
//#define APC_CMD_INLET_PRESS_PRGM_1		0x19
//#define APC_CMD_INLET_PRESS_PRGM_2		0x1A
//#define APC_CMD_INLET_PRESS_PRGM_3		0x1B

#define rs422_APC_CMD_DET_CONFIG_1		0x14
#define rs422_APC_CMD_DET_CONFIG_2		0x15
#define rs422_APC_CMD_DET_CONFIG_3		0x16

#define rs422_APC_CMD_DET_SETTING_1		0x17
#define rs422_APC_CMD_DET_SETTING_2		0x18
#define rs422_APC_CMD_DET_SETTING_3		0x19

#define rs422_APC_CMD_AUX_SETTING_1		0x1A
#define rs422_APC_CMD_AUX_SETTING_2		0x1B
#define rs422_APC_CMD_AUX_SETTING_3		0x1C

#define rs422_APC_CMD_CALIB_SETTING		0x1D

#define rs422_APC_CMD_MAIN_STATE		0x1F

//================================
// APC --> SOC
#define rs422_APC_CMD_INLET_DATA		0x20
#define rs422_APC_CMD_DET_DATA		0x21
#define rs422_APC_CMD_AUX_DATA		0x22
#define rs422_APC_CMD_CALIB_DATA		0x23
#define rs422_APC_CMD_VOLT_DATA		0x24

#define rs422_APC_CMD_INLET_SETTING_1		0x25
#define rs422_APC_CMD_INLET_SETTING_2		0x26
#define rs422_APC_CMD_INLET_SETTING_3		0x27

#define rs422_APC_CMD_INLET_FLOW_PRGM_1		0x28
#define rs422_APC_CMD_INLET_FLOW_PRGM_2		0x29
#define rs422_APC_CMD_INLET_FLOW_PRGM_3		0x2A

#define rs422_APC_CMD_INLET_PRESS_PRGM_1	0x2B
#define rs422_APC_CMD_INLET_PRESS_PRGM_2	0x2C
#define rs422_APC_CMD_INLET_PRESS_PRGM_3	0x2D

#define rs422_MAX_PACKET_NO    	0x2D		// Auto Mode APC -> SOC

// 아래는 사용안함. - 테스트용임.
#define APC_CMD_CON_STATUS    	0xC0
#define APC_CMD_START    		0xB0
#define APC_CMD_STOP    		0xBF
/*
#define APC_CMD_INLET_SET    	0xD0
#define APC_CMD_DET_SET    		0xD1
#define APC_CMD_AUX_SET    		0xD2
#define APC_CMD_CALIB_SET    	0xD3
*/
// ==================================

typedef struct
{
	uint8_t		RunMode; // 0:CPU Mode, 1:else Auto Mode
	uint8_t		State;
	uint8_t		slvch; 	// slave channel number
	
	uint8_t		RSTAT;
	uint8_t		rx_slvid;	

//	uint8_t		firstSend;	// 맨처음에는 
	uint8_t		TSTAT;	// 송신완료 면 1 , 아니면 0
	uint8_t		busyTxHead;	// 저장할 펑션 번지
	uint8_t		busyTxTail;	// 전송할 펑션 번지 마지막
	uint8_t		busyFn[8];	// 전송할 펑션번호

	
	uint16_t	TxTime;
	
	uint8_t		AutoEnd;
	uint8_t		rx_intr;		// ryu 추가 
	uint8_t		RxAllAllow; // when this bit is set, receive the frame no matter what the destination id

	uint8_t		sendCmd[5];
	uint8_t		sendStart;
	uint8_t		sendSop;
	

	uint32_t	Config[APC_SLAVE_NUMBER][APC_CONFIG_STASUS_SIZE];
	uint32_t	MainStatus[APC_CONFIG_STASUS_SIZE];
	uint32_t	SlaveStatus[APC_SLAVE_NUMBER][APC_CONFIG_STASUS_SIZE];

	uint8_t		TST_patternA;
} APC_ST; // GC Service struct

typedef struct
{
	uint8_t		RunMode; // 0:CPU Mode, 1:else Auto Mode
	uint8_t		State;
	uint8_t		slvch; 	// slave channel number
	
	uint8_t		RSTAT;
	uint8_t		rx_slvid;	

//	uint8_t		firstSend;	// 맨처음에는 
	uint8_t		TSTAT;		// 송신완료 면 1 , 아니면 0
	
	uint16_t	TxTime;
	
	uint8_t		AutoEnd;
	uint8_t		rx_intr;		// ryu 추가 
	uint8_t		RxAllAllow; 	// when this bit is set, receive the frame no matter what the destination id

	uint8_t		sendCmd[5];
	uint8_t		sendStart;
	uint8_t		sendSop;
	

	uint32_t	Config[APC_SLAVE_NUMBER][APC_CONFIG_STASUS_SIZE];
	uint32_t	MainStatus[APC_CONFIG_STASUS_SIZE];
	uint32_t	SlaveStatus[APC_SLAVE_NUMBER][APC_CONFIG_STASUS_SIZE];

	uint8_t		TST_patternA;
} RS422_ST; // GC Service struct

void apc_RS422_init(void);

void apc_CheckTxState(void);
void apc_CheckRxState(void);

//================================================================================
//================================================================================
void apc_process(void);

void apc_APCSendStaus(uint8_t dest_addr);
void apc_APCSendStausAuto(void);
void apc_APCSendConfig(uint8_t dest_addr);

void apc_APCConfigSendBack(void);

void rs422_Test();


#endif /* APC_H_ */
