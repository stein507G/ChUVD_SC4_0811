/*
 * test.h
 *
 *  Created on: 2017. 2. 8.
 *      Author: chanii
 */

#ifndef TEST_H_
#define TEST_H_

#include "type_def.h"

#define ADC_TEST_MODE	0x01
#define APC_TEST_MODE	0x02
#define DET_TEST_MODE	0x03
#define VALV_TEST_MODE	0x04
#define KEY_TEST_MODE	0x05
#define DA_TEST_MODE	0x06

BEGIN_PACK_STRUCT

typedef struct
{
	uint16_t	timer;
	uint8_t		mode; // test mode
	uint8_t		auto_test; //
	uint16_t	i;
	uint16_t	j;
	uint8_t 	Dbgen;
} PACK_STRUCT FTEST_ST; //

END_PACK_STRUCT

void Function_test(uint8_t Cmd);
void TestModeSetting(void);
void FunctionTest_process(void);

#endif /* TEST_H_ */
