/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Network connection configuration
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
//#include "lwip/udp.h"
#include "netif/etharp.h"
#include "dhcp.h"
//#include "ethernetif.h"
#include "config.h"



//#define USE_DHCP
#define SYSTEMTICK_PERIOD_MS  1

/* Private typedef -----------------------------------------------------------*/
#define MAX_DHCP_TRIES        1

/* Private define ------------------------------------------------------------*/
typedef enum 
{ 
  DHCP_START=0,
  DHCP_WAIT_ADDRESS,
  DHCP_ADDRESS_ASSIGNED,
  DHCP_TIMEOUT
} 
DHCP_State_TypeDef;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif netif;
uint32_t TCPTimer = 0;
uint32_t ARPTimer = 0;
uint32_t IPaddress = 0;
//extern volatile uint32_t LocalTime;		//(-/+) 210603 CJKIM, volatile keyword added

#if LWIP_DHCP == 1
uint32_t DHCPfineTimer = 0;
uint32_t DHCPcoarseTimer = 0;
DHCP_State_TypeDef DHCP_state = DHCP_START;
#endif

//(-/+) 210701 CJKIM, Modified
//	void Time_Update(void)
//	{
//	  LocalTime += SYSTEMTICK_PERIOD_MS;
//	}


/* Private functions ---------------------------------------------------------*/
void LwIP_DHCP_Process_Handle(void);
/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */

/**
  * @brief  Called when a frame is received
  * @param  None
  * @retval None
  */
//void LwIP_Pkt_Handle(void)
//{
  /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
//  ethernetif_input(&netif);
//}

/**
  * @brief  LwIP periodic tasks
  * @param  localtime the current LocalTime value
  * @retval None
  */
void LwIP_Periodic_Handle(__IO uint32_t localtime)
{
	uint32_t elapse = 0;
#if LWIP_TCP
#define	TCP_TMR_INTERVAL_ACME	50			// mSec


	//210618 CJKIM
	if(localtime > TCPTimer) {
		elapse = localtime - TCPTimer;
	}
	else {
		//elapse = (MAX_UINT32_VALUE  - TCPTimer) -  localtime;
		elapse = (UINT_MAX - TCPTimer) -  localtime;
	}
	
	/* TCP periodic process every 250 ms */
	// RYU
	//  if (localtime - TCPTimer >= TCP_TMR_INTERVAL)
	if (elapse >= TCP_TMR_INTERVAL_ACME)	//(-/+) 210603 CJKIM, 10 -> 50 mS
	{
		TCPTimer =  localtime;
		tcp_tmr();
		//printf("tcp_tmr\r\n");
	}
#endif
  
	/* ARP periodic process every 5s */
	//  if ((localtime - ARPTimer) >= ARP_TMR_INTERVAL)
	//  if ((localtime - ARPTimer) >= 2000)
	if ((localtime - ARPTimer) >= 5000)
	{
		ARPTimer =  localtime;
		etharp_tmr();
	}

#if LWIP_DHCP == 1
	/* Fine DHCP periodic process every 500ms */
	if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
	{
		DHCPfineTimer =  localtime;
		dhcp_fine_tmr();
		if ((DHCP_state != DHCP_ADDRESS_ASSIGNED)&&(DHCP_state != DHCP_TIMEOUT))
		{ 
			/* toggle LED1 to indicate DHCP on-going process */

			/* process DHCP state machine */
			LwIP_DHCP_Process_Handle();    
		}
	}

	/* DHCP Coarse periodic process every 60s */
	if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
	{
		DHCPcoarseTimer =  localtime;
		dhcp_coarse_tmr();
	}
  
#endif
}

#if LWIP_DHCP == 1
/**
  * @brief  LwIP_DHCP_Process_Handle
  * @param  None
  * @retval None
  */
void LwIP_DHCP_Process_Handle()
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    uint8_t iptab[4];
//    uint8_t iptxt[20];

    switch (DHCP_state)
    {
        case DHCP_START:
        {
            dhcp_start(&netif);
            IPaddress = 0;
            DHCP_state = DHCP_WAIT_ADDRESS;
            iprintf("Looking for DHCP server\r\n");
            iprintf("please wait...\r\n");
        }
        break;

    case DHCP_WAIT_ADDRESS:
    {
        /* Read the new IP address */
        IPaddress = netif.ip_addr.addr;

        if (IPaddress!=0) 
        {
            DHCP_state = DHCP_ADDRESS_ASSIGNED;	

            /* Stop DHCP */
            dhcp_stop(&netif);

            iptab[0] = (uint8_t)(IPaddress >> 24);
            iptab[1] = (uint8_t)(IPaddress >> 16);
            iptab[2] = (uint8_t)(IPaddress >> 8);
            iptab[3] = (uint8_t)(IPaddress);

            printf("IP address assigned by a DHCP server\r\n");
            printf("%d.%d.%d.%d\r\n", iptab[3], iptab[2], iptab[1], iptab[0]);

            tcp_echoserver_init();

        }
        else
        {
            /* DHCP timeout */
            if (netif.dhcp->tries > MAX_DHCP_TRIES)
            {
                DHCP_state = DHCP_TIMEOUT;

                /* Stop DHCP */
                dhcp_stop(&netif);

                /* Static address used */
                IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
                IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
                IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
                netif_set_addr(&netif, &ipaddr , &netmask, &gw);

                iptab[0] = IP_ADDR3;
                iptab[1] = IP_ADDR2;
                iptab[2] = IP_ADDR1;
                iptab[3] = IP_ADDR0;

                printf("DHCP timeout\r\n");
                printf("Static IP address\r\n");
                printf("%d.%d.%d.%d\r\n", iptab[3], iptab[2], iptab[1], iptab[0]);

                tcp_echoserver_init();
            }
        }
    }
    break;
    default: break;
  }
}
#endif      

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
