#ifndef	_CHROGEN_LC_UVD_UV_INIT_FUNC_H_
#define	_CHROGEN_LC_UVD_UV_INIT_FUNC_H_

#include "UV_Struct.h"

#if 1
void UV_Hard_Init(void);
void InitValue(void);
void defaultSetting(void);
void DiagPowerCheck(void);
void Initialize(void);
char Diagnostic(unsigned char option);
char FindWave(unsigned short startStep, unsigned short endStep, Ads1252_Cmd_t *photoData, bool scanEnd);

void calParaset(void);
char calPara(void);
void dispaly_wave_motor_step(void);	//(+)210804 CJKIM, added
char saveWave(void);
float CalFindWave(unsigned short sFindmin, unsigned short sFindmax, unsigned long wavePeak);

char Calibrate(void);
char BacklashTest(void);
char wLampTest(void);
char cutFilterTest(void);
char DaAdTest(void);
char holmiumTest(void);
char noiseDriftTest(void);
char readyInitSet(void);

int motorInitSet(unsigned short sWaveA, unsigned short sWaveB, unsigned char cutFilter);
int D2lampCon(unsigned char d2lamp);
void WlampCon(unsigned char wlamp);
void cutFilterSelection(unsigned char cutFilter);
int remoteSetup(unsigned char markOutDefault, unsigned short nExtoutTime);
int remoteAction(unsigned char startOut, unsigned char readyOut, unsigned char markOut);
int readyOnOff(bool cmd);
void remoteChkEn(bool cmd);
char ErrorProcess(int err);
#endif

#endif	//	_CHROGEN_LC_UVD_UV_INIT_FUNC_H_
