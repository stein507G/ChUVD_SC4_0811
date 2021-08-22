/*
 * config.h
 *
 *  Created on: 2015. 4. 7.
 *      Author: chanii
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdarg.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>


#include "GC_hw_platform.h"
//#include "integer.h"

#include "lwip/tcp.h"	//210617 CJKIM added
#include "version.h"		//(+)210629 CJKIM, added
#include "UV_Initfunc.h"	//(+)210629 CJKIM, added


/***************************** CMSIS includes ************************/
#include "system_m2sxxx.h"
#include "sysconfig.h"
#include "interrupt.h"

/***************************** Driver includes **********************/
//#include "mss_gpio.h"
#include "mss_uart.h"
//#include "mss_rtc.h"
#include "mss_watchdog.h"
#include "mss_timer.h"
#include "mss_nvm.h"
#include "mss_spi.h"
#include "mss_ethernet_mac.h"
//#include "spi_M.h"
//#include "spi_H.h"

//====================================================================
// ryu
//#include "../ryu_inc/define_GC.h"

//#include "../ryu_inc/Def_name.h"
//#include "../ryu_inc/Def_range.h"
//#include "../ryu_inc/Def_HeaterCon.h"

//#include "../ryu_inc/type_def_GC.h"

//#include "../ryu_inc/ComType_6700GC.h"
//#include "../ryu_inc/ComType_LCD.h"
//#include "../ryu_inc/ComType_APC422.h"

//#include "../ryu_inc/GC_Type.h"


//#include "../ryu_inc/Protocol_YL6700GC.h"

//#include "../ryu_inc/global_extern.h"

//====================================================================
// Yi
#include "Protocol_ChroZen_UVD.h"
#include "Conf_eth.h"
/*************************** lwIP includes. *************************/
#include "tcp_server.h"
#include "netconf.h"
#include "util.h"
#include "type_def.h"

#include "fpga_if.h"
#include "apc_if.h"
//#include "adc_if.h"
//#include "detector_if.h"
//#include "MSPI_if.h"
#include "fram_if.h"

#include "debug.h"
#include "test.h"

#include "queue.h"	//(+)210715 CJKIM, added

#include "uvd_state_str.h"	//(+)210701 CJKIM, added

#include "UV_define.h"
#include "UV_Struct.h"
#include "UV_Global_Extern.h"
#include "UV_Protocol.h"
#include "UV_MotorCtrl.h"
#include "UV_Remote.h"
#include "UV_Main.h"

#define		DEVICE_TEST		1		//(+)210603 CJKIM, defined : 장비 테스트 , Undefined : 보드 TCP 통신 테스트.

#define 	USED_RECV_QUEUE		1	//(+)210715 CJKIM, Received data process changing

#define		READ_ADC_INTR		1	//(+)210727 CJKIM, ADC 값을 ISR 에서 Read
#define		PHOTO_ACT_INTR		0	//(+)210727 CJKIM, Photo Read ADC ISR 에서 처리
#define		MOTOR_TIME_INTR		1	//(+)210727 CJKIM, Motor time cotrol timer2 ISR 에서 처리

#endif /* CONFIG_H_ */
