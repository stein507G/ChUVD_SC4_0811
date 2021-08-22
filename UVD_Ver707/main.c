/*-------------------------------------------------------------------------*/

#include "config.h"

#define TIME_50MS_CNT			50
#define TIME_10MS_CNT			10

#define CNT_LWIP_PERIODIC_HANDLE			1		// 첫번째
#define CNT_READY_POW_BTN_BASE			2
#define CNT_ADC_PROCESS					3						
//#define CNT_4								4
#define CNT_SVCDATA_SEND_BASE				4
#define CNT_SVCDATA_SEND					100
#define CNT_EVENT_TIME_BASE				5
#define CNT_EVENT_TIME						5				// 100HZ / 5 = 20Hz	// 50ms
#define CNT_STATE_CHECK_BASE				6
#define CNT_STATE_CHECK					100				// 100HZ / 100 = 1Hz


#define CNT_STATE_SEND_BASE				7
#define CNT_STATE_SEND						100				// 100HZ / 100 = 1Hz
#define CNT_WATCHDOG_RTC_BASE			8
#define CNT_WATCHDOG_RTC					100				// 100HZ / 100 = 1Hz
#define CNT_RS422_TSTAT_CHECK_BASE		9
#define CNT_RS422_TSTAT_CHECK				5				// 100HZ / 5 = 20Hz
#define CNT_RESET_10						10 


#define	MAX_STATE_ACTION_COUNT			5		// 2mS(500Hz) * 50 
#define	MAX_MOTOR_TIME_COUNT			2		// 125uS * 2 


/*==============================================================================
 * Global variables.
 */
volatile UV_FUNC_t st_UV_func;		//(-/+) 210603 CJKIM, volatile keyword added



uint8_t FrameReceived = 0;

uint8_t Time1S;
uint16_t Time1S_cnt;

uint32_t timeStateActCnt = 0;
uint32_t timeSataeSendCnt = 0;
uint32_t timeCnt100Hz = CNT_LWIP_PERIODIC_HANDLE;	

uint32_t stateCheckCnt = 0;	
uint32_t rs422TstatCheckCnt = 0;	
uint32_t eventTimeCnt = 0;			
uint32_t wdg_reset_cnt=0;		// 1sec
uint32_t Test50HzCnt = 0;			

//(-/+) 210531 CJKIM, IP Addr  Header 파일 Define 변경,  값 동일
uint8_t IP_Addr0 = ETHERNET_CONF_IPADDR0;
uint8_t IP_Addr1 = ETHERNET_CONF_IPADDR1;
uint8_t IP_Addr2 = ETHERNET_CONF_IPADDR2;
uint8_t IP_Addr3 = ETHERNET_CONF_IPADDR3;

volatile unsigned char g_SendSignalData = NO;
volatile unsigned char g_SendState = NO;


extern volatile uint8_t TimeEvent;	// 1mS -> 0.125
extern volatile uint8_t Time1mS;
extern volatile uint16_t Time1mS_cnt;

//extern GCDBG_ST gcdbg;
extern GCSVC_ST gcsvc;

extern RS422_ST rs422if;

#if READY_INIT_MAIN		//(+)210810 CJKIM, Main에서 Ready Init 전송
extern volatile unsigned char g_SendReadyInit;	//(+)210810 CJKIM, Main에서 Ready Init 전송
#endif

extern volatile unsigned char g_SendScanData;

void lwip_init(void);
void netif_config(void);


void (*pfLwIP_Periodic_Handle)(uint32_t);
void LwIP_Periodic_Handle_blank(__IO uint32_t localtime){};

void (*pfM_SPI_function)(void);
void M_SPI_function_blank(void) {};


extern void ADC_Read(void);
extern void UV_Hard_Init(void);

extern void IP_Init();
extern void etharp_tmr();

extern void chrozen_recv();

int main()
{
	uint32_t wdg_reset;
	uint8_t signalOutCount = 0;
	uint8_t stateInverval = 0;

	static char LED_State = OFF;

#ifndef DEVICE_TEST		//(-/+) 210602 CJKIM, 보드 테스트를 위해 주석 처리	
	int State_Cnt = 0;
	int nAcqCount = 0;
#endif

	//set_dbg_level(DBG_LEVEL_WARN | DBG_LEVEL_INFO);		//(+)210701 CJKIM, Debug Level

	cpu_init();
	gcsvc.MasterSlave = Intro();

	wdg_reset = MSS_WD_timeout_occured();
	if(wdg_reset)
	{
		MSS_WD_clear_timeout_event();
	}

#if 0
	if(GCC_MASTER == gcsvc.MasterSlave)
	{
		lwip_init();
		netif_config();
		tcp_server_init(); //--> move to DHCP process

	}
	else
	{
	}
#endif

#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
	init_queue();
#endif

	UV_Hard_Init();
	Motor_InitValue();
	IP_Init();
	
	lwip_init();
	netif_config();
	tcp_server_init(); //--> move to DHCP process

	InterruptEnable();
	defaultSetting();

	/////////////////////
	// by ryu oven source
	MSS_WD_reload();

	tcp_tmr();
	etharp_tmr();
	/////////////////////

	/*---------------------------------------------------------------
	* Infinite loop.
	----------------------------------------------------------------*/
	for(;;)
	{
		if( st_UV_func.cRemoteIntr)
		{
			st_UV_func.cRemoteIntr = 0;
			remoteInfn();
		}


		if(TimeEvent)	// 0.125mS	// Motor
		{
			TimeEvent = 0;

#if !MOTOR_TIME_INTR		//(+)210727 CJKIM,  ISR 에서 처리	
			Motor_TimerCtrl();		
#endif		

#if 0		//(+)210811 CJKIM, Timer Interrupt 이동
			if(++Time1mS_cnt >= 8)	// 1ms
			{
				Time1mS_cnt =0;
				Time1mS = 1;
			}
#endif			
		}

		if(Time1mS)	// 1mS
		{
			Time1mS = 0;

#if ryuSF	//================================================================
			if(rs422if.rx_intr == 1) {	// RS422수신 인터럽트 발생
				rs422if.rx_intr = 0;
#if RYU_TCP_TEST_MAIN		// RYU_TCP				
				pfAPC_Rs422_Parse();	// APC_Rs422_Parse
#endif
			}
#endif

#if READY_INIT_MAIN		//(+)210810 CJKIM, Main에서 Ready Init 전송
			if( YES == g_SendReadyInit ) {
				g_SendReadyInit = NO;
				sendSelfMsgState(UVD_STATE_READYINIT);
				//DEBUG_I(DBG_LEVEL_INFO, "() Send Ready [%d]\r\n", g_SendReadyInit);
			}
#endif

			switch(timeCnt100Hz++)		// 10mS ,  10(Count) * 1mS
			{
				case CNT_LWIP_PERIODIC_HANDLE:
					LwIP_Periodic_Handle(get_mili_local_time());
					break;
				case CNT_READY_POW_BTN_BASE:
					Ready_PowBtn_Check();
					break;
				case CNT_ADC_PROCESS:		// ADC init & Reading
					break;
				case CNT_SVCDATA_SEND_BASE:
					if( ++Test50HzCnt >= 2)	// 20mS ,	2 * 10(Count) * 1mS
					{
						Test50HzCnt = 0;
//						photoDataRead();
					}
					break;
				case CNT_EVENT_TIME_BASE:	
					if(++eventTimeCnt >= CNT_EVENT_TIME) {		// 100 / 5 20Hz -> 50ms
						eventTimeCnt = 0;
						eventCtrl.bTimeEvent = ON;
					}
					break;
				case CNT_STATE_CHECK_BASE:							
					if(++stateCheckCnt >= CNT_STATE_CHECK) {		// 1Hz -> 1Sec
						stateCheckCnt = 0;
		
#ifdef DEVICE_TEST		//(-/+) 210602 CJKIM, 보드 테스트를 위해 주석 처리
						stateCheck();	// error Check
#endif						
					}
					break;
				case CNT_STATE_SEND_BASE:		
#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
					chrozen_recv();
#endif
					break;
				case CNT_RS422_TSTAT_CHECK_BASE:
					break;
				case CNT_WATCHDOG_RTC_BASE:
					if(++wdg_reset_cnt >= CNT_WATCHDOG_RTC) {	// 1Hz -> 1Sec
						wdg_reset_cnt =0;
						MSS_WD_reload();	// 1Sec 마다 실행할 것
					}
					break;
				case CNT_RESET_10:
					timeCnt100Hz = CNT_LWIP_PERIODIC_HANDLE;	
					break;
				default:
					timeCnt100Hz = CNT_LWIP_PERIODIC_HANDLE;	
					break;
			}

#ifndef DEVICE_TEST		//(-/+) 210602 CJKIM, 보드 테스트를 위해 주석 처리	
			if (Clarity_Connect == STATE_COMM_CONNECTED) {
				if(++State_Cnt == 1000) {	// 1 Sec
					State_Cnt=0;
					send_State();			// Client 연결되면 전송한다. : mainform.cpp에서 전송결정
					LED_State = ~LED_State;	// BD LED
					LED_OnOff(LED_State);
				}

				if(++nAcqCount == 200) {	// 200mS
					nAcqCount=0;
					sendAcqdata();	
				}

			}

#endif

#if USE_SEND_STATE_1MS		//(+)210811 CJKIM, ACQ 와 STATE Data 전송이  10mS 이내로 전송 하지 않도록 수정.
			//(+)210811 CJKIM, sigout 4번째 데이터 전송 후 180mSec 지나면 state 전송
			//(+)210716 CJKIM, Clarity 와 연결 되었을 시에만 전송 
			if(detStt.runstop == RUN_START) {

				if (4 == signalOutCount) {
					stateInverval++;				
					if ( 180 == stateInverval) {		

						if ( STATE_COMM_CONNECTED == Clarity_Connect) {
							send_State();	// Client 연결되면 전송한다. : mainform.cpp에서 전송결정
							//DEBUG_I(DBG_LEVEL_INFO, "() send_State [%d]\r\n", stateInverval);
						}
		
						LED_State = ~LED_State; // BD LED
						LED_OnOff(LED_State);
						g_SendState = NO;					
					}
				} else {
					stateInverval = 0;
				}

				
			}
			else {
				if(YES == g_SendState) {	//(+)210811 CJKIM, Flag change
	
					//(+)210716 CJKIM, Clarity 와 연결 되었을 시에만 전송 
					if ( STATE_COMM_CONNECTED == Clarity_Connect) {
						send_State();	// Client 연결되면 전송한다. : mainform.cpp에서 전송결정
					}
	
#if 0			// for Test	
					if(detStt.runstop != RUN_START) {
						DEBUG_I_F(DBG_LEVEL_INFO, "() Sam A[%.6f, %.6f, %.6f] Sam B[%.6f, %.6f, %.6f]\r\n",
							gUVD_STATE.SignalA.fAbsorbance, gUVD_STATE.SignalA.fReferenceE, gUVD_STATE.SignalA.fSampleE, 
							gUVD_STATE.SignalB.fAbsorbance, gUVD_STATE.SignalB.fReferenceE, gUVD_STATE.SignalB.fSampleE );
					}
#endif				
					
					LED_State = ~LED_State; // BD LED
					LED_OnOff(LED_State);
					g_SendState = NO;
				}
#endif				
			}

		}
		
#ifdef DEVICE_TEST		//(-/+) 210602 CJKIM, 보드 테스트를 위해 주석 처리	
		if(st_UV_func.b125Hz_Int)	// 125Hz
		{
			st_UV_func.b125Hz_Int = 0;

#if !PHOTO_READ_INTR			
			photoDataRead();	// ChroZen 125Hz (8mS)
#else	

			if (YES == g_SendScanData) {
				g_SendScanData = NO;
				sendScandata();
				
				if(detStt.scanDestWave >= detStt.scanEndWave) {
					detStt.waveMode = UVD_SINGLE_WAVE_MODE;
					readyInitSet();
					g_SendReadyInit = YES;
					//sendSelfMsg(UVD_SELFMSG_STATE, UVD_STATE_SCANNING_MODE, UVD_STATE_READYINIT , UVD_ERR_NONE);	//(+)210810 CJKIM, 중복 되어 제거
				}
				else {
					detStt.scanDestWave += detStt.scanInterval;
				}
			}

			if(YES == g_SendSignalData) {		// 200mSec로 전송		// 9120UVD
				g_SendSignalData = NO;
				
				if(detStt.runstop==RUN_START && STATE_COMM_CONNECTED == Clarity_Connect) {	//(+)210716 CJKIM, Clarity 와 연결 되었을 시에만 전송 
					signalOutCount %= 5;
					signalOut();
					signalOutCount++;


#if 0			// for Test	
					DEBUG_I_F(DBG_LEVEL_INFO, "() Sam A[%.6f, %.6f, %.6f] Sam B[%.6f, %.6f, %.6f]\r\n",
						gUVD_STATE.SignalA.fAbsorbance, gUVD_STATE.SignalA.fReferenceE, gUVD_STATE.SignalA.fSampleE, 
						gUVD_STATE.SignalB.fAbsorbance, gUVD_STATE.SignalB.fReferenceE, gUVD_STATE.SignalB.fSampleE );
#endif				
			
					gUVD_ACQDATA.fRunTime = gUVD_STATE.fElapseTime;
					detStt.cnt++;
					//DEBUG_I(DBG_LEVEL_INFO, "() SigOut [%d]\r\n", detStt.cnt);	//(+)210604 CJKIM, For Debug	
			
				}
				else {
					detStt.cnt = 0;
				}

			}
#endif

#if !USE_SEND_STATE_1MS
			if(YES == g_SendState) {	//(+)210811 CJKIM, Flag change

				//(+)210716 CJKIM, Clarity 와 연결 되었을 시에만 전송 
				if ( STATE_COMM_CONNECTED == Clarity_Connect) {
					send_State();	// Client 연결되면 전송한다. : mainform.cpp에서 전송결정
				}

#if 0			// for Test	
				if(detStt.runstop != RUN_START) {
					DEBUG_I_F(DBG_LEVEL_INFO, "() Sam A[%.6f, %.6f, %.6f] Sam B[%.6f, %.6f, %.6f]\r\n",
						gUVD_STATE.SignalA.fAbsorbance, gUVD_STATE.SignalA.fReferenceE, gUVD_STATE.SignalA.fSampleE, 
						gUVD_STATE.SignalB.fAbsorbance, gUVD_STATE.SignalB.fReferenceE, gUVD_STATE.SignalB.fSampleE );
				}
#endif				
				
				LED_State = ~LED_State;	// BD LED
				LED_OnOff(LED_State);
				g_SendState = NO;
			}
#endif				

		}
		
		if(st_UV_func.bADC_Int)	// 500Hz
		{
			st_UV_func.bADC_Int = 0;

#if !READ_ADC_INTR			//(+) 210611 CJKIM, 인터럽트 루틴 에서는 ADC 값만 읽고 처리는 main 루프에서
			ADC_Read();	// 500Hz
			ADC_Volt_Read();
#endif

			stateActFunc();				// UVD Ready & Run STATE 
		}
#endif
		
#if 0	// 테스트 - 터미널 입력
		if (((g_mss_uart0.hw_reg->LSR)&0x01) == 1 )
		{
			cmd = g_mss_uart0.hw_reg->RBR;
			Function_test(cmd);
			cmd = 'X';
		}
#endif
    }

	
#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
	deinit_queue();
#endif	
	
	
}


