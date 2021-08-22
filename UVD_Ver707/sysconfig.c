/*
 * sysconfig.c
 *
 *  Created on: 2015. 4. 14.
 *      Author: chanii
 */
#include "sys_config_mss_clocks.h"

#include "core_pwm.h"
#include "config.h"

#define WDOG_SYSREG_CR_ENABLE_MASK      0x00000001u

#define UV_FW_VER 99.01	// 20200625	// Test Ver 99.xx	


#define PWM_PRESCALE    32		// 35Mhz / 254 / 32 --> 4.3K
#define PWM_PERIOD      254 // Full값을 255로 정하기 위해서  // 256	


extern void uart_tx_handler(mss_uart_instance_t * this_uart);
extern void mss_uart1_rx_handler(mss_uart_instance_t * this_uart);
extern void spi_sd_init_Main();
extern void spi_sd_init();

extern pwm_instance_t the_pwm1;


void cpu_init(void)
{
	uint32_t timer1_load_value;
	uint32_t timer2_load_value;

	//uint32_t baud_value;

  /* Turn off the watchdog */
	SYSREG->WDOG_CR = 0;
	
//=============================================================================================
// Configure UART0 for Debug.
//---------------------------------------------
// Newlib_stubs.c     
//#define MICROSEMI_STDIO_BAUD_RATE  MSS_UART_460800_BAUD     
//---------------------------------------------
// Register the function that will be called on UART0 receive interrupts.
// 인터럽트를 사용하고자 할 때는 다음 심볼을 지워야한다.
// Property-->Setting-->Symbols--> MICROSEMI_STDIO_THRU_MMUART0 //ACTEL_STDIO_THRU_UART  
// MICROSEMI_STDIO_THRU_MMUART0
//---------------------------------------------
	//(+)210602 CJKIM, Baudrate 460K -> 921K
	MSS_UART_init(&g_mss_uart0, MSS_UART_460800_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

//	MSS_UART_set_rx_handler(&g_mss_uart0, mss_uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE /* MSS_UART_FIFO_FOUR_BYTES */);
//=========================================================================

#if 0
//=============================================================================================
// Configure UART1 for LCD
//---------------------------------------------
	MSS_UART_init(&g_mss_uart1,
								MSS_UART_460800_BAUD, // MSS_UART_230400_BAUD , // //MSS_UART_115200_BAUD,
                MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler(&g_mss_uart1, mss_uart1_rx_handler,MSS_UART_FIFO_SINGLE_BYTE /* MSS_UART_FIFO_FOUR_BYTES */);

//	MSS_UART_enable_irq(&g_mss_uart1,(MSS_UART_RBF_IRQ));
//	MSS_UART_set_tx_handler(&g_mss_uart1, uart_tx_handler);
//	MSS_UART_set_tx_handler(gp_my_uart, uart_tx_handler);	
//=========================================================================
#endif

//=============================================================================================
// Configure Timer1.
//---------------------------------------------
//	timer1_load_value = MSS_SYS_APB_0_CLK_FREQ / 1000;	// 1ms Timer Value
	timer1_load_value = MSS_SYS_APB_0_CLK_FREQ / 8000;	// 0.125ms Timer Value
	MSS_TIM1_init( MSS_TIMER_PERIODIC_MODE );
	MSS_TIM1_load_immediate( timer1_load_value );
	MSS_TIM1_start();
	MSS_TIM1_enable_irq();
//=========================================================================
	
#if MOTOR_TIME_INTR		//(+)210727 CJKIM,  ISR 에서 처리			
		//=============================================================================================
		// Configure Timer2.
		//---------------------------------------------
		timer2_load_value = MSS_SYS_APB_0_CLK_FREQ / 8000;	// 0.125ms Timer Value
		MSS_TIM2_init( MSS_TIMER_PERIODIC_MODE );
		MSS_TIM2_load_immediate( timer2_load_value );
		MSS_TIM2_start();
		MSS_TIM2_enable_irq();
		//=========================================================================
#endif

	SYSREG->WDOG_CR = WDOG_SYSREG_CR_ENABLE_MASK;
	/* Watchdog initialization. */
	MSS_WD_init();
	MSS_WD_enable_timeout_irq();

	NVIC_SetPriority(EthernetMAC_IRQn, 5u);

	spi_sd_init();

	DEBUG_I(DBG_LEVEL_INFO, "() Program Start !!\r\n");
	DEBUG_I(DBG_LEVEL_INFO, "() Build Time : %s %s\r\n", __DATE__, __TIME__);
	DEBUG_I(DBG_LEVEL_INFO, "() UVD Main Prog Ver %.2f\r\n", ((float)UVD_FIRMWARE_VERSION / 100.0f));

	SYSREG->WDOG_CR = WDOG_SYSREG_CR_ENABLE_MASK;
	/* Watchdog initialization. */
	MSS_WD_init();
	MSS_WD_enable_timeout_irq();

}


void InterruptEnable(void)
{
	// M1_Home Interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq0_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq0_IRQn );

	// M2_Home Interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq1_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq1_IRQn );

	NVIC_EnableIRQ( FabricIrq2_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq2_IRQn );

	// RS422 Auto interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq3_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq3_IRQn );

	// RS422 Rxv Auto interrupt ext enable ----------
	NVIC_EnableIRQ( FabricIrq4_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq4_IRQn );

	// ADS1252 Interrupt enable ---------
	NVIC_EnableIRQ( FabricIrq5_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq5_IRQn );
}

void cpu_init_Main(void)
{
	NVIC_SetPriority(EthernetMAC_IRQn, 5u);
//=============================================================================================
// Configure I2C for RTC(DS1308)
//---------------------------------------------
	//MSS_I2C_init( I2C_MASTER, MASTER_SER_ADDR, MSS_I2C_PCLK_DIV_160 );
//=========================================================================

//=============================================================================================
// Configure SPI for FRAM,  etc
//---------------------------------------------
	spi_sd_init_Main();
//=========================================================================
}


void InterruptEnable_Main(void)
{
	// Detector 1,2,3 Interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq0_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq0_IRQn );

	NVIC_EnableIRQ( FabricIrq1_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq1_IRQn );

	NVIC_EnableIRQ( FabricIrq2_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq2_IRQn );

	// APC Auto Completion interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq3_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq3_IRQn );

	/*
	// ADC Auto Sampling interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq4_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq4_IRQn );
	*/

	// START_IN : remote
	NVIC_EnableIRQ( FabricIrq5_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq5_IRQn );

	// Key In
	NVIC_EnableIRQ( FabricIrq6_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq6_IRQn );

	// RTC
	//    NVIC_EnableIRQ( FabricIrq7_IRQn );
	//    NVIC_ClearPendingIRQ( FabricIrq7_IRQn );

	// RS422 Rx intr
	NVIC_EnableIRQ( FabricIrq8_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq8_IRQn );
		
}

void InterruptEnable_Heater(void)
{
	// ADC Auto Sampling interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq4_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq4_IRQn );

	// RS422 Rx intr
#if 0
	NVIC_EnableIRQ( FabricIrq8_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq8_IRQn );
#endif		
}


