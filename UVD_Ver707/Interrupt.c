/*
 * Interrupt.c
 *
 *  Created on: 2015. 1. 7.
 *      Author: chanii
 */

#include "config.h"

extern void Time_Update(void);

extern volatile UV_FUNC_t st_UV_func;

volatile uint8_t TimeEvent; //(-/+) 210603 CJKIM, volatile keyword added

//extern GCDBG_ST gcdbg;
extern volatile APC_ST apcif;
//extern ADC_ST adcif;

#if !READ_ADC_INTR	//(-/+) 210611 CJKIM, ISR 에서는 ADC 값만 읽고 처리는 main 루프에서
extern volatile uint32_t g_Adc1Data;
extern volatile uint32_t g_Adc2Data;
extern volatile uint32_t g_Adc3Data;
#endif

static volatile uint32_t cnt = 0; //(-/+) 210604 CJKIM, volatile keyword added
volatile uint32_t g_PhotoCount = 0; //(-/+) 210604 CJKIM, volatile keyword added

//static char rwd = 0;
//static char PwrCheck_Cnt = 0;
//static char Pwrcheck_Start = 0;
//static char State_Cnt = 0;

extern void ADC_Read(void);	// 500Hz
extern void ADC_Volt_Read();
extern void photoDataRead();

//.word  Reset_Handler
//.word  NMI_Handler

void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
	/* These are volatile to try and prevent the compiler/linker optimising them
	 away as the variables never actually get used.  If the debugger won't show the
	 values of the variables, make them global my moving their declaration outside
	 of this function. */

	volatile unsigned int r0;
	volatile unsigned int r1;
	volatile unsigned int r2;
	volatile unsigned int r3;
	volatile unsigned int r12;
	volatile unsigned int lr; /* Link register. */
	volatile unsigned int pc; /* Program counter. */
	volatile unsigned int psr;/* Program status register. */

	r0 = pulFaultStackAddress[0];
	r1 = pulFaultStackAddress[1];
	r2 = pulFaultStackAddress[2];
	r3 = pulFaultStackAddress[3];

	r12 = pulFaultStackAddress[4];
	lr = pulFaultStackAddress[5];
	pc = pulFaultStackAddress[6];
	psr = pulFaultStackAddress[7];

	DEBUG_E("() Get Registers From Stack\r\n");
	DEBUG_E("() r0 [x%x]\r\n", r0);
	DEBUG_E("() r1 [x%x]\r\n", r1);
	DEBUG_E("() r2 [x%x]\r\n", r2);
	DEBUG_E("() r3 [x%x]\r\n", r3);

	DEBUG_E("() r12 [x%x]\r\n", r12);
	DEBUG_E("() lr [x%x]\r\n", lr);
	DEBUG_E("() pc [x%x]\r\n", pc);
	DEBUG_E("() psr [x%x]\r\n", psr);

}

__attribute__((__interrupt__)) void NMI_Handler(void)
{
	DEBUG_E("() NMI_Handler\r\n");
	//Soft_Reset_CPU();
}

__attribute__((__interrupt__)) void HardFault_Handler(void)
{
	uint32_t i;
#if 0
	 __asm volatile
	 (
	 " tst lr, #4                                                \n"
	 " ite eq                                                    \n"
	 " mrseq r0, msp                                             \n"
	 " mrsne r0, psp                                             \n"
	 " ldr r1, [r0, #24]                                         \n"
	 " ldr r2, handler2_address_const                            \n"
	 " bx r2                                                     \n"
	 " handler2_address_const: .word prvGetRegistersFromStack    \n"
	 );
#else
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" b prvGetRegistersFromStack                               \n"
	);
#endif
	DEBUG_E("() HardFault_Handler\r\n");
	while (1)
	{
		//Soft_Reset_CPU();
		i = 0xffffffff;
		while (i--)
		{
		};
	};
}
__attribute__((__interrupt__)) void MemManage_Handler(void)
{
	uint32_t i;
	DEBUG_E("() MemManage_Handler\r\n");
	while (1)
	{
		//Soft_Reset_CPU();
		i = 0xffffffff;
		while (i--)
		{
		};
	};
}
__attribute__((__interrupt__)) void BusFault_Handler(void)
{
	uint32_t i;
	DEBUG_E("() BusFault_Handler\r\n");
	while (1)
	{
		//Soft_Reset_CPU();
		i = 0xffffffff;
		while (i--)
		{
		};
	};

}
__attribute__((__interrupt__)) void UsageFault_Handler(void)
{
	uint32_t i;
	DEBUG_E("() UsageFault_Handler\r\n");
	while (1)
	{
		//Soft_Reset_CPU();
		i = 0xffffffff;
		while (i--)
		{
		};
	};
}

__attribute__((__interrupt__)) void WdogWakeup_IRQHandler(void)
{
	DEBUG_E("() WdogWakeup_IRQHandler\r\n");
	//Soft_Reset_CPU();
}

//(+)210609 CJKIM, 오류 발생 여부 확인  Interrup handler 추가
__attribute__((__interrupt__)) void CommSwitchError_IRQHandler(void)
{
	uint32_t i;
	DEBUG_E("() CommSwitchError_IRQHandler\r\n");
	while (1)
	{
		//Soft_Reset_CPU();
		i = 0xffffffff;
		while (i--)
		{
		};
	};
}

//(+)210609 CJKIM, 오류 발생 여부 확인  Interrup handler 추가
__attribute__((__interrupt__)) void CacheError_IRQHandler(void)
{
	uint32_t i;
	DEBUG_E("() CacheError_IRQHandler\r\n");
	while (1)
	{
		//Soft_Reset_CPU();
		i = 0xffffffff;
		while (i--)
		{
		};
	};
}

//(+)210609 CJKIM, 오류 발생 여부 확인  Interrup handler 추가
__attribute__((__interrupt__)) void ECC_Error_IRQHandler(void)
{
	uint32_t i;
	DEBUG_E("() ECC_Error_IRQHandler\r\n");
	while (1)
	{
		//Soft_Reset_CPU();
		i = 0xffffffff;
		while (i--)
		{
		};
	};
}

/*==============================================================================
 * TIM1 interrupt.
 * Do not use printf clause (Antony)
 */
__attribute__((__interrupt__)) void Timer1_IRQHandler(void)
{
	TimeEvent = 1; // 0.125ms time event flag :

	//LocalTime++;
	inc_local_time(); //(+)210701 CJKIM, Modified
	
	//	   Time_Update();

	MSS_TIM1_clear_irq(); /* Clear TIM1 interrupt */
}
extern uint8_t g_MotorTimeCount;		//(+)210804 CJKIM, Motor control Time when booting
extern volatile unsigned char g_Motor_125uS;	//(+)210804 CJKIM, Ready Init 이후 적용. Motor 제어 Interval - 부팅 : 1mS , Ready Init : 125uS

/*==============================================================================
 * TIM1 interrupt.
 */
__attribute__((__interrupt__)) void Timer2_IRQHandler(void)
{
#if MOTOR_TIME_INTR		//(+)210727 CJKIM,  ISR 에서 처리
#if 1
	Motor_TimerCtrl();
#else
	// Start && Dual Mode
	if (g_Motor_125uS) {
		Motor_TimerCtrl();		//(+)210803 CJKIM, 부팅완료 후 사용할 것.
	} 
	else if(++g_MotorTimeCount >= 2) //(+)210804 CJKIM, Motor control 250uS Interval when booting
	{
		Motor_TimerCtrl();		//(+)210803 CJKIM, , 부팅 시 calibration 에서만 사용
		g_MotorTimeCount =0;
	}
#endif
#endif


	MSS_TIM2_clear_irq(); /* Clear TIM1 interrupt */
}

__attribute__((__interrupt__)) void FabricIrq0_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(FabricIrq0_IRQn);

}

/*	ADC Data Interrupt	
 //	Data를 받아 64 평균을 취한다.
 */
__attribute__((__interrupt__)) void FabricIrq1_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(FabricIrq1_IRQn);
}

__attribute__((__interrupt__)) void FabricIrq2_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(FabricIrq2_IRQn);
}

// APC Auto Transmission completion interrupt
__attribute__((__interrupt__)) void FabricIrq3_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(FabricIrq3_IRQn);
	apcif.AutoEnd = 1;
}

// AD3 Interrupt
//(+)210629 CJKIM, 사용 안함. IRQ5에서 사용
__attribute__((__interrupt__)) void FabricIrq4_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(FabricIrq4_IRQn);
	//	adcif.intr=1;
	st_UV_func.bADC_Volt_Int = 1;

	/*
	 To ensure all the previous instructions are completed, data barrier
	 instruction is used. The �dsb� data barriers instruction completes,
	 only after all the previous instruction are completed.
	 */
	__ASM volatile ("dsb");		//(+) 210604 CJKIM

}
extern volatile uint32_t LocalTime;
volatile unsigned char g_DataCopy = NO;

extern void signal_data();

// ADC 1, 2 ,3 Interrupt
__attribute__((__interrupt__)) void FabricIrq5_IRQHandler(void) // ADC 500Hz
{
	NVIC_ClearPendingIRQ(FabricIrq5_IRQn);
	st_UV_func.bADC_Int = 1; // 500Hz

#if READ_ADC_INTR	//(-/+) 210611 CJKIM, ISR 에서는 ADC 값만 읽고 처리는 main 루프에서
	ADC_Read(); // 500Hz
	ADC_Volt_Read();
#else
	g_Adc1Data = AD1_READ;
	g_Adc2Data = AD2_READ;
	g_Adc3Data = AD3_READ;
#endif

	//(-/+) 210604 CJKIM, 비교문 수정
	if (++cnt >= 4) // 125Hz	// ChroZen 125Hz용
	{
		st_UV_func.b125Hz_Int = 1;
		cnt = 0;
#if PHOTO_ACT_INTR		
		photoDataRead();	// ChroZen 125Hz
		g_PhotoCount++;
		if(g_PhotoCount >= SIGNAL_SEND_COUNT && g_DataCopy == NO) {
			g_DataCopy = YES;
			signal_data();
		}
#endif			
	}
	
	if (UVD_STATE_INITIALIZING == gUVD_STATE.btState) {
			Initialize();
	}
	/*
	 To ensure all the previous instructions are completed, data barrier
	 instruction is used. The �dsb� data barriers instruction completes,
	 only after all the previous instruction are completed.
	 */
	__ASM volatile ("dsb");		//(+) 210604 CJKIM
	//printf("-%d\r\n", LocalTime >> 3);

	
	
}

__attribute__((__interrupt__)) void mss_uart1_rx_handler(
		mss_uart_instance_t * this_uart)
{
	NVIC_ClearPendingIRQ(UART1_IRQn);
	//rd_size = MSS_UART_get_rx( this_uart,  R_Buff, RBUF_SIZE);

}

extern volatile size_t g_tx_size;
extern volatile const uint8_t * g_tx_buffer;

__attribute__((__interrupt__)) void uart_tx_handler(
		mss_uart_instance_t * this_uart)
{
	size_t size_in_fifo;

	if (g_tx_size > 0)
	{
		size_in_fifo = MSS_UART_fill_tx_fifo(this_uart,
				(const uint8_t *) g_tx_buffer, g_tx_size);

		if (size_in_fifo == g_tx_size)
		{
			g_tx_size = 0;
			MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
		}
		else
		{
			g_tx_buffer = &g_tx_buffer[size_in_fifo];
			g_tx_size = g_tx_size - size_in_fifo;
		}
	}
	else
	{
		g_tx_size = 0;
		MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
	}
}

