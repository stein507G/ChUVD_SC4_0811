/*
 * UV_Global_Extern.h
 *
 *  Created on: 2020. 6. 28.
 *      Author: ksyi
 */
#ifndef __UV_GLOBAL_EX_H_
#define __UV_GLOBAL_EX_H_

extern volatile UV_FUNC_t st_UV_func;	//(-/+) 210603 CJKIM, volatile keyword added

extern Det_Dadata_t detDa;


extern volatile Det_Motor_Con_t detMotor;
extern volatile Det_Motor_State_t detMotorState;

extern void AD3_Read(void);

extern union ujData utRefData;
extern union ujData utSamData;

//extern Det_AZ_AO_data_t azao[AUTO_ZERO_OFFSET_DATA_NUM];


extern Det_Setup_t detSet;
extern Det_Diagnosis_t detDiag;

extern Event_Control_t eventCtrl;

extern Remote_State_t remote;
extern Gpio_RemoteSet_t remoteSet;	// 디바이스 드라이버에 설정값을 전달

extern Gpio_RemoteOut_t rmtout;		// 디바이스 드라이버에 설정값을 전달 - remote out
extern Gpio_RemoteIn_t rmtin;

extern Ads1252_Cmd_t Ads1252;
extern Ads1252_Cmd_t detAds;

extern Det_Dadata_t detDa;

extern UVD_LampTime_Check LampT;
extern UVD_Wave_Check dWave;


extern unsigned long rDataSum, rDataBuf[DET_PHOTO_DATA_AVE];
extern unsigned long *refData;
extern unsigned long sDataSum, sDataBuf[DET_PHOTO_DATA_AVE];
extern unsigned long *samData;

extern unsigned long rDualSumA, rDualBufA[DET_DUAL_PHOTO_DATA_AVE];
extern unsigned long sDualSumA, sDualBufA[DET_DUAL_PHOTO_DATA_AVE];

extern unsigned long rDualSumB, rDualBufB[DET_DUAL_PHOTO_DATA_AVE];
extern unsigned long sDualSumB, sDualBufB[DET_DUAL_PHOTO_DATA_AVE];

extern struct tcp_pcb *g_pAcceptPcb;

extern char pPacketBuff[PACKET_SIZE_MAX];
extern char LEDStateChange;
extern volatile char Clarity_Connect;

extern uint8_t IP_Addr0;
extern uint8_t IP_Addr1;
extern uint8_t IP_Addr2;
extern uint8_t IP_Addr3;

extern char Selfmsg_flag;

#endif
