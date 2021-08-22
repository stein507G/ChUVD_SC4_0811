/*
 * UV_Main.h
 *
 *  Created on: 2020. 6. 28.
 *      Author: ksyi
 */
#ifndef __UV_MAIN_H_
#define __UV_MAIN_H_

#define		MAX_GM_WAVE_COUNT		1000

volatile Det_Motor_Con_t detMotor;
volatile Det_Motor_State_t detMotorState;

Det_Acq_data_t	acqA[DET_TIME_CONSTANT_ACQ_ARRAY];
//Det_Acq_data_t	acqB[DET_TIME_CONSTANT_ACQ_ARRAY];

//Det_AZ_AO_data_t azao[AUTO_ZERO_OFFSET_DATA_NUM];


unsigned short GM_wave[MAX_GM_WAVE_COUNT];

Det_Setup_t detSet;
Det_Diagnosis_t detDiag;

Event_Control_t eventCtrl;

Remote_State_t remote;
Gpio_RemoteSet_t remoteSet;	// 디바이스 드라이버에 설정값을 전달

Gpio_RemoteOut_t rmtout;		// 디바이스 드라이버에 설정값을 전달 - remote out
Gpio_RemoteIn_t rmtin;
Gpio_RemoteChk_t rmtchk;

Ads1252_Cmd_t Ads1252;
Ads1252_Cmd_t detAds;

Det_Dadata_t detDa;

UVD_LampTime_Check LampT;
UVD_Wave_Check dWave;


void ADC_Volt_Read(void);

void photoDataRead();
void Ready_PowBtn_Check();

void signalOut();
void DAout();

void FLED_Ctrl(char FLED_Type);

void filterMotorHome();
void filterMotorGoPos();

void gratingMotorHome();
void gratingMotorGo(bool dir, unsigned int step);
void gratingMotorGoPos(unsigned int step);
bool gratingMotorGoPosChk();
void gratingMotorGoWave(unsigned int wLength);

void LampTimeCheck();
int stateCheck();

void remoteInfn();
void stateActFunc();
void startFunc();
void stopFunc();

void programMode();
void eventAction();
void eventExecution();

int diagFunc();

#endif
