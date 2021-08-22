/*
 * util.h
 *
 *  Created on: 2015. 5. 21.
 *      Author: chanii
 */

#ifndef UTIL_H_
#define UTIL_H_

enum {
	DBG_LEVEL_NONE		= 0x000000000,
	DBG_LEVEL_WARN		= 0x000000001,
	DBG_LEVEL_INFO	    = 0x000000002,
	DBG_LEVEL_DEBUG		= 0x000000004,
};


/*----------------------------------------------------------*\
 | Buffer Size                                              |
\*----------------------------------------------------------*/
#define TBUF_SIZE   	256	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   	256      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define CUART_RX_TIME	3		// milliseconds


/*--------------------------------------------*\
 | Buffer                                     |
\*--------------------------------------------*/
struct buf_st {
  unsigned int in;    			// Next In Index
  unsigned int out;         	// Next Out Index
  unsigned char buf[RBUF_SIZE]; // Buffer
};

void inc_local_time();	//(+)210701 CJKIM, added
void set_local_time(volatile uint32_t time);	//(+)210701 CJKIM, added
uint32_t get_mili_local_time();	//(+)210701 CJKIM, added


void usr_delay(unsigned int time);
// Do not use this function as possible -> performance decrease
void delay_ms(unsigned int time);
unsigned char hex2char_conv(unsigned char ch);
unsigned char char2hex_conv(unsigned char ch);
void PrintLine(void);
void ShowMemory(unsigned char Header, unsigned char * data, unsigned int Length);
void ShowMemoryWord(uint32_t * data, uint8_t Len);

uint8_t Intro(void);


	
/*
(+)210702 CJKIM, __YI_CM_DEBUG__ 
	- Softconsol Properties > GNU C Compiler Symbols
	- Debug Mode에서만 적용됨.
*/
#ifdef __YI_CM_DEBUG__

#define DEBUG_I(T,fmt,args...) printf("INFO. %s",__func__); printf(fmt, ##args);
#define DEBUG_E(fmt,args...)	printf("ERROR. %s",__func__); printf(fmt, ##args);


#else
#define DBG_LVL_I(T,fmt,args...)  
#define DBG_LVL_W(T,fmt,args...)  

#define DEBUG_I(T,fmt,args...)  
#define DEBUG_W(T,fmt,args...)  
#define DEBUG_E(fmt,args...)	

#endif

#endif /* UTIL_H_ */
