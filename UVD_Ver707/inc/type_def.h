#ifndef TYPE_DEF_H_
#define TYPE_DEF_H_


#if __GNUC__
#define BEGIN_PACK_STRUCT
#elif __ICCAVR32__
#define BEGIN_PACK_STRUCT _Pragma("pack(1)")
#endif

#if __GNUC__
//#define PACK_STRUCT __attribute__((packed))	//(-/+) 210705 CJKIM, 주석 처리 pack 처리 시 Clarity와 Protocol이 틀어짐 사용하지 말것.
#define PACK_STRUCT

#elif __ICCAVR32__
#define PACK_STRUCT
#endif

#if __GNUC__
#define END_PACK_STRUCT
#elif __ICCAVR32__
#define END_PACK_STRUCT _Pragma("pack()")
#endif


typedef uint8_t BYTE;
typedef uint16_t WORD;
//typedef uint32_t BOOL;
typedef uint8_t BOOL;

// System 사용
#if 1

#define CNT_5HZ_TO_MIN		300.0f			// 5hz * 60
#define CNT_1HZ_TO_MIN		60.0f			// 1hz * 60
#define SEC_TO_MIN				60.0f

//define CNT_TO_MIN		60.0f
#define RUN_EXCUTE_COUNT_5Hz			10  // 50hz / 10 => 5Hz
#define RUN_EXCUTE_COUNT_1Hz			5	// 5Hz / 5 => 1Hz

#endif

/*
typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;
*/
/*
union utData {
	uint32_t	ulData;
	uint8_t		ucData[4];
	uint16_t	uiData[2];
};

union utData16 {
	uint8_t		ucData[1];
	uint16_t	uiData;
};
*/
#endif	// #ifndef TYPE_DEF_H_

