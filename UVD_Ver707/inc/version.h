#ifndef		_CHROGEN_LC_UVD_VERSION_H_
#define		_CHROGEN_LC_UVD_VERSION_H_

/*
================================================================================================
					< Chrozen LC UVD Firmware History >
================================================================================================
Version	Date		Author		Description 
------------------------------------------------------------------------------------------------
v7.03	21.06.29	CJKIM		1. lwIP Send/Recv 시 큐 증가 오류로 Ethernet Interrupt 동기화 처리.
									- Disable Ethernet IRQ
									- Send data 
									- Enable Ethernet IRQ
								2. 메모리 부족 시 TCP Socket close 처리 안함.
								3. TCP Socket Close 시 메모리 초기화 되지 않는 오류 수정.
								4. FabricIrq5_IRQHandler 에서는 ADC 값만 읽고 처리는 main 루프에서 
									ADC_Read(); 
									ADC_Volt_Read();
								5. Client 연결 되었을 경우에만 TCP Data 전송 하도록 변경.
								6. tcp_server_send() 내 tcp_output() 함수는 tcp_write() 함수의 리턴값이 No_Err 일 때만 전송 하도록 변경. 
								7. TCP Socket Error 발생 시 TCP Close 추가.
								8. LwIP 메모리 사이즈 변경. ( lwIP 2.2.0 메모리 설정 반영  및 변경 ) - port_config\lwipopts.h
									- TCP segment 수  6 -> 16 변경
									  #define MEMP_NUM_TCP_SEG                16
									- MSS 536 -> 1460 변경
									  #define TCP_MSS                         1460
									- #define TCP_WND                         (4 * TCP_MSS)
									- #define TCP_SND_BUF                     (2 * TCP_MSS)
									- 큐 사이즈 2 -> 6 변경
									  #define TCP_SND_QUEUELEN                ((6 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
									- #define TCP_SNDLOWAT                    LWIP_MIN(LWIP_MAX(((TCP_SND_BUF)/2), (2 * TCP_MSS) + 1), (TCP_SND_BUF) - 1)
									- #define TCP_WND_UPDATE_THRESHOLD        LWIP_MIN((TCP_WND / 4), (TCP_MSS * 4))
									- #define PBUF_POOL_BUFSIZE               LWIP_MEM_ALIGN_SIZE(TCP_MSS+PBUF_IP_HLEN+PBUF_TRANSPORT_HLEN+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN)
								9. lwIP 메모리 부족 Test용으로 추가된 소스 필요 여부 확인 후 삭제 가능.
									- low_level_output() Time Interval 3mSec 추가 
									- low_level_output() 내 MSS_MAC_send_pkt() 후 g_mac_tx_buffer_used = RELEASE_BUFFER; 바로 처리되도록 추가.
									  원래는 tx_complete_handler() 호출 하여 해제 하게 되어 있음.
									- FabricIrq5_IRQHandler에  __ASM volatile ("dsb"); 추가
									- EthernetMAC_IRQHandler()   __ASM volatile ("dsb"); 추가
									- LwIP_Periodic_Handle() 내의 tcp_tmr() 호출 시간 변경. ( 250mS-> 50mS   )
									- tcp_enqueue() 내 pcb->snd_queuelen 값을 직접 처리.
									  이전에는 snd_queuelen값을 로컬변수로 복사 하여 연산 처리 후 로컬 변수를 snd_queuelen 값에 저장 
									- tcp_receive() 에  pcb->snd_queuelen > 0  체크 추가.
									  while (pcb->unacked != NULL && pcb->snd_queuelen > 0 && ...
								10. UVD FW용 debug 함수 추가.
									debug.c debug.h
								11. 컴파일 시 warning 발생 소스 일부 정리.
								12. 소스 정리.
								13. Packet struct 에 pack 옵션추가.

v7.04	21.07.02	KSLEE		1. FPGA Version 읽기 함수 추가. (2021/06/30)
								   Firmware와 SoC 버전을 모두 관리할 수 있도록하기 위함.
								2. setupFunc() UVD_SETUP_RESET_ERROR 상태일 경우 Selfmsg_flag OFF ->  ON 으로 변경; 	// 3sec 후 Error Check 다
								3. UV_Main.c stateCheck( ) 로직 변경.
								4. D2 ON, Wlamp/Heating On/Off, Motor 2종 동작 등 3초 간격으로 자동동작하도록 함. (2021/05/27)

v7.04	21.07.05	CJKIM		1. SoftConsole Project 이름 변경
									- GC_sb_MSS_CM3_app -> Chrogen_LC_SF2_UVD_app
									- GC_sb_MSS_CM3_hw_platform -> Chrogen_LC_SF2_UVD_hw_platform
								2. SoftConsole Project 변경 후 빌드 오류 및 환경 재설정 .
									- port_config\cpu_types.h "typedef uint32_t addr_t" 선언 오류 수정.
								3. Mac Address - IP Address설정과 동일 하게 적용.	
									Switch 2개로 Mac Addres[5] 설정.
								4. struct에 __attribute__((packed)) 사용 안함.
									- state packet 전송 시 byte Align 맞지 않아 데이터 오류 발생하여 광량 과 Lamp 상태 표시 안됨.
								5. SoftConsole Debug Configuration 오류 수정.
									- "Search Project"의 Program Select 시 Debug 파일 표시 오류 수정.

									* FW Release.

v7.05	21.07.16	CJKIM		1. 파일 삭제.
									Debug.c , Debug.h
								2. DEBUG_I() , DEBUG_E() 매크로 debug.h -> util.h 변경.
								3. uvd state() 함수 호출 시간 변경. ( 1 Sec -> 968mSec )
								4. TCP_SND_QUEUELEN 값 변경.
									- 변경 전
									    ((6 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
									- 변경 후
										((4 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
								5. UVD ACQ Data 전송 성능에 문제가 되지 않도록 lwIP 디버그 메세지 출력 조정.
								6. tcp_slowtmr() 함수내 tcp 데이터 전송 부분에 Ethernet Disable/Enable 추가.
								7. low_level_output() Tx Interval Time 변경 ( 3mSec -> 10mSec )
								8. low_level_output() g_mac_tx_buffer_used 처리 방식 변경
									Ethernet Interrupt 에서 처리 되도록 수정. (원래 상태로 되돌림)
								9. Ethernet Data Receive 방식 변경.
									- Rx Interrupt 발생 시 큐에 수신 데이터 저장.
									- main 루프에서 큐를 확인 하여 수신 데이터 처리.
									#define USED_RECV_QUEUE 처리.
								10. Ethernet Data 전송 시 메모리 부족할 경우 TCP Socket Close 처리. (원상태로 되돌림)
								11. LwIP_Periodic_Handle() 호출 되지 않는 오류 수정.
								12. send_state() TCP 연결 시에만 호출 하도록 변경.
								13. TCP Socket Close 시 UVD State RUN_START -> RUN_STOP 변경 추가.
								14. Debug Mode 빌드 시 Stack 사이즈 4K -> 6K 변경.
									debug-in-microsemi-smartfusion2-envm.ld

									* FW Release 하지 않음.

v7.06	21.07.21	CJKIM		1. tcp_server_connection_close() 함수 처리 시 tcp 메모리 해제 처리 변경.
									tcp_pcb_purge(tpcb); 사용 
								2. TCP Zero window 수신 시 데이터 전송 안하도록 수정 .
									tcp_slowtmr() , chrozen_send()
									volatile char g_TcpWindowState;
									#define STATE_TCP_NORMAL_WND 	0x00
									#define STATE_TCP_ZERO_WND 		0x01
								3. Debug Mode 빌드 시 Stack 사이즈 6K -> 4K 변경. (원상태로 되돌림)
									debug-in-microsemi-smartfusion2-envm.ld
								4. TCP_SND_QUEUELEN 값 변경. (v7.04 상태로 되돌림)
									- 변경 전
										((4 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
									- 변경 후
										((6 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
								5. low_level_output() 함수 내 Bug Fix.
							       MSS_MAC_send_pkt() 전송 실패 시 Timeout 시간 내 빠져 나오지 못하는 오류 수정.
							       m2sxxx_ethernetif.c
							    6. tcp_server_connection_abort() 함수 추가.
							    	tcp_server_connection_close() 대신 사용하려 추가 하였으나 문제 발생 되어 사용하지 않음.

									* FW Release 하지 않음.


v7.07	21.08.05	CJKIM		1. 부팅 시 UVD Lamp 켜지지 않으면서 진행 하지 않는 현상 Bug Fix
									Motor_TimerCtrl() 내 detMotorState.homeIndex = 1; 
								2. zero point 찾지 못해 반복적으로 zero point 체크 되는 Bug fix.
								3. Dual 파장 동작 시 광량 비정상 값으로 표시   및 Motor 멈추는 오류 수정.				
									- ADC 값을 ADC ISR 에서 Read (#define		READ_ADC_INTR		1)
									- Motor time cotrol timer2 ISR 에서 처리 (#define		MOTOR_TIME_INTR		1)
								4. Motor 가감속 처리 오류 수정.
								5. 부팅 시 네트워크 송수신 처리 반복되면 calibration 후    광량  비정상값으로 표시되는 오류 수정 처
									- Zero 점 Max Check 값 변경
									 	MAX_PHOTOCNT : (0xD80000 * DET_PHOTO_DATA_AVE) -> (0xF7FFFF * DET_PHOTO_DATA_AVE) 
									- initialize() main -> ISR 로 처리 변경.
									- ADC 값을 ADC ISR 에서 Read (#define		READ_ADC_INTR		1)
									- Motor time cotrol timer2 ISR 에서 처리 변경 (#define		MOTOR_TIME_INTR 	1)
								6. lwIP Debug 메세지 출력 안함. 
								
									* FW Release.
									

*/


#define UVD_FIRMWARE_VERSION	707


#endif	// _CHROGEN_LC_UVD_VERSION_H_
