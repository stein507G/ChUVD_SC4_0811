/*
 * apc_if.c
 *
 *  Created on: 2017. 9. 12.
 *      Author: Han
 */

#include "config.h"

#define SDLC_BASE_ADDR	0x51000000

uint32_t * pTxMem = (uint32_t *)(0x50008000);	// 0x2000 (32bit) -- 0010 0000 0000 0000 << 2  --> 1000 0000 0000 0000 (0x8000) --> 0x50008000
uint32_t * pRxMem = (uint32_t *)(0x50009000);	// 0x2400 (32bit) -- 0010 0100 0000 0000 << 2  --> 1001 0000 0000 0000 (0x9000) --> 0x50009000

uint8_t * pTxMem_byte = (uint8_t *)(0x50008000);	// 0x2000 (32bit) -- 0010 0000 0000 0000 << 2  --> 1000 0000 0000 0000 (0x8000) --> 0x50008000
uint8_t * pRxMem_byte = (uint8_t *)(0x50009000);	// 0x2400 (32bit) -- 0010 0100 0000 0000 << 2  --> 1001 0000 0000 0000 (0x9000) --> 0x50009000

//static int totalCntI=0;
//static int totalCntD=0;


#define APC_RS422_PACKET_FUNCTION_COUNT		(rs422_MAX_PACKET_NO + 1)

void (*pfAPC_Rs422_Packet[APC_RS422_PACKET_FUNCTION_COUNT])(uint32_t slvid_addr);

volatile APC_ST apcif;		//(+)210603 CJKIM, volatile keyword added

extern GCSVC_ST gcsvc;
uint8_t pattern=0;

void apc_APCSend(uint8_t dest_addr, uint8_t len, uint8_t CmdRespCode, uint32_t * Packet);

void apc_RS422_init(void)
{
	//uint8_t i=0,j=0;
	
	apcif.RunMode = APC_RUN_CPUMODE;
	apcif.State = APC_STATE_RUN;
	apcif.RxAllAllow = 0;
	
	apcif.TSTAT = 1;	// 처음엔 완료로 셋팅


}

void apc_CheckTxState(void)
{
	uint8_t TSTAT;
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		DEBUG_I(DBG_LEVEL_INFO, "() TSTAT %02X\r\n", TSTAT);	
		FPGA_WR_APC_TX_FLAG_CLEAR;
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//TSTAT;
	}
	//TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);		
	DEBUG_I(DBG_LEVEL_INFO, "() TSTAT %02X\r\n",TSTAT);	
}

void apc_CheckRxState(void)
{
	uint8_t RSTAT;
	uint32_t rxinfo;
	uint8_t rxid;
	uint8_t slvid;
	uint8_t rxlen;
	uint8_t rxcmd;

	RSTAT = FPGA_READ_WORD(APC_RS422_RX_STATE_ADDR);

	if(RSTAT != apcif.RSTAT)
	{
		apcif.RSTAT = RSTAT;
	}

	if(RSTAT&0x80)
	{
//		FPGA_WRITE_WORD(APC_RS422_RX_FLAG_RESET_ADDR,1); // flag clear
		FPGA_WR_APC_RX_FLAG_CLEAR;		// flag clear
		rxinfo = FPGA_READ_WORD(APC_RS422_RX_INFO_ADDR);
		rxid  = rxinfo; rxinfo = rxinfo>>8;
		rxlen = rxinfo; rxinfo = rxinfo>>8;
		rxcmd = rxinfo;
		slvid = rxid&0xF;

		// master
		DEBUG_I(DBG_LEVEL_INFO, "() [RX]ID:%02X,LEN:%02X,CMD:%02X,RSTAT:%02X,D[0]:%08lX\r\n",
			rxid,rxlen,rxcmd,RSTAT, FPGA_READ_WORD(0x2401 + (APC_CONFIG_STASUS_SIZE*(slvid-1))));
		ShowMemoryWord((pRxMem + (APC_CONFIG_STASUS_SIZE*(slvid-1))), rxlen);

	}
}

//================================================================================
//================================================================================
// This function may be called every 1 MS
void apc_process(void)
{
	if(apcif.State == APC_STATE_RUN)
	{
		if(GCC_MASTER == gcsvc.MasterSlave)
		{
			if(apcif.RunMode == APC_RUN_AUTOMODE)
			{
				if(apcif.AutoEnd == 1)
				{
					apcif.AutoEnd = 0;
				}
			}
			else
			{
				apc_CheckRxState();
			}
		}
		else
		{
			apc_CheckRxState();
		}

		apc_CheckTxState();

	}

}

void apc_APCSendStaus(uint8_t dest_addr)
{
	apc_APCSend(dest_addr, APC_CONFIG_STASUS_SIZE, APC_CMD_CON_STATUS, (uint32_t *)apcif.MainStatus);
}

void apc_APCSendStausAuto(void)
{
	apcif.slvch++;
	if(apcif.slvch >= APC_SLAVE_NUMBER) apcif.slvch=0;
	apc_APCSendStaus(apcif.slvch+1);
}

void apc_APCSendConfig(uint8_t dest_addr)
{
	uint8_t CmdRespCode=0;
	uint8_t DestinationAddr=0;

	if(GCC_MASTER == gcsvc.MasterSlave) // Configuration Set Command
	{
		if((dest_addr == 0)||(dest_addr > 9)) return;

		if(dest_addr < 4)  		CmdRespCode =	rs422_APC_CMD_INLET_CONFIG;
		else if(dest_addr == 4)  CmdRespCode =	rs422_APC_CMD_DET_CONFIG_1;
		else if(dest_addr == 5) CmdRespCode =	rs422_APC_CMD_AUX_SETTING_1;

		DestinationAddr = dest_addr;
		apc_APCSend(DestinationAddr, 63, CmdRespCode, (uint32_t *)apcif.Config[DestinationAddr-1]);
		
	}
	else // Configuration Response
	{
		if(gcsvc.ApcMyId < 4)  		CmdRespCode =	rs422_APC_CMD_INLET_CONFIG;
		else if(gcsvc.ApcMyId == 4)  CmdRespCode =	rs422_APC_CMD_DET_CONFIG_1;
		else if(gcsvc.ApcMyId == 5) CmdRespCode =	rs422_APC_CMD_AUX_SETTING_1;
 
		//apc_APCSend(0xE,APC_CONFIG_STASUS_SIZE,CmdRespCode, apcif.Config[gcsvc.ApcMyId]);
		apc_APCSend(0xE, 63, CmdRespCode, (uint32_t *)apcif.Config[gcsvc.ApcMyId - 1]);

	}
}

// this function is available in Slave mode
void apc_APCConfigSendBack(void)
{
	uint8_t CmdRespCode=0;
	uint8_t i;
	uint8_t dest_addr = 0xE;

	if(GCC_SLAVE == gcsvc.MasterSlave)
	{
		if(gcsvc.ApcMyId < 4)  		CmdRespCode =	APC_CMD_INLET_SET;
		else if(gcsvc.ApcMyId < 7)  CmdRespCode =	APC_CMD_DET_SET;
		else if(gcsvc.ApcMyId < 10) CmdRespCode =	APC_CMD_AUX_SET;

		pTxMem[0] = CmdRespCode;

		for(i=0;i<APC_CONFIG_STASUS_SIZE;i++)
			pTxMem[1+i] =  pRxMem[1+i];
		FPGA_WR_APC_TX_LENGTH(APC_CONFIG_STASUS_SIZE+1); 	// tx_len
		FPGA_WR_APC_SEND_TRIG(dest_addr);// tx_start
	}
}


void rs422_Test()
{

	uint8_t pBuffer[100];
	int i=0;
	//	pBuffer = (pTxMem + 1);
	
		for(i=0;i<48;i++) {
			pBuffer[i] = i;
		}


	pTxMem[0] = AUTO_MODE_CMD;	
	pTxMem[1] = rs422_APC_CMD_MAIN_STATE;

//	memcpy((uint8_t *)(pTxMem+2),&RS422_Main_State,sizeof(RS422_Main_State));

	memcpy((&pTxMem[1]), &pBuffer[0], 44);
	//memcpy((&pBuffer[0]), &apcif, 48);
	
	//		memcpy((void *)(&pTxMem[1]), (const void*)&pBuffer[0], size);
	//			memcpy((pTxMem + 1), (const void*)&bf->Data[bf->Head], size);
	
//	pTxMem[1] = 0x12345678;//bf->Data[bf->Head];
	
	for(i=0;i<48;i++) {
	//pTxMem[i+1] = 0x12345678;//bf->Data[bf->Head];
	iprintf("UVD 555 [%d]%x %x\n",i,pTxMem[i], pTxMem_byte[i]);
	
	}
	iprintf("===========================================================\n");
}



