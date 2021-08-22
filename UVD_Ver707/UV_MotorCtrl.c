/*/*
 * UV_MotorCtrl.c
 *
 *  Created on: 2020. 8. 4.
 *      Author: ksyi
 */
#include "config.h"

Det_Dadata_t detDa;


volatile Det_Motor_Con_t detMotor;
volatile Det_Motor_State_t detMotorState;

/*
const unsigned char M1stateH[40]={	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
									0x01,0x03,0x02,0x0A,0x0E,0x1A,0x12,0x52,0x72,0xD2,
									0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x93,
									0x91,0x91,0x90,0x98,0x8C,0x88,0x80,0xC0,0x60,0x40};
const unsigned char M1stateL[40]={	0x00,0x04,0x06,0x0C,0x08,0x28,0x38,0x68,0x48,0x48,
									0xC8,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,
									0x48,0x4C,0x46,0x44,0x40,0x60,0x30,0x20,0x00,0x00,
									0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
*/
//	Motor Data 15개로 1~15 data 사용함.	FPGA에서 매칭.
const unsigned int M1state[40]={	0x0000,0x0004,0x0006,0x000C,0x0008,0x0028,0x0038,0x0068,0x0048,0x0148,
									0x01C8,0x0348,0x0248,0x0A48,0x0E48,0x1A48,0x1248,0x5248,0x7248,0xD248,
									0x9248,0x924C,0x9246,0x9244,0x9240,0x9260,0x9230,0x9220,0x9200,0x9300,
									0x9180,0x9100,0x9000,0x9800,0x8C00,0x8800,0x8000,0xC000,0x6000,0x4000};

const unsigned char M2state[8] = {0x08,0x09,0x01,0x05,0x04,0x06,0x02,0x0a};

const unsigned char SpeedAcc[48] = {
		5,	10,	15,	20,	25,	30,	35,	40,	45,	50,	55,	60,	65,	70,	75,	80,
		83,	86,	89,	92,	95,	98,	101,104,107,110,113,116,119,122,125,128,
		130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160};
/*
const unsigned char SpeedRdc[48] = {
		2,	4,	6,	8,	10,	12,	14,	16,	18,	20,	22,	24,	26,	28,	30,	32,
		35,	38,	41,	44,	47,	50,	53,	56,	59,	62,	65,	68,	71,	74,	77,	80,
		85,	90,	95,	100,105,110,115,120,125,130,135,140,145,150,155,160};
*/
unsigned char M2p=0;
//unsigned char M2waiting=0;	//
unsigned short int M2waiting=0;	//

unsigned char AccCnt=0;
unsigned char AccArrayNo=0;

short int detTempData;

unsigned long rDataSum=0,/*rDataAve,*/rDataBuf[DET_PHOTO_DATA_AVE]={0};
unsigned long *refData = {0};
unsigned long sDataSum=0,/*sDataAve,*/sDataBuf[DET_PHOTO_DATA_AVE]={0};
unsigned long *samData = {0};

unsigned long rDualSumA=0,/*rDataAve,*/rDualBufA[DET_DUAL_PHOTO_DATA_AVE]={0};
unsigned long sDualSumA=0,/*sDataAve,*/sDualBufA[DET_DUAL_PHOTO_DATA_AVE]={0};

unsigned long rDualSumB=0,/*rDataAve,*/rDualBufB[DET_DUAL_PHOTO_DATA_AVE]={0};
unsigned long sDualSumB=0,/*sDataAve,*/sDualBufB[DET_DUAL_PHOTO_DATA_AVE]={0};



unsigned char M2CoreState=0x08;	//


void Motor_InitValue(void)
{
	DEBUG_I(DBG_LEVEL_INFO, "()\r\n");
	
	detMotorState.bGMError = NOT_CHECK;
	detMotorState.bGMHome = NO;
	detMotor.sGMDestStep=0;
	detMotorState.sGMCurStep=0;

//	wrDET_FM_CON = 0x08;

	detMotorState.bFMError = NOT_CHECK;
	detMotorState.bFMHome = NO;
	detMotor.btFMDestStep = 0;
	detMotorState.btFMCurStep = 0;
}


void GM_step(unsigned char dir)
{
	static unsigned char M1p=0;
	//Mark_Out_Ctrl(ON);

	if(dir) {
		M1p = (M1p<39)? M1p+1 : 0; // 정방향 데이터 출력
//		wrDET_GM_L = M1stateL[M1p];
//		wrDET_GM_H = M1stateH[M1p];
		G_Mot_Write(M1state[M1p]); 
		detMotorState.sGMCurStep++;
	}
	else {
		M1p = M1p? M1p-1 : 39; // 역방향 데이터 출력
//		wrDET_GM_H = M1stateH[M1p];
//		wrDET_GM_L = M1stateL[M1p];
		G_Mot_Write(M1state[M1p]); 
		detMotorState.sGMCurStep--;
	}
	//Mark_Out_Ctrl(OFF);
} 


void FM_go_pos()
{
	if(!M2waiting) {
		if(detMotorState.btFMCurStep > detMotor.btFMDestStep) {
			detMotorState.btFMCurStep--;
			FM_Write(M2state[(--M2p)&0x07]);
			
		}
		else if(detMotorState.btFMCurStep < detMotor.btFMDestStep) {
			detMotorState.btFMCurStep++;
			FM_Write(M2state[(++M2p)&0x07]);
		}
		else if(detMotorState.btFMCurStep == FM_POS_NONE) detMotor.btFMMode = FM_OFF;
		M2waiting = FM_STEP_WAIT_CNT;
	} else M2waiting--;
}

void FM_step()
{
	static unsigned char pulse=0;	//

	switch(detMotor.btFMMode) {
		case FM_OFF:		
			break;
		case CUTFILTER_NONE:	// 24
			detMotor.btFMDestStep = FM_POS_NONE;
			FM_go_pos();
			break;
		case CUTFILTER_1ST:	// 48
			detMotor.btFMDestStep = FM_POS_1ST;
			FM_go_pos();
			break;
		case CUTFILTER_2ND:	// 72
			detMotor.btFMDestStep = FM_POS_2ND;
			FM_go_pos();
			break;
		case CUTFILTER_HOLMIUM:	// 0	// home
			detMotor.btFMDestStep = FM_HOME_POS;
			FM_go_pos();
			break;
		case CUTFILTER_HOME:	// 0	// home
			if(!M2waiting) {
				if(Home2_READ) {
					
					// DEBUG_I(DBG_LEVEL_INFO, "() Home2_READ\r\n");
					
					detMotorState.btFMCurStep = 0;
					detMotor.btFMDestStep = 0;
					detMotorState.bFMHome = YES;
					detMotor.btFMMode = FM_OFF;
					pulse = 0;
				}
				else {
					FM_Write(M2state[(++M2p)&0x07]);
					detMotorState.btFMCurStep++;	M2waiting = FM_HOME_CHECK_WAIT_CNT;
					if(++pulse > 96) { // error
						detMotor.btFMMode = FM_OFF;
						pulse = 0;
						detMotorState.bFMError = ERROR;
					}
				}
			} else M2waiting--;
			break;
		case FM_INSTALL_ACT:
			FM_Write(M2state[0]);
			break;
	}
}

void Motor_TimerCtrl()
{
	static unsigned char waitCnt=0;
	static unsigned short pulse=0;

#if 0	
	static unsigned char oldMotorMode = -1;
	
	if (oldMotorMode != detMotor.btMode) {\
		DEBUG_I(DBG_LEVEL_INFO, "() Mode[%d, %s]\r\n", detMotor.btMode, motor_mode_str(detMotor.btMode));
		oldMotorMode = detMotor.btMode;
	}
#endif	

	switch(detMotor.btMode) {
		case GM_OFF:
			break;
	
		case GM_SINGLE_ACT:
			if(detMotor.sGMDestStep > detMotorState.sGMCurStep) {
				if(detMotor.sAccStartStep > detMotorState.sGMCurStep) {
					if(++AccCnt == SpeedAcc[AccArrayNo]) {
						GM_step(CW);
						AccArrayNo++;
					} else {
						//DEBUG_V(DBG_LEVEL_INFO, "1\r\n");						
					}
				}
				else if(detMotor.sRdcStartStep > detMotorState.sGMCurStep) {
					AccArrayNo = 47;
					GM_step(CW);
				}
				else {
					if(AccCnt-- == SpeedAcc[AccArrayNo]) {
						GM_step(CW);
						AccArrayNo--;
					} else {
						//DEBUG_V(DBG_LEVEL_INFO, "2\r\n");						
					}
				}
			}
			else if(detMotor.sGMDestStep < detMotorState.sGMCurStep) {
				if(detMotor.sAccStartStep < detMotorState.sGMCurStep) {
					if(++AccCnt == SpeedAcc[AccArrayNo]) {
						GM_step(CCW);
						AccArrayNo++;
					} else {
						//DEBUG_V(DBG_LEVEL_INFO, "3\r\n");						
					}
				}
				else if(detMotor.sRdcStartStep < detMotorState.sGMCurStep) {
					AccArrayNo = 47;
					GM_step(CCW);
				}
				else {
					if(AccCnt-- == SpeedAcc[AccArrayNo]) {
						GM_step(CCW);
						AccArrayNo--;
					} else {
						//DEBUG_V(DBG_LEVEL_INFO, "4\r\n");
					}
				}
			}
			else {
				if(detMotorState.sGMDestStepOld != detMotor.sGMDestStep) {
					detMotorState.sGMDestStepOld = detMotor.sGMDestStep;
					rDataSum = 0;
					sDataSum = 0;

//						memset(rDataBuf, 0x00, DET_PHOTO_DATA_AVE * 4);
//						memset(sDataBuf, 0x00, DET_PHOTO_DATA_AVE * 4);

					memset(rDataBuf, 0x00, sizeof(rDataBuf));
					memset(sDataBuf, 0x00, sizeof(sDataBuf));


					//DEBUG_V(DBG_LEVEL_INFO, "5\r\n");
					//detMotor.btMode = GM_OFF;

				} else {
//						detMotorState.sGMDestStepOld = detMotor.sGMDestStep;
//						rDataSum = 0;
//						sDataSum = 0;
//	
//						memset(rDataBuf, 0x00, DET_PHOTO_DATA_AVE * 4);
//						memset(sDataBuf, 0x00, DET_PHOTO_DATA_AVE * 4);


					//DEBUG_V(DBG_LEVEL_INFO, "6\r\n");
				}
				detMotor.btMode = GM_OFF;
			}
			break;
			
		case GM_DUAL_ACT:
			detMotor.sDualcnt++;
			if(detMotor.sGMDestStep > detMotorState.sGMCurStep) {
				GM_step(CW);
			}
			else if(detMotor.sGMDestStep < detMotorState.sGMCurStep) {
				GM_step(CCW);
			}
			else if(detMotor.sGMDestStep == detMotorState.sGMCurStep) {
				if(detMotor.sDualcnt >= detMotor.sDualReadcnt) {	// photo 읽기
					detMotorState.bReadable = YES;
				}
				if(detMotor.sDualcnt >= detMotor.sDualActcnt) {
					detMotorState.bReadable = NO;
					if(detMotor.bDualPos == 0) {
						detMotor.sGMDestStep = detMotor.sEndStep;
						detMotor.bDualPos = 1;
					}
					else {
						detMotor.sGMDestStep = detMotor.sStartStep;
						detMotor.bDualPos = 0;	// 시작파장으로
					}
					detMotor.sDualcnt = 0;
				}
			}
			break;
			
		case GM_SCAN_ACT:	// 파장간격별로 이동하면서 데이터를 읽는다.
			if(detMotor.sGMDestStep > detMotorState.sGMCurStep) GM_step(CW);
			else if(detMotor.sGMDestStep < detMotorState.sGMCurStep) GM_step(CCW);
			break;
			
		case GM_HOME_ACT:
			switch(detMotorState.homeIndex) {
				case 0:		// 현재영점위치에 있으면 영점위치에서 벗어나도록 모터를 돌린뒤 다시 영점을 찾는다.
					pulse=0;
					if(Home1_READ) {
						
						DEBUG_I(DBG_LEVEL_INFO, "() Home1_READ	step0\r\n");
						
						detMotor.sGMDestStep=GM_HOME_CHECK_STEP;	//1000
						detMotorState.sGMCurStep=GM_HOME_STEP;	// 영점위치를 벗어날수 있는 스텝수보다 약간 크게 입력하고						detMotorState.homeIndex = 1;
						//(+)210729 CJKIM, 부팅 시 UVD Lamp 켜지지 않으면서 진행 하지 않는 현상 Bug Fix
						detMotorState.homeIndex = 1;
					}
					else { 
						detMotorState.homeIndex = 2;
					}
					break;
				case 1: 	// 일정스텝을 돌린다.
					if(detMotor.sGMDestStep > detMotorState.sGMCurStep) {
						if(++waitCnt == GM_HOME_CHECK_WAIT_CNT) {
							waitCnt=0;
							GM_step(CW);
						}
					}
					else if(detMotor.sGMDestStep == detMotorState.sGMCurStep) {
						detMotorState.homeIndex = 2;
					} else {
						DEBUG_I(DBG_LEVEL_INFO, "() What is next action?\r\n");
					}
					break;
				case 2: 	// 홈위치를 찾기위하여 홈위치로 돌린다.
					if(++waitCnt == GM_HOME_CHECK_WAIT_CNT) {
						waitCnt=0;
						if(Home1_READ) {
							
							// DEBUG_I(DBG_LEVEL_INFO, "() Motor_TimerInterrupt		Home1_READ	step2\r\n");
							
							detMotor.sGMDestStep = 0;
							detMotorState.sGMCurStep = 0;

							detMotor.btMode = GM_OFF;
							detMotorState.bGMHome = YES;
						}
						else GM_step(CCW);

						if(++pulse > 5000) { // error
							detMotor.btMode = GM_OFF;
							pulse = 0;
							detMotorState.bGMError = ERROR;
						}

					}
					break;
			}
			break;
		case GM_CAL_SCAN_ACT:	// 한스텝씩만 움직인다.
			if(detMotor.sGMDestStep > detMotorState.sGMCurStep) GM_step(CW);
			else if(detMotor.sGMDestStep < detMotorState.sGMCurStep) GM_step(CCW);
			break;
	}

	FM_step();
}

int Motor_Ctrl_Command(void)
{
	unsigned short mid;
	static unsigned char oldMotorMode = -1;

	if (oldMotorMode != detMotor.btMode ) {
		DEBUG_I(DBG_LEVEL_INFO, "() Mode[%d, %s]\r\n", detMotor.btMode, motor_mode_str(detMotor.btMode));	
		oldMotorMode = detMotor.btMode;
	}


	switch(detMotor.btMode) {
		case GM_OFF:

			break;
//				case GM_FREE_ACT:
//					break;
		case GM_SINGLE_ACT:
			//DEBUG_V(DBG_LEVEL_INFO, "M-1\r\n");

			// AccCnt,AccArrayNo등을 초기화 하여야 할 듯하다.
			AccCnt=0;
			AccArrayNo=0;

			detMotor.sGMDestStep = detMotor.sStartStep;
			if(detMotor.sGMDestStep == detMotorState.sGMCurStep) ;
			else if(detMotor.sGMDestStep > detMotorState.sGMCurStep) {
				mid = (detMotor.sGMDestStep+1 - detMotorState.sGMCurStep) / 2;
				if(mid > ACCEL_STEP_CNT) {
					detMotor.sAccStartStep = detMotorState.sGMCurStep + ACCEL_STEP_CNT;
					detMotor.sRdcStartStep = detMotor.sGMDestStep - ACCEL_STEP_CNT;
				}
				else {
					detMotor.sAccStartStep = detMotorState.sGMCurStep + mid;
					detMotor.sRdcStartStep = detMotor.sGMDestStep - mid;
				}
			}
			else {
				mid = (detMotorState.sGMCurStep+1 - detMotor.sGMDestStep) / 2;
				if(mid > ACCEL_STEP_CNT) {
					detMotor.sAccStartStep = detMotorState.sGMCurStep - ACCEL_STEP_CNT;
					detMotor.sRdcStartStep = detMotor.sGMDestStep + ACCEL_STEP_CNT;
				}
				else {
					detMotor.sAccStartStep = detMotorState.sGMCurStep - mid;
					detMotor.sRdcStartStep = detMotor.sGMDestStep + mid;
				}
			}
			break;
		case GM_DUAL_ACT:

			detMotor.sGMDestStep = detMotor.sStartStep;
			detMotor.btFMMode = detMotor.btStartFilter;
			detMotorState.bReadable = NO;
			detMotor.sDualcnt = 0;
			break;
		case GM_SCAN_ACT:	// //1550 ~ 1650 까지 ( 486nm 피크찾기 )

//					detMotor.sStepInterval = 1;
			break;
		case GM_HOME_ACT:

			detMotorState.bGMError = NOT_CHECK; // 에러플래그 초기화
			detMotorState.bGMHome = NO; 	// 홈포지션을 못찾았다.
			detMotor.sGMDestStep = 0;
			detMotorState.sGMCurStep = 0;
			detMotorState.homeIndex = 0;
			break;
		case GM_CAL_SCAN_ACT:

			break;
	}

	switch(detMotor.btFMMode) {
/*
		case FM_OFF:					break;
		case CUTFILTER_NONE:				break;
		case CUTFILTER_1ST:					break;
		case CUTFILTER_2ND:					break;
//				case FM_TEST_ACT:					break;
		case CUTFILTER_HOLMIUM:				break;
*/
		case CUTFILTER_HOME:
			detMotorState.bFMError = NOT_CHECK;	// 에러플래그 초기화
			detMotorState.bFMHome = NO;		// 홈포지션을 못찾았다.
			detMotor.btFMDestStep = 0;
			detMotorState.btFMCurStep = 0;
//					detMotorState.homeIndex = 0;
			break;
		case FM_INSTALL_ACT:
			break;
	}
	return 0;
}











/*--------------------------------------------------------------------------------
-- <<go_cw / go_ccw>> :��
--------------------------------------------------------------------------------
-- 1. read state	:: check state - (st = IDLE) ? goto 2 : wait
-- 2. move 1 step	:: move_cw,move_ccw���
-- 3. read state	:: check state - (st = END) ? rst_state : wait  
-- 4. reset state	:: st <= (end --> IDLE)
--------------------------------------------------------------------------------*/

#define M1_State_R_Addr				0x1005	
#define M1_Cur_Pos_R_Addr			0x1003
#define M1_Set_Pos_R_Addr			0x1004
#define M1_Set_Pos_W_Addr			0x1004

#define M1_Go_CW_Addr					0x1008
#define M1_Go_CCW_Addr				0x1009




#define M2_State_R_Addr				0x1002
#define M2_Cur_Pos_R_Addr			0x1003
#define M2_Set_Pos_R_Addr			0x1004
#define M2_Set_Pos_W_Addr			0x1004

#define M2_Go_CW_Addr					0x1008
#define M2_Go_CCW_Addr				0x1009

#define M2_Reset_dir_Addr					0x100A
#define M2_Reset_Cur_Pos_Addr			0x100B
#define M2_Reset_State_Addr				0x100C

#define M2_Start_Addr					0x100D
#define M2_Stop_Addr					0x100E



#define M1_STATE_READ			FPGA_READ_WORD(M1_State_R_Addr)
#define M1_CUR_POS_READ		FPGA_READ_WORD(M1_Cur_Pos_R_Addr)
#define M1_SET_POS_READ		FPGA_READ_WORD(M1_Set_Pos_R_Addr)
#define M1_SET_POS_WRITE	FPGA_WRITE_WORD(M1_Set_Pos_W_Addr)

#define M2_STATE_READ			FPGA_READ_WORD(M2_State_R_Addr)
#define M2_CUR_POS_READ		FPGA_READ_WORD(M2_Cur_Pos_R_Addr)
#define M2_SET_POS_READ		FPGA_READ_WORD(M2_Set_Pos_R_Addr)
#define M2_SET_POS_WRITE	FPGA_WRITE_WORD(M2_Set_Pos_W_Addr)
#define M2_GO_CW					FPGA_WRITE_WORD(M2_Go_CW_Addr)
#define M2_GO_CCW					FPGA_WRITE_WORD(M2_Go_CCW_Addr)





#define G_Mot_Write(X)		FPGA_WriteWord(G_Mot_W_Addr, X)
#define FPGA_WRITE_WORD(X,Y)		 *(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2)) = (Y)
#define FPGA_READ_WORD(X)				*(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2))
#define FPGA_KEY_READ						FPGA_READ_WORD(0x02)



/*
void FPGA_WriteWord(uint32_t addr,uint32_t data)
{
	 *(volatile uint32_t *) (FPGA_BASE_ADDR + (addr<<2)) = data;
}

uint32_t FPGA_ReadWord(uint32_t addr)
{
	return *(volatile uint32_t *) (FPGA_BASE_ADDR + (addr<<2));
}
*/

/*--------------------------------------------------------------------------------
-- <<�>>
--------------------------------------------------------------------------------
-- 1. check state - (st = IDLE) ? goto 2 : wait
-- 2. move_cw,move_ccw���
-- 3. check state - (st = END) ? rst_state : wait
-- 4. check photo - (photo = open) ? goto 1 : curStep <= 0
-- 5. 
--------------------------------------------------------------------------------*/



