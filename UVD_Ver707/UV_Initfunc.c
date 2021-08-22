/*
 * UV_Initfunc.c
 *
 *  Created on: 2020. 8. 4.
 *      Author: ksyi
 */
#include <math.h>
#include "config.h"

//#define		USE_REFERNCE_ONLY		1		//(+)210803 CJKIM, Calibration reference 값으로만 체크

unsigned short ZeroStart=0, ZeroEnd=0;
float constA = CONSTA_START, constB = 0, constAmax = CONSTA_MAX, constAmin = CONSTA_MIN, constA_assum;
float constA_PI_2;

char Init_Sequence=INIT_LED_BLINK;
char Diag_Sequence=SEQ_POWER_CHECK;
char Calib_Sequence=CAL_CHECK_CALIB;
bool goStepCmd = NO;
bool bNotFoundSamZero = NO;
unsigned char Home_Check=NO;

char Calpara_Fail = NO;

volatile unsigned char g_CalibrationDone = NO;	//(+)210804 CJKIM, Calibration done
		


void UV_Hard_Init(void)
{
	unsigned int FPGA_Ver = 0x0000;		// 210702 KSLEE

	DEBUG_I(DBG_LEVEL_INFO, "()\r\n");
	
//	BootBuzzer;
//	PWR_CON_OnOff(ON);	// Power On

	// 210702 KSLEE , FPGA Version 
	FPGA_Ver = FPGA_Ver_READ;

	if( FPGA_Ver & 0x11110000)
		st_UV_func.uiFPGA_Ver = FPGA_Ver & 0x0000FFFF;
	else
		st_UV_func.uiFPGA_Ver = 0x00007001;
		
	DEBUG_I(DBG_LEVEL_INFO, "() FPGA Ver[0x%x]\r\n", st_UV_func.uiFPGA_Ver);


	FLED_Ctrl(FLED_Green);
	D2_BD_OnOff(ON);
	Tungsten_OnOff(OFF);
	D2_Strk(OFF);
	D2_Ctrl(OFF);
//	FrontLED_Ctrl(0x0);
//	ADselect_Ctrl(0x0);
	Ready_Out_Ctrl(OFF);
	Hold_Out_Ctrl(OFF);
	Mark_Out_Ctrl(OFF);

	Dac_Run_Write(ON);
	Dac_L_Write(0x0);
	Dac_R_Write(0x0);

	Heat_Ctrl_Write(0x0);

	st_UV_func.cM1_flag = 0;
	st_UV_func.cM2_flag = 0;
	st_UV_func.cStartIN_flag = 0;
	fram_LampTime_Load();
	fram_Wave_Load();


}

void InitValue(void)
{
	memset(GM_wave , 0 , sizeof(GM_wave));
	
	rmtout.r_markOut=ON;	
	rmtout.r_startOut=OFF;	
	rmtout.r_readyOut=OFF;

	detDa.cmd = UVD_DA_RUN_CMD;

	Remote_Command(RMT_OUT, 1);

// Da_Command ??
}

void defaultSetting(void)
{
	AD_RST_Write(0x1);

//	strcpy(gUVD_INFORM.Model,"ChroZen UV Det : 1.0.3");
//	strcpy(gUVD_INFORM.Model,"7.0.1");
//	strcpy(gUVD_INFORM.Model,"ChroZen HPLC UV Det");
	strcpy(gUVD_INFORM.Model,"YI9620");

//	gUVD_INFORM.nVersion = UVD_PROTOCOL_VERSION;
	gUVD_INFORM.nVersion = UVD_FIRMWARE_VERSION;


	detStt.offsetValueCnt = 0;
	detStt.bOffsetValueSave = NO;
	detStt.bOffsetValueSaveAct = NO;
	detStt.lOffsetValueRef = 0;
	detStt.lOffsetValueSam = 0;
	detStt.fOffsetValueRefAve = 0;
	detStt.fOffsetValueSamAve = 0;


	gUVD_SETUP.ChannelSetA.fTimeConst = 1.0;
//	detStt.timeConstCntA = (unsigned short)(gUVD_SETUP.ChannelSetA.fTimeConst * 50);
	detStt.timeConstCntA = (unsigned short)(gUVD_SETUP.ChannelSetA.fTimeConst * DET_TIME_CONSTANT_SIG_COUNT);	// ChroZen

	detStt.iAcqA = 0;
	detStt.acqSumA=0;
	memset(acqA, 0x00, sizeof(acqA));

	gUVD_SETUP.ChannelSetA.btAutoZero = 1; // 0: None, 1: 수집시작시, 2: 파장 변경시
	detStt.bAZstartA = NO;	// start명령시 AutoZero
	detStt.bAZwaveA = NO;		// 파장변경시 AutoZero

	gUVD_SETUP.ChannelSetA.btAutoOffset = 0;
	detStt.bAOwaveA = NO;
	detStt.bAOpolaA = NO;

	gUVD_SETUP.ChannelSetA.btDataFilter = 0;

	detStt.fAutoZeroDataA = 1.0;	//0.0;
	detStt.fAutoOffsetDataA = detStt.fAutoOffsetDataOldA = detStt.fAutoOffsetDataCurA = 0.0;

	gUVD_SETUP.ChannelSetB.fTimeConst = 1.0;
/*
//	detStt.timeConstCntB = (unsigned short)(gUVD_SETUP.ChannelSetB.fTimeConst * 50);
	detStt.timeConstCntB = (unsigned short)(gUVD_SETUP.ChannelSetB.fTimeConst * DET_TIME_CONSTANT_SIG_COUNT);	// ChroZen
	detStt.iAcqB = 0;
	detStt.acqSumB=0;
	memset(acqB, 0x00, sizeof(acqB));
*/
	gUVD_SETUP.ChannelSetB.btAutoZero = 1; // 0: None, 1: 수집시작시, 2: 파장 변경시
	detStt.bAZstartB = NO;
	detStt.bAZwaveB = NO;

	gUVD_SETUP.ChannelSetB.btAutoOffset = 0;
	detStt.bAOwaveB = NO;
	detStt.bAOpolaB = NO;

	gUVD_SETUP.ChannelSetB.btDataFilter = 0;

	detStt.fAutoZeroDataB = 1.0;	//0.0;
	detStt.fAutoOffsetDataB = 0.0;
	detStt.fAutoOffsetDataB = detStt.fAutoOffsetDataOldB = detStt.fAutoOffsetDataCurB = 0.0;

	gUVD_SETUP.nProgramCount = 0;

	gUVD_EVENT[0].fTime 		= gUVD_SETUP.InitEvent.fTime = 0.0;

	gUVD_EVENT[0].sWavelengthA	= gUVD_SETUP.InitEvent.sWavelengthA=254;	// 190 ~ 1024
	gUVD_EVENT[0].sWavelengthB	= gUVD_SETUP.InitEvent.sWavelengthB=254;
	detStt.waveMode = UVD_SINGLE_WAVE_MODE;

	gUVD_EVENT[0].btD2Lamp		= gUVD_SETUP.InitEvent.btD2Lamp = ON;
	gUVD_EVENT[0].btWLamp		= gUVD_SETUP.InitEvent.btWLamp = OFF;

	gUVD_EVENT[0].btCutFilter	= gUVD_SETUP.InitEvent.btCutFilter = CUTFILTER_NONE;

	detStt.bPolarity = gUVD_EVENT[0].btPolarity = gUVD_SETUP.InitEvent.btPolarity = POSITIVE;
	detStt.bAutozeroA = detStt.bAutozeroB = gUVD_EVENT[0].btAutoZero = gUVD_SETUP.InitEvent.btAutoZero = UVD_SETUP_AUTOZERO_NONE;

	gUVD_EVENT[0].btMarkOut 	= gUVD_SETUP.InitEvent.btMarkOut = CONTACT_OPEN;	// 초기값이 pulse 일 때는 동작하지 앟는다.

	gUVD_SETUP.btAZExtIn		= NONE;
	gUVD_SETUP.btLampExtIn		= NONE;
	gUVD_SETUP.btChartExtIn 	= NONE;

	gUVD_SETUP.btStartExtIn 	= 2;	// falling edge
	gUVD_SETUP.btReadyExtIn 	= NONE;

	gUVD_SETUP.btStartExtOut = CONTACT_CLOSE;
	gUVD_SETUP.btReadyExtOut = CONTACT_CLOSE;
	gUVD_SETUP.btDefaultMarkOut = CONTACT_OPEN;
	gUVD_SETUP.nExtoutTime = EXTOUTTIME_DEFAULT;

	remoteSetup(gUVD_SETUP.btDefaultMarkOut, gUVD_SETUP.nExtoutTime);
// 출력 - 지금동작
	remoteAction(OFF, OFF , CONTACT_OPEN);

	gUVD_SETUP.SigoutSet[0].btSource = 0; // 0: 채널 A, 1: 채널 B, 2: A+B, 3: A-B, 4: B-A
	gUVD_SETUP.SigoutSet[0].btType = 0; 	// 0: 흡광도, 1: 투과도(10^(-흡광도)), 2: 참조광량, 3: 샘플광량
											// btType == 0 :Voilt/Abs, btType == 1:Volt, btType == 2 or 3:Volt/nA
	gUVD_SETUP.SigoutSet[0].fConversion = 1.0;
	gUVD_SETUP.SigoutSet[0].fOffset = 0.0;	// Volt

	gUVD_SETUP.SigoutSet[1].btSource = 0; // 0: 채널 A, 1: 채널 B, 2: A+B, 3: A-B, 4: B-A
	gUVD_SETUP.SigoutSet[1].btType = 0; 	// 0: 흡광도, 1: 투과도(10^(-흡광도)), 2: 참조광량, 3: 샘플광량
											// btType == 0 :Voilt/Abs, btType == 1:Volt, btType == 2 or 3:Volt/nA
	gUVD_SETUP.SigoutSet[1].fConversion = 1.0;
	gUVD_SETUP.SigoutSet[1].fOffset = 0.0;	// Volt

	gUVD_SETUP.ScanSet.sStartWavelength = 254;	// 시작 파장
	gUVD_SETUP.ScanSet.sStopWavelength = 254;	// 종료 파장
	gUVD_SETUP.ScanSet.btScanInterval = 1;	// 스캔 간격
	gUVD_SETUP.ScanSet.btSamplingCount = 1; // 샘플링 횟수(255를 넘지않는다는 조건)

	gUVD_SETUP.DiagnosisSet.btVCCCheck = NO;		// 5V, 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btVDDCheck = NO;		// 15V, 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btVSSCheck = NO;		// -15V, 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btDCCheck = NO; 	// +12V, 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btWPowerCheck = NO; 	// 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btSignal1Check = NO;		// 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btSignal2Check = NO;		// 0: None, 1: Check

	gUVD_SETUP.DiagnosisSet.btD2LampON = NO;		// 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btCalibration = NO; 	// 0: None, 1: Check, 2: 파장교정 실행
	gUVD_SETUP.DiagnosisSet.btWLampON = NO; 	// 0: None, 1: Check
	gUVD_SETUP.DiagnosisSet.btCutFilterCheck = NO;	// 0: None, 1: Check

// =======================================================================
// initial ==> State
// =======================================================================
	gUVD_STATE.btState = UVD_STATE_INITIALIZING;
	detStt.oldState = UVD_STATE_INITIALIZING;

// 오프셋 값을 구하기 위하여 lamp off
	Heat_Ctrl_Write(OFF);
	D2lampCon(OFF);
	WlampCon(UVDMSG_VALUE_WLAMP_OFF);

	gUVD_STATE.btCutFilter = CUTFILTER_NONE;
	gUVD_STATE.uErrorCode = UVD_ERR_NONE;
	gUVD_STATE.nEventStep = 0;
	gUVD_STATE.fElapseTime = 0.0;
	gUVD_STATE.sWavelengthA = 254;
	gUVD_STATE.sWavelengthB = 254;

	gUVD_STATE.SignalA.fAbsorbance = 0;
	gUVD_STATE.SignalA.fReferenceE = 0;
	gUVD_STATE.SignalA.fSampleE = 0;

	gUVD_STATE.SignalB.fAbsorbance = 0;
	gUVD_STATE.SignalB.fReferenceE = 0;
	gUVD_STATE.SignalB.fSampleE = 0;
	gUVD_STATE.btReadyExtIn = NOT_READY;

	detStt.runstop=RUN_STOP;

// =======================================================================
// initial ==> Config
// =======================================================================
	gUVD_CONFIG.sCutWavelength0 = UVD_CUT_WAVELENGTH_0;
	gUVD_CONFIG.sCutWavelength1 = UVD_CUT_WAVELENGTH_1; //
	fram_LampTime_Load();
	gUVD_CONFIG.nD2LampElapse = (unsigned int)detStt.fD2LampElapse;
	gUVD_CONFIG.nWLampElapse = (unsigned int)detStt.fWLampElapse;

// ================================================================================
	eventCtrl.bTimeEvent=OFF;
	eventCtrl.nTimeEventCnt=0;

	detStt.isConnectContSoftware = NO;
	detStt.connectChk = YES;
	detStt.fractionState = NO;

	detStt.leakError = NO;
	detStt.InitialError = NO;

}


void DiagPowerCheck(void)
{
}

void Initialize(void)
{
//	static unsigned char sequence=INIT_LED_BLINK;
	//static unsigned char diagCheck[10]={0};
	static short timeCnt=0;
	static unsigned char calAct = NO;	// calibration을 실행할 것인지 확인
	//unsigned int w,s;
	//float fStep;
	float tmp;

	static char oldInitSequence = -1;	// calibration을 실행할 것인지 확인


	detStt.oldState = gUVD_STATE.btState;
	gUVD_STATE.btState = UVD_STATE_INITIALIZING;


	if (oldInitSequence != Init_Sequence) {
		DEBUG_I(DBG_LEVEL_INFO, "() [%d, %s]\r\n", Init_Sequence, init_sequence_str(Init_Sequence));
		oldInitSequence = Init_Sequence;
	}

	switch(Init_Sequence) {
		case INIT_LED_BLINK:
//			ledReadyRun(LED_BLINK);
			Init_Sequence=INIT_OFFSET_VALUE_SAVE;
			break;
		case INIT_OFFSET_VALUE_SAVE:
			if(++timeCnt < OFFSET_VALUE_DELAY_COUNT) break;	// 5 * 500
			timeCnt=OFFSET_VALUE_DELAY_COUNT;
			detStt.bOffsetValueSaveAct = YES;
		
			if(detStt.bOffsetValueSave == YES) {
				Init_Sequence=INIT_DIAGNOSTIC;
				detStt.bOffsetValueSaveAct = NO;
				timeCnt= 0;
			}
			break;
		case INIT_DIAGNOSTIC:	// 자기진단 및 Motor Home Position , Lamp 점등및 확인.
			if( Diagnostic(DIAG_ALL) ) {
				Init_Sequence = INIT_CALIBRATE ;
			
				calAct = NO;
				fram_Wave_Load();

				if( dWave.fGM_ZeroWaveStep > 0) detStt.fGM_ZeroWaveStep = dWave.fGM_ZeroWaveStep;
				else calAct = YES;

				tmp = (float)dWave.GM_wave[1] - detStt.fGM_ZeroWaveStep - dWave.fGM_486WaveStep;
				if( tmp <= 1 && tmp >= -1) detStt.fGM_486WaveStep = dWave.fGM_486WaveStep;
				else calAct = YES;

				tmp = (float)dWave.GM_wave[2] - detStt.fGM_ZeroWaveStep - dWave.fGM_656WaveStep;
				if( tmp <= 1 && tmp >= -1) detStt.fGM_656WaveStep = dWave.fGM_656WaveStep;
				else calAct = YES;

				if(detStt.fGM_486WaveStep > WAVE_STEP_486_MIN && detStt.fGM_486WaveStep < WAVE_STEP_486_MAX
					&& detStt.fGM_656WaveStep > WAVE_STEP_656_MIN && detStt.fGM_656WaveStep < WAVE_STEP_656_MAX)
				{
				}
				else calAct = YES;
				
				if(calAct == YES) {
					detStt.sFindZeromin = FIND_ZERO_WAVE_MIN;
					detStt.sFind486Diff = FIND_486_WAVE_ZERO_DIFF;
					detStt.sFind656Diff = FIND_656_WAVE_ZERO_DIFF;

					DEBUG_I(DBG_LEVEL_INFO, "() Cal Need = YES 	default value Apply - zeroMin[%d] 486Diff[%d] 656Diff[%d]\r\n",
						detStt.sFindZeromin, detStt.sFind486Diff, detStt.sFind656Diff);
				}
				else {
					detStt.sFindZeromin = (unsigned short)(detStt.fGM_ZeroWaveStep-40);
					detStt.sFind486Diff = (unsigned short)(detStt.fGM_486WaveStep -10);
					detStt.sFind656Diff = (unsigned short)(detStt.fGM_656WaveStep -10);
					
					DEBUG_I(DBG_LEVEL_INFO, "() Cal Need = NO 	load value Apply - zeroMin[%d] 486Diff[%d] 656Diff[%d]\r\n",
						detStt.sFindZeromin, detStt.sFind486Diff, detStt.sFind656Diff);
				}
			}			
			break;
		case INIT_CALIBRATE:	// 분광기 교정.
			// 백업된 데이터가 있고 값이 올바른지 확인하여 실행여부를 결정한다.
			if( Calibrate() ) Init_Sequence = INIT_BACKLASH_TEST ;
			break;
		case INIT_BACKLASH_TEST:
			if( BacklashTest() ) Init_Sequence = INIT_W_LAMP_TEST ;
			break;
		case INIT_W_LAMP_TEST:
			if( wLampTest() ) Init_Sequence = INIT_CUTFILTER_TEST ;
			break;
		case INIT_CUTFILTER_TEST:
			if( cutFilterTest() ) Init_Sequence = INIT_DA_AD_TEST ;
			break;
		case INIT_DA_AD_TEST:
			if( DaAdTest() ) Init_Sequence = INIT_HOLMIUM_TEST ;
			break;
		case INIT_HOLMIUM_TEST:
			if( holmiumTest() ) Init_Sequence = INIT_NOISE_DRIFT_TEST ;
			break;
		case INIT_NOISE_DRIFT_TEST:
			if( noiseDriftTest() ) Init_Sequence = INIT_READYINIT_SET ;
			break;
		case INIT_READYINIT_SET:
			if( readyInitSet() ) {
//				ledReadyRun(LED_ON);
				Init_Sequence=INIT_LED_BLINK;

				g_CalibrationDone = YES;		//(+)210804 CJKIM, Calibration done

			}
			break;
		}
}

char Diagnostic(unsigned char option)
{
//	static unsigned char sequence=SEQ_POWER_CHECK;
	static unsigned char diagCheck[10]={0};
	static short timeCnt=0;

//	static unsigned char Check=NO;
	static char oldSequence=-1;


	if (oldSequence != Diag_Sequence) {
		DEBUG_I(DBG_LEVEL_INFO, "(%d)  [%d, %s]\r\n", option, Diag_Sequence, diagnostic_seq_str(Diag_Sequence));
		oldSequence = Diag_Sequence;
	}

	switch(Diag_Sequence) {
// 1. 전원 검사 - 검사가 끝날 때까지 기다린다.
		case SEQ_POWER_CHECK:

#if 0
			if(++timeCnt < 1000) break;	// 1000 ==> 2초 정도: 일정시간 기다린다.

			timeCnt=1000;	Check=OK;
			for(j=0;j<8;j++) {
				Check&=detDiag.checkVolt[j];
				if(Check == ERROR) break;
			}
			if(Check) {
				diagCheck[SEQ_POWER_CHECK]=OK;
				sequence=SEQ_GRATING_HOME;
				Check = NO; //
				timeCnt = 0; // 밑에서 다시 사용하기위하여 초기화
			}
			else {
				for(k=0; k<8; k++)
				{
					printf("detDiag.sysVolt[%d]=%f\n",k, detDiag.sysVolt[k]);
				}
				sleep(1);
				ErrorProcess(1);
				diagCheck[SEQ_POWER_CHECK] = FAIL;
			}
#else
			if(++timeCnt < 1000) break;	// 1000 ==> 2초 정도: 일정시간 기다린다.

			timeCnt = 0;
			diagCheck[SEQ_POWER_CHECK]=OK;
			Diag_Sequence=SEQ_GRATING_HOME;
			Home_Check = NO;
#endif
			break;
// 2. Grating Motor 및 Filter Motor를 홈위치로 이동한다.
		case SEQ_GRATING_HOME:
			if(!Home_Check) {
				Home_Check = OK;
				detMotor.btMode = GM_HOME_ACT;
//				while(1) {
//					if(ioctl(dev_gpio, IOCTL_DET_MOTOR_CON, &detMotor) >= 0) break;
//					else printf("GRATING_HOME COMMAND ERROR!!!\n");
//				}
				Motor_Ctrl_Command();
			}
// 상태 읽기
//			ioctl(dev_gpio, IOCTL_DET_MOTOR_STATE, &detMotorState);
			if(detMotorState.bGMHome==YES) {
				diagCheck[SEQ_GRATING_HOME]=OK;
				Diag_Sequence=SEQ_FILTER_HOME;
				Home_Check = NO;
			}
			if(detMotorState.bGMError==ERROR) {
				Diag_Sequence = SEQ_ERROR_OCCUR;
				ErrorProcess(-1);
			}
			break;
		case SEQ_FILTER_HOME:
			if(!Home_Check) {
				Home_Check = OK;
				detMotor.btFMMode = CUTFILTER_HOME;
//				while(1) {
//					if(ioctl(dev_gpio, IOCTL_DET_MOTOR_CON, &detMotor) >= 0) break;
//					else qDebug("FILTER_HOME COMMAND ERROR!!!");
//				}
				Motor_Ctrl_Command();
			}
// 상태 읽기
//			ioctl(dev_gpio, IOCTL_DET_MOTOR_STATE, &detMotorState);
			if(detMotorState.bFMHome==YES) {
				diagCheck[SEQ_FILTER_HOME]=OK;
				Diag_Sequence=SEQ_D2_LAMP_ON;
				gUVD_STATE.btCutFilter = CUTFILTER_HOLMIUM;
				D2lampCon(ON);	//		wD2_HEATING_ON;// D2 lamp 예열
			}
			if(detMotorState.bFMError==ERROR) {
				Diag_Sequence = SEQ_ERROR_OCCUR;
				ErrorProcess(-2);
			}
			break;
// 3. Lamp on  및 검사
		case SEQ_D2_LAMP_ON:
			if(++timeCnt > SEQ_D2_LAMP_HEATING_TIME) { // 20초 대기
//				wD2_STRIKE_ON;
//				D2_Ctrl(ON);
				D2_Strk(ON);
				if(timeCnt > SEQ_D2_LAMP_STRIKE_ON_TIME) { // 1초 대기
//					D2_Ctrl(OFF);
					D2_Strk(OFF);
					timeCnt = 0;
					Diag_Sequence=SEQ_D2_LAMP_CHECK;

			// cut filter
					detMotor.btFMMode = CUTFILTER_NONE;
//					ioctl(dev_gpio, IOCTL_DET_MOTOR_CON, &detMotor);
					Motor_Ctrl_Command();
					gUVD_STATE.btCutFilter = CUTFILTER_NONE;
				}
			}
			else {
				if(detStt.btD2State == UVDMSG_VALUE_D2LAMP_ON) {
					timeCnt = 0;
					Diag_Sequence=SEQ_D2_LAMP_CHECK;
				}
			}
			break;
		case SEQ_D2_LAMP_CHECK:	// D2확인 및 텅스텐램프 점등
			if(detStt.btD2State == UVDMSG_VALUE_D2LAMP_ON) {
				if(++timeCnt > 500) {
					DEBUG_I(DBG_LEVEL_INFO, "() D2 LAMP ON!!!! -- PASS !!!\r\n");
					
					// 다음을 위한 초기화
					Diag_Sequence=0;
					timeCnt=0;
					Home_Check = NO;
					return 1;		// 끝	Calibrate();실행
				}
			}
			else {// 실패
			}
			break;
		case SEQ_ERROR_OCCUR:	// D2확인 및 텅스텐램프 점등

			break;
	}
	
	return 0;	// 아직 끝나지 않음.
}

char FindWave(unsigned short startStep, unsigned short endStep, Ads1252_Cmd_t *photoData ,bool scanEnd)
{
	static unsigned char doAdcRd = NO;
	static short timeCnt=0;
	char adcRdOk = ADC_RD_ING;	//	adc 값을 읽었는가?
	
	static unsigned char oldAdcRd = -1;
	static unsigned short oldGMCurStep = -1;
	static unsigned short oldGMDestStep = -1;

//		if (oldAdcRd != doAdcRd && detMotorState.sGMCurStep != oldGMCurStep && detMotor.sGMDestStep != oldGMDestStep) {
//			DEBUG_I(DBG_LEVEL_INFO, "()  	doAdcRd=%d	sGMCurStep=%d	sGMDestStep=%d\r\n", doAdcRd, detMotorState.sGMCurStep, detMotor.sGMDestStep);
//			oldAdcRd = doAdcRd;
//			oldGMCurStep = detMotorState.sGMCurStep;
//			oldGMDestStep = detMotor.sGMDestStep;
//		}
	
	photoData->refD = photoData->samD = 0;

	if(!doAdcRd) {
		if(detMotor.sGMDestStep++ != endStep) {

			detMotor.btMode = GM_CAL_SCAN_ACT;
			detMotor.sStepInterval = 1;
//			ioctl(dev_gpio, IOCTL_DET_MOTOR_CON, &detMotor);
			Motor_Ctrl_Command();
			doAdcRd = YES;
		}
		else {	// 다음을 위한 초기화
			scanEnd = YES;
		}
	}
	else {
//		ioctl(dev_gpio, IOCTL_DET_MOTOR_STATE, &detMotorState);
		if(detMotorState.sGMCurStep == detMotor.sGMDestStep) {
			if(++timeCnt > FIND_WAVE_WAIT_TIME_CNT) {
				// 대기후 읽어라
				photoData->refD = Ads1252.refD; // reference Data
				photoData->samD = Ads1252.samD; // sample Data
				doAdcRd = NO;
				adcRdOk = ADC_RD_OK;
				timeCnt = 0;
				
				//DEBUG_I(DBG_LEVEL_INFO, "() refD = 0x%x		samD = 0x%x\r\n", photoData->refD, photoData->samD);
			}
		}
		else if(FIND_WAVE_MAX_STEP > detMotorState.sGMCurStep) {
			ErrorProcess(-3); // error --> Flow cell check.
		}
	}

	if(scanEnd) {
		doAdcRd = NO;
		timeCnt=0;
		adcRdOk = ADC_RD_STEP_END;
		DEBUG_I(DBG_LEVEL_INFO, "() init -------\r\n");
	}

	return adcRdOk; //	adc 값을 읽었는가?
}


void calReset()
{
//	init_adc_value();
	
	fram_Wave_Reset();

	goStepCmd = NO;
	Home_Check = NO;
	ZeroStart = 0;
	ZeroEnd = 0;
	Init_Sequence = INIT_LED_BLINK;
	Diag_Sequence = SEQ_POWER_CHECK;
	Calib_Sequence = CAL_CHECK_CALIB;
	Calpara_Fail = NO;

	DEBUG_I(DBG_LEVEL_INFO, "()\n");

}

void calParaset(void)
{
	constA_assum = detStt.fGM_656WaveStep * PI_2 / ( asin(656.2/2/cos(DEVIATION_ANGLE/90*PI_2)*0.0012) );
	constAmax = constA_assum * 1.2; // 20% 더 큰값으로 지정
	constA = constAmax / 2.;	// 초기값
}


char calPara(void)
{
	float result;
	static unsigned int CONST_LIMIT_P_cnt = 0;
	static unsigned int CONST_LIMIT_M_cnt = 0;
	static int cnt = 0;
	
// constA값을 구한다.
	constA_PI_2 = constA/PI_2;
	result = sin(detStt.fGM_656WaveStep/constA_PI_2) / sin(detStt.fGM_486WaveStep/constA_PI_2) - CONST_RESULT;

	DEBUG_I(DBG_LEVEL_INFO, "() %d	result = %f, detStt.fGM_656WaveStep = %f, detStt.fGM_486WaveStep = %f, constA_PI_2 = %f, constA = %f\r\n",
		++cnt , result, detStt.fGM_656WaveStep, detStt.fGM_486WaveStep, constA_PI_2, constA);

	if(result > CONST_LIMIT_P) {
		constAmax = constA;
		constA = (constAmax + constAmin) / 2.;

		if(CONST_LIMIT_P_cnt > 100)
		{
			DEBUG_I(DBG_LEVEL_INFO, "() CONST_LIMIT_P_cnt[%d]\r\n", CONST_LIMIT_P_cnt);
//			while(1)
//				sleep(1);
//			gUVD_STATE.uErrorCode = UVD_ERR_INITIAL;
//			gUVD_STATE.btState = UVD_STATE_FAULT;
//			detStt.InitialError = YES;
			cnt = 0;
			fram_Wave_Reset();
			CONST_LIMIT_P_cnt = 0;
			goStepCmd = NO;
			Calpara_Fail = YES;
			Home_Check = NO;
			ZeroStart = 0;
			ZeroEnd = 0;
			ZeroStart = 0;
			ZeroEnd = 0;
			return 0;
		}
		else
		{
			CONST_LIMIT_P_cnt++;
		}
	}
	else if(result < CONST_LIMIT_M) {
		constAmin = constA;
		constA = (constAmax + constAmin) / 2.;

		if(CONST_LIMIT_M_cnt > 100)
		{
			DEBUG_I(DBG_LEVEL_INFO, "() CONST_LIMIT_M_cnt[%d]\r\n", CONST_LIMIT_M_cnt);
//			while(1)
//				sleep(1);
//			gUVD_STATE.uErrorCode = UVD_ERR_INITIAL;
//			gUVD_STATE.btState = UVD_STATE_FAULT;
//			detStt.InitialError = YES;
			cnt = 0;
			fram_Wave_Reset();
			CONST_LIMIT_M_cnt = 0;
			goStepCmd = NO;
			Calpara_Fail = YES;
			Home_Check = NO;
			ZeroStart = 0;
			ZeroEnd = 0;
			return 0;
		}
		else
		{
			CONST_LIMIT_M_cnt++;
		}
	}
	else { // 범위안에 들어온다.
		CONST_LIMIT_P_cnt = 0;
		CONST_LIMIT_M_cnt = 0;
		constB = 656.2 / sin(detStt.fGM_656WaveStep/constA*PI_2) ;
		detStt.InitialError = NO;
		
		DEBUG_I(DBG_LEVEL_INFO, "() CAL_CAL_PARA --- constA = [%f] constB = [%f]\n",constA,constB);
		return 1;
	}
	return 0;

}

//(+)210804 CJKIM, added
void dispaly_wave_motor_step(void)
{
	unsigned short i = 0;
	for (i=0 ; i< MAX_GM_WAVE_COUNT ; i++) {
		DEBUG_I(DBG_LEVEL_INFO, "() wave %d = %d motor step\r\n", i, GM_wave[i]);
	}
}


char saveWave(void)
{
	static unsigned short wave=0;

	if(wave < MAX_GM_WAVE_COUNT) {
		GM_wave[wave]=(unsigned short int) ( asin((float)wave/constB) * constA_PI_2 + 0.5 + detStt.fGM_ZeroWaveStep); // 반올림 하기위한 0.5 더하기

		switch(wave)
		{
			case 0:
				dWave.fGM_ZeroWaveStep = detStt.fGM_ZeroWaveStep;
				dWave.wave[0] = wave;
				dWave.GM_wave[0] = GM_wave[wave];
				fram_Wave_Save();
				break;
			case 486:
				dWave.fGM_486WaveStep = detStt.fGM_486WaveStep;
				dWave.wave[1] = wave;
				dWave.GM_wave[1] = GM_wave[wave];
				fram_Wave_Save();
				break;
			case 656:
				dWave.fGM_656WaveStep = detStt.fGM_656WaveStep;
				dWave.wave[2] = wave;
				dWave.GM_wave[2] = GM_wave[wave];
				fram_Wave_Save();
				break;
		}
		//DEBUG_I(DBG_LEVEL_INFO, "() wave %d = %d motor step\r\n", wave, GM_wave[wave]);

		wave++;
		return 0;
	}
	else {
		wave = 0;
		return 1;
	}
}


float CalFindWave(unsigned short sFindmin,unsigned short sFindmax,unsigned long wavePeak)
{
	static bool goStepCmd=NO, moveEnd=NO;
	Ads1252_Cmd_t photoData;
	char adcRdOk = ADC_RD_ING;	//	adc 값을 읽었는가?
	static unsigned char peak=0;

	static float fFindStep=0.0;
	static long lolprintfhotoData = 0,lpeakData=0;

	//DEBUG_I(DBG_LEVEL_INFO, "() \r\n");
	
	if(goStepCmd == NO) {
		gratingMotorGoPos(sFindmin);
		goStepCmd = YES;
		moveEnd = NO;
	}
	else if(moveEnd == NO) {
		if(gratingMotorGoPosChk()) moveEnd = YES;
	}

	adcRdOk = FindWave(sFindmin, sFindmax,&photoData,0);
	if(adcRdOk == ADC_RD_OK) {

#ifdef USE_REFERNCE_ONLY	//(+)210803 CJKIM, Calibration reference 값으로만 체크
//#if 0
		if( (photoData.refD - lolprintfhotoData) > wavePeak ) peak++;
		if(peak > 2) {
			if(lpeakData < photoData.refD) {
				fFindStep=detMotorState.sGMCurStep-detStt.fGM_ZeroWaveStep; lpeakData=photoData.refD;
			}
		}
		lolprintfhotoData = photoData.refD;

//			DEBUG_I(DBG_LEVEL_INFO, "() photoData.refD - lolprintfhotoData[%d] wavePeak[%ld] peak[%ld] refD[%d]\r\n",
//								photoData.refD - lolprintfhotoData,wavePeak, peak, photoData.refD);

#else
		if( (photoData.samD-lolprintfhotoData) > wavePeak ) peak++;
		if(peak > 2) {
			if(lpeakData < photoData.samD) {
				fFindStep=detMotorState.sGMCurStep-detStt.fGM_ZeroWaveStep; lpeakData=photoData.samD;
			}
		}
		lolprintfhotoData = photoData.samD;

//			DEBUG_I(DBG_LEVEL_INFO, "() photoData.refD - lolprintfhotoData[%d] wavePeak[%ld] peak[%ld] samD[%d]\r\n",
//								photoData.refD - lolprintfhotoData,wavePeak, peak, photoData.samD);

#endif

	} else if(adcRdOk == ADC_RD_STEP_END) { // 영역내 파장스캔 끝남.

		peak = 0;
		lolprintfhotoData = 0;
		lpeakData = 0;

		goStepCmd = NO;
		moveEnd = NO;
		return fFindStep;
	}
	return 0;

}

char Calibrate(void)
{
//	static unsigned char sequence=CAL_CHECK_CALIB;
	//static unsigned char diagCheck[10]={0};
	static short timeCnt=0;

	static bool moveEnd=NO;

	Ads1252_Cmd_t photoData;

	char adcRdOk = ADC_RD_ING;	//	adc 값을 읽었는가?

	long lPhotoData;
//	static bool bNotFoundSamZero = NO;
	static unsigned char peak = 0;
	//static long lolprintfhotoData = 0;
	static long lpeakData=0;

	static char oldCalibSeq = -1;

	if(oldCalibSeq != Calib_Sequence) {
		DEBUG_I(DBG_LEVEL_INFO, "() [%d, %s] \r\n", Calib_Sequence, calibrate_str(Calib_Sequence));
		oldCalibSeq = Calib_Sequence;
	}

	switch(Calib_Sequence) {
// 원점을 찾는다.
// D2 Lamp만 켠 상태에서 파장 30nm추정점까지 스캔하여
// 30nm 추정 파장까지 스캔하여 0nm의 최대광량점을 구함.
// 0nm의 광량은 값이 포화광량이므로 최대광량의 1/2값에 해당하는
// 두위치의 중간값으로 0nm의 위치를 결정함.  이곳을 파장 0점으로 설정
		case CAL_NONE:
//			wWLAMP_ON;
			Tungsten_OnOff(UVDMSG_VALUE_WLAMP_ON);
			sendSelfMsg(UVD_SELFMSG_WLAMP, gUVD_STATE.btWLamp, UVDMSG_VALUE_WLAMP_ON , UVD_ERR_NONE);
			gUVD_STATE.btWLamp = UVDMSG_VALUE_WLAMP_ON;

			adcRdOk = FindWave(0,4000,&photoData,0);
			if(adcRdOk == ADC_RD_OK) {
				//DEBUG_I(DBG_LEVEL_INFO, "() scan step : %4d [%08x] [%08x]\r\n", 
				//	detMotorState.sGMCurStep,photoData.refD,photoData.samD);
			}
			else if(adcRdOk == ADC_RD_STEP_END) {	// 영역내 파장스캔 끝남.
			}
			break;
		case CAL_CHECK_CALIB:
// 파장저장값을 검사하여 맞는지 검토한다.
			Calib_Sequence = CAL_FIND_SAM_ZERO;
			break;
		case CAL_FIND_SAM_ZERO:

			if(goStepCmd == NO) {
				gratingMotorGoPos(detStt.sFindZeromin);
				goStepCmd = YES;
				moveEnd = NO;
			}
			else if(moveEnd == NO) {
				if(gratingMotorGoPosChk()) moveEnd = YES;
			}

			adcRdOk = FindWave(detStt.sFindZeromin, FIND_ZERO_WAVE_MAX, &photoData, 0);
			if(adcRdOk == ADC_RD_OK) {
#ifdef USE_REFERNCE_ONLY	//(+)210803 CJKIM, Calibration reference 값으로만 체크
				lPhotoData = photoData.refD;

#else		
				if(bNotFoundSamZero) lPhotoData = photoData.refD;
				else				 lPhotoData = photoData.samD;
#endif
				DEBUG_I(DBG_LEVEL_INFO, "() bNotFoundSamZero=%d	lPhotoData=%x	ZeroEnd=%d	ZeroStart=%d	sGMCurStep=%d\r\n", 
					bNotFoundSamZero, lPhotoData, ZeroEnd, ZeroStart, detMotorState.sGMCurStep);
				
//				if(lPhotoData > MAX_PHOTOCNT) {
				if(lPhotoData >=MAX_PHOTOCNT) {		//(+)210802 CJKIM, change

					if(!ZeroStart) {
						ZeroStart = detMotorState.sGMCurStep;
						//DEBUG_I(DBG_LEVEL_INFO, "() ZeroStart+++++++++++[%08x] \n",lPhotoData);
					}
					else {
						//DEBUG_I(DBG_LEVEL_INFO, "() ZeroEnd+++++++++++++[%08x] \n",lPhotoData);
						ZeroEnd = detMotorState.sGMCurStep;
					}
				}
				else {
					if((ZeroEnd - ZeroStart) > 10) { // 찾았다. break;
						detStt.fGM_ZeroWaveStep = (float)(ZeroStart+ZeroEnd)/2.;	// 파장 제로에서의 스텝수

						DEBUG_I(DBG_LEVEL_INFO, "() Zero found! ZeroStart [%d] ZeroEnd [%d] GM_ZeroWaveStep = [%f] \r\n",
							ZeroStart, ZeroEnd,detStt.fGM_ZeroWaveStep);
					
						FindWave(detStt.sFindZeromin, FIND_ZERO_WAVE_MAX, &photoData, 1);	// 스캔을 끝내라.
						detStt.sFind486min = (unsigned short)detStt.fGM_ZeroWaveStep + detStt.sFind486Diff;
						detStt.sFind486max = detStt.sFind486min + FIND_486_WAVE_END;

						detStt.sFind656min = (unsigned short)detStt.fGM_ZeroWaveStep + detStt.sFind656Diff;
						detStt.sFind656max = detStt.sFind656min + FIND_656_WAVE_END;

						goStepCmd = NO;
						moveEnd = NO;
						Calib_Sequence = CAL_FIND_W486;
					}
				}
			} 
			else if(adcRdOk == ADC_RD_STEP_END)	{ // 영역내 파장스캔 끝남.
				if(bNotFoundSamZero) {
					if(!ZeroStart) { }	// flow cell is contaminated!!
					ErrorProcess(-4); // error --> Flow cell check.
					
					//DEBUG_I(DBG_LEVEL_INFO, "() Scan Error !!! \n");
				}
				else {
//					bNotFoundSamZero = YES;
					bNotFoundSamZero = ~bNotFoundSamZero;
					if(!ZeroStart) {}	// flow cell is contaminated!!
					ErrorProcess(-5); // error --> Flow cell check.

					DEBUG_I(DBG_LEVEL_INFO, "() Scan Error. : \n");
				}

#if 1			//(+)210803 CJKIM, zero point 찾지 못해 반복적으로 zero point 체크 되는 Bug fix.
				calReset();
				moveEnd = NO;
#else
				ZeroStart=0; ZeroEnd=0;
				goStepCmd = NO;
				moveEnd = NO;
#endif				
			} else {
			}
			break;
			
		case CAL_FIND_W486:
			detStt.fGM_486WaveStep = CalFindWave(detStt.sFind486min, detStt.sFind486max, FIND_486_WAVE_PEAK);
			if(detStt.fGM_486WaveStep > 0) {
				Calib_Sequence = CAL_FIND_W656;
				DEBUG_I(DBG_LEVEL_INFO, "() Found 486 peak[%d] ref[%ld] sam[%ld] Wv486[%f] step[%f] peak[%ld] \r\n",
					peak, photoData.refD, photoData.samD, detStt.fGM_486WaveStep, detStt.fGM_486WaveStep + detStt.fGM_ZeroWaveStep, lpeakData);
			}
			break;
			
		case CAL_FIND_W656:
			detStt.fGM_656WaveStep = CalFindWave(detStt.sFind656min, detStt.sFind656max, FIND_656_WAVE_PEAK);
			if(detStt.fGM_656WaveStep > 0) {
				calParaset();
				Calib_Sequence = CAL_CAL_PARA;
			
				DEBUG_I(DBG_LEVEL_INFO, "() Found 656 peak[%d] ref[%ld] sam[%ld] Wv656[%f] step[%f] peak[%ld]\r\n ",
					peak, photoData.refD, photoData.samD, detStt.fGM_656WaveStep, detStt.fGM_656WaveStep + detStt.fGM_ZeroWaveStep, lpeakData);
				
				DEBUG_I(DBG_LEVEL_INFO, "() 656.2 step[%f] 486.1 step[%f]\r\n",
					detStt.fGM_656WaveStep, detStt.fGM_486WaveStep);
				
				DEBUG_I(DBG_LEVEL_INFO, "()  constAmax = [%f] constA_assum = [%f]  DEVIATION_ANGLE = [%f]\r\n",
					constAmax, constA_assum, DEVIATION_ANGLE);
			}
			break;
			
		case CAL_CAL_PARA:
// Wv486, Wv656을 검사하여 에러인지 판단한다.
// constA값을 구한다.
			if(calPara()) Calib_Sequence = CAL_FILE_OPEN_STEP;
			else 
			{
				if(Calpara_Fail == YES)
				{
#if 0				
					Init_Sequence = INIT_LED_BLINK;
					Diag_Sequence = SEQ_POWER_CHECK;
					Calib_Sequence = CAL_CHECK_CALIB;
					Calpara_Fail = NO;
#else
					calReset();
#endif
					DEBUG_E("() Failed to calibration\r\n ");
				
				}
			}
			break;
			
		case CAL_FILE_OPEN_STEP:
			Calib_Sequence = CAL_SAVE_GRWAVE_STEP;
			break;
		
		case CAL_SAVE_GRWAVE_STEP:
			if(saveWave()) {
				//dispaly_wave_motor_step();
				Calib_Sequence = CAL_CHECK_CALIB;
				timeCnt = 0;
				return 1;
			}
			break;
	}
	return 0;
}


//656.2를 거꾸로 스캔하여 피크점의 오차가 3카운트 이상인 경우 실패이며,	실패의 경우 기기를 정지시킴
char BacklashTest(void)
{
	static unsigned char sequence=0;
	static short timeCnt=0;

	sequence = 0;
	timeCnt = 0;
	return 1;
}

char wLampTest(void)
{
	static unsigned char sequence=0;
	static short timeCnt=0;

	sequence = 0;
	timeCnt = 0;
	return 1;
}


char cutFilterTest(void)
{
	static unsigned char sequence=0;
	static short timeCnt=0;

	sequence = 0;
	timeCnt = 0;
	return 1;
}

char DaAdTest(void)
{
	static unsigned char sequence=0;
	static short timeCnt=0;

	sequence = 0;
	timeCnt = 0;
	return 1;
}

char holmiumTest(void)
{
	static unsigned char sequence=0;
	static short timeCnt=0;

	sequence = 0;
	timeCnt = 0;
	return 1;
}

char noiseDriftTest(void)
{
	static unsigned char sequence=0;
	static short timeCnt=0;

	sequence = 0;
	timeCnt = 0;
	return 1;
}

char readyInitSet(void)
{
	static unsigned char sequence=0;
	static short timeCnt=0;
	unsigned short sComp;

	DEBUG_I(DBG_LEVEL_INFO, "() ... %d  %d %d\r\n", gUVD_SETUP.InitEvent.sWavelengthA, gUVD_SETUP.InitEvent.sWavelengthB, sizeof(acqA));
	
	// Time constant가 변경되면 필터링 데이터를 다시 저장하거나 영역을 다시 설정하여야 한다.
	sComp = detStt.timeConstCntA;
//	detStt.timeConstCntA = (unsigned short)(gUVD_SETUP.ChannelSetA.fTimeConst * 50);
	detStt.timeConstCntA = (unsigned short)(gUVD_SETUP.ChannelSetA.fTimeConst * DET_TIME_CONSTANT_SIG_COUNT);	// ChroZen
	if(sComp != detStt.timeConstCntA) {
		detStt.iAcqA = 0;
		detStt.acqSumA=0;
		memset(acqA, 0x00, sizeof(acqA));
		if(detStt.timeConstCntA <= 0) detStt.timeConstCntA = 1;
	}
/*
	sComp = detStt.timeConstCntB;
//	detStt.timeConstCntB = (unsigned short)(gUVD_SETUP.ChannelSetB.fTimeConst * 50);
	detStt.timeConstCntB = (unsigned short)(gUVD_SETUP.ChannelSetB.fTimeConst * DET_TIME_CONSTANT_SIG_COUNT);	// ChroZen
	if(sComp != detStt.timeConstCntB) {
		detStt.iAcqB = 0;
		detStt.acqSumB=0;
		memset(acqB, 0x00, sizeof(acqB));
		if(detStt.timeConstCntB <= 0) detStt.timeConstCntB = 1;
	}
*/
	gUVD_STATE.btCutFilter = gUVD_SETUP.InitEvent.btCutFilter;

	motorInitSet(gUVD_SETUP.InitEvent.sWavelengthA, gUVD_SETUP.InitEvent.sWavelengthB, gUVD_SETUP.InitEvent.btCutFilter);

	D2lampCon(gUVD_SETUP.InitEvent.btD2Lamp);

	WlampCon(gUVD_SETUP.InitEvent.btWLamp);

	detStt.bPolarity = gUVD_SETUP.InitEvent.btPolarity; //	= gUVD_SETUP.InitEvent.btPolarity = POSITIVE;
	detStt.bAutozeroA = detStt.bAutozeroB = gUVD_SETUP.InitEvent.btAutoZero;	// = gUVD_SETUP.InitEvent.btAutoZero = UVD_SETUP_AUTOZERO_NONE;

	remoteSetup(gUVD_SETUP.btDefaultMarkOut, gUVD_SETUP.nExtoutTime);
	// 출력 - 지금동작
	remoteAction(OFF, OFF , gUVD_SETUP.btDefaultMarkOut);

	// 아래 셋업은 저장만 한다.
	detStt.oldState = gUVD_STATE.btState;
	gUVD_STATE.btState = UVD_STATE_READYINIT;
	sendSelfMsgState(UVD_STATE_READYINIT);
	remoteChkEn(YES);
	detStt.runstop=RUN_STOP;
	sequence = 0;
	timeCnt = 0;

	LEDStateChange = 0;

	return 1;
}

int motorInitSet(unsigned short sWaveA, unsigned short sWaveB, unsigned char cutFilter)
{
//	int waveDiff = 0;

	if(sWaveA != sWaveB) {
		detStt.dualStartStep = detMotor.sStartStep = GM_wave[sWaveA];
		detStt.dualEndStep = detMotor.sEndStep = GM_wave[sWaveB];
		detMotor.btMode = GM_SINGLE_ACT;
		detMotor.sStepInterval = gUVD_SETUP.ScanSet.btScanInterval;

		detMotor.bDualPos = 0;
		detStt.waveMode = UVD_DUAL_WAVE_MODE; // 0:single , 1:dual , 2:scan ,
		detStt.dualMode = UVD_DUAL_MOVEING; //
		if(sWaveA > sWaveB) {
//			detStt.dualWaitCnt = DUALMODE_WAITING_CNT + (detStt.dualStartStep - detStt.dualEndStep) / 160 + 1;
			detStt.dualWaitCnt = DUALMODE_WAITING_CNT + (detStt.dualStartStep - detStt.dualEndStep) / DUALMODE_INTERVAL_CNT + 1;	// ChroZen
//			waveDiff = sWaveA - sWaveB;
			if(cutFilter == CUTFILTER_AUTO) {
				if(sWaveA <= gUVD_CONFIG.sCutWavelength0) { 	// 필터 none
					detMotor.btStartFilter = CUTFILTER_NONE;	// A
					detMotor.btEndFilter = CUTFILTER_NONE;		// B
				}
				else if(sWaveA <= gUVD_CONFIG.sCutWavelength1) {				// filter 1
					detMotor.btStartFilter = CUTFILTER_1ST;
					if(sWaveB <= gUVD_CONFIG.sCutWavelength0)	detMotor.btEndFilter = CUTFILTER_NONE;
					else detMotor.btEndFilter = CUTFILTER_1ST;
				}
				else {
					detMotor.btStartFilter = CUTFILTER_2ND;
					if(sWaveB <= gUVD_CONFIG.sCutWavelength0)	detMotor.btEndFilter = CUTFILTER_NONE;
					else if(sWaveB <= gUVD_CONFIG.sCutWavelength1) detMotor.btEndFilter = CUTFILTER_1ST;
					else detMotor.btEndFilter = CUTFILTER_2ND;
				}
			}
			else {
				cutFilterSelection(cutFilter);
			}
		}
		else {
//			detStt.dualWaitCnt = DUALMODE_WAITING_CNT + (detStt.dualEndStep - detStt.dualStartStep) / 160 + 1;
			detStt.dualWaitCnt = DUALMODE_WAITING_CNT + (detStt.dualEndStep - detStt.dualStartStep) / DUALMODE_INTERVAL_CNT + 1;	// ChroZen
//			waveDiff = sWaveB - sWaveA;
			if(cutFilter == CUTFILTER_AUTO) {
				if(sWaveB <= gUVD_CONFIG.sCutWavelength0) { 					// 필터 none
					detMotor.btStartFilter = CUTFILTER_NONE;
					detMotor.btEndFilter = CUTFILTER_NONE;
				}
				else if(sWaveB <= gUVD_CONFIG.sCutWavelength1) {				// filter 1
					detMotor.btEndFilter = CUTFILTER_1ST;
					if(sWaveA <= gUVD_CONFIG.sCutWavelength1)	detMotor.btStartFilter = CUTFILTER_NONE;
					else detMotor.btStartFilter = CUTFILTER_1ST;
				}
				else {
					detMotor.btEndFilter = CUTFILTER_2ND;
					if(sWaveA <= gUVD_CONFIG.sCutWavelength0) detMotor.btStartFilter = CUTFILTER_NONE;
					else if(sWaveA <= gUVD_CONFIG.sCutWavelength1)	detMotor.btStartFilter = CUTFILTER_1ST;
					else detMotor.btStartFilter = CUTFILTER_2ND;
				}
			}
			else {
				cutFilterSelection(cutFilter);
			}

		}

//		detStt.dualSamplingCountA = detStt.dualSamplingCountB = 25 - detStt.dualWaitCnt;
		detStt.dualSamplingCountA = DUALMODE_CHANNEL_CNT_A - detStt.dualWaitCnt;	// ChroZen
		detStt.dualSamplingCountB = DUALMODE_CHANNEL_CNT_B - detStt.dualWaitCnt;

	}
	else {
		detStt.waveMode = UVD_SINGLE_WAVE_MODE; // 0:single , 1:dual , 2:scan ,
		detMotor.sStartStep = detMotor.sEndStep = GM_wave[sWaveA];
		detMotor.btMode = GM_SINGLE_ACT;
		detMotor.sStepInterval = gUVD_SETUP.ScanSet.btScanInterval;

		if(cutFilter == CUTFILTER_AUTO) {
			if(sWaveA <= gUVD_CONFIG.sCutWavelength0) { 	// 필터 none
				detMotor.btStartFilter = detMotor.btEndFilter = CUTFILTER_NONE;
			}
			else if(sWaveA <= gUVD_CONFIG.sCutWavelength1) {				// filter 1
				detMotor.btStartFilter = detMotor.btEndFilter = CUTFILTER_1ST;
			}
			else {
				detMotor.btStartFilter = detMotor.btEndFilter = CUTFILTER_2ND;
			}
		}
		else {
			cutFilterSelection(cutFilter);
		}
		detMotor.btFMMode = detMotor.btStartFilter;
	}

	gUVD_STATE.sWavelengthA = sWaveA;
	gUVD_STATE.sWavelengthB = sWaveB;

	
	Motor_Ctrl_Command();
	return 0;
}


int D2lampCon(unsigned char d2lamp)
{
	if(d2lamp != gUVD_STATE.btD2Lamp) {
		if(d2lamp)	{
			D2_Ctrl(ON);
			detStt.btD2State = UVDMSG_VALUE_D2LAMP_HEATING;
			gUVD_STATE.btD2Lamp = UVDMSG_VALUE_D2LAMP_HEATING;
			detStt.btD2HeatingCnt = 0;
		}
		else {
			D2_Ctrl(OFF);
			detStt.btD2State = UVDMSG_VALUE_D2LAMP_OFF;
			gUVD_STATE.btD2Lamp = UVDMSG_VALUE_D2LAMP_OFF;
		}
	}
	// 점등 명령을 주고 20초 후에 검사하여야 한다.
	return 1;
}


void WlampCon(unsigned char wlamp)
{
	if(wlamp != gUVD_STATE.btWLamp) {
		if(wlamp)	{ Tungsten_OnOff(ON); }
		else		{ Tungsten_OnOff(OFF); }

//		sendSelfMsg(UVD_SELFMSG_WLAMP, gUVD_STATE.btWLamp, wlamp , UVD_ERR_NONE);
		gUVD_STATE.btWLamp = wlamp;
	}
}


void cutFilterSelection(unsigned char cutFilter)
{
	switch(cutFilter) {
		case CUTFILTER_NONE:
			detMotor.btStartFilter = detMotor.btEndFilter = CUTFILTER_NONE;
			break;
		case CUTFILTER_1ST:
			detMotor.btStartFilter = detMotor.btEndFilter = CUTFILTER_1ST;
			break;
		case CUTFILTER_2ND:
			detMotor.btStartFilter = detMotor.btEndFilter = CUTFILTER_2ND;
			break;
		case CUTFILTER_HOLMIUM:
			detMotor.btStartFilter = detMotor.btEndFilter = CUTFILTER_HOLMIUM;
			break;
	}
}

int remoteSetup(unsigned char markOutDefault, unsigned short nExtoutTime)
{
	remoteSet.r_startInSet = gUVD_SETUP.btStartExtIn;
	remoteSet.r_readyInSet = gUVD_SETUP.btReadyExtIn;

	remoteSet.r_AZExtInSet = gUVD_SETUP.btAZExtIn;
	remoteSet.r_LampExtInSet = gUVD_SETUP.btLampExtIn;
	remoteSet.r_ChartExtInSet = gUVD_SETUP.btChartExtIn;

	remoteSet.r_startOutSet = gUVD_SETUP.btStartExtOut; 	// 스타트 신호가 나갈때의 값	pulse로 동작함.
	remoteSet.r_readyOutSet = gUVD_SETUP.btReadyExtOut; 	// ready 상태일 때의 값

	if(markOutDefault == CONTACT_OPEN) remoteSet.r_markOutSet = CONTACT_CLOSE;		// 이벤트 출력이 pulse일때 동작하는 값 이벤트 출력이 CONTACT_CLOSE 이거나 CONTACT_OPEN이면 그대로 동작함.
	else		remoteSet.r_markOutSet = CONTACT_OPEN;		// 이벤트 출력이 pulse일때 동작하는 값 이벤트 출력이 CONTACT_CLOSE 이거나 CONTACT_OPEN이면 그대로 동작함.

	gUVD_EVENT[0].btMarkOut;					// 초기값이 pulse 일 때는 동작하지 앟는다.
	remoteSet.r_outCntSet = gUVD_SETUP.nExtoutTime / 50;

	Remote_Command( RMT_SET, 1);
	return 0;
}

int remoteAction(unsigned char startOut, unsigned char readyOut, unsigned char markOut)
{
	rmtout.r_startOut = startOut;			// 현재 상태
	rmtout.r_readyOut = readyOut;			// 현재 상태
	rmtout.r_markOut = markOut; // 신호 출력의 디폴트 값

	// 즉시 동작하고 설정에 따라 다시 원상회복된다.	??
//	while(1) {
//		if(ioctl(dev_gpio, IOCTL_GPIO_RMT_OUT, &rmtout) >= 0) return 1;
//		else  qDebug("rmtAction ERROR!!!");
//	}
	Remote_Command( RMT_OUT, 1);
	return 0;
}
/*
int readyOnOff(bool cmd)
{
	if(cmd == ON) {
		if(remoteSet.r_readyOutSet == CONTACT_CLOSE) {
			DET_READY_OUT_CONTACT_CLOSE;
		}
		else {
			DET_READY_OUT_CONTACT_OPEN;
		}
	}
	else {
		if(remoteSet.r_readyOutSet == CONTACT_CLOSE) {
			DET_READY_OUT_CONTACT_OPEN;
		}
		else {
			DET_READY_OUT_CONTACT_CLOSE;
		}
	}

}
*/
void remoteChkEn(bool cmd)
{
	Remote_Command( RMT_Check_ClrSet, cmd);
}

char ErrorProcess(int err)
{
	char ret= 0;
	static int oldErr = 0;
	
	if (oldErr != err) {
		DEBUG_I(DBG_LEVEL_INFO, "(%d)\r\n", err);
		oldErr = err;
	}
	
	return ret;
}

