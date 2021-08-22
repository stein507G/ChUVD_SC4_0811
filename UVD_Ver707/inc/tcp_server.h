/*
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
 * This file is part of the lwIP TCP/IP stack.
 * 
 */
#ifndef __TCP_ECHOSERVER_H__
#define __TCP_ECHOSERVER_H__

#include <config.h>

#define GCC_MASTER   	0
#define GCC_SLAVE   	1
#define GCC_SLAVE_422   	3

#define STATE_COMM_NOT_CONNECTED 	0x00	// CJKIM, Client Connection state
#define STATE_COMM_CONNECTED 		0x01

#define STATE_TCP_NORMAL_WND 	0x00	//(+)210722 CJKIM, TCP Zero Window
#define STATE_TCP_ZERO_WND 		0x01	//(+)210722 CJKIM, TCP Zero Window

/* ECHO protocol states */
enum tcp_server_states
{
  ES_NONE = 0,
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING
};

	
typedef struct
{
	uint8_t		MasterSlave; //.Slave function is used for test.
	uint8_t		ApcDestId;
	uint8_t		ApcMyId;
	uint8_t		GCDB_SendEnable;
	uint16_t	GCDB_SendInterval; // unit (MS)
	uint16_t 	SendPcTime;
	uint8_t 	TCP_LineState; // 1:connected, else disconnected
	uint32_t	fail_cnt;
	uint32_t	pass_cnt;
} GCSVC_ST; // GC Service struct

/* structure for maintaing connection infos to be passed as argument 
   to LwIP callbacks*/
struct tcp_server_struct
{
	uint8_t state;             /* current connection state */
	struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
//  struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
	int transmitBuf_length;
//	char transmitBuf[PACKET_SIZE_MAX<<1];
	char transmitBuf[PACKET_SIZE_MAX];
} TCP_SERVER_STRICT;



void chrozen_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es, char * p, int size);

void tcp_server_init(void);

void tcp_send(void);
void tcp_esstate_print(void);

void Gc_ServiceInit(void);

int netConnectAction();




int check_Command(char *rcvData);
int net_Command(char *pData);


void make_MACADDR();

int (*pSend_State_Fn) (); // 함수포인터 선언
int send_State_Blank();
#if 0
int send_State();
#else
void send_State();
#endif

int (*pSend_SignalData_Fn)();
int send_SignalData_Blank();
int send_SignalData();


int (*pLCD_Send_Data_Fn)();
//LCD_Send_DET_SIGNAL_DATA


int	errorFunction(unsigned char	err);

void setupFunc(unsigned char btCommand);
void ErrorSetupMsg();
int setupDataValidation();
void netConnection();
int net_SystemInform(char *pData);
int net_SystemConfig(char *pData);
int net_Setup(char *pData);
int net_Event(char *pData);


int send_State_Blank();
void send_State();

int send_SignalData_Blank();
int send_SignalData();

int send_SelfMessage(unsigned char ucMessage, unsigned char ucNewValue);

void sendConfig();
void sendSetup();

void sendEvent(unsigned int nEventIndex);


void sendSelfMsg(unsigned char btMessage, unsigned short sOldValue, unsigned short	sNewValue, unsigned int uErrorCode);
void sendSelfMsgState(unsigned short sNewValue);
void sendSelfMsgExtin(unsigned short sNewValue);
void sendSelfMsgExtout(unsigned short	sNewValue);
void sendSelfMsgError(unsigned short	sNewValue);

void sendDiagdata();
void sendAcqdata();
void sendScandata();

#endif /* __TCP_ECHOSERVER */
