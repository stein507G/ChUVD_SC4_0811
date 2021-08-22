/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable echo example.
 *
 **/

 /* This file was modified by ST */


#include "config.h"
#include "lwip/tcp.h"
//#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/memp.h"

#include "util.h"

//extern volatile uint32_t LocalTime; 	//CJKIM

#if LWIP_TCP

Main_DEV_STATE_t mainDevStt;

//u8_t data_buff[610]={0x30,};
static struct tcp_pcb *tcp_server_pcb;
//uint8_t FrameSendRequest;
struct tcp_server_struct SvrStruct;
struct tcp_server_struct *g_es;
u8_t esstate_save;
struct tcp_pcb *g_pcb;
//struct pbuf *g_p;
GCSVC_ST gcsvc;
//extern GCDBG_ST gcdbg;


uint8_t ConnTime=0;

struct tcp_pcb *g_pAcceptPcb;

char pPacketBuff[PACKET_SIZE_MAX];

char Selfmsg_flag = OFF;

volatile char Clarity_Connect = STATE_COMM_NOT_CONNECTED;	// CJKIM
volatile char g_TcpWindowState = STATE_TCP_NORMAL_WND;		//(+)210722 CJKIM, TCP Zero Window

const char* pkt_code_str(int code);

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_server_error(void *arg, err_t err);
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static void tcp_server_connection_abort(struct tcp_pcb *tpcb, struct tcp_server_struct *es);		//(+)210722 CJKIM, added



err_t tcp_GCDB_Send(struct tcp_pcb *tpcb, struct tcp_server_struct *es, struct pbuf *gcd_pbuf);

void PC_CommandCheck(struct pbuf *p);
int PacketParse(unsigned int nPacketCode, char *pData);

//int net_CheckModel(char *pData);


extern void MSS_MAC_enable_intr();				//(+) 210621 CJKIM, For atomic tx .
extern void MSS_MAC_disable_intr();				//(+) 210621 CJKIM, For atomic tx .

extern void comm_disconnected_uvd_init();				//(+)210716 CJKIM, re-init uvd state when network disconnecting

void make_MACADDR()
{
}
/**
  * @brief  Initializes the tcp server
  * @param  None
  * @retval None
  */
void tcp_server_init(void)
{
	/* create new tcp pcb */

	tcp_server_pcb = tcp_new();

	if (tcp_server_pcb != NULL)
	{
		err_t err;

		/* bind pcb to port IP_PORT_NUMBER */
		err = tcp_bind(tcp_server_pcb, IP_ADDR_ANY, IP_PORT_NUMBER);

		if (err == ERR_OK)
			{
			/* start tcp listening for echo_pcb */
			tcp_server_pcb = tcp_listen(tcp_server_pcb);

			/* initialize LwIP tcp_accept callback function */
			tcp_accept(tcp_server_pcb, tcp_server_accept);
		}
		else 
		{
			DEBUG_E("() Can not bind pcb\n");
		}
	}
	else
	{
		DEBUG_E("() Can not create new pcb\n");
	}
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used 
  * @retval err_t: error status
  */
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	err_t ret_err;
	struct tcp_server_struct *es;

	//DEBUG_I(DBG_LEVEL_INFO, "() Client is connected \r\n");

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	/* set priority for the newly accepted tcp connection newpcb */
	tcp_setprio(newpcb, TCP_PRIO_MIN);

	/* allocate structure es to maintain tcp connection informations */
	//es = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct));
	es = &SvrStruct;
	g_es = es;

	newpcb->flags |= TF_NODELAY;

	if (es != NULL)
	{
		es->state = ES_ACCEPTED;
		es->pcb = newpcb;
		//   es->p = NULL;
		es->transmitBuf_length = 0;

		/* pass newly allocated es structure as argument to newpcb */
		tcp_arg(newpcb, es);
		/* initialize lwip tcp_recv callback function for newpcb  */ 
		tcp_recv(newpcb, tcp_server_recv);
		/* initialize lwip tcp_err callback function for newpcb  */
		tcp_err(newpcb, tcp_server_error);
#if 0 // RYU_TCP     
		/* initialize lwip tcp_poll callback function for newpcb */
		tcp_poll(newpcb, tcp_server_poll, 1);
#endif
		// RYU
		g_pAcceptPcb = newpcb;

		//(-/+) 210611 CJKIM, For Debug
		DEBUG_I(DBG_LEVEL_INFO, "() TCP MSS[%d]\r\n", tcp_mss(newpcb));
	
		netConnection();	
		LEDStateChange = 0;
		Clarity_Connect = STATE_COMM_CONNECTED;

		ret_err = ERR_OK;
	}
	else
	{
		/* return memory error */
		ret_err = ERR_MEM;
	}
	return ret_err;  
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the received pbuf
  * @retval err_t: error code
  */

#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
void chrozen_recv()
{
#define	 MAX_BUFFER 1024

	int ret = 0;
	int packetLength = 0;

	char pBuffer[MAX_BUFFER];
	UVD_PACKET* pPacket;

	do {
		
		if (get_available() >= PACKET_SIZE_MIN) {
			memset(pBuffer, 0 , MAX_BUFFER);
			ret = peek(pBuffer, 0, PACKET_SIZE_MIN);
			pPacket = (UVD_PACKET*)(pBuffer); 

			packetLength = pPacket->nPacketLength;
			if (get_available() >= packetLength ) {		// Packet Data

				ret = dequeue(pBuffer, 0, packetLength);
				
//					DEBUG_I(DBG_LEVEL_INFO, "() RECV[%d]\r\n", ret);	//(+)210604 CJKIM, For Debug
//					ShowMemory(0, pBuffer, ret);
				
				if(PacketParse(pPacket->nPacketCode, pBuffer) == RE_ERR) 
				{
					DEBUG_E("() Failed to parse data[%d]\r\n", packetLength);
					ShowMemory(0, pBuffer, packetLength);
				};
			} else {
				break;
			}
			
		} else {
			break;
		}
	}while(get_available() >= PACKET_SIZE_MIN);

}


#else
static bool chrozen_recv(struct tcp_pcb *tpcb, struct pbuf *p)
{
	bool retValue = true;
	UVD_PACKET* pPacket;
	int iPacketLength = 0;			// 읽을 패킷 길이
	static char acData_in[PACKET_BUFF_SIZE];
	static int iSaveDatainPos = 0;
	struct pbuf *pp = p;
	
	if(tpcb == NULL)
	{
		iSaveDatainPos = 0;
		return false;
	}

	//DEBUG_I(DBG_LEVEL_INFO, "() RECV[%d] : %s\r\n", p->len, dump_data(p->payload, 0, p->len));	//(+)210604 CJKIM, For Debug

	//DEBUG_I(DBG_LEVEL_INFO, "() RECV[%d]\r\n", p->len);	//(+)210604 CJKIM, For Debug
	//ShowMemory(0, p->payload, p->len);

	
	while(p != NULL)
	{
		if(p->len <= PACKET_BUFF_SIZE - iSaveDatainPos)
		{
			memcpy(acData_in + iSaveDatainPos, p->payload, p->len);
			iSaveDatainPos += p->len;
			p = p->next;
		}	

		while(iSaveDatainPos >= PACKET_SIZE_MIN) 
		{
			pPacket = (UVD_PACKET*)(acData_in);	// + iPacketStartPos);
			iPacketLength = pPacket->nPacketLength;
			if(iPacketLength > 1024 ||
				(iPacketLength != PACKET_SIZE_MIN && iPacketLength != PACKET_SIZE_MIN + pPacket->nSlotSize))
			{
				iSaveDatainPos--;
				memcpy(acData_in, acData_in+1, iSaveDatainPos);
				continue;
			}
			if(iSaveDatainPos < iPacketLength) {	// 데이터를 더 받아야 한다.
				break;
			}

			if(PacketParse(pPacket->nPacketCode, acData_in) == RE_ERR) 
			{
				retValue = false;
			};

			iSaveDatainPos = iSaveDatainPos - iPacketLength;

			if(iSaveDatainPos != 0) 
			{ 
				memcpy(acData_in, acData_in + iPacketLength, iSaveDatainPos);		// 데이터를 맨 앞으로 이동
			}
			iPacketLength = 0;
		}
	}
	/* Update tcp window size to be advertized : should be called when received
	data (with the amount plen) has been processed by the application layer */
	tcp_recved(tpcb, pp->tot_len);
	
	pbuf_free(pp);

	DEBUG_I(DBG_LEVEL_INFO, "() [%d]\n", retValue);	//(+)210604 CJKIM, For Debug
	return retValue;
}
#endif

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	struct tcp_server_struct *es;
	err_t ret_err;
	
	LWIP_ASSERT("arg != NULL",arg != NULL);

	es = (struct tcp_server_struct *)arg;
	g_es = es;
	g_pcb = tpcb;

	/* if we receive an empty tcp frame from client => close connection */
	if (p == NULL)
	{
		DEBUG_E("() receive an empty tcp frame from client => close connection\r\n");
		/* remote host closed connection */
		es->state = ES_CLOSING;
		tcp_server_connection_close(tpcb, es);	//210624 CJKIM 

// 		if(es->p == NULL)
// 		{
// 			/* we're done sending, close connection */
// 			tcp_server_connection_close(tpcb, es);
// 		}
// 		else
// 		{
// #if 1 // RYU_TCP
// 			/* we're not done yet */
// 			/* acknowledge received packet */
// 			tcp_sent(tpcb, tcp_server_sent);

// 			/* send remaining data */
// 			tcp_server_send(tpcb, es);
// #endif			
//		}
		ret_err = ERR_OK;
	}
	/* else : a non empty frame was received from client but for some reason err != ERR_OK */
	else if(err != ERR_OK)
	{
		/* free received pbuf */
		if (p != NULL)
		{
			tcp_recved(tpcb, p->tot_len);
//			es->p = NULL;
			pbuf_free(p);
		}
		ret_err = err;
	}
	else if(es->state == ES_ACCEPTED)
	{
		/* first data chunk in p->payload */
		es->state = ES_RECEIVED;
		/* store reference to incoming pbuf (chain) */
//		es->p = p;
		/* initialize LwIP tcp_sent callback function */
#if 0
		tcp_sent(tpcb, tcp_server_sent);
#endif
		/* send back the received data (echo) */
//		tcp_server_send(tpcb, es);

		ret_err = ERR_OK;

		gcsvc.TCP_LineState=1;
		g_pcb 	= tpcb;
		g_es 	= es;
//		g_p 	= p;

#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
		enqueue(p->payload, 0, p->len);

		/* Update tcp window size to be advertized : should be called when received
		data (with the amount plen) has been processed by the application layer */
		tcp_recved(tpcb, p->tot_len);

		pbuf_free(p);
#else
		chrozen_recv(NULL, NULL);
	
		if(!chrozen_recv(tpcb, p))
		{
			DEBUG_E("() Failed to parse received data.\r\n");
			//tcp_server_connection_close(tpcb, es);	//210624 CJKIM 
		}
#endif

		
	}
	else if (es->state == ES_RECEIVED)
	{
#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
		enqueue(p->payload, 0, p->len);

		/* Update tcp window size to be advertized : should be called when received
		data (with the amount plen) has been processed by the application layer */
		tcp_recved(tpcb, p->tot_len);

		pbuf_free(p);
#else	
		chrozen_recv(tpcb, p);
#endif
		ret_err = ERR_OK;
	}

  /* data received when connection already closed */
	else
	{
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);

		/* free pbuf and do nothing */
		//es->p = NULL;
		pbuf_free(p);
		ret_err = ERR_OK;
	}
	return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs. 
  * @param  arg: pointer on argument parameter 
  * @param  err: not used
  * @retval None
  */
static void tcp_server_error(void *arg, err_t err)
{
	struct tcp_server_struct *es;

#ifdef LWIP_DEBUG
	DEBUG_E("() Errror Code[%d, %s]\r\n", err, lwip_strerr(err));
#else
	DEBUG_E("() Errror Code[%d]\r\n", err);
#endif

	LWIP_UNUSED_ARG(err);
	es = (struct tcp_server_struct *)arg;

	//210615 CJKIM , 비정상으로 연결 해제 시 close
	tcp_server_connection_close(g_pAcceptPcb, es);	//210624 CJKIM 

	if (es != NULL)
	{
		/*  free es structure */
		es->transmitBuf_length = 0;
		//mem_free(es);	//210624 CJKIM , enable
						//210701 CJKIM , tcp_server_accept() 함수에서 pointer로 사용 하지 않음. 메모리 해제 금지
	}
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_server_struct *es;

	es = (struct tcp_server_struct *)arg;
	if (es != NULL)
	{
		if (es->transmitBuf_length)
		{
			tcp_server_send(tpcb, es);
		}
		else
		{
			/* no remaining pbuf (chain)  */
			if(es->state == ES_CLOSING)
			{
				DEBUG_E("() no remaining pbuf (chain)  => close connection\r\n");
				/*  close tcp connection */
				tcp_server_connection_close(tpcb, es);
			}
		}
		ret_err = ERR_OK;
	}
	else
	{
		/* nothing to be done */
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}
	return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data) 
  * @param  None
  * @retval None
  */
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_server_struct *es;

	LWIP_UNUSED_ARG(len);

	es = (struct tcp_server_struct *)arg;

	if(es->transmitBuf_length > 0)
	{
		/* still got pbufs to send */
		//    tcp_server_send(tpcb, es);
	}
	else
	{
		/* if no more data to send and client closed connection*/
		if(es->state == ES_CLOSING) {
			DEBUG_E("() no more data to send and client closed connection  => close connection\n", __FUNCTION__);
			tcp_server_connection_close(tpcb, es);
		}
	}
	return ERR_OK;
}


void chrozen_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es, char * p, int size)
{
	
	UVD_PACKET *pPkt = p; 
	//DEBUG_I(DBG_LEVEL_INFO, "() PKT Code[x%X, %s]\r\n", pPkt->nPacketCode, pkt_code_str(pPkt->nPacketCode));
	
	//(+)210615 CJKIM, Clarity 와 연결 되었을 시에만 전송 
	//(+)210722 CJKIM, TCP Zero Window 아닌 경우 에만 전송
	if ( STATE_COMM_CONNECTED == Clarity_Connect && STATE_TCP_NORMAL_WND == g_TcpWindowState ) {

		if(es != NULL)
		{
			int remain = (PACKET_SIZE_MAX) - es->transmitBuf_length;
			if(size <= remain)
			{
				memcpy(es->transmitBuf + es->transmitBuf_length, p, size);
				es->transmitBuf_length += size;
		
				tcp_server_send(tpcb, es);
		
//					DEBUG_I(DBG_LEVEL_INFO, "() SEND[%d]\n", size);
//					ShowMemory(0, p, size );
			}
			else
			{
				DEBUG_E("() Transmit buffer has been fulled![%d, %d]\n", size, remain);
				tcp_server_connection_close(tpcb, es);	//210624 CJKIM 
				//tcp_server_connection_abort(tpcb, es);	//(+)210722 CJKIM, added
			}
		}

	}
	else {
		DEBUG_E("() SEND[%d] Failed to Send [%d, %d] \n", size, Clarity_Connect, g_TcpWindowState);
	}

}


/**
  * @brief  This function is used to send data for tcp connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
  
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{

   	err_t wr_err = ERR_OK;
	int len =0 ;
	int available = tcp_sndbuf(tpcb);	//(-/+) 210611 CJKIM, Debug 용으로 변수 변경.

	
	MSS_MAC_disable_intr();		//(+) 210621 CJKIM, disable mac interrupt before sending.

	if(es != NULL && available > 0 && es->transmitBuf_length > 0)
	{
		len = available < es->transmitBuf_length? available : es->transmitBuf_length;

		//(-/+) 210528 CJKIM, 1 -> TCP_WRITE_FLAG_COPY 변경, 값은 동일
     	wr_err = tcp_write(tpcb, es->transmitBuf, len, TCP_WRITE_FLAG_COPY);		
		if(wr_err == ERR_OK)
		{
			es->transmitBuf_length -= len;
			if(es->transmitBuf_length>0) {
				memcpy(es->transmitBuf, es->transmitBuf + len, es->transmitBuf_length);
				DEBUG_I(DBG_LEVEL_INFO, "() remian transmitBuf[%d]\n", es->transmitBuf_length);
			}
		}
	}

	// 210616 CJKIM , Send data when No Error
	if (ERR_OK == wr_err) {

		tcp_output(tpcb);	
		
		MSS_MAC_enable_intr();	//(+) 210621 CJKIM, enable mac interrupt after sending.
	}
	else {
		DEBUG_E("() Failed to write tcp data [%d, %d] [%d]\n", available, es->transmitBuf_length, wr_err);

		MSS_MAC_enable_intr();	//(+) 210621 CJKIM, enable mac interrupt after sending.
		
		// 210615 CJKIM , TCP Closing added
		if (ERR_MEM == wr_err) {
			tcp_server_connection_close(tpcb, es);	//210624 CJKIM 
			//tcp_server_connection_abort(tpcb, es);	//(+)210722 CJKIM, added
		}
	}

}

/**
  * @brief  This functions closes the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	DEBUG_I(DBG_LEVEL_INFO, "()\r\n");

	comm_disconnected_uvd_init(); 	//(+)210716 CJKIM, re-init uvd state when network disconnecting

	gcsvc.TCP_LineState = 0;

	Clarity_Connect = STATE_COMM_NOT_CONNECTED;
	g_TcpWindowState = STATE_TCP_NORMAL_WND;		//(+)210722 CJKIM, TCP Zero Window
	
	/* remove all callbacks */
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, NULL);
	tcp_recv(tpcb, NULL);
	tcp_err(tpcb, NULL);
	tcp_poll(tpcb, NULL, 0);

	/* delete es structure */
	if (es != NULL)
	{
		es->transmitBuf_length = 0;
		g_es = NULL;
		//mem_free(es);
	}

#if 1	// 210625 CJKIM Memory free added,  clarity에서 데이터 수신 안되어 close 반복 시 memory leak 발생.
	tcp_pcb_purge(tpcb);


#endif // 210625 CJKIM

	/* close tcp connection */
	tcp_close(tpcb);

	LEDStateChange = 0;

#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
	clear_queue();
#endif
}


static void tcp_server_connection_abort(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	DEBUG_I(DBG_LEVEL_INFO, "()\r\n");

	comm_disconnected_uvd_init(); 	//(+)210716 CJKIM, re-init uvd state when network disconnecting

	gcsvc.TCP_LineState = 0;

	Clarity_Connect = STATE_COMM_NOT_CONNECTED;
	g_TcpWindowState = STATE_TCP_NORMAL_WND;		//(+)210722 CJKIM, TCP Zero Window
	
	/* remove all callbacks */
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, NULL);
	tcp_recv(tpcb, NULL);
	tcp_err(tpcb, NULL);
	tcp_poll(tpcb, NULL, 0);

	/* delete es structure */
	if (es != NULL)
	{
		es->transmitBuf_length = 0;
		g_es = NULL;
		//mem_free(es);
	}

	tcp_pcb_purge(tpcb); 	 
	
	tcp_rst(tpcb->snd_nxt, tpcb->rcv_nxt, &tpcb->local_ip, &tpcb->remote_ip,
		tpcb->local_port, tpcb->remote_port);

	memp_free(MEMP_TCP_PCB, tpcb);

	LEDStateChange = 0;

#ifdef USED_RECV_QUEUE		//(+)210715 CJKIM, Received data process changing
	clear_queue();
#endif
}



/*
 * GCDB(GC Data Block) send
 */
err_t tcp_GCDB_Send(struct tcp_pcb *tpcb, struct tcp_server_struct *es, struct pbuf *gcd_pbuf)
{
	err_t wr_err = ERR_OK;
	//u16_t len;
	//uint16_t i;
/*
	if(gcsvc.GCDB_SendEnable == 0) return ERR_OK;

	for(i=0;i<600;i++)
		data_buff[i] = hex2char_conv(i%10);
	data_buff[598] = '\r';
	data_buff[599] = '\n';

	len = 600;

	wr_err = tcp_write(tpcb, (void *)data_buff, len, 1);

	if (wr_err == ERR_OK)
	{
		tcp_output(g_pcb);
		gcsvc.fail_cnt = 0;
	}
	else
	{
		////printf("send fail\r\n");
		gcsvc.fail_cnt++;
		if(gcsvc.fail_cnt > 20)
		{
			gcsvc.GCDB_SendEnable = 0;
			//printf("Auto send stop\r\n");
		}
	}
*/
	return wr_err;
}

void tcp_esstate_print(void)
{
	if(GCC_MASTER != gcsvc.MasterSlave)
		return;

	if(esstate_save != g_es->state)
	{
		switch(g_es->state)
		{
			case ES_NONE : 		break;//printf("NONE\r\n"); 
			case ES_ACCEPTED : 	break;//printf("ACCEPTED\r\n"); 
			case ES_RECEIVED : 	break;//printf("RECEIVED\r\n"); 
			case ES_CLOSING : 	break;//printf("CLOSING\r\n"); 
		}

		esstate_save = g_es->state;
	}

	if(g_es->state == ES_CLOSING)
	{
		ConnTime++;
		if(ConnTime > 5)
		{
			ConnTime = 0;
			g_es->state = ES_NONE;
		}
	}
	else
		ConnTime = 0;
}

void PC_CommandCheck(struct pbuf *p)
{
	uint8_t * cmd;

	//printf("PC_CommandCheck\r\n");

	cmd = (uint8_t *)(p->payload);
	if(p->len == 11)
	{
		if((cmd[0]=='S')&&(cmd[1]=='V')&&(cmd[2]=='C')) // Service Related Command
		{
			if((cmd[3]=='0')&&(cmd[4]=='0')){
				if(gcsvc.GCDB_SendEnable != 0)
				{
					gcsvc.GCDB_SendEnable = 0;
					//printf("GCDB send disabled\r\n");
				}
			}
			if((cmd[3]=='0')&&(cmd[4]=='1')){
				if(gcsvc.GCDB_SendEnable != 1)
				{
					gcsvc.GCDB_SendEnable = 1;
					//printf("GCDB send enabled\r\n");
				}
			}
		}
		else if((cmd[0]=='P')&&(cmd[1]=='A')&&(cmd[2]=='R')) // Parameter Command
		{
			if((cmd[3]=='0')&&(cmd[4]=='1'))
			{
				gcsvc.GCDB_SendInterval = char2hex_conv(cmd[5]);
				gcsvc.GCDB_SendInterval = (gcsvc.GCDB_SendInterval<<4) + char2hex_conv(cmd[6]);
				gcsvc.GCDB_SendInterval = (gcsvc.GCDB_SendInterval<<4) + char2hex_conv(cmd[7]);
				gcsvc.GCDB_SendInterval = (gcsvc.GCDB_SendInterval<<4) + char2hex_conv(cmd[8]);
				if(gcsvc.GCDB_SendInterval < 50) gcsvc.GCDB_SendInterval = 50; // Maximum speed 20Hz
				//printf("GCDB_SendInterval : %d\r\n", gcsvc.GCDB_SendInterval);

			}
		}

	}
}


const char* pkt_code_str(int code)
{
	static char str[32] ={0};
	memset( str, 0 , strlen(str));
	
	switch(code) {
	case PACKCODE_YL9020_INFORM :
		memcpy( str, "PACKCODE_YL9020_INFORM", strlen("PACKCODE_YL9020_INFORM"));
		break;
	case PACKCODE_YL9020_CONFIG :		
		memcpy( str, "PACKCODE_YL9020_CONFIG", strlen("PACKCODE_YL9020_CONFIG"));
		break;
	case PACKCODE_YL9020_SETUP	:	
		memcpy( str, "PACKCODE_YL9020_SETUP", strlen("PACKCODE_YL9020_SETUP"));
		break;
	case PACKCODE_YL9020_EVENT	:		
		memcpy( str, "PACKCODE_YL9020_EVENT", strlen("PACKCODE_YL9020_EVENT"));
		break;
	case PACKCODE_YL9020_STATE	:		
		memcpy( str, "PACKCODE_YL9020_STATE", strlen("PACKCODE_YL9020_STATE"));
		break;
	case PACKCODE_YL9020_SELFMSG	:
		memcpy( str, "PACKCODE_YL9020_SELFMSG", strlen("PACKCODE_YL9020_SELFMSG"));
		break;
	case PACKCODE_YL9020_DIAGDATA	:
		memcpy( str, "PACKCODE_YL9020_DIAGDATA", strlen("PACKCODE_YL9020_DIAGDATA"));
		break;
	case PACKCODE_YL9020_ACQDATA	:		
		memcpy( str, "PACKCODE_YL9020_ACQDATA", strlen("PACKCODE_YL9020_ACQDATA"));
		break;
	case PACKCODE_YL9020_SCANDATA	:	
		memcpy( str, "PACKCODE_YL9020_SCANDATA", strlen("PACKCODE_YL9020_SCANDATA"));
		break;
	case PACKCODE_YL9020_SERVICE	:		
		memcpy( str, "PACKCODE_YL9020_SERVICE", strlen("PACKCODE_YL9020_SERVICE"));
		break;
	case PACKCODE_YL9020_SVCDATA	:		
		memcpy( str, "PACKCODE_YL9020_SVCDATA", strlen("PACKCODE_YL9020_SVCDATA"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}

int PacketParse(unsigned int nPacketCode, char *pData)
{
	int ret = RE_OK ;
	
	unsigned int tmp,tmp1;
	unsigned int pIndex;		// 전달받은 이벤트 테이블 인덱스
	int slotsize;
	//uint32_t portNo;

#if 0
//=======================================================================
// 처음으로 연결 - 정당한 연결인지 확인한다,		
//=======================================================================
	if(	mainDevStt.connected == NO) {	
		switch(nPacketCode) {
			case PACKCODE_YL9020_INFORM:
				if(IS_REQUEST_PACKET(pData)) {	// 데이터 요청
					SEND_RESPONSE_UVD_PACKET(gUVD_INFORM,pData);
				}
				else if(IS_ACK_PACKET(pData)){
					//printf("ack--PACKCODE_HPLC_SYSTEM_INFORM\n");
				}
				else {
					//printf("rcv--PACKCODE_HPLC_SYSTEM_INFORM\n");
					return net_CheckModel(pData);
				}
				break;
			default:
				//printf("default\n");
				return RE_ERR;
		}
//printf("mainDevStt.connected == %d\n",mainDevStt.connected);		
	}
//=======================================================================
#endif

	//(+)210611 CJKIM, added 
	DEBUG_I(DBG_LEVEL_INFO, "() PKT Code[x%X, %s, %s]\r\n", nPacketCode, pkt_code_str(nPacketCode),
		IS_REQUEST_PACKET(pData) ? "Req" : IS_ACK_PACKET(pData) ? "Ack" : "Rcv");

	switch(nPacketCode)
	{
		case PACKCODE_YL9020_INFORM:
			if(IS_REQUEST_PACKET(pData)) {	// 데이터 요청
				SEND_RESPONSE_UVD_PACKET(gUVD_INFORM,pData);

			}
			else if(IS_ACK_PACKET(pData)){

			}
			else {

				return net_SystemInform(pData);
			}
			break;
		case PACKCODE_YL9020_CONFIG:
			if(IS_REQUEST_PACKET(pData)) {	// 데이터 요청
				SEND_RESPONSE_UVD_PACKET(gUVD_CONFIG,pData);

			}
			else if(IS_ACK_PACKET(pData)){

			}
			else {
				USE_UVD_PACKET(pData,tmpgUVD_CONFIG);
				SEND_ACK_UVD_PACKET(pData);
				if(gUVD_STATE.btState != UVD_STATE_INITIALIZING && gUVD_STATE.btState != UVD_STATE_READYINIT) 
					goto errorConfig;		// Setup을 적용할 수 없는 상태
				if( IS_INVALID_LIMITS(tmpgUVD_CONFIG.sCutWavelength0, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) ) 
					goto errorConfig;
				if( IS_INVALID_LIMITS(tmpgUVD_CONFIG.sCutWavelength1, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) ) 
					goto errorConfig;

				gUVD_CONFIG.sCutWavelength0 = tmpgUVD_CONFIG.sCutWavelength0;
				gUVD_CONFIG.sCutWavelength1 = tmpgUVD_CONFIG.sCutWavelength1;
				// 시간이 0xffff ffff면 사용시간 설정을 안함.
				// 램프사용 시간 저장				
				if(tmpgUVD_CONFIG.nD2LampElapse != 0xffffffff) {
					tmp = tmpgUVD_CONFIG.nD2LampElapse;
					tmp1 = tmp/10000;
					if(tmp1 == 9620){
						tmp = tmp - tmp1*10000;
						gUVD_CONFIG.nD2LampElapse = tmp;
						detStt.fD2LampElapse = (float)tmp;
					}
					else if(tmp1 == 1234)	// test	// Wave 초기화
					{
						fram_Wave_Reset();
					}
					
				}
				if(tmpgUVD_CONFIG.nWLampElapse != 0xffffffff) {
					tmp = tmpgUVD_CONFIG.nWLampElapse;
					tmp1 = tmp/10000;
					if(tmp1 == 9620){
						tmp = tmp - tmp1*10000;
						gUVD_CONFIG.nWLampElapse = tmp;
						detStt.fWLampElapse = (float)tmp;
					 }
				}
				fram_LampTime_Save();

				return net_SystemConfig(pData);
			}
			break;
errorConfig:			
			ErrorSetupMsg();		// 에러 self message 전송 및 현재설정값 전송 
			sendConfig();		// 현재 가지고 있는 데이터를 보낸다.
			break;
		case PACKCODE_YL9020_SETUP:
			//portNo = ((UVD_PACKET*)pData)->nEventIndex;	// EVENT INDEX	--> Inlet No.			
			if(IS_REQUEST_PACKET(pData)) {	// 데이터 요청
				SEND_RESPONSE_UVD_PACKET(gUVD_SETUP,pData);

			}
			else if(IS_ACK_PACKET(pData)){

			}
			else {
				USE_UVD_PACKET(pData,tmpgUVD_SETUP);
				SEND_ACK_UVD_PACKET(pData);
				setupFunc(tmpgUVD_SETUP.btCommand);

				//return net_Setup(portNo, pData);
				return net_Setup(pData);
			}
			break;
		case PACKCODE_YL9020_EVENT:
			if(IS_REQUEST_PACKET(pData)) {	// 데이터 요청
				pIndex = ((YL9000HPLC_PACKET*)pData)->nEventIndex + 1;
				sendEvent(pIndex);
			}
			else if(IS_ACK_PACKET(pData)){
			}
			else {

				pIndex = ((YL9000HPLC_PACKET*)pData)->nEventIndex + 1;		// EVENT INDEX	--> 1 부터 시작 
				slotsize = ((YL9000HPLC_PACKET*)pData)->nSlotSize;			// EVENT INDEX	--> 1 부터 시작 

				USE_UVD_PACKET(pData,tmpgUVD_EVENT);
				SEND_ACK_UVD_PACKET(pData);
				if(detStt.runstop == RUN_START) goto errorEvent;	// Event을 적용할 수 없는 상태
				
				// gUVD_EVENT[0].fTime는 무조건 0.0
				if(pIndex > PROGRAM_COUNT_MAX)  goto errorEvent;	// INDEX 에러
				if( IS_INVALID_LIMITS(tmpgUVD_EVENT.fTime, gUVD_EVENT[pIndex-1].fTime, EVENT_TIME_MAX) ) 
					goto errorEvent;
				if( IS_INVALID_LIMITS(tmpgUVD_EVENT.sWavelengthA, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) ) 
					goto errorEvent;
				if( IS_INVALID_LIMITS(tmpgUVD_EVENT.sWavelengthB, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) ) 
					goto errorEvent;
				if(tmpgUVD_EVENT.btCutFilter > UVD_SETUP_CUTFILTER_MAX) 
					goto errorEvent;

				memcpy(&gUVD_EVENT[pIndex], &tmpgUVD_EVENT,sizeof(YL9020_UVD_EVENT));
				//DEBUG_I(DBG_LEVEL_INFO, "() gUVD_EVENT[%d].sWavelengthA = %d ,, %d\n", 
				//	pIndex, gUVD_EVENT[pIndex].sWavelengthA, tmpgUVD_EVENT.sWavelengthA);

				return net_Event(pData);
			}
			break;
errorEvent:			
			ErrorSetupMsg();		// 에러 self message 전송 및 현재설정값 전송
			//sendEvent(pIndex -1);	// 현재 가지고 있는 데이터를 보낸다. -- 보내야 되나? 안보내는 것이 나을 듯하다.
			break;
		case PACKCODE_YL90XX_UPDATE:		// 업데이트 패킷이 들오면
			break;
		case PACKCODE_YL9020_SELFMSG:

			break;
		default:

			break;
	}

	return ret;
}

void setupFunc(unsigned char btCommand)
{
	static unsigned char oldCommand = -1;
	static int i=0;
	i++;

	if(oldCommand != btCommand) {
		DEBUG_I(DBG_LEVEL_INFO, "(%d) Cmd [%s]\n", btCommand, uvd_setup_str(btCommand));	
		DEBUG_I(DBG_LEVEL_INFO, "(%d) UVD_STATE[%s]\n", btCommand, uvd_act_state_str(gUVD_STATE.btState));	
		oldCommand = btCommand;
	}
	
	
//	//DEBUG_I(DBG_LEVEL_INFO, "() gUVD_SETUP.InitEvent.btWLamp = %d\n", gUVD_SETUP.InitEvent.btWLamp);
//	//DEBUG_I(DBG_LEVEL_INFO, "() gUVD_SETUP.InitEvent.btD2Lamp = %d\n", gUVD_SETUP.InitEvent.btD2Lamp);
	switch(btCommand) 
	{
		case UVD_SETUP_NONE:	// setup 명령을 받으면 값만 적용하고 - 실행(ready init)은 안한다.
			if(setupDataValidation() == -1) goto errorSetup;

			memcpy(&gUVD_SETUP, &tmpgUVD_SETUP,sizeof(tmpgUVD_SETUP));
			memcpy(&gUVD_EVENT[0], &tmpgUVD_SETUP.InitEvent,sizeof(tmpgUVD_SETUP.InitEvent));
			// PACKCODE_YL9020_SETUP receive - endloop
			break;
errorSetup:
				ErrorSetupMsg();		// 에러 self message 전송 및 현재설정값 전송
				sendSetup();			// 현재 가지고 있는 데이터를 보낸다.
			break;
		case UVD_SETUP_ACQUISITION_START:
			if(gUVD_STATE.btState == UVD_STATE_READYINIT) 
			{ // || gUVD_STATE.btState == UVD_STATE_RUN_MODE) {

				startFunc();
			}
			else {
				ErrorSetupMsg();		// 에러 self message 전송 및 현재설정값 전송
				sendSetup();			// 현재 가지고 있는 데이터를 보낸다.
			}
			break;
		case UVD_SETUP_AUTO_ZERO:
			detStt.bAutozeroA = detStt.bAutozeroB = YES;
			break;
		case UVD_SETUP_SCAN_START:
			if(gUVD_STATE.btState != UVD_STATE_READYINIT) 
			{
				ErrorSetupMsg();		// 에러 self message 전송 및 현재설정값 전송
			}
			else 
			{	// 스캔모드
				// grating motor를 이동하고 
				
				//DEBUG_I(DBG_LEVEL_INFO, "() start = %d : stop = %d int = %d cnt = %d \n",
				//	gUVD_SETUP.ScanSet.sStartWavelength, gUVD_SETUP.ScanSet.sStopWavelength, gUVD_SETUP.ScanSet.btScanInterval,
				//	gUVD_SETUP.ScanSet.btSamplingCount);
				detStt.waveMode = UVD_SCAN_WAVE_MODE;	// SCAN MODE
				detMotor.sStartStep 
					= GM_wave[gUVD_SETUP.ScanSet.sStartWavelength];
				detStt.scanDestWave = gUVD_SETUP.ScanSet.sStartWavelength;
				detMotor.sEndStep = GM_wave[gUVD_SETUP.ScanSet.sStopWavelength];
				detStt.scanEndWave = gUVD_SETUP.ScanSet.sStopWavelength;

				detMotor.btMode = GM_SINGLE_ACT;

				detStt.scanInterval= gUVD_SETUP.ScanSet.btScanInterval;
				detStt.scanMode = UVD_SCAN_MOVEING;
				detStt.scanWaitCnt = SCANMODE_WAITING_CNT + detStt.scanInterval / SCANMODE_INTERVAL_CNT + 1;	// 0.05
				detStt.scanReadCnt = detStt.scanWaitCnt + SCANMODE_READING_CNT;

				detStt.scanSamplingCount = gUVD_SETUP.ScanSet.btSamplingCount;

				Motor_Ctrl_Command();
				detStt.oldState = gUVD_STATE.btState;
				gUVD_STATE.btState = UVD_STATE_SCANNING_MODE;
				sendSelfMsgState(gUVD_STATE.btState);
				LEDStateChange = 0;
			};
			break;
		case UVD_SETUP_DIAGNOSIS_START:
			if(gUVD_STATE.btState != UVD_STATE_RUN_MODE && gUVD_STATE.btState != UVD_STATE_SCANNING_MODE) {
//			if(gUVD_STATE.btState == UVD_STATE_READYINIT) {
				detStt.oldState = gUVD_STATE.btState;
				gUVD_STATE.btState = UVD_STATE_DIAGNOSIS_MODE;
				sendSelfMsgState(gUVD_STATE.btState);
				detStt.diagStart = YES;
				LEDStateChange = 0;
 			}
			else {
				ErrorSetupMsg();		// 에러 self message 전송 및 현재설정값 전송
			}
			break;
		case UVD_SETUP_STOP:
			if(gUVD_STATE.btState == UVD_STATE_RUN_MODE || gUVD_STATE.btState == UVD_STATE_SCANNING_MODE
				|| gUVD_STATE.btState == UVD_STATE_DIAGNOSIS_MODE) {
	
				stopFunc();	// 실행후 셀프메시지 -- UVD_SETUP_READY_INIT 로 말까?
			}
			else {
				DEBUG_I(DBG_LEVEL_INFO, "() Ignore\r\n");
			}	// 무시
			break;
		case UVD_SETUP_RESET_ERROR:	// INITIALIZING 부터 다시한다.
			if(gUVD_STATE.btState == UVD_STATE_FAULT) {	
				motorInitSet(254, 254, CUTFILTER_NONE);
				LEDStateChange = 0;
//				ledError(LED_OFF);
				Initialize();
				//Selfmsg_flag = OFF;
				Selfmsg_flag = ON;	// 210721 KSLEE, 3sec 후 Error Check 다
				gUVD_STATE.uErrorCode = UVD_ERR_NONE;
				BUZZER_CON_OFF;

			}
			break;
		case UVD_SETUP_READY_INIT:	// INITIALIZING이 끝난 상태 - 런이 끝나고 나서 다시 스타트 할 수 있는 상태로 초기화 한다
			if(gUVD_STATE.btState != UVD_STATE_INITIALIZING && gUVD_STATE.btState != UVD_STATE_FAULT) 
			{
				detStt.oldState = gUVD_STATE.btState;
				readyInitSet();
				sendSelfMsgState(gUVD_STATE.btState);
			}
			else 
			{
				DEBUG_I(DBG_LEVEL_INFO, "() Ignore\r\n");
				// ErrorProcess(unsigned char err);
				// ready Init 할 수 없는 상태
			}
			break;
	}


}

// Setup을 적용할 수 없는 상태
void ErrorSetupMsg()	
{
//	//printf("ErrorSetupMsg send\n");
	sendSelfMsg(UVD_SELFMSG_ERROR, 0, 0, UVD_ERR_INVALID_SETUP);
//	sendSetup();
}

int setupDataValidation()
{
	// btAutoZero ,btAutoOffset, btDataFilter는 검사하지 않고 셋팅값이 넘으면 마지막값 설정으로 함.
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.ChannelSetA.fTimeConst, UVD_SETUP_TIMECONST_MIN, UVD_SETUP_TIMECONST_MAX) ) return -1;
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.ChannelSetB.fTimeConst, UVD_SETUP_TIMECONST_MIN, UVD_SETUP_TIMECONST_MAX) ) return -1;
	if(tmpgUVD_SETUP.nProgramCount > PROGRAM_COUNT_MAX) return -1;
	
	// gUVD_EVENT[0].fTime는 무조건 0.0
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.InitEvent.sWavelengthA, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) ) return -1;
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.InitEvent.sWavelengthB, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) ) return -1;
	if(tmpgUVD_SETUP.InitEvent.btCutFilter > UVD_SETUP_CUTFILTER_MAX) return -1;
	// btD2Lamp, btWLamp ,btPolarity, btAutoZero, btMarkOut
	// btAZExtIn, btLampExtIn ,btChartExtIn, btStartExtIn, btReadyExtIn
	// btReadyExtOut, btStartExtOut ,btDefaultMarkOut
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.nExtoutTime, EXTOUTTIME_MIN, EXTOUTTIME_MAX) ) return -1;
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.ScanSet.sStartWavelength, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) )	return -1;

	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.ScanSet.sStopWavelength, UVD_WAVELENGTH_MIN, UVD_WAVELENGTH_MAX) )	return -1;
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.ScanSet.btScanInterval, UVD_SETUP_SCANINTERVAL_MIN, UVD_SETUP_SCANINTERVAL_MAX) ) return -1;
	if( IS_INVALID_LIMITS(tmpgUVD_SETUP.ScanSet.btSamplingCount, UVD_SETUP_SAMPLINGCOUNT_MIN, UVD_SETUP_SAMPLINGCOUNT_MAX) ) return -1;
//	DEBUG_I(DBG_LEVEL_INFO, "() sizeof(gUVD_SETUP)	= %d\r\n",	sizeof(gUVD_SETUP));
//	DEBUG_I(DBG_LEVEL_INFO, "() tmpgUVD_SETUP.nExtoutTime	= %d\r\n",tmpgUVD_SETUP.nExtoutTime);


	return 0;	// 에러없음
}

/*
 * This function should be called every 1 MS
 */
/*
int netConnectAction()
{
	// 함수포인터 설정
	if(mainDevStt.connected) {
		pSend_State_Fn = send_State;
		pSend_SignalData_Fn = send_SignalData;
	}
	else {
		pSend_State_Fn = send_State_Blank;
		pSend_SignalData_Fn = send_SignalData_Blank;		
	}
}
*/
// 여기서 접속된 기기에 정보을 요청한다.
void netConnection()
{
//	err_t wr_err = ERR_OK;

//	SEND_REQUEST_UVD_PACKET(PACKCODE_YL6200_SYSTEM_INFORM, SysInform, SysInform);
	SEND_REQUEST_UVD_PACKET(PACKCODE_YL9020_INFORM, gUVD_INFORM, gUVD_INFORM);

// //printf("SEND_REQUEST_UVD_PACKET(PACKCODE_YL9020_INFORM, gUVD_INFORM, gUVD_INFORM)\n");

/*
//printf("===SEND_REQUEST_UVD_PACKET=== L=%d D=%X C=%X E=%d O=%d S=%d\r\n"
,((UVD_PACKET *)buffer)->nPacketLength
,((UVD_PACKET *)buffer)->nDeviceID
,((UVD_PACKET *)buffer)->nPacketCode
,((UVD_PACKET *)buffer)->nEventIndex
,((UVD_PACKET *)buffer)->nSlotOffset
,((UVD_PACKET *)buffer)->nSlotSize
);
*/
}	

/*
// 맨처음 연결시만 동작한다.
int net_CheckModel(char *pData)
{
	YL9020_UVD_INFORM tmp;
	USE_UVD_PACKET(pData,tmp);

	// 처음으로 연결 - 정당한 연결인지 확인한다,
	if( (strcmp(gUVD_INFORM.Model, tmp.Model)) )
	{
		//printf("check--ERROR--PACKCODE_UVD_SYSTEM_INFORM %s tmp=%s \n", tmp.Model, gUVD_INFORM.Model);									
		return RE_ERR;
	}
	else {
	}

	SEND_ACK_UVD_PACKET(pData);
	//printf("check--EQUAL--PACKCODE_UVD_SYSTEM_INFORM\n");

#if 1
	mainDevStt.connected = YES;
	mainDevStt.disconnected = NO;
		
	netConnectAction();	
#endif

	make_MACADDR();	// mac생성

	return RE_OK;
}
*/
int net_SystemInform(char *pData)
{
	YL9020_UVD_INFORM tmp;

	int retSerialNo = RE_ERR;
	int retIpAddr = RE_ERR;
	
	if(gUVD_STATE.btState == UVD_STATE_RUN_MODE) {
		send_SelfMessage(UVD_SELF_COMMAND_REJECT,0);	// 실행할 수 없음.
		return RE_ERR;
	}

	USE_UVD_PACKET(pData,tmp);

	SEND_ACK_UVD_PACKET(pData);

	if(retSerialNo == RE_OK || retIpAddr == RE_OK) {
		fram_Inform_Save();
	}
	return RE_OK;
}

int net_SystemConfig(char *pData)
{
	return RE_OK;
}

//int net_Setup(uint32_t portNo, char *pData)
int net_Setup(char *pData)
{
	int result = RE_OK;
/*	
	YL9020_UVD_SETUP tmpSetup;

	if(gUVD_STATE.btState == UVD_STATE_RUN_MODE || gUVD_STATE.btState == UVD_STATE_SCANNING_MODE || gUVD_STATE.btState == UVD_STATE_DIAGNOSIS_MODE
		|| gUVD_STATE.btState == UVD_STATE_FAULT || gUVD_STATE.btState == UVD_STATE_INITIALIZING)
	{
		send_SelfMessage(UVD_SELF_COMMAND_REJECT,0);	// 실행할 수 없음.
		return RE_ERR;
	}

	USE_UVD_PACKET(pData,tmpSetup);

	if(!check_Setup(portNo, &tmpSetup)) {
		errorFunction(SELF_ERROR_COMMAND_UNKNOWN);
		SEND_UVD_PACKET(PACKCODE_YL9020_SETUP, gUVD_SETUP, gUVD_SETUP);
		
		return RE_ERR;
	}

	SEND_ACK_UVD_PACKET(pData);

	memcpy(&gUVD_SETUP, &gUVD_SETUP, sizeof(PACKCODE_YL9020_SETUP));
	
	setupFunc(tmpgUVD_SETUP.btCommand);

	RS422_Inlet_Setting_fn(portNo);
	apc_APCSend((portNo+1), sizeof(RS422_Inlet_Setting_t), rs422_APC_CMD_INLET_SETTING, &RS422_Inlet_Setting);
#endif

	LCD_Send_Data(sizeof(CHROZEN_INLET_SETTING_t), (YL6700GC_LCD_COMM_INLET_SETTING_1 + portNo) , &czInlet[portNo]); // LCD_Send_Data(sizeof(nsOven), &((char*)nsOven));
*/
	return result;
}

int net_Event(char *pData)
{
	int result = RE_OK;
	return result;
}

int send_State_Blank()
{
	return 0;
}

void send_State()
{
	//err_t ret = ERR_OK;

	//printf("INFO. %s()+\n",__FUNCTION__); 	//(+)210604 CJKIM, For Debug	
	SEND_UVD_PACKET(PACKCODE_YL9020_STATE, gUVD_STATE, gUVD_STATE);
	//printf("INFO. %s() -\n",__FUNCTION__);	//(+)210604 CJKIM, For Debug	
	
	//return ret;
}

int send_SignalData_Blank()
{
	return 0;
}

int send_SignalData()
{
//	DetSignalData.sigData.nIndex++;

//	SEND_UVD_PACKET(PACKCODE_YL6200_SIGNAL, DetSignalData.sigData, DetSignalData.sigData);
	return 0;
}

int send_SelfMessage(unsigned char ucMessage, unsigned char ucNewValue)
{
	gUVD_SELFMSG.btMessage = ucMessage;
	gUVD_SELFMSG.sNewValue = ucNewValue;
	SEND_UVD_PACKET(PACKCODE_YL9020_SELFMSG, gUVD_SELFMSG, gUVD_SELFMSG);
	
	return 0;
}

void sendConfig()
{
	SEND_UVD_PACKET(PACKCODE_YL9020_CONFIG, gUVD_CONFIG, gUVD_CONFIG);
}

void sendSetup()
{
	SEND_UVD_PACKET(PACKCODE_YL9020_SETUP, gUVD_SETUP, gUVD_SETUP);
}

void sendEvent(unsigned int nEventIndex)
{
	SEND_UVD_PACKET(PACKCODE_YL9020_EVENT, gUVD_EVENT[nEventIndex], gUVD_EVENT[nEventIndex]);
	//printf("---- send Event ----\r\n");
}

void sendSelfMsg(unsigned char btMessage, unsigned short sOldValue, unsigned short	sNewValue, unsigned int uErrorCode)
{
	gUVD_SELFMSG.btMessage = btMessage;
	gUVD_SELFMSG.sOldValue = sOldValue;
	gUVD_SELFMSG.sNewValue = sNewValue;
	gUVD_SELFMSG.uErrorCode = uErrorCode;


	////printf("btState = %d\n", gUVD_SELFMSG.btMessage);
	////printf("sOldValue = %d\n", gUVD_SELFMSG.sOldValue);
	////printf("sNewValue = %d\n", gUVD_SELFMSG.sNewValue);
	////printf("uErrorCode = %d\n", gUVD_SELFMSG.uErrorCode);

	SEND_UVD_PACKET(PACKCODE_YL9020_SELFMSG, gUVD_SELFMSG, gUVD_SELFMSG);
}

void sendSelfMsgState(unsigned short sNewValue)
{
	gUVD_SELFMSG.btMessage = UVD_SELFMSG_STATE;
	gUVD_SELFMSG.sOldValue = detStt.oldState;
	gUVD_SELFMSG.sNewValue = sNewValue;
	gUVD_SELFMSG.uErrorCode = UVD_ERR_NONE;

	SEND_UVD_PACKET(PACKCODE_YL9020_SELFMSG, gUVD_SELFMSG, gUVD_SELFMSG);
	////printf("sendSelfMsgState = %d : %d : %d : %d \n", gUVD_SELFMSG.btMessage,gUVD_SELFMSG.sOldValue,gUVD_SELFMSG.sNewValue,gUVD_SELFMSG.uErrorCode);
}

void sendSelfMsgExtin(unsigned short sNewValue)
{
	gUVD_SELFMSG.btMessage = UVD_SELFMSG_EXTIN;
	gUVD_SELFMSG.sOldValue = 0;
	gUVD_SELFMSG.sNewValue = sNewValue;
	gUVD_SELFMSG.uErrorCode = UVD_ERR_NONE;

	SEND_UVD_PACKET(PACKCODE_YL9020_SELFMSG, gUVD_SELFMSG, gUVD_SELFMSG);
}

void sendSelfMsgExtout(unsigned short	sNewValue)
{
	gUVD_SELFMSG.btMessage = UVD_SELFMSG_EXTOUT;
	gUVD_SELFMSG.sOldValue = 0;
	gUVD_SELFMSG.sNewValue = sNewValue;
	gUVD_SELFMSG.uErrorCode = UVD_ERR_NONE;

	SEND_UVD_PACKET(PACKCODE_YL9020_SELFMSG, gUVD_SELFMSG, gUVD_SELFMSG);
}

void sendSelfMsgError(unsigned short	sNewValue)
{
	gUVD_SELFMSG.btMessage = UVD_SELFMSG_ERROR;
	gUVD_SELFMSG.sOldValue = 0;
	gUVD_SELFMSG.sNewValue = 0;
	gUVD_SELFMSG.uErrorCode = sNewValue;

	////printf("sendSelfMsgError(%d)\n",sNewValue);
	SEND_UVD_PACKET(PACKCODE_YL9020_SELFMSG, gUVD_SELFMSG, gUVD_SELFMSG);
}

void sendDiagdata()
{
	SEND_UVD_PACKET(PACKCODE_YL9020_DIAGDATA, gUVD_DIAGDATA, gUVD_DIAGDATA);
}

void sendAcqdata()
{
	SEND_UVD_PACKET(PACKCODE_YL9020_ACQDATA, gUVD_ACQDATA, gUVD_ACQDATA);
}

void sendScandata()
{
	SEND_UVD_PACKET(PACKCODE_YL9020_SCANDATA, gUVD_SCANDATA, gUVD_SCANDATA);
}


#endif /* LWIP_TCP */
