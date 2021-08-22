/*
 * Protocol_UVD.h
 *
 *  Created on: 2020. 8. 3.
 *      Author: ksyi
 */

#ifndef PROTOCOL_UVD_H_
#define PROTOCOL_UVD_H_

#include "type_def.h"

/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// protocol_hplc.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////

// PACKET CODE
//#define PACKCODE_YL9010		(0x901)
#define PACKCODE_YL9020		(0x902)
//#define PACKCODE_YL9030		(0x903)

#define PACKCODE_UPGRADE_PRGM_INFORM	(0x90000)	// upgrade prgm에서 사용한다.
#define PACKCODE_YL90XX_UPDATE		(0x90000)

BEGIN_PACK_STRUCT

typedef struct _YL9000HPLC_PACKET
{
	unsigned int nPacketLength;			// 전체 길이
	unsigned int nDeviceID;				// reserved for future, must be 0
	unsigned int nPacketCode;
	unsigned int nEventIndex;			// Event일 때의 Index	// 0 부터 보낸다. --> 어레이 1번에 저장하라
										// 제어기로 보낼때는 1번 어레이 부터 보내고 ,nEventIndex는 0부터 시작하라
	unsigned int nSlotOffset;
	unsigned int nSlotSize;				// slot's size
	unsigned char Slot[0];
} PACK_STRUCT YL9000HPLC_PACKET;


typedef struct _YL9000HPLC_UPDATE
{
	unsigned int file_name;
	unsigned int file_size;
//	char data[0];
} PACK_STRUCT YL9000HPLC_UPDATE;

END_PACK_STRUCT


// Packet 조립
//
//		buffer:			조립한 packet이 저장될 pointer
//		full_data:		packet code에 해당하는 data 변수
//		entry_data:		full_data 변수 자체 혹은 그 멤버 변수
//		request:		명령 packet이면 0, 응답 요구 packet이면 1
//
/*
#define MAKE_YL9000HPLC_PACKET(buffer,full_data,entry_data,request)	{ \
	unsigned char request_flag = request; \
	((YL9000HPLC_PACKET *)buffer)->nPacketLength = sizeof(YL9000HPLC_PACKET) + ( request_flag? 0 : sizeof(entry_data) );	\
	((YL9000HPLC_PACKET *)buffer)->nSlotOffset = ((int)&entry_data) - ((int)&full_data);	\
	((YL9000HPLC_PACKET *)buffer)->nSlotSize = sizeof(entry_data);	\
	if( !request_flag )	memcpy(((YL9000HPLC_PACKET*)buffer)->Slot, &entry_data, sizeof(entry_data)); \
}

// 데이터를 요구하는 Packet인지 확인
#define IS_REQUEST_PACKET(buffer) \
	( ((YL9000HPLC_PACKET*)buffer)->nPacketLength < sizeof(YL9000HPLC_PACKET) + ((YL9000HPLC_PACKET*)buffer)->nSlotSize )

// 받은 Packet을 처리
//
//		buffer:			받은 packet, 응답 요구 packet이면 응답 packet으로 재조립됨
//		full_data:		packet_code에 해당하는 data 변수
//		response:		응답 packet으로 재조립되면 1로 셋팅됨

#define USE_HPLC_PACKET(buffer,full_data)	\
		memcpy(((unsigned char *)&full_data) + ((YL9000HPLC_PACKET*)buffer)->nSlotOffset, \
				((YL9000HPLC_PACKET*)buffer)->Slot, \
				((YL9000HPLC_PACKET*)buffer)->nSlotSize);

*/
#define IS_INVALID_LIMITS(value, MIN, MAX)		( (value) < (MIN) || (value) > (MAX) )

#define IS_COMPARE_VALUE(value)					( (value) != tmp##value )

#define IF_COMPARE_VALUE(value, func)	\
	if( ( (value) != tmp##value )) { value = tmp##value; func;}

#define APPLY_VALID_LIMITS_CHECK(value, MIN, MAX)	\
	if( (value) < (MIN) || (value) > (MAX) ) return -1; \
	else {value = tmp##value;}

#define APPLY_VALUE(value)	(value = tmp##value)

// PACKET CODE
// RYU
#define CODE_UVD_INFORM		PACKCODE_YL9020_INFORM
#define CODE_UVD_CONFIG		PACKCODE_YL9020_CONFIG
#define CODE_UVD_SETUP		PACKCODE_YL9020_SETUP
#define CODE_UVD_EVENT		PACKCODE_YL9020_EVENT
#define CODE_UVD_STATE		PACKCODE_YL9020_STATE
#define CODE_UVD_SELFMSG		PACKCODE_YL9020_SELFMSG
#define CODE_UVD_DIAGDATA	PACKCODE_YL9020_DIAGDATA
#define CODE_UVD_ACQDATA		PACKCODE_YL9020_ACQDATA
#define CODE_UVD_SCANDATA	PACKCODE_YL9020_SCANDATA
#define CODE_UVD_SERVICE		PACKCODE_YL9020_SERVICE
#define CODE_UVD_SVCDATA		PACKCODE_YL9020_SVCDATA

#define PACKET_SAVE_COUNT	16
#define PACKET_SAVE_COUNT_CHECK	15
#define PACKET_SIZE_NAX		384



extern unsigned int nPacketHeaderSize;

extern unsigned char modeWho;
extern unsigned char sysID;

extern unsigned int update_mode;
extern YL9000HPLC_UPDATE t_update_head;


/*///////////////////////////////////////////////////////////////////////
 //////////////////////////// protocol_uvd.h ////////////////////////////
 *///////////////////////////////////////////////////////////////////////

#define UVD_PROTOCOL_VERSION	100



// UVD 설정
#define UVD_SETUP_CODE_MIN		0
#define UVD_SETUP_NONE				0
#define UVD_SETUP_AUTO_ZERO			1
#define UVD_SETUP_ACQUISITION_START	2
#define UVD_SETUP_SCAN_START		3
#define UVD_SETUP_DIAGNOSIS_START	4
#define UVD_SETUP_STOP				5
#define UVD_SETUP_RESET_ERROR		6
#define UVD_SETUP_READY_INIT		7
#define UVD_SETUP_CODE_MAX		7

// MODE WAVELENGTH
#define UVD_SINGLE_WAVE_MODE	0
#define UVD_DUAL_WAVE_MODE		1
#define UVD_SCAN_WAVE_MODE		2

// DUAL MODE ACTION
#define UVD_DUAL_MOVEING	0
#define UVD_DUAL_WAITING	1
#define UVD_DUAL_READING	2
//#define DUALMODE_WAITING_CNT	5		// 100ms -- // 이동후 대기시간 : 100ms = 5Cnt
#define DUALMODE_WAITING_CNT	50 // 13 ChroZen 	// 5		// 100ms -- // 이동후 대기시간 : 100ms = 10Cnt

#define DUALMODE_INTERVAL_CNT	400 	

#define DUALMODE_CHANNEL_CNT_A		63 	// 125Hz / 2 = 62.5 ==> 63	
#define DUALMODE_CHANNEL_CNT_B		62 //  125hz / 2 = 62.5 ==> 62	


// SCAN MODE ACTION
#define UVD_SCAN_MOVEING	0
#define UVD_SCAN_WAITING	1
#define UVD_SCAN_READING	2
#define SCANMODE_WAITING_CNT	13	// chroZen 125Hz	// 5		// 100ms -- // 이동후 대기시간 : 100ms = 5Cnt
#define SCANMODE_READING_CNT	10 	// chroZen 125Hz	// 4		// 80ms

#define SCANMODE_INTERVAL_CNT		50 // chroZen 125Hz

// UVD 상태
#define UVD_STATE_CODE_MIN		0
#define UVD_STATE_INITIALIZING		0
#define UVD_STATE_READYINIT			1
#define UVD_STATE_RUN_MODE			2	// run
#define UVD_STATE_SCANNING_MODE		3
#define UVD_STATE_DIAGNOSIS_MODE	4
#define UVD_STATE_FAULT				5
#define UVD_STATE_CODE_MAX		5
//#define UVD_STATE_SERVICE_MODE		6

#define UVD_DIAG_POWER_CHECK	(0)
#define UVD_DIAG_POWER_V5D_CHECK	(0)
#define UVD_DIAG_POWER_V15V_CHECK	(1)
#define UVD_DIAG_POWER_NV15D_CHECK	(2)
#define UVD_DIAG_POWER_V12D_CHECK	(3)
#define UVD_DIAG_POWER_WPOWER_CHECK		(4)

#define UVD_DIAG_SIGNAL1_CHECK	(5)
#define UVD_DIAG_SIGNAL2_CHECK	(6)

#define UVD_DIAG_D2LAMP_CHECK		(7)
#define UVD_DIAG_CALIBRATION_CHECK		(8)
#define UVD_DIAG_WLAMP_CHECK		(9)
#define UVD_DIAG_CUTFILTER_CHECK		(10)

// self message
#define UVD_SELFMSG_STATE		(1)
#define UVD_SELFMSG_D2LAMP		(2)
#define UVD_SELFMSG_WLAMP		(3)
#define UVD_SELFMSG_EXTIN		(4)
#define UVD_SELFMSG_EXTOUT		(5)
#define UVD_SELFMSG_ERROR		(6)

#define UVD_SELF_COMMAND_REJECT	(8)	// 전송 명령 거부, 전송된 명령을 실행할 수 없는 상태일 때 전송됨. ( Run ...)
// btMessage == 2
// 반복시도후 Fail이면 state가 fault로 바뀐다.
#define UVDMSG_VALUE_D2LAMP_OFF		(0)
#define UVDMSG_VALUE_D2LAMP_ON		(1)
#define UVDMSG_VALUE_D2LAMP_FAIL	(2)
#define UVDMSG_VALUE_D2LAMP_HEATING	(3)

// btMessage == 3
#define UVDMSG_VALUE_WLAMP_OFF		(0)
#define UVDMSG_VALUE_WLAMP_ON		(1)
// btMessage == 4
// 무시일때는 보내지 말것
#define UVDMSG_VALUE_EXTIN_AZ		(0)
#define UVDMSG_VALUE_EXTIN_LAMP		(1)
#define UVDMSG_VALUE_EXTIN_CHART	(2)
#define UVDMSG_VALUE_EXTIN_START	(3)
#define UVDMSG_VALUE_EXTIN_STOP		(4)

#define UVDMSG_VALUE_EXTIN_READY	(5)
#define UVDMSG_VALUE_EXTIN_NOTREADY	(6)
// btMessage == 5
#define UVDMSG_VALUE_EXTOUT_MKOFF	(0)
#define UVDMSG_VALUE_EXTOUT_MKON	(1)
#define UVDMSG_VALUE_EXTOUT_MKPULSE	(2)
#define UVDMSG_VALUE_EXTOUT_READY	(3)
#define UVDMSG_VALUE_EXTOUT_NOTREADY	(4)

// btMessage == 6
// ErrorCode
#define UVD_ERR_NONE				(0)
// 아래의 INVALIDXXXXX는 STATE에 적용안되고 새로운 값을 전달한다.
#define UVD_ERR_INVALID_CONFIG	(1)
#define UVD_ERR_INVALID_SETUP	(2)
#define UVD_ERR_INVALID_SERVICE	(3)
// STATE에 적용
#define UVD_ERR_D2_LAMP			(4)

#define UVD_ERR_LEAK			(7)
#define UVD_ERR_POWER			(8)

#define UVD_ERR_INITIAL			(9)	// Initial 과정 중 Error 발생한 경우. Calpara()->ConstA,ConstB 값


#define UVD_WAVELENGTH_MAX		900
#define UVD_WAVELENGTH_MIN		190

#define UVD_CUT_WAVELENGTH_0	350
#define UVD_CUT_WAVELENGTH_1	580


#define UVD_SETUP_TIMECONST_MIN		0.1
#define UVD_SETUP_TIMECONST_MAX		9.9

#define UVD_SETUP_CUTFILTER_MAX		4

#define UVD_SETUP_AUTOZERO_NONE		NONE
#define UVD_SETUP_AUTOZERO_AZ		1

#define UVD_CHANNEL_AUTOZERO_NONE		NONE
#define UVD_CHANNEL_AUTOZERO_START		1
#define UVD_CHANNEL_AUTOZERO_WAVE		2
#define UVD_CHANNEL_AUTOZERO_BOTH		3

#define UVD_CHANNEL_AUTOOFFSET_NONE		NONE
#define UVD_CHANNEL_AUTOOFFSET_WAVE		1
#define UVD_CHANNEL_AUTOOFFSET_POLAR	2
#define UVD_CHANNEL_AUTOOFFSET_BOTH		3

//#define UVD_SETUP_EXTOUTTIME_MIN	100
//#define UVD_SETUP_EXTOUTTIME_MAX	100000

#define UVD_SETUP_SCANINTERVAL_MIN	1
#define UVD_SETUP_SCANINTERVAL_MAX	255

#define UVD_SETUP_SAMPLINGCOUNT_MIN		1
#define UVD_SETUP_SAMPLINGCOUNT_MAX		255

BEGIN_PACK_STRUCT

typedef struct _YL9020_UVD_INFORM
{
	char 	Model[32];			// 기기 정보 ex) "UV Detector ver. 1.0.0"
	unsigned int nVersion;			// 프로토콜 버전(100)
	//	unsigned char btInstrumentID;		// 기기 ID, Master / Slave
	// Diagnosis 관련 사항들이 추가되어야 함
} PACK_STRUCT YL9020_UVD_INFORM;

typedef struct _YL9020_UVD_CONFIG
{
	unsigned short	sCutWavelength0;		// Cut Filter 변환 파장
	unsigned short	sCutWavelength1;
	unsigned int	nD2LampElapse;			// D2 사용 시간	: 0x4444	- 17476
	unsigned int	nWLampElapse;			// W  사용 시간 : 0x4444	- 17476
} PACK_STRUCT YL9020_UVD_CONFIG;


typedef struct _YL9020_UVD_EVENT
{
	float 	fTime;			// 실행 시간 [min]
	unsigned short 	sWavelengthA;	// 190 ~ 1024
	unsigned short	sWavelengthB;
	// Lamp 셋업
	unsigned char 	btD2Lamp;	// 0: OFF, 1:ON
	unsigned char	btWLamp;		// 0: OFF, 1:ON
	// Cut Filter 셋업
	unsigned char 	btCutFilter;	// 0: Auto, 1: None, 2: 1st Filter, 3: 2nd Filter, 4: Holium Filter
	// Data Process
	unsigned char	btPolarity;		// 0: Positive, 1:Negative
	unsigned char	btAutoZero;	// 0: None, 1: AZ
	// 외부 출력
	unsigned char	btMarkOut;	// 0: CONTACT_CLOSE, 1: CONTACT_OPEN, 2:PULSE(CONTACT_PULSE)	-- default : 1
} PACK_STRUCT YL9020_UVD_EVENT;

typedef struct _YL9020_UVD_SCAN	{
		unsigned short	sStartWavelength ;	// 시작 파장
		unsigned short	sStopWavelength ;	// 종료 파장
		unsigned char	btScanInterval ;	// 스캔 간격
		unsigned char	btSamplingCount ;	// 샘플링 횟수(255를 넘지않는다는 조건)
        char dum1, dum2;
} PACK_STRUCT YL9020_UVD_SCAN;

typedef struct _YL9020_UVD_DIAGNOSIS	{
//		char dummy;// DIAGNOSIS 설정 데이터
	unsigned char	btVCCCheck;		// +5V, 0: None, 1: Check	5V
	unsigned char 	btVDDCheck;		// 12V, 0: None, 1: Check	12V
	unsigned char	btVSSCheck;		// -12V, 0: None, 1: Check	-12V
	unsigned char 	btDCCheck;		// +12V, 0: None, 1: Check	- motor
	unsigned char	btWPowerCheck;		// 12V 0: None, 1: Check - wLamp
	unsigned char	btSignal1Check;		// 0: None, 1: Check
	unsigned char	btSignal2Check;		// 0: None, 1: Check

	unsigned char	btD2LampON;		// 0: None, 1: Check
	unsigned char 	btCalibration;		// 0: None, 1: Check, 2: 파장교정 실행
	unsigned char	btWLampON;		// 0: None, 1: Check
	unsigned char	btCutFilterCheck;	// 0: None, 1: Check
} PACK_STRUCT YL9020_UVD_DIAGNOSIS;

typedef struct _YL9020_UVD_SETUP
{
	// 명령 코드
	unsigned char	btCommand;
	// 0: None -- NONE으로 설정된 상태에서만 다른 값을 셋팅가능함.
	// 1: AutoZero, 2: Acquisition Start, 3: Scan Start, 4: Diagnosis Start  -- ReadyInit 상태에서 만 실행가능한 명령
	// 5: Stop
	// 6: Reset Error
	// 명령 실행 후 자동으로 reset 되어야 하는 일회성 명령임.

	struct YL9020_UVD_CHANNEL	{
		float		fTimeConst;		// 시정수 [sec]
		unsigned char	btAutoZero;		// 0: None, 1: 수집시작시, 2: 파장 변경시,	3: 둘다적용
		unsigned char	btAutoOffset;	// 0: None, 1: 파장변경시, 2: 극 변환시 ,	3: 둘다적용
		unsigned char	btDataFilter;	// 0: Moving Average, 1: RC, 2: Bessel
	};

	struct YL9020_UVD_CHANNEL	ChannelSetA;
	struct YL9020_UVD_CHANNEL	ChannelSetB;

	unsigned int		nProgramCount;
	YL9020_UVD_EVENT	InitEvent;		// Event Table [0]번과는 같은 값 것이다.

	// 외부 입력
	unsigned char	btAZExtIn;			// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char	btLampExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
	unsigned char	btChartExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)

	unsigned char	btStartExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
//	unsigned char 	btMarkExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)

	unsigned char	btReadyExtIn;		// 0: LEVEL_LOW - Level0, 1: LEVEL_HIGH - Level1
	unsigned char	btReadyExtOut;		// 0: CONTACT_CLOSE (Level0), 1: CONTACT_OPEN (Level1)

	unsigned char	btStartExtOut;		// 0: CONTACT_CLOSE (Level0), 1: CONTACT_OPEN (Level1)

	unsigned char	btDefaultMarkOut;	// 0: CONTACT_CLOSE, 1: CONTACT_OPEN	-- default : 1		 0:Off, 1:On
	unsigned int	nExtoutTime;		// mSec -> Pulse일경우만 해당(100--100000)		-- default : 100


	// SigoutSet setup
	struct YL9020_UVD_SIGOUT	{
		unsigned char	btSource ;	// 0: 채널 A, 1: 채널 B, 2: A+B, 3: A-B, 4: B-A
		unsigned char	btType ;	// 0: 흡광도, 1: 투과도(10^(-흡광도)), 2: 참조광량, 3: 샘플광량
									// btType == 0 :Voilt/Abs, btType == 1:Volt, btType == 2 or 3:Volt/nA
		float fConversion ;			//
		float fOffset ;				// Volt
									// 출력전압 = ( x(btSource,btType) * fConversion) + fOffset
	} SigoutSet[2];

/*
	// Scan setup
	struct YL9020_UVD_SCAN	{
		unsigned short	sStartWavelength ;	// 시작 파장
		unsigned short	sStopWavelength ;	// 종료 파장
		unsigned char	btScanInterval ;	// 스캔 간격
		unsigned char	btSamplingCount ;	// 샘플링 횟수(255를 넘지않는다는 조건)
	} ScanSet;
*/
	YL9020_UVD_SCAN	ScanSet;

	// Diagnosis Setup
	YL9020_UVD_DIAGNOSIS DiagnosisSet;

} YL9020_UVD_SETUP;
/*
struct YL9020_UVD_SIGNAL	{
	float 	fAbsorbance;
	float	fReferenceE;
	float	fSampleE;
};
*/

typedef struct _YL9020_UVD_SIGNAL	{
	float 	fAbsorbance;
	float	fReferenceE;
	float	fSampleE;
} PACK_STRUCT YL9020_UVD_SIGNAL;


typedef struct _YL9020_UVD_STATE		// 기기와 연결된 후  1초마다 자동 수신
{
	unsigned char 	btState;
				// 0: Initializing 1: Ready, 2: Run, 3: Scanning,
				// 4: Diagnosis, 5: Fault
				// Fault 상태가 되면 Setup의 Reset/Init명령이 들어올 때까지 대기(다른 명령은 안먹음
	unsigned char 	btD2Lamp;			// 0: OFF, 1:ON		//추가하여야 할 듯  2 : fail 3 : ing(heating중)
	unsigned char	btWLamp;			// 0: OFF, 1:ON
	unsigned char 	btCutFilter;		// 0: Auto, 1: None, 2: 1st Filter, 3: 2nd Filter, 4: Holium Filter
	unsigned int	uErrorCode;			// selfmsg Error 참조

	unsigned int	nEventStep;	// 현재 실행 중 인 Step 번호
	float		fElapseTime;	// Running Time[min]

	unsigned short 	sWavelengthA;
	unsigned short 	sWavelengthB;

	YL9020_UVD_SIGNAL SignalA;
	YL9020_UVD_SIGNAL SignalB;

	// 외부 단자 상태
	unsigned char	btReadyExtIn;		// 0 :NOT_READY, 1 : READY
}PACK_STRUCT YL9020_UVD_STATE;

typedef struct _YL9020_UVD_ACQDATA
{
	float	fRunTime;
	YL9020_UVD_SIGNAL SignalA[SIGNAL_SEND_COUNT];
	YL9020_UVD_SIGNAL SignalB[SIGNAL_SEND_COUNT];
} PACK_STRUCT YL9020_UVD_ACQDATA;


typedef struct _YL9020_UVD_SCANDATA
{
	unsigned short sWavelength;
	float fAbsorbance;
	float fReferenceE;
	float fSampleE;
} PACK_STRUCT YL9020_UVD_SCANDATA;

typedef struct _YL9020_UVD_DIAGDATA
{
	unsigned int	uDiagnosis;
	float		fValue;
	unsigned char	btPass;			// 0: reject, 1: Pass
} PACK_STRUCT YL9020_UVD_DIAGDATA;


typedef struct _YL9020_UVD_SELFMSG
{
	unsigned char	btMessage;
			// 1: State, 2: D2Lamp, 3: WLamp,
			// 4: ExtIn, 5: ExtOut, 6: Error
	unsigned short	sOldValue, sNewValue;
	unsigned int uErrorCode;
} PACK_STRUCT YL9020_UVD_SELFMSG;

END_PACK_STRUCT


typedef struct _UVD_FN_DEF
{
	int (*proc)();
} UVD_FN_DEF;

extern YL9020_UVD_INFORM	 gUVD_INFORM;
extern YL9020_UVD_CONFIG	 gUVD_CONFIG;
extern YL9020_UVD_SETUP	 gUVD_SETUP;
extern YL9020_UVD_EVENT	 gUVD_EVENT[EVENT_TABLE_COUNT];
extern YL9020_UVD_STATE	 gUVD_STATE;
extern YL9020_UVD_SELFMSG	 gUVD_SELFMSG;
extern YL9020_UVD_DIAGDATA gUVD_DIAGDATA;
extern YL9020_UVD_ACQDATA	 gUVD_ACQDATA;
extern YL9020_UVD_SCANDATA gUVD_SCANDATA;

extern YL9020_UVD_INFORM	 tmpgUVD_INFORM;
extern YL9020_UVD_CONFIG	 tmpgUVD_CONFIG;
extern YL9020_UVD_SETUP	 tmpgUVD_SETUP;
extern YL9020_UVD_EVENT	 tmpgUVD_EVENT;
extern YL9020_UVD_STATE	 tmpgUVD_STATE;
extern YL9020_UVD_SELFMSG	 tmpgUVD_SELFMSG;
extern YL9020_UVD_DIAGDATA tmpgUVD_DIAGDATA;
extern YL9020_UVD_ACQDATA	 tmpgUVD_ACQDATA;
extern YL9020_UVD_SCANDATA tmpgUVD_SCANDATA;

extern Det_State_t		detStt;	// 통신프로토콜에 없는 것

#endif /* PROTOCOL_UVD_H_ */
