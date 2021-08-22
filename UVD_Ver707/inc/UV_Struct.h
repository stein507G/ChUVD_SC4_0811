/*
 * UV_Struct.c
 *
 *  Created on: 2020. 6. 28.
 *      Author: ksyi
 */
#ifndef __UV_STRUCTURE_H_
#define __UV_STRUCTURE_H_

#include "type_def.h"

BEGIN_PACK_STRUCT

typedef struct _UV_FUNC_t	// FPGA 적용
{
//	int iBuz_Cnt;	// Buzzer Time (ms)
//	char cBuz_Type;	// Boot, Oper, Error(Pulse)... etc
//	char cBuz_Pulse_Cnt;	// Buzzer Pulse Cnt

	char cPWR_Button_flag;
	char cM1_flag;
	char cM2_flag;
	char cReadyIN_flag;
	char cMarkIN_flag;
	char cStartIN_flag;
	char cLeak_flag;
	char cD2_State_flag;
	char cThermal_flag;

	char bADC1_Int;
	char bADC2Int;
	char bADC3_Int;

	char bADC_Int;
	char bADC_Volt_Int;
	char b125Hz_Int;

	char cRemoteIntr;

	unsigned int uiFPGA_Ver;		// 210702 KSLEE , FPGA Version

	int iVolt_Read[8];

	unsigned long AD_Data[3];
} PACK_STRUCT UV_FUNC_t;

typedef struct _Det_Cmd_t
{
	bool			enable;
	unsigned char	index;
	unsigned char	cmd_index_head;
	unsigned char	cmd_index_tail;

	long			data;
	float			fdata;
	char			*sdata;
} PACK_STRUCT Det_Cmd_t;

typedef struct _Det_Diagnosis_t
{
// 전압check & 저장
	unsigned char checkVolt[8];	// ChroZen UVD	24V / 5V / 3.3V / Tung / X / DA1 / DA2 / Temp
	float sysVolt[8];
} PACK_STRUCT Det_Diagnosis_t;

typedef struct _Det_Setup_t
{
// 오븐 기기 설정
	unsigned char index;
	unsigned char instrumentId;	// Oven ID

// 외부 출력
	unsigned char btMarkExtOut;	// 0: None, 1: Pulse(0>1), 2: 1>0, 3: Level 0, 4: Level 1
	unsigned char btStartExtOut;	// 0: None, 1: Pulse(0>1), 2: 1>0, 3: Level 0, 4: Level 1
	unsigned char btReadyExtOut;	// 0: None, 3: Level 0, 4: Level 1

	unsigned int nExtoutTime;	// mSec -> Pulse일경우만 해당(100--100000)




	bool tempUnit;				// 온도표시단위
	unsigned char tempLimit;	// 온도 리미트
	float fSettemp;

	unsigned char LampWarming;
	unsigned char preampGain;

	unsigned short startWave;
	unsigned short endWave;
	unsigned char scanStep;
	unsigned char sampleCnt;

	unsigned short maxWaveLength;
	unsigned short minWaveLength;

	unsigned short secondFilter1WL;
	unsigned short secondFilter2WL;

	float channelCalibA;
	float channelCalibB;

	unsigned char timeConstant;
	unsigned char sensitivity;
	unsigned char offset;
	unsigned char polarity;
	unsigned char plot;			// max , add , diff , ratio

	bool leakDetect;			// on & off

	bool remoteMode;			// on & off
	bool keylock;			// on & off

	bool mkoutDefault;	// Low Active & High Active
	bool mkoutMethod;	// high & low & pulse
	unsigned char mkoutTime;	//

	bool AZonStart;
	bool AZonWlChange;

	bool CMKonStart;

	bool extAutozero;		// low & high
	bool extChartmark;		// low & high
	bool extStart;		// low & high
	bool extStop;		// low & high

	unsigned int maxChain;		// 999

	unsigned char extChannel;
	float extThreshold;
	unsigned char extPeakwidth;
	unsigned char extOutput;	// on/off
	float extPulsewidth;
	unsigned char extStartdelay;
	unsigned char extStopdelay;
} PACK_STRUCT Det_Setup_t;

typedef struct _Det_State_t
{
	unsigned char selftest;		// ok & error
	unsigned char cmosBackup;	// ok & error

	float ftemp;

	unsigned char btCalibState;

	unsigned char startIn;			// 외부에서 스타트 명령이 들어와서 스타트 한 경우
	unsigned char startInCmd;		// 스타트 명령이 들어왔다

//	bool sendInformNow;		// UVD_INFORM을 보내야 되는가?
	unsigned char isConnectContSoftware;		// 제어기가 연결되었나?

	unsigned char powerCheck;		// 전원검사가 끝났는가?
	unsigned char powerState;		// 전원상태-에러가 있는가?
	unsigned char sysCheck;			// 전원온후 시스템 체크

	unsigned char setupInitcmdRcv;	// 셋업 init 명령을 전달받았다.
	unsigned char setupStopcmdRcv;	// 셋업 Stop명령을 전달받았다.
//	bool prgmStopcmd;		// 프로그램모드끝남.

	unsigned char runstop;	// run중인지 stop인지
	unsigned char nowStart; // 지금시작
	unsigned char oldState;	// 이전 상태 
	bool diagStart;	// 
	bool serviceStart;	// 서비스 모드 시작

	unsigned char waveMode;	// 0:single , 1:dual , 2:scan , 

	unsigned char dualMode;	// 0:move , 1:wait , 2:read , 
	unsigned char dualWaitCnt;	
	unsigned char dualReadCnt;
	unsigned short dualStartStep;
	unsigned short dualEndStep;
//		unsigned short scanInterval;
	unsigned char dualSamplingCountA;
	unsigned char dualSamplingCountB;
		
	unsigned char scanMode;	// 0:move , 1:wait , 2:read , 
//		unsigned char scanMoveCnt;	
	unsigned char scanWaitCnt;
	unsigned char scanReadCnt;
	unsigned short scanDestWave;
	unsigned short scanEndWave;
	unsigned short scanInterval;
	unsigned char scanSamplingCount;
//	unsigned int nNextEventStep;	// 다음에 실행할 테이블

	float fAbsorbance[2];
	float transmittance[2];
	float fAutoZeroDataA;
	float fAutoZeroDataB;

	unsigned char bAutozeroA;	// 이벤트테이블/readyInit에서 AutoZero
	unsigned char bAutozeroB;
//	unsigned char bAutozero;	// 이벤트테이블/readyInit에서 AutoZero

	unsigned char bAZstartA;	// start명령시 AutoZero
	unsigned char bAZstartB;

	unsigned char bAZwaveA;		// 파장변경시 AutoZero
	unsigned char bAZwaveB;

	float fAutoOffsetDataA;
	float fAutoOffsetDataB;

	float fAutoOffsetDataOldA;
	float fAutoOffsetDataOldB;

	float fAutoOffsetDataCurA;
	float fAutoOffsetDataCurB;

//	unsigned char bAutoOffsetA;
//	unsigned char bAutoOffsetB;

//	unsigned char bPolarityA;	
//	unsigned char bPolarityB;
	unsigned char bPolarity;	// 이벤트테이블/readyInit에서 AutoZero

	unsigned char bAOwaveA;
	unsigned char bAOwaveB;

	unsigned char bAOpolaA;
	unsigned char bAOpolaB;

	float acqSumA;	//
	float acqSumB;	//

	float acqAverageA;	//
	float acqAverageB;	//
	
	unsigned short iAcqA;
//	unsigned short iAcqB;
	unsigned short timeConstCntA;
	unsigned short timeConstCntB;	//

// 파장 캘리브레이션에서 사용하는 변수들 
	float fGM_ZeroWaveStep;
	float fGM_486WaveStep;
	float fGM_656WaveStep;
	
	unsigned short sFindZeromin;
//	unsigned short sFindZeromax;
	unsigned short sFind486min;
	unsigned short sFind486max;
	unsigned short sFind486Diff;	
	unsigned short sFind656min;
	unsigned short sFind656max;
	unsigned short sFind656Diff;		

	bool leakError;		// yes & no
	bool InitialError;		// yes & no
	unsigned char btD2State;
	unsigned char btD2HeatingCnt;

	bool bOffsetValueSave;
	bool bOffsetValueSaveAct;
	unsigned short offsetValueCnt;
	unsigned long lOffsetValueRef;
	unsigned long lOffsetValueSam;
	float fOffsetValueRefAve;
	float fOffsetValueSamAve;

	float fD2LampElapse;
	float fWLampElapse;

	bool connectChk;
	bool fractionState;

	int cnt;
}PACK_STRUCT Det_State_t;

typedef struct _Det_Motor_Con_t
{
	unsigned char bGMInit;		// 위치저장 변수 초기화 할 것인가?

	unsigned char btMode;	// 0:off, 1: free, 2: single, 3: dual, 4: scan, 5: home

	unsigned short sGMDestStep;
	unsigned short sStartStep;	// 듀얼 파장이나 스캔모드의 처음 파장
	unsigned short sEndStep;	
	unsigned short sStepInterval;	// 스캔모드의 처음 스텝간격(파장간격)
	bool bDualPos;	// 0 : 시작파장으로 가야함. 1: 끝파장으로 가야함.
	unsigned short sDualcnt;	// 다른파장으로 이동을 위한 카운트 ==> 2000카운트 후 이동(250ms)
	unsigned short sDualActcnt;	// 다른파장으로 이동을 위한 기준 카운트 ==> 2000 or 4000 카운트
	unsigned short sDualReadcnt;

	unsigned short sAccStartStep;	// 가속시작 점 
	unsigned short sRdcStartStep;	// 감속시작 점

	unsigned char bFMInit;		// 위치저장 변수 초기화 할 것인가?
	unsigned char btFMMode;	// 0: Auto, 1: None, 2: 1st Filter, 3: 2nd Filter, 4: Holium Filter
	unsigned char btFMDestStep;
	unsigned char btStartFilter;	// 듀얼 파장이나 스캔모드의 처음 Filter
	unsigned char btEndFilter;
} PACK_STRUCT Det_Motor_Con_t;

typedef struct _Det_Motor_State_t
{
	unsigned char bGMInitOk;		// 위치 초기화 되었는가?  YES/NO
	unsigned char bGMHome;		// 홈위치인가?   YES/NO
	unsigned char homeIndex;
	unsigned short sGMCurStep;
	unsigned short sGMDestStepOld;
	unsigned char bGMError;		// 모터에러
	

	unsigned char bFMInitOk;		// 위치 초기화 되었는가?	YES/NO
	unsigned char bFMHome;		// 홈위치인가?	YES/NO
	unsigned char btFMCurStep;
	unsigned char bFMError;		// 모터에러

	unsigned char bReadable;		// photo Data를 읽어도 되는가?
} PACK_STRUCT Det_Motor_State_t;

typedef struct _Det_Acq_data_t
{
//	float fAbs;
	float fTrans;	
//	float fRef;
//	float fSam;
	
} PACK_STRUCT Det_Acq_data_t;

typedef struct _Det_AZ_AO_data_t
{
	float fAutozeroA;
	float fAutozeroB;	
	float fAutooffsetA;
	float fAutooffsetB;
} PACK_STRUCT Det_AZ_AO_data_t;

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// hplc_type.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////
typedef struct _Ads1252_Cmd_t
{
	unsigned int who;	// 0 : channel A data, 1 : channel B data
	unsigned long refD; // oven temp , ref Energy  , pressure 
	unsigned long samD; // sam Energy
} PACK_STRUCT Ads1252_Cmd_t;

typedef struct _Det_Dadata_t
{
	unsigned char cmd;
	unsigned long leftData;
	unsigned long rightData;
} PACK_STRUCT Det_Dadata_t;


typedef struct _Gpio_RemoteOut_t
{
	unsigned char r_startOut;
	unsigned char r_readyOut;
	unsigned char r_markOut;

//unsigned char swOut1;
//unsigned char swOut2;
	uint32_t remoteStartOutSig;	// 리모트 단자로 출력해야 된다.
	uint32_t remoteStartOutCnt;		
} PACK_STRUCT Gpio_RemoteOut_t;

typedef struct _Gpio_RemoteIn_t	// 리모트를 검사하기
{
	unsigned char r_markIn;		// 0
	unsigned char r_startIn;	// 1
	unsigned char r_readyIn;	// 2

	unsigned char r_AZExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char r_LampExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char r_ChartExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
} PACK_STRUCT Gpio_RemoteIn_t;

typedef struct _Gpio_RemoteChk_t	// 리모트를 검사하기 위하여 임시로 사용
{
	unsigned char markIn;	// 0
	unsigned char startIn;	// 1
	unsigned char readyIn;	// 2

//	unsigned char chartIn;
//	unsigned char lampIn;
//	unsigned char autozeroIn;
	
	unsigned char bEdgeCheckSet[6]; // 에지트리거 모드 입력으로 설정 1: RISING_EDGE - Pulse(0>1), 0: FALLING_EDGE - Pulse(1>0)

	unsigned char checkCnt[6];	// high 나 low상태에서 얼마만큼(시간-2ms) 유지하였나?
	unsigned char bCheckEdge[6];	// Edge 트리거모드에서 사용하는 변수로 Edge가 발생하였는가?
	unsigned char bCheckEnable[6];	// remote 시그널을 입력받을 수 있는 상태인가?

	unsigned char bStartchkEn;	//
} PACK_STRUCT Gpio_RemoteChk_t;

typedef struct _Gpio_RemoteSet_t
{
	unsigned char r_markInSet;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char r_startInSet;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char r_readyInSet;		// 0: LEVEL_LOW - Level0, 1: LEVEL_HIGH - Level1

	unsigned char r_AZExtInSet;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char r_LampExtInSet;	// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char r_ChartExtInSet;	// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)


	unsigned char r_startOutSet;	// 0: CONTACT_CLOSE (Level0), 1: CONTACT_OPEN (Level1)
	unsigned char r_readyOutSet;	// 0: CONTACT_CLOSE (Level0), 1: CONTACT_OPEN (Level1)
	unsigned char r_markOutSet;		// 0: CONTACT_CLOSE, 1: CONTACT_OPEN	-- default : 1		 0:Off, 1:On

//unsigned char swOutSet1;	// 0: CONTACT_CLOSE, 1: CONTACT_OPEN	-- default : 1		 0:Off, 1:On
//unsigned char swOutSet2;	// 0: CONTACT_CLOSE, 1: CONTACT_OPEN	-- default : 1		 0:Off, 1:On

	unsigned short r_outCntSet;		// high 나 low상태에서 얼마만큼(시간-50ms) 유지할 것인가?

} PACK_STRUCT Gpio_RemoteSet_t;

typedef struct _Packet_Header_t
{
	unsigned char packetSize;
	unsigned char toGroupid;
	unsigned char toSubid;
	unsigned char fromGroupid;
	unsigned char fromSubid;
	unsigned char CMD;
} PACK_STRUCT Packet_Header_t;

typedef struct _NetPacket_t
{
	Packet_Header_t header;
	char *pdata;
} PACK_STRUCT NetPacket_t;

typedef struct _System_Info_t
{
	bool com_enable;	// 시리얼로 데이터전송
	bool net_enable;	// 네트워크로 데이터전송

	bool key_enable;
	bool tftlcd_enable;
	bool charlcd_enable;

	unsigned char system_id;
	unsigned char masterNslave;

	char *sdata;
} PACK_STRUCT System_Info_t;

typedef struct _Remote_State_t
{
	unsigned char markIn;	
	bool startIn;			// 단자대의 상태점검.
	unsigned char readyIn;	// 단자대의 상태점검.
/*
	unsigned char error;
	unsigned char stopIn;

	bool leakErr;
	bool gasErr;
	bool therErr1;	// 오븐
	bool therErr2;
	bool heatErr;
*/
} PACK_STRUCT Remote_State_t;

typedef struct _Event_Control_t
{
	unsigned short eventSortArray[1000];
	
	bool bTimeEvent;				// mainform 에서 이벤트발생 -- Timetable실행용
	unsigned char btTimeCnt50m;		// 1초단위 카운트를 위한 임시변수

	unsigned int nTimeEventCnt;		// 이벤트 발생수	50ms단위
	unsigned int nTimeEventSec;		// 이벤트 발생수	초단위

	bool bTableExist;			// 실행해야할 현재 이벤트 테이블의 존재유무
	
	unsigned char btCurve;		//
	float fCurveExp;			// 지수
	float fTimeTableElapse;		// 현재테이블의  경과시간

	float fTimeStart;			// 현재테이블의 시작 시간
	float fTimeEnd;				// 현재테이블의 끝 시간(다음 테이블의 시작시간)
	float fTimeInterval;		// 현재테이블의 시각과 다음 테이블의 시각의 차

	float fValueStart;			// 현재테이블의 시작 온도
	float fValueEnd;				// 현재테이블의 끝 온도(다음 테이블의 시작온도)
	float fValueDiff;			// 현재테이블의 온도과 다음 테이블의 온도의 차
	
	float fValveRateDiff[4];		// 현재테이블의 유속A과 다음 테이블의 유속A의 차
	float fValveStart[4];
	float fValveEnd[4];
	float fValveReturnValue[4];

	float fTgFactor;				// 용리기울기 지수

}PACK_STRUCT Event_Control_t;

typedef struct _UVD_LampTime_Check	// 저장
{
#if 0
	float fD2LampElapse;
	float fWLampElapse;
#else
	unsigned int uiD2LampElapse;
	unsigned int uiWLampElapse;
#endif
} __attribute__((packed)) UVD_LampTime_Check;


typedef struct _UVD_Wave_Check	// 저장 
{
// 파장 캘리브레이션에서 사용하는 변수들 
	unsigned short wave[3];		// 0 486 656
	unsigned short GM_wave[3];

	float fGM_ZeroWaveStep;
	float fGM_486WaveStep;
	float fGM_656WaveStep;
} PACK_STRUCT UVD_Wave_Check;


typedef struct _Main_DEV_STATE_t
{
	unsigned char stateOld;

	unsigned char connected;
	unsigned char disconnected;

//	unsigned char lcdConnect;		// lcd가 연결되어 있는지(통신이 원활하게 되는지?)  - 0x13이후로 

//	unsigned char userDevConfigSave;	

	uint8_t InitIndex;	// 초기화 처리여부
	uint8_t FaultIndex;	// Fault 발생 처리 여부 

	uint8_t requestEnd;// = YES;

// APC Command Send State
	uint32_t apcCmdSendInlet[3];	// 전송 완료 0
	uint32_t apcCmdSendDet;				// 전송 완료 0
	uint32_t apcCmdSendAux;				// 전송 완료 0
	

} PACK_STRUCT Main_DEV_STATE_t;

END_PACK_STRUCT

#endif

