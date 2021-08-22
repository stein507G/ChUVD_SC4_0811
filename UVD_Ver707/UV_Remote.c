/*
 * UV_Remote.c
 *
 *  Created on: 2020. 8. 5.
 *      Author: ksyi
 */
 
#include "config.h"
	 
Remote_State_t detRemote;
Gpio_RemoteChk_t rmtchk={0};
Gpio_RemoteSet_t remoteSet;

Gpio_RemoteOut_t rmtout;
Gpio_RemoteIn_t rmtin={0};

/*******************************************************************************
* FUNCTION : remoteCheckClearInit
* DESCRIPTION :
*	입력신호 검사에 사용되는 변수 초기화
*	입력신호 검출방식 설정
*******************************************************************************/
void remoteCheckClearInit(unsigned char Enable)
{
	if(Enable) {
		rmtchk.checkCnt[REMOTE_START] = 0;
		rmtchk.bCheckEdge[REMOTE_START] = NOT_CHECKED;
		rmtchk.bStartchkEn = YES;
	}
	else {
		rmtchk.bStartchkEn = NO;
	}
}

/*******************************************************************************
* FUNCTION : remoteCheck
* DESCRIPTION :
*	인터럽트를 이용하지 말고 포트로 사용하여 500Hz interrupt(2ms)에서 검사 한다.
*	remote 입력방식을 선택할 수있도록 만들어야 한다.
*	하드웨어 적으로 터미널에서 입력 받은 값을 인버팅하여 읽어들인다.

*	start in ==> 0:None  1: rising edge, 2: falling edge
*	Mark in ==>  0:None  1: rising edge, 2: falling edge
*	ready in ==> 0: Low level, 1: High level  <== 에지검출 안함.

* unsigned char	btAZExtIn;			// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
* unsigned char	btLampExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)
* unsigned char	btChartExtIn;		// 0: None, 1: RISING_EDGE - Pulse(0>1), 2: FALLING_EDGE - Pulse(1>0)

	bCheckEnable : 입력을 받을 수 잇는 상태인가 -  옵션에서 disable / 
		--> start : 이곳에서는 고려하지 않고 run 중이서서 스타트 할 수 없는 상태는 어플에서 결정
		
*******************************************************************************/
void remoteCheck()
{
	static int notReadyChkcnt=0;
	//unsigned char terminal;
	
//	rmtchk.startIn = Start_IN_READ;
//	rmtchk.readyIn = Ready_IN_READ;
//	rmtchk.markIn = Mark_IN_READ;
	rmtchk.startIn = (Start_IN_READ & 0x01);
	rmtchk.readyIn = (Ready_IN_READ & 0x01);
	rmtchk.markIn = (Mark_IN_READ & 0x01);

//	rmtchk.chartIn =	(terminal >> 2) & 0x01;
//	rmtchk.lampIn =		(terminal >> 3) & 0x01;
//	rmtchk.autozeroIn =	(terminal >> 4) & 0x01;

// remote start in
// 한번 입력을 받으면 다시 입력받을 수 있도록 프로그램 상에서 설정을 해줘야한다.
// software 연결되여야만 받을 수잇다. --> rmtchk.bStartchkEn
	if(rmtchk.bCheckEnable[REMOTE_START]==YES && rmtchk.bStartchkEn==YES) {	// 현재 입력을 받을 수 잇는가?
		if(rmtchk.startIn == rmtchk.bEdgeCheckSet[REMOTE_START]) {
			if(rmtchk.bCheckEdge[REMOTE_START]) {
				if(rmtchk.checkCnt[REMOTE_START]++ > REMOTEIN_TRIGGER_CNT) {
					st_UV_func.cRemoteIntr = 1;
					rmtin.r_startIn = YES;

					rmtchk.checkCnt[REMOTE_START]= REMOTEIN_TRIGGER_CNT;
					rmtchk.bCheckEdge[REMOTE_START] = NOT_CHECKED;
					rmtchk.bCheckEnable[REMOTE_START]= NO;
				}
			}
		}
		else {
			rmtin.r_startIn = NO;
			rmtchk.checkCnt[REMOTE_START] = 0;
			rmtchk.bCheckEdge[REMOTE_START] = CHECKED;
		}
	}

	if(rmtchk.bCheckEnable[REMOTE_MARK]==YES) {	// 현재 입력을 받을 수 잇는가?
		if(rmtchk.markIn == rmtchk.bEdgeCheckSet[REMOTE_MARK]) {
			if(rmtchk.bCheckEdge[REMOTE_MARK]) {
				if(rmtchk.checkCnt[REMOTE_MARK]++ > REMOTEIN_TRIGGER_CNT) {
					st_UV_func.cRemoteIntr = 1;
					rmtin.r_markIn = YES;

					rmtchk.checkCnt[REMOTE_MARK]= REMOTEIN_TRIGGER_CNT;
					rmtchk.bCheckEdge[REMOTE_MARK] = NOT_CHECKED;
					rmtchk.bCheckEnable[REMOTE_MARK]= NO;
				}
			}
		}
		else {
			rmtin.r_markIn = NO;
			rmtchk.checkCnt[REMOTE_MARK] = 0;
			rmtchk.bCheckEdge[REMOTE_MARK] = CHECKED;
		}
	}

// remote ready in
	if(rmtchk.readyIn == rmtchk.bEdgeCheckSet[REMOTE_READY] ) {	// or LEVEL_LOW
		if(rmtchk.checkCnt[REMOTE_READY]++ > REMOTEIN_TRIGGER_CNT) {
			if(rmtin.r_readyIn == NO) {
				st_UV_func.cRemoteIntr = 1;
				rmtin.r_readyIn = YES;
				notReadyChkcnt = 0;
			}
			rmtchk.checkCnt[REMOTE_READY]= REMOTEIN_TRIGGER_CNT;					
		}
	}
	else {
		if(notReadyChkcnt++ > REMOTEIN_TRIGGER_CNT) {
			if(rmtin.r_readyIn == YES) {
				st_UV_func.cRemoteIntr = 1;
				rmtin.r_readyIn = NO;
				rmtchk.checkCnt[REMOTE_READY] = 0;				
			}
			notReadyChkcnt = REMOTEIN_TRIGGER_CNT;
		}
	}
}

void Remote_Command(char Comm, unsigned long arg)
{
	switch(Comm)
	{
		case RMT_SET:
  			if(remoteSet.r_startInSet) {
  				rmtchk.bCheckEnable[REMOTE_START]=YES;	// 리모트상태를 읽어야 한다.
				if(remoteSet.r_startInSet == RISING_EDGE) rmtchk.bEdgeCheckSet[REMOTE_START] = CONTACT_OPEN;	// 1
				else rmtchk.bEdgeCheckSet[REMOTE_START] = CONTACT_CLOSE;	// 0
			}
			else {	// disable
				rmtchk.bCheckEnable[REMOTE_START]=NO;
			}

  			if(remoteSet.r_markInSet) {
  				rmtchk.bCheckEnable[REMOTE_MARK]=YES;
				if(remoteSet.r_markInSet == RISING_EDGE) rmtchk.bEdgeCheckSet[REMOTE_MARK] = CONTACT_OPEN;	// 1
				else rmtchk.bEdgeCheckSet[REMOTE_MARK] = CONTACT_CLOSE;	// 0
			}
			else {
				rmtchk.bCheckEnable[REMOTE_MARK]=NO;
			}

			rmtchk.bEdgeCheckSet[REMOTE_READY] = remoteSet.r_readyInSet;
			rmtchk.bCheckEnable[REMOTE_READY]=YES;

  			if(remoteSet.r_AZExtInSet) {
  				rmtchk.bCheckEnable[REMOTE_AUZERO]=YES;
				if(remoteSet.r_AZExtInSet == RISING_EDGE) rmtchk.bEdgeCheckSet[REMOTE_AUZERO] = CONTACT_OPEN;	// 1
				else rmtchk.bEdgeCheckSet[REMOTE_AUZERO] = CONTACT_CLOSE;	// 0
			}
			else {
				rmtchk.bCheckEnable[REMOTE_AUZERO]=NO;
			}

  			if(remoteSet.r_LampExtInSet) {
  				rmtchk.bCheckEnable[REMOTE_LAMP]=YES;
				if(remoteSet.r_LampExtInSet == RISING_EDGE) rmtchk.bEdgeCheckSet[REMOTE_LAMP] = CONTACT_OPEN;	// 1
				else rmtchk.bEdgeCheckSet[REMOTE_LAMP] = CONTACT_CLOSE;	// 0
			}
			else {
				rmtchk.bCheckEnable[REMOTE_LAMP]=NO;
			}

  			if(remoteSet.r_ChartExtInSet) {
  				rmtchk.bCheckEnable[REMOTE_CHART]=YES;
				if(remoteSet.r_ChartExtInSet == RISING_EDGE) rmtchk.bEdgeCheckSet[REMOTE_CHART] = CONTACT_OPEN;	// 1
				else rmtchk.bEdgeCheckSet[REMOTE_CHART] = CONTACT_CLOSE;	// 0
			}
			else {
				rmtchk.bCheckEnable[REMOTE_CHART]=NO;
			}
			break;
		case RMT_OUT:
			if(rmtout.r_markOut == CONTACT_CLOSE) { MARK_OUT_CONTACT_CLOSE; }
			else if(rmtout.r_markOut == CONTACT_OPEN) { MARK_OUT_CONTACT_OPEN; }
			else if(rmtout.r_markOut == CONTACT_PULSE) {
				if(remoteSet.r_markOutSet == CONTACT_CLOSE) { MARK_OUT_CONTACT_CLOSE; }	// on
				else { MARK_OUT_CONTACT_OPEN; }	// off
			}
			if(rmtout.r_startOut == ON) {
				if(remoteSet.r_startOutSet == CONTACT_CLOSE) { START_OUT_CONTACT_CLOSE; }	// on
				else { START_OUT_CONTACT_OPEN; }	// off
			}
			break;
		case RMT_Check_ClrSet:
			remoteCheckClearInit((unsigned char)arg);
			break;
		case RMT_Read:
			rmtin.r_startIn = NO;
			rmtchk.bCheckEnable[REMOTE_MARK] = YES; 		
			rmtchk.bCheckEnable[REMOTE_READY] = YES;			
			rmtchk.bCheckEnable[REMOTE_AUZERO] = YES;			
			rmtchk.bCheckEnable[REMOTE_LAMP] = YES; 
			rmtchk.bCheckEnable[REMOTE_CHART] = YES;			
			break;
	}
}
