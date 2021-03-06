#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/* Include user defined options first */
#include "conf_eth.h"

#define LWIP_PROVIDE_ERRNO 1

/* These two control is reclaimer functions should be compiled
   in. Should always be turned on (1). */
#define MEM_RECLAIM             1
#define MEMP_RECLAIM            1


/*
 * enable SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task protection
 * for certain critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT            1

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#define NO_SYS                  1

//#define ETH_PAD_SIZE 2

/* ---------- Memory options ---------- */
#define MEM_LIBC_MALLOC                 0

/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                10 * 1024

//#define MEMP_SANITY_CHECK       1
#define MEMP_SANITY_CHECK       0		//210618 CJKIM , No Check 

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF           6

/* Number of raw connection PCBs */
#define MEMP_NUM_RAW_PCB                1

/* ---------- UDP options ---------- */
#define LWIP_UDP                0
#define UDP_TTL                 255

/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        1


/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        14
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 4
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
   
#define MEMP_NUM_TCP_SEG                16		//(-/+) 210607 CJKIM, 6 -> 16 ??????.
//#define MEMP_NUM_TCP_SEG        6	

/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT    6

/* The following four are used only with the sequential API and can be
   set to 0 if the application only will use the raw API. */
/* MEMP_NUM_NETBUF: the number of struct netbufs. */
#define MEMP_NUM_NETBUF         3
/* MEMP_NUM_NETCONN: the number of struct netconns. */
#define MEMP_NUM_NETCONN        6


/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */

#define PBUF_POOL_SIZE          6
//#define PBUF_POOL_SIZE          12

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */

//#define PBUF_POOL_BUFSIZE       1024
//#define PBUF_POOL_BUFSIZE       860

/* PBUF_LINK_HLEN: the number of bytes that should be allocated for a
   link level header. */
#define PBUF_LINK_HLEN          16

/* ---------- TCP options ---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         1

#if 0 	//(-/+) 210610 CJKIM, ?????? ?????????
/* TCP receive window. */
//#define TCP_WND                 1500
#define TCP_WND                 1600

/* TCP Maximum segment size. */
#define TCP_MSS                 800	// jkh advise

/* TCP sender buffer space (bytes). */
//#define TCP_SND_BUF             2150
//#define TCP_SND_BUF             4096
#define TCP_SND_BUF             3200

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
#define TCP_SND_QUEUELEN        2 * TCP_SND_BUF/TCP_MSS
//#define TCP_SND_QUEUELEN        6 * TCP_SND_BUF/TCP_MSS
#else	//(-/+) 210610 CJKIM, lwIP 2.2.0 ??????, MSS ?????? 1460?????? ??????
 /**
 * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
 * you might want to increase this.)
 * For the receive side, this MSS is advertised to the remote side
 * when opening a connection. For the transmit size, this MSS sets
 * an upper limit on the MSS advertised by the remote host.
 */
//#define TCP_MSS                         1024		//210618 CJKIM,  1460 -> 1024 For Test
#define TCP_MSS                         1460		//(+)210630 CJKIM, 1024 -> 1460 For Test

/**
 * TCP_WND: The size of a TCP window.  This must be at least
 * (2 * TCP_MSS) for things to work well.
 * ATTENTION: when using TCP_RCV_SCALE, TCP_WND is the total size
 * with scaling applied. Maximum window value in the TCP header
 * will be TCP_WND >> TCP_RCV_SCALE
 */
#define TCP_WND                         (4 * TCP_MSS)
 
 /**
 * TCP_SND_BUF: TCP sender buffer space (bytes).
 * To achieve good performance, this should be at least 2 * TCP_MSS.
 */
#define TCP_SND_BUF                     (2 * TCP_MSS)
 
 /**
 * TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
 * as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work.
 */
// 210621 CJKIM , 4 -> 6
//#define TCP_SND_QUEUELEN                ((2 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))
#define TCP_SND_QUEUELEN                ((6 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))


 /**
 * TCP_SNDLOWAT: TCP writable space (bytes). This must be less than
 * TCP_SND_BUF. It is the amount of space which must be available in the
 * TCP snd_buf for select to return writable (combined with TCP_SNDQUEUELOWAT).
 */
#define TCP_SNDLOWAT                    LWIP_MIN(LWIP_MAX(((TCP_SND_BUF)/2), (2 * TCP_MSS) + 1), (TCP_SND_BUF) - 1)

 /**
 * TCP_WND_UPDATE_THRESHOLD: difference in window to trigger an
 * explicit window update
 */
#define TCP_WND_UPDATE_THRESHOLD        LWIP_MIN((TCP_WND / 4), (TCP_MSS * 4))

 /**
 * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
 * designed to accommodate single full size TCP frame in one pbuf, including
 * TCP_MSS, IP header, and link header.
 */
#define PBUF_POOL_BUFSIZE               LWIP_MEM_ALIGN_SIZE(TCP_MSS+PBUF_IP_HLEN+PBUF_TRANSPORT_HLEN+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN)
#endif


/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX              12

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX           4

/* ---------- ARP options ---------- */
#define ARP_TABLE_SIZE 10
#define ARP_QUEUEING 0

/* ---------- IP options ---------- */
/* Define IP_FORWARD to 1 if you wish to have the ability to forward
   IP packets across network interfaces. If you are going to run lwIP
   on a device with only one network interface, define this to 0. */
#define IP_FORWARD              0

/* If defined to 1, IP options are allowed (but not parsed). If
   defined to 0, all packets with IP options are dropped. */
#define IP_OPTIONS              1

/* ---------- ICMP options ---------- */
#define ICMP_TTL                255


/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               0

#define LWIP_ARP 1

/* 1 if you want to do an ARP check on the offered address
   (recommended). */
#define DHCP_DOES_ARP_CHECK     0

//#define TCPIP_THREAD_PRIO               lwipINTERFACE_TASK_PRIORITY


#define TCPIP_THREAD_STACKSIZE 4096
/* ---------- Statistics options ---------- */


//-------------------------------------------------------
//(+)210615 CJKIM, For lwIP Debugging

//#define	LWIP_DEBUG	1
#define LWIP_STATS 0			
#define LWIP_STATS_DISPLAY 0	

#ifdef LWIP_DEBUG	
#define LWIP_STATS 					1			
#define LWIP_STATS_DISPLAY          1	

#define LWIP_DBG_MIN_LEVEL          LWIP_DBG_LEVEL_ALL	
#define MEMP_DEBUG                  LWIP_DBG_ON		
#define TCP_DEBUG                   LWIP_DBG_ON		
#define TCP_INPUT_DEBUG             LWIP_DBG_ON	
//#define TCP_FR_DEBUG                LWIP_DBG_ON	
//#define TCP_RTO_DEBUG				LWIP_DBG_ON	
//#define TCP_CWND_DEBUG              LWIP_DBG_ON	
//#define TCP_WND_DEBUG				LWIP_DBG_ON	
#define TCP_OUTPUT_DEBUG            LWIP_DBG_ON	
#define TCP_QLEN_DEBUG              LWIP_DBG_ON		

#endif

#if LWIP_STATS
#define LINK_STATS 1
#define IP_STATS   1
#define ICMP_STATS 1
#define UDP_STATS  1
#define TCP_STATS  1
#define MEM_STATS  1
#define MEMP_STATS 1
#define PBUF_STATS 1
#define SYS_STATS  1
#endif /* STATS */


#endif /* __LWIPOPTS_H__ */
