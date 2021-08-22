/*
 * Protocol_YL6700GC.h
 *
 *  Created on: 2020. 8. 14.
 *      Author: ksyi
 */

#ifndef PROTOCOL_CHROZEN_UVD_H_
#define PROTOCOL_CHROZEN_UVD_H_

typedef struct _UVD_PACKET
{
	unsigned int nPacketLength;			// 전체 길이

	unsigned int nDeviceID;				// #define PACKET_DEVICE_ID_YL6500				(0x97000)
	unsigned int nPacketCode;
	unsigned int nEventIndex;			// Event(Table 데이터)일 때의 Index	// 0 부터 보낸다. --> 어레이 1번에 저장하라
										// 제어기로 보낼때는 1번 어레이 부터 보내고 ,nEventIndex는 0부터 시작하라
	unsigned int nSlotOffset;
	unsigned int nSlotSize;				// slot's size
	unsigned char Slot[0];
} UVD_PACKET;

typedef struct _UVD_UPDATE_PACKET
{
	char Type[32];
	char Name[32];
	char Version[32];
	char Date[32];

	unsigned int nSize;
	unsigned int nCheckSum;
} UVD_UPDATE_PACKET;

#define PACKET_HEADER_SIZE		(24)	//(28)

#define PACKET_SIZE_MIN			PACKET_HEADER_SIZE
//#define PACKET_SIZE_MAX			256
#define PACKET_SIZE_MAX			1024
//#define PACKET_BUFF_SIZE		4096
#define PACKET_BUFF_SIZE		1024
#define PACKET_BUFFER_MAX		PACKET_BUFF_SIZE

#define PACKET_LENGTH_SIZE		4

#define PA_RESPONSE			(0)
#define PA_REQUEST			(1)

#define PA_TABLE_ZERO		(0)
#define PA_TABLE			(1)

/*
#define PACKET_DEVICE_ID_YL6700				(0)
#define PACKET_DEVICE_ID_YL6500				(0x67000)

#define PACKCODE_CHROZEN_SYSTEM_INFORM		(PACKET_DEVICE_ID_YL6500 + 0x100)
#define PACKCODE_CHROZEN_SYSTEM_CONFIG		(PACKET_DEVICE_ID_YL6500 + 0x110)
#define PACKCODE_CHROZEN_OVEN_SETTING		(PACKET_DEVICE_ID_YL6500 + 0x120)

#define PACKCODE_CHROZEN_INLET_SETTING		(PACKET_DEVICE_ID_YL6500 + 0x130)

#define PACKCODE_CHROZEN_DET_SETTING			(PACKET_DEVICE_ID_YL6500 + 0x140)

#define PACKCODE_CHROZEN_VALVE_SETTING		(PACKET_DEVICE_ID_YL6500 + 0x150)

#define PACKCODE_CHROZEN_AUXILIARY_SETTING	(PACKET_DEVICE_ID_YL6500 + 0x160)

#define PACKCODE_CHROZEN_AUX_APC_SETTING	(PACKET_DEVICE_ID_YL6500 + 0x165)

#define PACKCODE_YL6200_SIGNAL_SETTING		(PACKET_DEVICE_ID_YL6500 + 0x170)

#define PACKCODE_CHROZEN_SPECIAL_FUNCTION	(PACKET_DEVICE_ID_YL6500 + 0x180)
#define PACKCODE_YL6200_TIME_CTRL_SETTING	(PACKET_DEVICE_ID_YL6500 + 0x371)

#define PACKCODE_CHROZEN_SYSTEM_STATE		(PACKET_DEVICE_ID_YL6500 + 0x500)
#define PACKCODE_YL6200_SLFEMSG				(PACKET_DEVICE_ID_YL6500 + 0x510)

#define PACKCODE_YL6200_COMMAND				(PACKET_DEVICE_ID_YL6500 + 0x520)

#define PACKCODE_YL6200_SIGNAL				(PACKET_DEVICE_ID_YL6500 + 0x530)

#define PACKCODE_YL6200_DIAGDATA			(PACKET_DEVICE_ID_YL6500 + 0x600)
	#define PACKCODE_YL6200_SERVICE				(PACKET_DEVICE_ID_YL6500 + 0x610)
	#define PACKCODE_YL6200_SVCDATA				(PACKET_DEVICE_ID_YL6500 + 0x620)
*/

#if 0
#define PACKET_DEVICE_ID_UVD_PACKET	(0)
#define PACKET_DEVICE_ID_UVD		(0x90000)
#define PACKCODE_YL9020_INFORM		(PACKET_DEVICE_ID_UVD + 200)
#define PACKCODE_YL9020_CONFIG		(PACKET_DEVICE_ID_UVD + 201)
#define PACKCODE_YL9020_SETUP		(PACKET_DEVICE_ID_UVD + 202)
#define PACKCODE_YL9020_EVENT		(PACKET_DEVICE_ID_UVD + 203)
#define PACKCODE_YL9020_STATE		(PACKET_DEVICE_ID_UVD + 204)
#define PACKCODE_YL9020_SELFMSG		(PACKET_DEVICE_ID_UVD + 205)
#define PACKCODE_YL9020_DIAGDATA	(PACKET_DEVICE_ID_UVD + 206)
#define PACKCODE_YL9020_ACQDATA		(PACKET_DEVICE_ID_UVD + 207)
#define PACKCODE_YL9020_SCANDATA	(PACKET_DEVICE_ID_UVD + 208)
#define PACKCODE_YL9020_SERVICE		(PACKET_DEVICE_ID_UVD + 209)
#define PACKCODE_YL9020_SVCDATA		(PACKET_DEVICE_ID_UVD + 210)
#else
#define PACKET_DEVICE_ID_UVD_PACKET	(0)
#define PACKET_DEVICE_ID_UVD		(0x90000)
#define PACKCODE_YL9020_INFORM		(PACKET_DEVICE_ID_UVD + 0x200)
#define PACKCODE_YL9020_CONFIG		(PACKET_DEVICE_ID_UVD + 0x201)
#define PACKCODE_YL9020_SETUP		(PACKET_DEVICE_ID_UVD + 0x202)
#define PACKCODE_YL9020_EVENT		(PACKET_DEVICE_ID_UVD + 0x203)
#define PACKCODE_YL9020_STATE		(PACKET_DEVICE_ID_UVD + 0x204)
#define PACKCODE_YL9020_SELFMSG		(PACKET_DEVICE_ID_UVD + 0x205)
#define PACKCODE_YL9020_DIAGDATA	(PACKET_DEVICE_ID_UVD + 0x206)
#define PACKCODE_YL9020_ACQDATA		(PACKET_DEVICE_ID_UVD + 0x207)
#define PACKCODE_YL9020_SCANDATA	(PACKET_DEVICE_ID_UVD + 0x208)
#define PACKCODE_YL9020_SERVICE		(PACKET_DEVICE_ID_UVD + 0x209)
#define PACKCODE_YL9020_SVCDATA		(PACKET_DEVICE_ID_UVD + 0x210)

#endif

#define TCP_WRITE_PACKET  chrozen_send(g_pAcceptPcb, g_es, pPacketBuff, ((UVD_PACKET *)pPacketBuff)->nPacketLength)


// Packet 조립
//
//		buffer:			조립한 packet이 저장될 pointer
//		full_data:		packet code에 해당하는 data 변수
//		entry_data:		full_data 변수 자체 혹은 그 멤버 변수
//		request:		명령 packet이면 0, 응답 요구 packet이면 1
//
// 패킷 만들기
#define MAKE_UVD_PACKET(Code,full_data,entry_data) { \
	char *buffer = pPacketBuff; \
	((UVD_PACKET *)buffer)->nPacketLength = sizeof(UVD_PACKET) + sizeof(entry_data); \
	((UVD_PACKET *)buffer)->nDeviceID = (PACKET_DEVICE_ID_UVD_PACKET); \
	((UVD_PACKET *)buffer)->nPacketCode = (Code); \
	((UVD_PACKET *)buffer)->nEventIndex = 0; \
	((UVD_PACKET *)buffer)->nSlotOffset = ((int)&entry_data) - ((int)&full_data);	\
	((UVD_PACKET *)buffer)->nSlotSize = sizeof(entry_data); \
	memcpy(((UVD_PACKET*)buffer)->Slot, &entry_data, sizeof(entry_data)); \
}

#define SEND_UVD_PACKET(Code,full_data,entry_data)	{ \
	MAKE_UVD_PACKET(Code,full_data,entry_data); \
	TCP_WRITE_PACKET;\
}

#define MAKE_UVD_TABLE_PACKET(Code,TableNo,full_data,entry_data)	{ \
	char *buffer = pPacketBuff; \
	((UVD_PACKET *)buffer)->nPacketLength = sizeof(UVD_PACKET) + sizeof(entry_data); \
	((UVD_PACKET *)buffer)->nDeviceID = (PACKET_DEVICE_ID_UVD_PACKET); \
	((UVD_PACKET *)buffer)->nPacketCode = (Code); \
	((UVD_PACKET *)buffer)->nEventIndex = TableNo;	\
	((UVD_PACKET *)buffer)->nSlotOffset = ((int)&entry_data) - ((int)&full_data);	\
	((UVD_PACKET *)buffer)->nSlotSize = sizeof(entry_data); \
	memcpy(((UVD_PACKET*)buffer)->Slot, &entry_data, sizeof(entry_data)); \
}

#define SEND_UVD_TABLE_PACKET(Code,TableNo,full_data,entry_data)	{ \
	MAKE_UVD_TABLE_PACKET(Code,TableNo,full_data,entry_data); \
	TCP_WRITE_PACKET; \
}

// 요청
#define REQUEST_UVD_PACKET(Code,full_data,entry_data)	{ \
	char *buffer = pPacketBuff; \
	((UVD_PACKET *)buffer)->nPacketLength = sizeof(UVD_PACKET); \
	((UVD_PACKET *)buffer)->nDeviceID = (PACKET_DEVICE_ID_UVD_PACKET); \
	((UVD_PACKET *)buffer)->nPacketCode = (Code); \
	((UVD_PACKET *)buffer)->nEventIndex = 0;	\
	((UVD_PACKET *)buffer)->nSlotOffset = ((int)&entry_data) - ((int)&full_data);	\
	((UVD_PACKET *)buffer)->nSlotSize = sizeof(entry_data); \
}

#define SEND_REQUEST_UVD_PACKET(Code,full_data,entry_data)	{ \
	REQUEST_UVD_PACKET(Code,full_data,entry_data); \
	TCP_WRITE_PACKET; \
}

#define REQUEST_UVD_TABLE_PACKET(Code,TableNo,full_data,entry_data)	{ \
	char *buffer = pPacketBuff; \
	((UVD_PACKET *)buffer)->nPacketLength = sizeof(UVD_PACKET); \
    ((UVD_PACKET *)buffer)->nDeviceID = (PACKET_DEVICE_ID_UVD_PACKET); \
	((UVD_PACKET *)buffer)->nPacketCode = (Code); \
	((UVD_PACKET *)buffer)->nEventIndex = TableNo;	\
	((UVD_PACKET *)buffer)->nSlotOffset = ((int)&entry_data) - ((int)&full_data);	\
	((UVD_PACKET *)buffer)->nSlotSize = sizeof(entry_data); \
}

#define SEND_REQUEST_UVD_TABLE_PACKET(Code,TableNo,full_data,entry_data)	{ \
	REQUEST_UVD_TABLE_PACKET(Code,TableNo,full_data,entry_data); \
	TCP_WRITE_PACKET; \
}

// 데이터를 요청하는 Packet인지 확인
#define IS_REQUEST_PACKET(buffer) \
	( ((UVD_PACKET*)buffer)->nPacketLength < sizeof(UVD_PACKET) + ((UVD_PACKET*)buffer)->nSlotSize )

// 받은 Packet을 처리
//		buffer:			받은 packet, 응답 요구 packet이면 응답 packet으로 재조립됨
//		full_data:		packet_code에 해당하는 data 변수
//		response:		응답 packet으로 재조립되면 1로 셋팅됨
#define USE_UVD_PACKET(buffer,full_data)	\
		memcpy(((unsigned char *)&full_data) + ((UVD_PACKET*)buffer)->nSlotOffset, \
				((UVD_PACKET*)buffer)->Slot, \
				((UVD_PACKET*)buffer)->nSlotSize);

// [ACK](Acknowledge) Packet인지 확인
#define IS_ACK_PACKET(buffer) \
	( ((UVD_PACKET*)buffer)->nPacketLength == sizeof(UVD_PACKET) && ((UVD_PACKET*)buffer)->nSlotSize == 0)

#define RESPONSE_UVD_PACKET(full_data,ReqHeader)	{ \
	char *buffer = pPacketBuff; \
	((UVD_PACKET *)buffer)->nPacketLength = sizeof(UVD_PACKET) + ((UVD_PACKET*)ReqHeader)->nSlotSize; \
	((UVD_PACKET *)buffer)->nDeviceID = (PACKET_DEVICE_ID_UVD_PACKET); \
	((UVD_PACKET *)buffer)->nPacketCode = ((UVD_PACKET*)ReqHeader)->nPacketCode; \
	((UVD_PACKET *)buffer)->nEventIndex = 0;	\
	((UVD_PACKET *)buffer)->nSlotOffset = ((UVD_PACKET*)ReqHeader)->nSlotOffset; \
	((UVD_PACKET *)buffer)->nSlotSize = ((UVD_PACKET*)ReqHeader)->nSlotSize; \
	memcpy( &((UVD_PACKET*)buffer)->Slot, \
		((unsigned char *)&full_data) + ((UVD_PACKET*)buffer)->nSlotOffset, \
		((UVD_PACKET*)ReqHeader)->nSlotSize ); \
}

#define SEND_RESPONSE_UVD_PACKET(full_data,ReqHeader)	{ \
	RESPONSE_UVD_PACKET(full_data,ReqHeader); \
	TCP_WRITE_PACKET; \
}

#define RESPONSE_UVD_TABLE_PACKET(TableNo,full_data,ReqHeader)	{ \
	char *buffer = pPacketBuff; \
	((UVD_PACKET *)buffer)->nPacketLength = sizeof(UVD_PACKET) + ((UVD_PACKET*)ReqHeader)->nSlotSize; \
    ((UVD_PACKET *)buffer)->nDeviceID = (PACKET_DEVICE_ID_UVD_PACKET); \
	((UVD_PACKET *)buffer)->nPacketCode = ((UVD_PACKET*)ReqHeader)->nPacketCode; \
	((UVD_PACKET *)buffer)->nEventIndex = TableNo;	\
	((UVD_PACKET *)buffer)->nSlotOffset = ((UVD_PACKET*)ReqHeader)->nSlotOffset; \
	((UVD_PACKET *)buffer)->nSlotSize = ((UVD_PACKET*)ReqHeader)->nSlotSize; \
	memcpy( &((UVD_PACKET*)buffer)->Slot, \
		((unsigned char *)&full_data) + ((UVD_PACKET*)buffer)->nSlotOffset, \
		((UVD_PACKET*)ReqHeader)->nSlotSize ); \
}

#define SEND_RESPONSE_UVD_TABLE_PACKET(TableNo,full_data,ReqHeader)	{ \
	RESPONSE_UVD_TABLE_PACKET(TableNo,full_data,ReqHeader); \
	TCP_WRITE_PACKET; \
}
		
#define ACK_UVD_PACKET(ReqHeader)	{ \
	char *buffer = pPacketBuff; \
	((UVD_PACKET *)buffer)->nPacketLength = sizeof(UVD_PACKET); \
	((UVD_PACKET *)buffer)->nDeviceID = (PACKET_DEVICE_ID_UVD_PACKET); \
	((UVD_PACKET *)buffer)->nPacketCode = ((UVD_PACKET*)ReqHeader)->nPacketCode; \
	((UVD_PACKET *)buffer)->nEventIndex = ((UVD_PACKET*)ReqHeader)->nEventIndex; \
	((UVD_PACKET *)buffer)->nSlotOffset = ((UVD_PACKET*)ReqHeader)->nSlotOffset; \
	((UVD_PACKET *)buffer)->nSlotSize = 0; \
}

#define SEND_ACK_UVD_PACKET(ReqHeader)	{ \
	ACK_UVD_PACKET(ReqHeader);\
	TCP_WRITE_PACKET;\
}


#pragma pack(push,1)

#pragma pack(pop)

#endif /* PROTOCOL_CHROZEN_UVD_H_ */
