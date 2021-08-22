/*
 * util.c
 *
 *  Created on: 2015. 4. 8.
 *      Author: chanii
 */
#include <stdio.h>

#include "config.h"

volatile uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */


extern volatile uint8_t TimeEvent;

void inc_local_time()
{
	LocalTime++;
}

void set_local_time(volatile uint32_t time)
{
	LocalTime = time;
}

uint32_t get_mili_local_time()
{
	return (LocalTime >> 3);
}


// 500nS
void usr_delay(unsigned int time)
{
	uint32_t i;
	for(i=0;i<time;i++)
	{
		__asm volatile ("nop");
	}
}

// Do not use this function as possible -> performance decrease
void delay_ms(unsigned int time)
{
	uint32_t i;
	for(i=0;i<time;i++)
	{
		usr_delay(2000);		// 500nS
	}
}

unsigned char hex2char_conv(unsigned char ch)
{
  unsigned char ret = 0;
  unsigned char va; // clear High Nibble

  va = ch&0x0F;

  if((va>=0x0)&&(va<=0x9)) ret=(0x30 + va);
  else if(va==0xa) ret=0x41;
  else if(va==0xb) ret=0x42;
  else if(va==0xc) ret=0x43;
  else if(va==0xd) ret=0x44;
  else if(va==0xe) ret=0x45;
  else if(va==0xf) ret=0x46;

  return ret;
}

unsigned char char2hex_conv(unsigned char ch)
{
  unsigned char ret=0;

  if((ch>=0x30)&&(ch<=0x39)) ret=ch&0xF;
  else if(ch==0x41) ret=0x0A;
  else if(ch==0x42) ret=0x0B;
  else if(ch==0x43) ret=0x0C;
  else if(ch==0x44) ret=0x0D;
  else if(ch==0x45) ret=0x0E;
  else if(ch==0x46) ret=0x0F;
  else if(ch==0x61) ret=0x0A;
  else if(ch==0x62) ret=0x0B;
  else if(ch==0x63) ret=0x0C;
  else if(ch==0x64) ret=0x0D;
  else if(ch==0x65) ret=0x0E;
  else if(ch==0x66) ret=0x0F;

  return ret;
}



void PrintLine(void)
{
  printf("-----------------------------------------------------\r\n");
}

// data should have 32 Byte area
void ShowMemory(unsigned char Header, unsigned char * data, unsigned int Length)
{
	unsigned int i;

    if(Header==1) // Display HEX for Length with header
    {
        printf("----------------- Display Memory --------------------\r\n");
        printf("NUM : 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F \r\n");
        PrintLine();

       	for(i=0;i<Length;i++)
        {
            if((i%0x10)==0) printf("%03X : ",(i>>4));
            printf("%02X ",data[i]);
            if((i%0x10)==0xF) printf("\r\n");
        }
       	printf("\r\n");PrintLine();printf("\r\n");
    }
    else if(Header==2) // Display ASCII for Length
    {
        printf("\r\n");
       	for(i=0;i<Length;i++)
        {
            if((i%0x10)==0) printf("%03X : ",(i>>4));
            printf("%02X ",data[i]);
            if((i%0x10)==0xF) printf("\r\n");
        }
        printf("\r\n");
    }
    else
    {
		for(i=0;i<Length;i++)
		{
			printf("%02X ",data[i]);
			if((i%0x10)==0xF) printf("\r\n");
		}
		printf("\r\n");
    }

}


// data should have 32 Byte area
void ShowMemoryWord(uint32_t * data, uint8_t Len)
{
	uint8_t i;
//	printf("Memory contents ....\r\n");
	for(i=0;i<Len;i++)
//	for(i=12;i<Len;i++)
	{
		if((i&0x3)==0) printf("%03X : ",(i>>2));
		printf("%08lX ",data[i]);
		if((i&0x3)==1){}
		if((i&0x3)==2){}
		if((i&0x3)==3) printf("\r\n");
	}
	printf("\r\n");
}

uint8_t Intro(void)
{
	uint8_t MasterSlave;
	
	printf("\n=================================================\r\n");
	printf("SF2 - Youngin Chromass Instruments Co., Ltd.\r\n");	
	printf("               Main Function\r\n");
	printf("-------------------------------------------------\r\n");
	printf("          Build Time : ["__DATE__"] ["__TIME__"]\r\n");		
	printf("-------------------------------------------------\r\n");
	printf("          K.S.Yi  20210421  \r\n");
	printf("-------------------------------------------------\r\n");	
	printf("......... Program Start !!  \r\n");
	printf("=================================================\r\n\n");	

	
	MasterSlave = GCC_MASTER;

	return MasterSlave;
}






