/*
 * UV_Main.cpp
 *
 *  Created on: 2020. 8. 4.
 *      Author: ksyi
 */

#include "config.h"
#include <math.h>
#include "stdlib.h"
#include "lwIP/err.h"


extern char AD_Disp;
char LEDStateChange = 0;

extern volatile char Clarity_Connect;	// CJKIM

#ifdef FIX_SAM_REF_OFFSET
	float fEnergyOffsetDummy;
#endif

union ujData {
	unsigned long	ulData;		
//	unsigned char	ucData[4];
//	unsigned int	uiData[2];
};

union ujData utRefData;
union ujData utSamData;

unsigned long *ref={0};
unsigned long *sam={0};

unsigned long *refA={0};
unsigned long *samA={0};

unsigned long *refB={0};
unsigned long *samB={0};

extern struct tcp_server_struct *g_es;

#if !READ_ADC_INTR //(-/+) 210611 CJKIM, 인터럽트 루틴 에서는 ADC 값만 읽고 처리는 main 루프에서
volatile uint32_t g_Adc1Data = 0;		// ADC Reference
volatile uint32_t g_Adc2Data = 0;		// ADC Sample
volatile uint32_t g_Adc3Data = 0;		// ADC Volt , 24V / 5V / 3.3V / Tung / X / DA1 / DA2 / Temp
#endif

//(+)210716 CJKIM, re-init uvd state when network disconnecting
void comm_disconnected_uvd_init()
{
	detStt.runstop = RUN_STOP;
}

void init_adc_value()
{


	rDataSum=0;
	memset(rDataBuf, 0x00, DET_PHOTO_DATA_AVE);
	refData = 0;
	
	sDataSum=0;
	memset(sDataBuf, 0x00, DET_PHOTO_DATA_AVE);
	samData = 0;
	
	rDualSumA=0;
	memset(rDualBufA, 0x00, DET_DUAL_PHOTO_DATA_AVE);
	sDualSumA=0;
	memset(sDualBufA, 0x00, DET_DUAL_PHOTO_DATA_AVE);
	
	rDualSumB=0;
	memset(rDualBufB, 0x00, DET_DUAL_PHOTO_DATA_AVE);
	sDualSumB=0;
	memset(sDualBufB, 0x00, DET_DUAL_PHOTO_DATA_AVE);

	DEBUG_I(DBG_LEVEL_INFO, "()\r\n");

}

void ADC_Read(void)	// 500Hz
{
	//static unsigned char i=0;
	static unsigned char index=0;
	static unsigned char dualindexA=0;
	static unsigned char dualindexB=0;
	//static int Cnt = 0;
	uint32_t Data1 = 0;
	uint32_t Data2 = 0;
	//static uint32_t Data3 = 0;

	//static char Volt_Cnt = 0;	// Mux 0~7
	//static char Volt_Sum_Cnt = 0;	// 8번 ave
	//static uint32_t Volt_Sum[8] = {0};

	//int M1_read_test = 0;
	//int M2_read_test = 0;

#if READ_ADC_INTR	
	Data1 = AD1_READ;
	Data2 = AD2_READ;	
#else	//(-/+) 210611 CJKIM, 인터럽트 루틴의 읽은 ADC 전역변수 사용
	//NVIC_DisableIRQ( FabricIrq5_IRQn );		//(+)210723 CJKIM, added
	Data1 = g_Adc1Data;
	Data2 = g_Adc2Data;	
	//NVIC_EnableIRQ( FabricIrq5_IRQn );		//(+)210723 CJKIM, added
#endif

//		static unsigned char oldMotorMode = -1;
//	
//		if (oldMotorMode != detMotor.btMode) {\
//			DEBUG_I(DBG_LEVEL_INFO, "() Mode[%d, %s]\r\n", detMotor.btMode, motor_mode_str(detMotor.btMode));
//			oldMotorMode = detMotor.btMode;
//		}
	
	switch(detMotor.btMode) {
		default:
			
#if 0 	//(-/+) 210611 CJKIM, 인터럽트 루틴의 읽은 ADC 전역변수 사용
			Data1 = AD1_READ;
			Data2 = AD2_READ;	
#endif
			utRefData.ulData = Data1 >> 5;
			utSamData.ulData = Data2 >> 5;

			rDataSum -= rDataBuf[index];
			rDataBuf[index] = utRefData.ulData;

			rDataSum += rDataBuf[index];

			detAds.refD = rDataSum; // refData

			sDataSum -= sDataBuf[index];
			sDataBuf[index] = utSamData.ulData;

			sDataSum += sDataBuf[index];
			detAds.samD = sDataSum; // samData
			if(++index == DET_PHOTO_DATA_AVE) {
				index =0;
			}

			break;
		case GM_DUAL_ACT:
			if(detMotorState.bReadable) {
				if(detMotor.bDualPos) { // 파장 B == 1
//					refB[2]=rdDET_REF_DATA_HIGH;		refB[1]=rdDET_REF_DATA_MID; 	refB[0]=rdDET_REF_DATA_LOW;
//					samB[2]=rdDET_SAM_DATA_HIGH;		samB[1]=rdDET_SAM_DATA_MID; 	samB[0]=rdDET_SAM_DATA_LOW;

#if 0 	//(-/+) 210611 CJKIM, 인터럽트 루틴의 읽은 ADC 전역변수 사용
					Data1 = AD1_READ;
					Data2 = AD2_READ;	
#endif
					refB = (Data1 >> 5);
					samB = (Data2 >> 5);

					detAds.who = 1; //
					rDualSumB -= rDualBufB[dualindexB];
					refData = (unsigned long*)(refB);
					rDualBufB[dualindexB] = *refData;

					rDualSumB += rDualBufB[dualindexB];

					detAds.refD = rDualSumB;	// refData

					sDualSumB -= sDualBufB[dualindexB];
					samData = (unsigned long*)(samB);
					sDualBufB[dualindexB] = *samData;

					sDualSumB += sDualBufB[dualindexB];
					detAds.samD = sDualSumB;	// samData
					if(++dualindexB == DET_PHOTO_DATA_AVE) {
						dualindexB =0;
					}
				}
				else {
//					refA[2]=rdDET_REF_DATA_HIGH;		refA[1]=rdDET_REF_DATA_MID; 	refA[0]=rdDET_REF_DATA_LOW;
//					samA[2]=rdDET_SAM_DATA_HIGH;		samA[1]=rdDET_SAM_DATA_MID; 	samA[0]=rdDET_SAM_DATA_LOW;

#if 0 	//(-/+) 210611 CJKIM, 인터럽트 루틴의 읽은 ADC 전역변수 사용
					Data1 = AD1_READ;
					Data2 = AD2_READ;	
#endif
					refA = (Data1 >> 5);
					samA = (Data2 >> 5);

					detAds.who = 0; //
					rDualSumA -= rDualBufA[dualindexA];
					refData = (unsigned long*)(refA);	// refA
					rDualBufA[dualindexA] = *refData;

					rDualSumA += rDualBufA[dualindexA];

					detAds.refD = rDualSumA;	// refData

					sDualSumA -= sDualBufA[dualindexA];
					samData = (unsigned long*)(samA);	// SamA
					sDualBufA[dualindexA] = *samData;

					sDualSumA += sDualBufA[dualindexA];
					detAds.samD = sDualSumA;	// samData
					if(++dualindexA == DET_PHOTO_DATA_AVE) {
						dualindexA =0;
					}
				}
			}
			break;
		case GM_SCAN_ACT:
			if(detMotorState.bReadable) {
//				ref[2]=rdDET_REF_DATA_HIGH; 	ref[1]=rdDET_REF_DATA_MID;		ref[0]=rdDET_REF_DATA_LOW;
//				sam[2]=rdDET_SAM_DATA_HIGH; 	sam[1]=rdDET_SAM_DATA_MID;		sam[0]=rdDET_SAM_DATA_LOW;

#if 0 	//(-/+) 210611 CJKIM, 인터럽트 루틴의 읽은 ADC 전역변수 사용
				Data1 = AD1_READ;
				Data2 = AD2_READ;	
#endif
				ref = (Data1 >> 5);
				sam = (Data2 >> 5);
			}
			break;
	}

	remoteCheck();
}

void ADC_Volt_Read()	// 500Hz
{
	uint32_t Volt_Data = 0x0;
	uint8_t Volt_Sel = 0;
	static unsigned int Volt_Read[8] = {0};
	static char Cnt_5Hz = 0;
	static float Volt_Check[8] = {0};
	float Volt_Ref[8] = { 24, 5, 3, 5, 0, 1, 1, 1.2};	// 24V / 5V / 3.3V / Tung / X / DA1 / DA2 / Temp
	float minPwr[8]	= {22, 4.5, 3.1, 0, 0, 0, 0, 0};
	float maxPwr[8]	= {26, 5.5, 3.5, 0, 0, 0, 0, 0};

	if(++Cnt_5Hz == 100)
	{
		Cnt_5Hz = 0;

#if READ_ADC_INTR	
		Volt_Data = AD3_READ;
#else	//(-/+) 210611 CJKIM, 인터럽트 루틴의 읽은 ADC 전역변수 사용	
		//NVIC_DisableIRQ( FabricIrq5_IRQn ); 	//(+)210723 CJKIM, added
		Volt_Data = g_Adc3Data;
		//NVIC_EnableIRQ( FabricIrq5_IRQn );		//(+)210723 CJKIM, added

#endif		
		Volt_Sel = Volt_Data & 0x7;

		switch(Volt_Sel)
		{
			case ADC_24V:		// 24V
				Volt_Read[ADC_24V] = (Volt_Data >> 4) >> 4;	// sum/16
				detDiag.sysVolt[ADC_24V] = Volt_Check[ADC_24V] = ((float)Volt_Read[ADC_24V]/ADC_FULL_COUNT) * ADC_REF_VOLTAGE * Volt_Ref[ADC_24V];
				if(detDiag.sysVolt[ADC_24V] > minPwr[ADC_24V] && detDiag.sysVolt[ADC_24V] < maxPwr[ADC_24V]) detDiag.checkVolt[ADC_24V] = OK;

				break;
			case ADC_5V:		// 5V
				Volt_Read[ADC_5V] = (Volt_Data >> 4) >> 4;	// sum/16
				detDiag.sysVolt[ADC_5V] = Volt_Check[ADC_5V] = ((float)Volt_Read[1]/ADC_FULL_COUNT) * ADC_REF_VOLTAGE * Volt_Ref[ADC_5V];
				if(detDiag.sysVolt[ADC_5V] > minPwr[ADC_5V] && detDiag.sysVolt[ADC_5V] < maxPwr[ADC_5V]) detDiag.checkVolt[ADC_5V] = OK;
				break;
			case ADC_3_3V:		// 3.3V
				Volt_Read[ADC_3_3V] = (Volt_Data >> 4) >> 4;	// sum/16
				detDiag.sysVolt[ADC_3_3V] = Volt_Check[ADC_3_3V] = ((float)Volt_Read[ADC_3_3V]/ADC_FULL_COUNT) * ADC_REF_VOLTAGE * Volt_Ref[ADC_3_3V];
				if(detDiag.sysVolt[ADC_3_3V] > minPwr[ADC_3_3V] && detDiag.sysVolt[ADC_3_3V] < maxPwr[ADC_3_3V]) detDiag.checkVolt[ADC_3_3V] = OK;
				break;
			case ADC_TUNGSTEN:		// tung 0V/5V
				Volt_Read[ADC_TUNGSTEN] = (Volt_Data >> 4) >> 4;	// sum/16
				detDiag.sysVolt[ADC_TUNGSTEN] = Volt_Check[ADC_TUNGSTEN] = ((float)Volt_Read[ADC_TUNGSTEN]/ADC_FULL_COUNT) * ADC_REF_VOLTAGE * Volt_Ref[ADC_TUNGSTEN];
				if(detDiag.sysVolt[ADC_TUNGSTEN] > minPwr[ADC_TUNGSTEN] && detDiag.sysVolt[ADC_TUNGSTEN] < maxPwr[ADC_TUNGSTEN]) detDiag.checkVolt[ADC_TUNGSTEN] = OK;
				break;
			case ADC_UNUSED:		// UNUSED
				break;
			case ADC_DA1:		// DA1
				break;
			case ADC_DA2:		// DA2
				break;
			case ADC_TEMP:		// TEMP
				break;

		}
/*
		DEBUG_I(DBG_LEVEL_INFO, "() 		Sel1=%x %.3f %d		Sel2=%x  %.3f %d		Sel3=%x %.3f %d\r\n"
			, Volt_Read[0] , detDiag.sysVolt[0] , detDiag.checkVolt[0]
			, Volt_Read[1] , detDiag.sysVolt[1] , detDiag.checkVolt[1]
			, Volt_Read[2] , detDiag.sysVolt[2] , detDiag.checkVolt[2] );
*/
	}

}

volatile unsigned char g_ScanDataDone = NO;

void photoDataRead()	// ChroZen 125Hz
{
	float refData,samData;
	static float refDataSum = 0, samDataSum = 0;
	static volatile unsigned int indexA=0;	//, indexB=0;	//(-/+) 210604 CJKIM, volatile keyword
	static unsigned int indexScan=0;
	static unsigned int samplingCount=0;
	static unsigned int indexDual=0;
	static unsigned int dualSamplingCountA=0,dualSamplingCountB=0;
	static unsigned int who=0;	// 0 : wave A , 1 : wave B
	static unsigned int AZcountA = 0,AZcountB = 0;
	static unsigned int AOcountA = 0,AOcountB = 0;
	static float fAZdataA = 0.0,fAZdataB = 0.0;
	static float fAOdataA = 0.0,fAOdataB = 0.0;

	static unsigned char oldWaveMode = -1;
	static unsigned char motorOff = YES;

	memcpy(&Ads1252, &detAds, sizeof(detAds));

	refData = (float)(Ads1252.refD) - detStt.fOffsetValueRefAve;
	samData = (float)(Ads1252.samD) - detStt.fOffsetValueSamAve;

	if(refData <= ENERGY_DATA_OFFSET_ZERO) refData = ENERGY_DATA_OFFSET_ZERO;
	if(samData <= ENERGY_DATA_OFFSET_ZERO) samData = ENERGY_DATA_OFFSET_ZERO;
	if(detStt.runstop==RUN_START && detStt.nowStart == YES)
	{
		indexA = 0;
		gUVD_ACQDATA.fRunTime = 0.0;
		detStt.nowStart = NO;
	}

//		if( detStt.waveMode != oldWaveMode) {
//			DEBUG_I(DBG_LEVEL_INFO, "() [%d, %s]\r\n", detStt.waveMode, uvd_wave_mode_str(detStt.waveMode));
//			oldWaveMode = detStt.waveMode;
//		}


	switch (detStt.waveMode)
	{
		case UVD_SINGLE_WAVE_MODE:	// SINGLE
			gUVD_STATE.SignalA.fReferenceE = gUVD_STATE.SignalB.fReferenceE
				= gUVD_ACQDATA.SignalA[indexA].fReferenceE = gUVD_ACQDATA.SignalB[indexA].fReferenceE
				= refData * CONV_AD_TO_ENERGY;

			gUVD_STATE.SignalA.fSampleE = gUVD_STATE.SignalB.fSampleE
				= gUVD_ACQDATA.SignalA[indexA].fSampleE = gUVD_ACQDATA.SignalB[indexA].fSampleE
				= samData * CONV_AD_TO_ENERGY;

			detStt.acqSumA = detStt.acqSumA - acqA[detStt.iAcqA].fTrans;
			acqA[detStt.iAcqA].fTrans = samData / refData;
			detStt.acqSumA = detStt.acqSumA + acqA[detStt.iAcqA].fTrans;	// transmittance 합
			if(++detStt.iAcqA == detStt.timeConstCntA) detStt.iAcqA = 0;

			detStt.acqAverageA = detStt.acqSumA / detStt.timeConstCntA;		// transmittance 평균

			if(detStt.acqAverageA <= TRANS_ZERO_VALUE) detStt.acqAverageA = TRANS_ZERO_VALUE;

			if(detStt.bAZstartA) {// || detStt.bAutozero || detStt.bAZwaveA) {
				detStt.fAutoZeroDataA = detStt.acqAverageA;
				detStt.bAZstartA = detStt.bAutozeroA = detStt.bAZwaveA = detStt.bAOpolaA = detStt.bAOwaveA = NO;
				AZcountA = 0;
				detStt.fAutoOffsetDataA = 0.0;
			}
			else if(detStt.bAutozeroA || detStt.bAZwaveA) {
				detStt.bAutozeroA = detStt.bAZwaveA = detStt.bAOpolaA = detStt.bAOwaveA = NO;
				AZcountA = AUTO_ZERO_OFFSET_COUNT;	// 2초
				fAZdataA = 0.0;
				detStt.fAutoOffsetDataA = 0.0;
			}

			if(AZcountA > 0) {	// 0.0으로 값을 보내다가 2초후에 제로(오프셋)값을 적용한다.
				detStt.fAbsorbance[0] = fAZdataA;	// 0.0
				--AZcountA;
				detStt.fAutoZeroDataA = detStt.acqAverageA;
			}
			else {
				detStt.fAbsorbance[0] = -log10(detStt.acqAverageA / detStt.fAutoZeroDataA);
			}

			if(detStt.bAOpolaA) {	// offset은 absorbance 값을 해야 한다. old값의 2배를 더해 준다.
				if(detStt.bPolarity) {	// negative
					detStt.fAutoOffsetDataA = -gUVD_STATE.SignalA.fAbsorbance * 2;		// 밑이 + 면 위도 + / -면 -
				}
				else {
					detStt.fAutoOffsetDataA = 0.0;
				}

				detStt.bAOpolaA = detStt.bAOwaveA = NO;
				AOcountA = 0;
			}
			else if(detStt.bAOwaveA) {
				detStt.bAOwaveA = NO;
				AOcountA = AUTO_ZERO_OFFSET_COUNT;	// 2초
				if(detStt.bPolarity) {
					fAOdataA = detStt.fAutoOffsetDataOldA = gUVD_STATE.SignalA.fAbsorbance;
				}
				else {
					fAOdataA = detStt.fAutoOffsetDataOldA = gUVD_STATE.SignalA.fAbsorbance;
				}
			}

			if(AOcountA > 0) {	// 0.0으로 값을 보내다가 2초후에 제로(오프셋)값을 적용한다.
				gUVD_STATE.SignalA.fAbsorbance = gUVD_STATE.SignalB.fAbsorbance
					= gUVD_ACQDATA.SignalA[indexA].fAbsorbance = gUVD_ACQDATA.SignalB[indexA].fAbsorbance
					= fAOdataA;	// 0.0	// polarity적용 이후 값 (-)곱함
				--AOcountA;
				if(detStt.bPolarity) {
					detStt.fAutoOffsetDataCurA = -detStt.fAbsorbance[0];
				}
				else {
					detStt.fAutoOffsetDataCurA = detStt.fAbsorbance[0];
				}
				detStt.fAutoOffsetDataA = detStt.fAutoOffsetDataCurA - detStt.fAutoOffsetDataOldA;
			}
			else {
				if(detStt.bPolarity) {	// negative
					gUVD_STATE.SignalA.fAbsorbance = gUVD_STATE.SignalB.fAbsorbance
						= gUVD_ACQDATA.SignalA[indexA].fAbsorbance = gUVD_ACQDATA.SignalB[indexA].fAbsorbance
						= -detStt.fAbsorbance[0] - detStt.fAutoOffsetDataA;
				}
				else {
					gUVD_STATE.SignalA.fAbsorbance = gUVD_STATE.SignalB.fAbsorbance
						= gUVD_ACQDATA.SignalA[indexA].fAbsorbance = gUVD_ACQDATA.SignalB[indexA].fAbsorbance
						= detStt.fAbsorbance[0] - detStt.fAutoOffsetDataA;
				}
			}
			break;
		case UVD_DUAL_WAVE_MODE:	// DUAL
// 	데이터 필터링모드는 없다.
			switch(detStt.dualMode) {
				case UVD_DUAL_MOVEING:
					//if(GM_OFF == detMotor.btMode) {
						++indexDual;
						dualSamplingCountA = dualSamplingCountB = 0;
						refDataSum = 0;
						samDataSum = 0;

						if(!who) {
							detMotor.sStartStep = detStt.dualStartStep;
							detMotor.btFMMode = detMotor.btStartFilter;
						}
						else {
							detMotor.sStartStep = detStt.dualEndStep;
							detMotor.btFMMode = detMotor.btEndFilter;
						}
						
						NVIC_DisableIRQ(Timer2_IRQn );
						detMotor.btMode = GM_SINGLE_ACT;
						//iprintf("M\r\n");
						Motor_Ctrl_Command();
						NVIC_EnableIRQ(Timer2_IRQn );
							
						detStt.dualMode = UVD_DUAL_WAITING;
					//}

					break;
				case UVD_DUAL_WAITING:

#if 1
					if(++indexDual >= detStt.dualWaitCnt) {	// ChroZen 	// 50Hz->125Hz
						//iprintf("N\r\n");
						detStt.dualMode = UVD_DUAL_READING;
					}
#else
					if(detMotor.sGMDestStep == detMotorState.sGMCurStep) {
						detStt.dualMode = UVD_DUAL_READING;
					}
#endif
			
					break;
				case UVD_DUAL_READING:
					refDataSum += refData;// * CONV_AD_TO_ENERGY;
					samDataSum += samData;
					if(!who) {
						if(++dualSamplingCountA >= detStt.dualSamplingCountA) {
							gUVD_STATE.SignalA.fReferenceE = refDataSum * CONV_AD_TO_ENERGY / (float)dualSamplingCountA;
							gUVD_STATE.SignalA.fSampleE = samDataSum * CONV_AD_TO_ENERGY / (float)dualSamplingCountA;

							detStt.transmittance[0] = samDataSum / refDataSum;
							if(detStt.transmittance[0] <= TRANS_ZERO_VALUE) detStt.transmittance[0] = TRANS_ZERO_VALUE;

							detStt.dualMode = UVD_DUAL_MOVEING;
							indexDual = 0;
							who = 1;
							//iprintf("O\r\n");
						}
					}
					else {
						if(++dualSamplingCountB >= detStt.dualSamplingCountB) {
							gUVD_STATE.SignalB.fReferenceE = refDataSum * CONV_AD_TO_ENERGY / (float)dualSamplingCountB;
							gUVD_STATE.SignalB.fSampleE = samDataSum * CONV_AD_TO_ENERGY / (float)dualSamplingCountB;

							detStt.transmittance[1] = samDataSum / refDataSum;
							if(detStt.transmittance[1] <= TRANS_ZERO_VALUE) detStt.transmittance[1] = TRANS_ZERO_VALUE;

							detStt.dualMode = UVD_DUAL_MOVEING;
							indexDual = 0;
							who = 0;
							//iprintf("P\r\n");

						}
					}
					break;
			}

				if(detStt.bAZstartA || detStt.bAutozeroA || detStt.bAZwaveA) {
					detStt.bAZstartA = detStt.bAutozeroA = detStt.bAZwaveA = detStt.bAOpolaA = detStt.bAOwaveA = NO;
					AZcountA = AUTO_ZERO_OFFSET_COUNT;	// 2초
					fAZdataA = 0.0;
					detStt.fAutoOffsetDataA = 0.0;
				}

				if(AZcountA > 0) {	// 0.0으로 값을 보내다가 2초후에 제로(오프셋)값을 적용한다.
					detStt.fAbsorbance[0] = fAZdataA;	// 0.0
					--AZcountA;
					detStt.fAutoZeroDataA = detStt.transmittance[0];
				}
				else {
					detStt.fAbsorbance[0] = -log10(detStt.transmittance[0] / detStt.fAutoZeroDataA);
				}
				if(detStt.bAOpolaA) {	// offset은 absorbance 값을 해야 한다. old값의 2배를 더해 준다.
					if(detStt.bPolarity) {	// negative
						detStt.fAutoOffsetDataA = -gUVD_STATE.SignalA.fAbsorbance * 2;		// 밑이 + 면 위도 + / -면 -
					}
					else {
						detStt.fAutoOffsetDataA = 0.0;
					}

					detStt.bAOpolaA = detStt.bAOwaveA = NO;
					AOcountA = 0;
				}
				else if(detStt.bAOwaveA) {
					detStt.bAOwaveA = NO;
					AOcountA = AUTO_ZERO_OFFSET_COUNT;	// 2초
					if(detStt.bPolarity) {
						fAOdataA = detStt.fAutoOffsetDataOldA = gUVD_STATE.SignalA.fAbsorbance;
					}
					else {
						fAOdataA = detStt.fAutoOffsetDataOldA = gUVD_STATE.SignalA.fAbsorbance;
					}
				}

				if(AOcountA > 0) {
					// 0.0으로 값을 보내다가 2초후에 제로(오프셋)값을 적용한다.
					gUVD_STATE.SignalA.fAbsorbance
						= gUVD_ACQDATA.SignalA[indexA].fAbsorbance
						= fAOdataA;	// 0.0	// polarity적용 이후 값 (-)곱함
					--AOcountA;
					if(detStt.bPolarity) {
						detStt.fAutoOffsetDataCurA = -detStt.fAbsorbance[0];
					}
					else {
						detStt.fAutoOffsetDataCurA = detStt.fAbsorbance[0];
					}
					detStt.fAutoOffsetDataA = detStt.fAutoOffsetDataCurA - detStt.fAutoOffsetDataOldA;
				}
				else {
					if(detStt.bPolarity) {	// negative
						gUVD_STATE.SignalA.fAbsorbance
							= gUVD_ACQDATA.SignalA[indexA].fAbsorbance
							= -detStt.fAbsorbance[0] - detStt.fAutoOffsetDataA;
					}
					else {
						gUVD_STATE.SignalA.fAbsorbance
							= gUVD_ACQDATA.SignalA[indexA].fAbsorbance
							= detStt.fAbsorbance[0] - detStt.fAutoOffsetDataA;
					}
				}

				if(detStt.bAZstartB || detStt.bAutozeroB || detStt.bAZwaveB) {
					detStt.bAZstartB = detStt.bAutozeroB = detStt.bAZwaveB = detStt.bAOpolaB = detStt.bAOwaveB = NO;
					AZcountB = AUTO_ZERO_OFFSET_COUNT;	// 2초
					fAZdataB = 0.0;
					detStt.fAutoOffsetDataB = 0.0;
				}

				if(AZcountB > 0) {	// 0.0으로 값을 보내다가 2초후에 제로(오프셋)값을 적용한다.
					detStt.fAbsorbance[1] = fAZdataB;	// 0.0
					--AZcountB;
					detStt.fAutoZeroDataB = detStt.transmittance[1];
				}
				else {
					detStt.fAbsorbance[1] = -log10(detStt.transmittance[1] / detStt.fAutoZeroDataB);
				}
				if(detStt.bAOpolaB) {	// offset은 absorbance 값을 해야 한다. old값의 2배를 더해 준다.
					if(detStt.bPolarity) {	// negative
						detStt.fAutoOffsetDataB = -gUVD_STATE.SignalB.fAbsorbance * 2;		// 밑이 + 면 위도 + / -면 -
					}
					else {
						detStt.fAutoOffsetDataB = 0.0;
					}

					detStt.bAOpolaB = detStt.bAOwaveB = NO;
					AOcountB = 0;
				}
				else if(detStt.bAOwaveB) {
					detStt.bAOwaveB = NO;
					AOcountB = AUTO_ZERO_OFFSET_COUNT;	// 2초
					if(detStt.bPolarity) {
						fAOdataB = detStt.fAutoOffsetDataOldB = gUVD_STATE.SignalB.fAbsorbance;
					}
					else {
						fAOdataB = detStt.fAutoOffsetDataOldB = gUVD_STATE.SignalB.fAbsorbance;
					}
				}

				if(AOcountB > 0) {
					// 0.0으로 값을 보내다가 2초후에 제로(오프셋)값을 적용한다.
					gUVD_STATE.SignalB.fAbsorbance = gUVD_ACQDATA.SignalB[indexA].fAbsorbance = fAOdataB;	// 0.0	// polarity적용 이후 값 (-)곱함
					--AOcountB;
					if(detStt.bPolarity) {
						detStt.fAutoOffsetDataCurB = -detStt.fAbsorbance[1];
					}
					else {
						detStt.fAutoOffsetDataCurB = detStt.fAbsorbance[1];
					}
					detStt.fAutoOffsetDataB = detStt.fAutoOffsetDataCurB - detStt.fAutoOffsetDataOldB;
				}
				else {
					if(detStt.bPolarity) {	// negative
						gUVD_STATE.SignalB.fAbsorbance //= gUVD_STATE.SignalB.fAbsorbance
							= gUVD_ACQDATA.SignalB[indexA].fAbsorbance //= gUVD_ACQDATA.SignalB[indexA].fAbsorbance
							= -detStt.fAbsorbance[1] - detStt.fAutoOffsetDataB;
					}
					else {
						gUVD_STATE.SignalB.fAbsorbance //= gUVD_STATE.SignalB.fAbsorbance
							= gUVD_ACQDATA.SignalB[indexA].fAbsorbance //= gUVD_ACQDATA.SignalB[indexA].fAbsorbance
							= detStt.fAbsorbance[1] - detStt.fAutoOffsetDataB;
					}
				}

			gUVD_ACQDATA.SignalA[indexA].fReferenceE = gUVD_STATE.SignalA.fReferenceE;
			gUVD_ACQDATA.SignalA[indexA].fSampleE = gUVD_STATE.SignalA.fSampleE;
			gUVD_ACQDATA.SignalB[indexA].fReferenceE = gUVD_STATE.SignalB.fReferenceE;
			gUVD_ACQDATA.SignalB[indexA].fSampleE = gUVD_STATE.SignalB.fSampleE;

			break;

		case UVD_SCAN_WAVE_MODE:	// SCAN
			switch(detStt.scanMode) {
				case UVD_SCAN_MOVEING:
					++indexScan;
					samplingCount = 0;
					refDataSum = 0;
					samDataSum = 0;
					gUVD_SCANDATA.sWavelength = detStt.scanDestWave;
					detMotor.sStartStep = detMotor.sGMDestStep = GM_wave[detStt.scanDestWave];	//Step;
					detMotor.btMode = GM_SINGLE_ACT;
					Motor_Ctrl_Command();
					detStt.scanMode = UVD_SCAN_WAITING;
					break;
				case UVD_SCAN_WAITING:
					if(++indexScan > detStt.scanWaitCnt) {
						detStt.scanMode = UVD_SCAN_READING;
					}
					break;
				case UVD_SCAN_READING:
					refDataSum += refData;// * CONV_AD_TO_ENERGY;
					samDataSum += samData;
					if(++samplingCount > detStt.scanSamplingCount) {
						gUVD_SCANDATA.fReferenceE = gUVD_STATE.SignalA.fReferenceE = gUVD_STATE.SignalB.fReferenceE
							= refDataSum * CONV_AD_TO_ENERGY / (float)samplingCount;
						gUVD_SCANDATA.fSampleE = gUVD_STATE.SignalA.fSampleE = gUVD_STATE.SignalB.fSampleE
							= samDataSum * CONV_AD_TO_ENERGY / (float)samplingCount;

						detStt.transmittance[0] = samDataSum / refDataSum;
						if(detStt.transmittance[0] <= TRANS_ZERO_VALUE) detStt.transmittance[0] = TRANS_ZERO_VALUE;

						gUVD_SCANDATA.fAbsorbance = gUVD_STATE.SignalA.fAbsorbance = gUVD_STATE.SignalB.fAbsorbance
							= -log10(detStt.transmittance[0]);

						detStt.scanMode = UVD_SCAN_MOVEING;

						indexScan = 0;

#if !PHOTO_ACT_INTR							
						sendScandata();

						if(detStt.scanDestWave >= detStt.scanEndWave) {
							detStt.waveMode = UVD_SINGLE_WAVE_MODE;
							readyInitSet();
							sendSelfMsg(UVD_SELFMSG_STATE, UVD_STATE_SCANNING_MODE, UVD_STATE_READYINIT , UVD_ERR_NONE);
						}
						else {
							detStt.scanDestWave += detStt.scanInterval;
						}
#else
						g_ScanDataDone = YES;
#endif
					}
					break;
			}
			break;

	}

	if(detStt.bOffsetValueSaveAct ==YES) {
		detStt.lOffsetValueRef += Ads1252.refD;
		detStt.lOffsetValueSam += Ads1252.samD;
		if(++detStt.offsetValueCnt == OFFSET_VALUE_COUNT) {
#ifdef FIX_SAM_REF_OFFSET
			detStt.fOffsetValueRefAve = (float)(detStt.lOffsetValueRef)/OFFSET_VALUE_COUNT + fEnergyOffsetDummy;
			detStt.fOffsetValueSamAve = (float)(detStt.lOffsetValueSam)/OFFSET_VALUE_COUNT + fEnergyOffsetDummy;
#else
			detStt.fOffsetValueRefAve = (float)(detStt.lOffsetValueRef)/OFFSET_VALUE_COUNT + ENERGY_OFFSET_DUMMY;
			detStt.fOffsetValueSamAve = (float)(detStt.lOffsetValueSam)/OFFSET_VALUE_COUNT + ENERGY_OFFSET_DUMMY;
#endif
			detStt.bOffsetValueSave = YES;
			detStt.bOffsetValueSaveAct = NO;
			detStt.offsetValueCnt = OFFSET_VALUE_COUNT;
		}
	}


#if !PHOTO_ACT_INTR			
	if(++indexA >= SIGNAL_SEND_COUNT) {		// 500Hz / 5 / 2 / 10 // 5Hz로 전송 [10] * 5 50Hz		// 9120UVD

		indexA = 0;								// 500Hz / 4 / 1 / 25 // 5Hz로 전송 [25] * 5 125Hz	// ChroZen
	
		if(detStt.runstop==RUN_START && STATE_COMM_CONNECTED == Clarity_Connect) {	//(+)210716 CJKIM, Clarity 와 연결 되었을 시에만 전송 
			signalOut();
	
			gUVD_ACQDATA.fRunTime = gUVD_STATE.fElapseTime;
			detStt.cnt++;
			//DEBUG_I(DBG_LEVEL_INFO, "() SigOut [%d]\r\n", detStt.cnt); 	//(+)210604 CJKIM, For Debug	
	
		}
		else 
			detStt.cnt = 0;
	}
#else
	if(++indexA >= SIGNAL_SEND_COUNT)
		indexA = 0; 
#endif
	
	
}

void Ready_PowBtn_Check()		// ready & Power Btn check
{
	//uint32_t signalIn;
	//static uint32_t	cnt	=	0;
	static int notReadyChkcnt=0;
	//static char Button_Press = NO;
		
// ============ Remote Ready In Check
	rmtchk.readyIn = Ready_IN_READ;	// signalIn & 0x01;
	
	if(rmtchk.readyIn == rmtchk.bEdgeCheckSet[REMOTE_READY] ) {
		if(rmtchk.checkCnt[REMOTE_READY]++ > REMOTEIN_TRIGGER_CNT) {
			if(rmtin.r_readyIn == NO) {
				rmtin.r_readyIn = YES;
				notReadyChkcnt = 0;
			}
			rmtchk.checkCnt[REMOTE_READY]= REMOTEIN_TRIGGER_CNT;					
		}
	}
	else {
		if(notReadyChkcnt++ > REMOTEIN_TRIGGER_CNT) {
			if(rmtin.r_readyIn == YES) {
				rmtin.r_readyIn = NO;
				rmtchk.checkCnt[REMOTE_READY] = 0;				
			}
			notReadyChkcnt = REMOTEIN_TRIGGER_CNT;
		}
	}

// ============ Remote Start out Time
	if (rmtout.r_startOut == ON) {
		if(rmtout.remoteStartOutSig ==	YES) {	// 시그널	출력해야 하다.
			if(++rmtout.remoteStartOutCnt > remoteSet.r_outCntSet) {
				rmtout.remoteStartOutCnt	=	0;
				rmtout.remoteStartOutSig= NO;

				if(remoteSet.r_startOutSet ==	CONTACT_CLOSE) { REMOTE_START_OUT_CONTACT_OPEN; }	// off
				else { REMOTE_START_OUT_CONTACT_CLOSE;	}	// on
			}
		}
	}

// ============= Power Button detection
/*
//	if(PWR_BUTTON_IN_READ == 0x0)
//	{
		if(++cnt ==	1) {
			BUZZER_CON_ON;
		}
		else if(cnt	== 10)	{	// 0.1sec // 0.2sec	// 2sec	
			BUZZER_CON_OFF;
			cnt = 10;
		}
//	}
*/
}

YL9020_UVD_ACQDATA	 g_Send_ACQDATA;

void signal_data()
{
	memcpy(&g_Send_ACQDATA, &gUVD_ACQDATA, sizeof(gUVD_ACQDATA));
}


void signalOut()
{
#if PHOTO_ACT_INTR

	DEBUG_I(DBG_LEVEL_INFO, "() Sam A[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f]\r\n",
		g_Send_ACQDATA.SignalA[0].fSampleE, g_Send_ACQDATA.SignalA[1].fSampleE, g_Send_ACQDATA.SignalA[2].fSampleE, g_Send_ACQDATA.SignalA[3].fSampleE,
		g_Send_ACQDATA.SignalA[4].fSampleE, g_Send_ACQDATA.SignalA[5].fSampleE, g_Send_ACQDATA.SignalA[6].fSampleE, g_Send_ACQDATA.SignalA[7].fSampleE,
		g_Send_ACQDATA.SignalA[8].fSampleE, g_Send_ACQDATA.SignalA[9].fSampleE, g_Send_ACQDATA.SignalA[10].fSampleE, g_Send_ACQDATA.SignalA[11].fSampleE,
		g_Send_ACQDATA.SignalA[12].fSampleE, g_Send_ACQDATA.SignalA[13].fSampleE, g_Send_ACQDATA.SignalA[14].fSampleE, g_Send_ACQDATA.SignalA[15].fSampleE,
		g_Send_ACQDATA.SignalA[16].fSampleE, g_Send_ACQDATA.SignalA[17].fSampleE, g_Send_ACQDATA.SignalA[18].fSampleE, g_Send_ACQDATA.SignalA[19].fSampleE, 
		g_Send_ACQDATA.SignalA[20].fSampleE, g_Send_ACQDATA.SignalA[21].fSampleE, g_Send_ACQDATA.SignalA[22].fSampleE, g_Send_ACQDATA.SignalA[23].fSampleE,
		g_Send_ACQDATA.SignalA[24].fSampleE);

	SEND_UVD_PACKET(PACKCODE_YL9020_ACQDATA, g_Send_ACQDATA, g_Send_ACQDATA);

#else

	SEND_UVD_PACKET(PACKCODE_YL9020_ACQDATA, gUVD_ACQDATA, gUVD_ACQDATA);

#endif

}

void DAout()
{
	static unsigned char who =0;
	int tempDig;
	float DAAborbanceA, DAAborbanceB;

	if(who) {
// DA후단 ==> 0x000000 : 0.987 0xffffff : 4.06 / 0x800000 : 2.527 / 0x6fffff : 2.335
// 터미널 ==>	0x800000 (8388608) 에서 0볼트로 세팅한다. ==> -0.000
//				0xffffff 에서 전압을 측정한다.	==> -1.537
//				0x000000 에서 전압을 측정한다.	==> 1.537
//				5457780.0910865322055953155497723(5457780.09)  ==> 1V


		DAAborbanceA = gUVD_STATE.SignalA.fAbsorbance * gUVD_SETUP.SigoutSet[0].fConversion;

		if(DAAborbanceA > DET_DA_SIGNAL_MAX) DAAborbanceA = DET_DA_SIGNAL_MAX;
		else if(DAAborbanceA < DET_DA_SIGNAL_MIN) DAAborbanceA = DET_DA_SIGNAL_MIN;
		tempDig = (int)(DAAborbanceA * DET_DA_1V_DIG_FACTOR);
		detDa.leftData = DET_DA_ZERO_VOLT_DIG - tempDig;
		detDa.cmd = UVD_LEFT_DATA_WRITE;
		who = 0;
	}
	else {
// DA후단 ==> 0x000000 : 0.975 / 0x800000 : 2.533 / 0x7fffff : 2.532 / 0xffffff : 4.08 / 0x6fffff : 2.338
// 터미널 ==>	0x800000 에서 0볼트로 세팅한다. ==> -0.000
//				0xffffff 에서 전압을 측정한다.	==> -1.557
//				0x000000 에서 전압을 측정한다.	==> 1.558

		DAAborbanceB= gUVD_STATE.SignalB.fAbsorbance * gUVD_SETUP.SigoutSet[1].fConversion;;

		if(DAAborbanceB > DET_DA_SIGNAL_MAX) DAAborbanceB = DET_DA_SIGNAL_MAX;
		else if(DAAborbanceB < DET_DA_SIGNAL_MIN) DAAborbanceB = DET_DA_SIGNAL_MIN;
		tempDig = (int)(DAAborbanceB * DET_DA_1V_DIG_FACTOR);
		detDa.rightData = DET_DA_ZERO_VOLT_DIG - tempDig;
		detDa.cmd = UVD_RIGHT_DATA_WRITE;
		who = 1;
	}
//	ioctl(dev_gpio, IOCTL_DET_DA_OUT_DATA, &detDa);
}

/*/////////////////////////////////////////////////////////////////////////
////////////	+24, +5, +3.3, Tung(5V), X, DA1, DA2, Temp	///////////////
*//////////////////////////////////////////////////////////////////
bool powerCheck()
{
	bool result;
//	float minPwr[8]	=		{1.4 ,2.45, 3.1, 4.8, 4.8,11.5,11.5,11.5};
//	float maxPwr[8]	=		{1.6 ,2.55, 3.5, 5.2, 5.2,13.0,14.2,13.2};
//	float referencePwr[8]=	{1.50, 2.5, 3.3, 5.0, 5.0,12.0,12.0,12.0};
//	float factorPwr[8] =		{1.0 , 1.0, 3.0, 3.0, 3.0,11.0,21.0,11.0};

	float minPwr[8]	=		{22, 4.5, 3.1, 4.5, 0, 0, 0, 0};
	float maxPwr[8]	=		{26, 5.5, 3.5, 5.5, 0, 0, 0, 0};
	//float referencePwr[8]=	{24, 5.0, 3.3, 5.0, 0, 0, 0, 0};
//	float factorPwr[8] =	{1.0 , 1.0, 3.0, 3.0, 3.0,11.0,21.0,11.0};

	float sysVolt[8];
	//unsigned short sysVoltave[8]={0};
	//static unsigned short voltSystem[ADC_TEMP_AVERAGE_COUNT][8]={0};
	//static unsigned short sysVoltsum[8]={0};
	static unsigned char index=0;
	unsigned short j;
	
	for(j=0;j<8;j++) {
//		sysVoltsum[j] -= voltSystem[index][j];
//		voltSystem[index][j] = adcCont.rdata[0][j];
//		sysVoltsum[j] += voltSystem[index][j];
//		sysVoltave[j] = sysVoltsum[j] >> ADC_TEMP_AVERAGE_SHIFT;
		
//		detDiag.sysVolt[j] = sysVolt[j] = sysVoltave[j]*ADC_REF_VOLTAGE/ADC_FULL_COUNT*factorPwr[j];
		detDiag.sysVolt[j] = sysVolt[j] = st_UV_func.iVolt_Read[j]*ADC_REF_VOLTAGE/ADC_FULL_COUNT;

		// DEBUG_I(DBG_LEVEL_INFO, "() system power check!!! [%d] : %f \n\r",j , sysVolt[j]/*adcCont.rdata[0][j]*/ );
		if(sysVolt[j] > minPwr[j] && sysVolt[j] < maxPwr[j]) {
			detDiag.checkVolt[j] = OK;
			result = OK;
		}
		else {
			detDiag.checkVolt[j] = ERROR;
			result = ERROR;
// 여기에 에러처리를 삽입하라.
		}

////////////////////////////////////////////////////////////////////////////// Test
	detDiag.checkVolt[j] = OK;
	result = OK;
//////////////////////////////////////////////////////////////////////////////
		
	}

	if(++index == ADC_TEMP_AVERAGE_COUNT) index = 0;

	return result;
}

void IP_Init()
{
	uint8_t SW1_Buff;
	uint8_t SW2_Buff;
	//uint8_t SW_Buff;

	IP_Addr0 = ETHERNET_CONF_IPADDR0;
	IP_Addr1 = ETHERNET_CONF_IPADDR1;
	IP_Addr2 = ETHERNET_CONF_IPADDR2;
//	IP_Addr3 = ETHERNET_CONF_IPADDR3;
	SW1_Buff = SW1_READ;
	SW2_Buff = SW2_READ;

	IP_Addr3 = ETHERNET_CONF_IPADDR3 + (SW1_Buff + (SW2_Buff << 1));	//(-/+) 210531 CJKIM, 20 -> ETHERNET_CONF_IPADDR3,  기존값 동일
	

	DEBUG_I(DBG_LEVEL_INFO, "() ----	IP Set	%d	----\r\n", IP_Addr3);
 }

void FLED_Ctrl(char FLED_Type)
{
	switch(FLED_Type)
	{
		case FLED_Green:	// Ready
			FLED_LED1_Write(FLED_Green_Color);	// 0.5sec
			FLED_LED2_Write(FLED_Green_Color);	// 0.5sec
			FLED_LED3_Write(FLED_Green_Color);	// 0.5sec
			FLED_LED_Load(0);
			break;
		case FLED_Blue:			// Run
			FLED_LED1_Write(FLED_Blue_Color);	// 0.5sec
			FLED_LED2_Write(FLED_Blue_Color);
			FLED_LED3_Write(FLED_Blue_Color);
			FLED_LED_Load(0);
			break;
		case FLED_White:		// x -> init
			FLED_LED1_Write(FLED_White_Color);
			FLED_LED2_Write(FLED_White_Color);
			FLED_LED3_Write(FLED_White_Color);
			FLED_LED_Load(0);
			break;
		case FLED_Red:			// Error // Leak
			FLED_LED1_Write(FLED_Red_Color);	// 0.5sec
			FLED_LED2_Write(FLED_Red_Color);
			FLED_LED3_Write(FLED_Red_Color);
			FLED_LED_Load(0);
			break;
		case FLED_Red_Blink:	// Error // D2Lamp
			FLED_LED1_Write(FLED_Red_Color_0_4sec);
			FLED_LED2_Write(FLED_Black_Color);	// 0.5sec
			FLED_LED3_Write(FLED_Red_Color_0_1sec);
			FLED_LED_Load(0);
			break;
#if 0
		case FLED_Green_Blink:	// Ready // Green / OFF Blink 1Sec -> 사용안함.
			FLED_LED1_Write(FLED_Green_Color_0_4sec);
			FLED_LED2_Write(FLED_Black_Color);	// 0.5sec
			FLED_LED3_Write(FLED_Green_Color_0_1sec);
			FLED_LED_Load(0);
			break;
		case FLED_Blue_Blink:	// Ready	// 통신 x -> 사용안함.
			FLED_LED1_Write(FLED_Blue_Color_0_4sec);
			FLED_LED2_Write(FLED_Black_Color);
			FLED_LED3_Write(FLED_Black_Color_0_1sec);
			FLED_LED_Load(0);
			break;
		case FLED_BlueGreen_Blink:	// Ready	// 통신 o -> 사용안함.
			FLED_LED1_Write(FLED_Blue_Color_0_4sec);
			FLED_LED2_Write(FLED_Green_Color);
			FLED_LED3_Write(FLED_Blue_Color_0_1sec);
			FLED_LED_Load(0);
			break;	
		case FLED_Yellow:	// Ready -> 사용안함.
			FLED_LED1_Write(FLED_Yellow_Color);	// 0.5sec
			FLED_LED2_Write(FLED_Yellow_Color);
			FLED_LED3_Write(FLED_Yellow_Color);
			FLED_LED_Load(0);
			break;
		case FLED_Black:		// x
			FLED_LED1_Write(FLED_Black_Color);
			FLED_LED2_Write(FLED_Black_Color);
			FLED_LED3_Write(FLED_Black_Color);
			FLED_LED_Load(0);
			break;
		case FLED_YellowBlack_Blink:	// x
			FLED_LED1_Write(FLED_Yellow_Color_0_5sec);
			FLED_LED2_Write(FLED_Black_Color_0_5sec);
			FLED_LED3_Write(FLED_Black_Color_0_1sec);
			FLED_LED_Load(0);
			break;	
		case FLED_YellowGreen_Blink:	// x
			FLED_LED1_Write(FLED_Yellow_Color_0_5sec);
			FLED_LED2_Write(FLED_Green_Color_1Sec);
			FLED_LED3_Write(FLED_Yellow_Color_0_5sec);
			FLED_LED_Load(0);
			break;	
#endif
		default:
			FLED_LED1_Write(FLED_Green_Color);
			FLED_LED2_Write(FLED_Green_Color);
			FLED_LED3_Write(FLED_Green_Color);
			FLED_LED_Load(0);
			break;
	}
}

/*******************************************************************************
* FUNCTION : filterMotorHome
* DESCRIPTION : 홈위치 찾기 및 이동
* 변수를 초기화한다.
*******************************************************************************/
void filterMotorHome()
{
// 모터를 홈위치로 돌린다. 센서가 인식할 때까지
	detMotorState.btFMCurStep = 0;
	detMotor.btFMDestStep = 0;
}

/*******************************************************************************
* FUNCTION : filterMotorGoPos
* DESCRIPTION : 절대위치로 이동
*******************************************************************************/
void filterMotorGoPos()
{
}

/*******************************************************************************
* FUNCTION : gratingMotorHome
* DESCRIPTION : 홈위치 찾기 및 이동
* 변수를 초기화한다.
*******************************************************************************/
void gratingMotorHome()
{
// 모터를 홈위치로 돌린다. 센서가 인식할 때까지
	detMotorState.sGMCurStep = 0;
	detMotor.sGMDestStep = 0;
}

/*******************************************************************************
* FUNCTION : gratingMotorGo
* DESCRIPTION : 현재 위치로 부터 일정 방향을 일정 스텝이동
*******************************************************************************/
void gratingMotorGo(bool dir, unsigned int step){}

/*******************************************************************************
* FUNCTION : gratingMotorGoPos
* DESCRIPTION : Home을 기준으로 절대위치로 이동
*******************************************************************************/
void gratingMotorGoPos(unsigned int Step)
{
	detMotor.sStartStep = detMotor.sGMDestStep = Step;
	detMotor.btMode = GM_SINGLE_ACT;
	Motor_Ctrl_Command();

}

bool gratingMotorGoPosChk()
{
	if(detMotorState.sGMCurStep == detMotor.sGMDestStep) return 1;
	else return 0;
}

/*******************************************************************************
* FUNCTION : gratingMotorGoWave
* DESCRIPTION : Home을 기준으로 절대위치로 이동(파장)
*******************************************************************************/
void gratingMotorGoWave(unsigned int wLength)
{
}

void LampTimeCheck()
{
	static unsigned short d2ElapseCnt = 0, wElapseCnt;	// 전원을 켠 이후 램프 온시간 - 초

// D2Lamp /Wlamp 사용시간 -- 저장은 30분 단위로 저장하자, 단 1800초가 되가전 900초에서 저장을 한다.
	if(gUVD_STATE.btD2Lamp == UVDMSG_VALUE_D2LAMP_ON)
	{
		if(++d2ElapseCnt == LAMP_USE_SAVE_POINT)
		{ // 저장
			fram_LampTime_Save();
			d2ElapseCnt = 0;
		}
		detStt.fD2LampElapse += LAMP_USE_TIME_CAL ;			// D2 사용 시간	: 0x4444	- 17476
		gUVD_CONFIG.nD2LampElapse = (unsigned int)(detStt.fD2LampElapse);
	}

#ifdef FIX_SAM_REF_OFFSET
	if(gUVD_STATE.btWLamp == ON)
	{
		if(++wElapseCnt == LAMP_USE_SAVE_POINT)
		{ // 저장
			fram_LampTime_Save();
			wElapseCnt = 0;
		}
		detStt.fWLampElapse += LAMP_USE_TIME_CAL ;			// W 사용 시간	: 0x4444	- 17476
		gUVD_CONFIG.nWLampElapse = (unsigned int)(detStt.fWLampElapse);

		if(gUVD_STATE.btD2Lamp == UVDMSG_VALUE_D2LAMP_ON) {
			fEnergyOffsetDummy = DET_ENERGY_NANOA_DIG * DET_PHOTO_DATA_AVE * DET_OFFSET_ENERGY_D2_W_ON;
		}
		else {
			fEnergyOffsetDummy = DET_ENERGY_NANOA_DIG * DET_PHOTO_DATA_AVE * DET_OFFSET_ENERGY_W_ON;
		}
	}
	else {
		if(gUVD_STATE.btD2Lamp == UVDMSG_VALUE_D2LAMP_ON) {
			fEnergyOffsetDummy = DET_ENERGY_NANOA_DIG * DET_PHOTO_DATA_AVE * DET_OFFSET_ENERGY_D2_ON;
		}
		else {
			fEnergyOffsetDummy = 0.0;
		}
	}
#else
	if(gUVD_STATE.btWLamp == ON)
	{
		if(++wElapseCnt == LAMP_USE_SAVE_POINT)
		{ // 저장
			fram_LampTime_Save();
			wElapseCnt = 0;
		}
		detStt.fWLampElapse += LAMP_USE_TIME_CAL ;			// D2 사용 시간	: 0x4444	- 17476
		gUVD_CONFIG.nWLampElapse = (unsigned int)(detStt.fWLampElapse);
	}
#endif
}
////////////////////////////////////////////////////////
//	Error Check
// 1Hz
////////////////////////////////////////////////////////
int stateCheck( )
{
	//static unsigned char d2Retry=NO;
	static unsigned char d2RetryCnt=0;
	static unsigned char tryStrike=NO;
	static unsigned char d2ErrorCnt=0;
	static unsigned char leakErrorCnt=0;
	unsigned char oldD2State = 0, oldLeakState = 0, oldInitialState = 0;
	//bool leakErrchk;

	//static unsigned int connectChkCnt = 0;
	//static bool buzzer_cnt = 0;

	//static unsigned char leak_delay = 0;
	//static unsigned int leak_delay_cnt = 0;
	//static char Errr_Buz_OnOff = OFF;
	static char Selfmsg_Cnt = 0;
	static char Selfmsg_1time = 0;
	
	if( Selfmsg_flag == ON)		// 210702 KSLEE
	{
		if( ++Selfmsg_Cnt == 3) Selfmsg_flag = OFF;
		else
		{
			detStt.leakError = NO;
			leakErrorCnt = 0;
			
			detStt.btD2HeatingCnt = 0;
			d2RetryCnt = 0;
			d2ErrorCnt = 0;
			Selfmsg_1time = 0;
		}
	}
	else
	{

		if(Leak_READ & 0x01)
		{
			if(++leakErrorCnt >= LEAK_DETECT_ERROR_CNT)
			{
				detStt.leakError = YES;
				gUVD_STATE.uErrorCode = UVD_ERR_LEAK;
	//			sendSelfMsgError(UVD_ERR_LEAK);
	//			sendSelfMsgState(UVD_STATE_FAULT);
	//			DEBUG_I(DBG_LEVEL_INFO, "() -- Leak Error --\r\n");
				gUVD_STATE.btState = UVD_STATE_FAULT;
				leakErrorCnt = LEAK_DETECT_ERROR_CNT;
	//			Error 처리 	fault
	//			Errr_Buz_OnOff = ON;
				LEDStateChange = 0;
			}
		}
		else
		{
			detStt.leakError = NO;
			leakErrorCnt = 0;
	//		if( gUVD_STATE.btState != UVD_STATE_FAULT)
	//		{
	//			if(Errr_Buz_OnOff) 
	//			{
	//				Errr_Buz_OnOff = OFF;
	//				BUZZER_CON_OFF;
	//			}
	//		}
		}

		if(detStt.btD2State == UVDMSG_VALUE_D2LAMP_HEATING)
		{ // 현재 히팅중 20초 대기
			if(D2_State_READ & 0x01)
			{
	//			sendSelfMsg(UVD_SELFMSG_D2LAMP, gUVD_STATE.btD2Lamp, UVDMSG_VALUE_D2LAMP_ON , UVD_ERR_NONE);
				gUVD_STATE.btD2Lamp = UVDMSG_VALUE_D2LAMP_ON;
				detStt.btD2State = UVDMSG_VALUE_D2LAMP_ON;
				detStt.btD2HeatingCnt = 0;
				d2RetryCnt = 0;
	//			BUZZER_CON_OFF;
			}
			else
			{ // 다시 시도 2번 -- 처음 시도 부터 총3번
				if(++detStt.btD2HeatingCnt >= STATE_CHK_D2_LAMP_HEATING_CNT)
				{
					if(tryStrike == NO)
					{
						tryStrike = YES;
						D2_Strk(ON);
					}
					else {
						D2_Strk(OFF);
						if(++d2RetryCnt <= STATE_CHK_D2_LAMP_ON_RETRY_CNT)
						{
							D2lampCon(ON);
							gUVD_STATE.btD2Lamp = UVDMSG_VALUE_D2LAMP_HEATING;
							detStt.btD2State = UVDMSG_VALUE_D2LAMP_HEATING;
							detStt.btD2HeatingCnt = 0;

							d2RetryCnt++;
							tryStrike = NO;
						}
						else
						{
							D2lampCon(OFF);
							d2RetryCnt=0;
							tryStrike = NO;
							gUVD_STATE.uErrorCode = UVD_ERR_D2_LAMP;
	//						sendSelfMsg(UVD_SELFMSG_D2LAMP, gUVD_STATE.btD2Lamp, UVDMSG_VALUE_D2LAMP_FAIL , UVD_ERR_D2_LAMP);
							gUVD_STATE.btD2Lamp = UVDMSG_VALUE_D2LAMP_FAIL;
							detStt.btD2State = UVDMSG_VALUE_D2LAMP_FAIL;
							// error처리
	//						sendSelfMsgError(UVD_ERR_D2_LAMP);

							DEBUG_I(DBG_LEVEL_INFO, "() -- D2 lamp Heating State Error --\r\n");
							
							gUVD_STATE.btState = UVD_STATE_FAULT;
							LEDStateChange = 0;
						}
					}
				}
			}
		}
		else {
			if(detStt.btD2State == UVDMSG_VALUE_D2LAMP_ON)
			{
	//DEBUG_I(DBG_LEVEL_INFO, "() D2_State_READ == %x\r\n", D2_State_READ);
				if(D2_State_READ & 0x01)
				{
					d2RetryCnt = 0;
					d2ErrorCnt = 0;
				}
				else
				{	// error
					if(++d2ErrorCnt == D2_DETECT_ERROR_CNT)
					{
						tryStrike = NO;
						d2ErrorCnt = D2_DETECT_ERROR_CNT;
						ErrorProcess(-6);
						gUVD_STATE.uErrorCode = UVD_ERR_D2_LAMP;
	//					sendSelfMsg(UVD_SELFMSG_D2LAMP, gUVD_STATE.btD2Lamp, UVDMSG_VALUE_D2LAMP_FAIL , UVD_ERR_D2_LAMP);
	//					sendSelfMsgError(UVD_ERR_D2_LAMP);

						DEBUG_I(DBG_LEVEL_INFO, "() -- D2 lamp ON State Error --\r\n");

	//					sendSelfMsgState(UVD_STATE_FAULT);
						gUVD_STATE.btD2Lamp = UVDMSG_VALUE_D2LAMP_FAIL;
						detStt.btD2State = UVDMSG_VALUE_D2LAMP_FAIL;
						gUVD_STATE.btState = UVD_STATE_FAULT;
					}
				}
			}
		}

		if(gUVD_STATE.btD2Lamp != UVDMSG_VALUE_D2LAMP_FAIL && detStt.leakError != YES && detStt.InitialError != YES)
		{
	//		gUVD_STATE.uErrorCode = UVD_ERR_NONE;
	//		BUZZER_CON_OFF;
			if( oldLeakState == YES || oldD2State == UVDMSG_VALUE_D2LAMP_FAIL)
			{
				if(LEDStateChange) LEDStateChange = 0;
			}
		}
		else {
			if(gUVD_STATE.btD2Lamp != UVDMSG_VALUE_D2LAMP_OFF)
			{
				oldD2State = gUVD_STATE.btD2Lamp;
				D2lampCon(OFF);	// leak error상태도 fault 이므로 램프오프
	//			sendSelfMsg(UVD_SELFMSG_D2LAMP, oldD2State, gUVD_STATE.btD2Lamp , gUVD_STATE.uErrorCode);
			}

	//		ledError(LED_BLINK);
		}
		oldD2State = detStt.btD2State;
		oldLeakState = detStt.leakError;
		oldInitialState = detStt.InitialError;
		LampTimeCheck();

	//	if( !Selfmsg_flag )
		if( tmpgUVD_STATE.btState != UVD_STATE_FAULT)
		{
			if(gUVD_STATE.uErrorCode != UVD_ERR_NONE) {
				DEBUG_E("() UVD Error[%d, %s]\r\n", gUVD_STATE.uErrorCode, uvd_error_str(gUVD_STATE.uErrorCode));
			}
			
			
	//		Selfmsg_flag = ON;
			switch(gUVD_STATE.uErrorCode)
			{
				case UVD_ERR_NONE:
					break;
				case UVD_ERR_LEAK:
					if( Selfmsg_1time == 0)
					{
						Selfmsg_1time = 1;
						sendSelfMsgError(UVD_ERR_LEAK);
					}
					break;
				case UVD_ERR_D2_LAMP:
					if( Selfmsg_1time == 0)
					{
						Selfmsg_1time = 1;
						sendSelfMsgError(UVD_ERR_D2_LAMP);
					}
					break;
				case UVD_ERR_INITIAL:
					if( Selfmsg_1time == 0)
					{
						Selfmsg_1time = 1;
						sendSelfMsgError(UVD_ERR_INITIAL);
					}
					break;
			}
		}	
	}

	return detStt.btD2State;
}

void remoteInfn()
{

	if(	rmtin.r_startIn ) {
		if(detStt.runstop != RUN_START) {
			// 스타트 메시지만 보내고 스타트는 기다린다.
			sendSelfMsgExtin(UVDMSG_VALUE_EXTIN_START);
		}
		else {}
	}
	if( rmtin.r_readyIn != gUVD_STATE.btReadyExtIn) {
		if(rmtin.r_readyIn) {
			sendSelfMsgExtin(UVDMSG_VALUE_EXTIN_READY);
		}
		else {
			sendSelfMsgExtin(UVDMSG_VALUE_EXTIN_NOTREADY);
		}
		gUVD_STATE.btReadyExtIn = rmtin.r_readyIn;
	}
	if(	rmtin.r_ChartExtIn) {
//		sendSelfMsgExtin(UVDMSG_VALUE_EXTIN_CHART);
	}
	if(	rmtin.r_LampExtIn) {
//		sendSelfMsgExtin(UVDMSG_VALUE_EXTIN_LAMP);
	}
	if(	rmtin.r_AZExtIn) {
//		sendSelfMsgExtin(UVDMSG_VALUE_EXTIN_AZ);
	}
}

void stateActFunc()	// 500Hz
{
	static int Err_Buzz_cnt = 0;
	static unsigned char oldActState = -1;

//		if (gUVD_STATE.btState != oldActState) {
//			DEBUG_I(DBG_LEVEL_INFO, "() state[%d, %s]\r\n", gUVD_STATE.btState, uvd_act_state_str(gUVD_STATE.btState));
//			oldActState = gUVD_STATE.btState;
//		}
	
	
	switch(gUVD_STATE.btState) {
		case UVD_STATE_RUN_MODE://			2	// run  ==> single , dual mode
			if(!LEDStateChange)
			{
				LEDStateChange = 1;
				FLED_Ctrl(FLED_Blue);
			}

			if(eventCtrl.bTimeEvent && detStt.runstop==RUN_START) {		// 50ms
				eventCtrl.bTimeEvent = OFF;
				eventCtrl.nTimeEventCnt++ ;
				if(++eventCtrl.btTimeCnt50m == EVENT_TIME_COUNT_0_2SEC) {	// 0.2초
					gUVD_STATE.fElapseTime = (float)eventCtrl.nTimeEventCnt / 1200.f;	// ChroZen
					eventCtrl.btTimeCnt50m = 0;
					programMode();
				}
			}
			break;
		case UVD_STATE_INITIALIZING://		0
// 초기화가 끝나면 UVD_STATE_READYINIT로
			if(!LEDStateChange)
			{
				LEDStateChange = 1;
				FLED_Ctrl(FLED_White);
			}
			//	Initialize();		//(+)210805 CJKIM, Interrupt
			break;
		case UVD_STATE_READYINIT://			1	==> single , dual mode
			if(!LEDStateChange)
			{
				LEDStateChange = 1;
//				if( Clarity_Connect ) FLED_Ctrl(FLED_BlueGreen_Blink);	// ready + clarity 연결 o
//				else FLED_Ctrl(FLED_Blue_Blink);	// ready + clarity 연결 x
				FLED_Ctrl(FLED_Green);
			}
// 이벤트 테이블 첫행을 준비한다.
			break;
		case UVD_STATE_SCANNING_MODE://		3
			if(!LEDStateChange)
			{
				LEDStateChange = 1;
				FLED_Ctrl(FLED_Green);
			}
			break;
		case UVD_STATE_DIAGNOSIS_MODE://	4
			if(!LEDStateChange)
			{
				LEDStateChange = 1;
				FLED_Ctrl(FLED_Green);
			}
			diagFunc();
			break;
		case UVD_STATE_FAULT://				5
			if(!LEDStateChange)
			{
				LEDStateChange = 1;
				if(gUVD_STATE.uErrorCode == UVD_ERR_LEAK)
				{
					FLED_Ctrl(FLED_Red);
				}
				else
				{
					FLED_Ctrl(FLED_Red_Blink);
					Err_Buzz_cnt = 0;
				}
			}

			if(gUVD_STATE.uErrorCode == UVD_ERR_LEAK)
			{
				BUZZER_CON_ON;
			}
			else
			{
				if(gUVD_STATE.uErrorCode != UVD_ERR_NONE)
				{
					if( ++Err_Buzz_cnt <= 250)
					{
						BUZZER_CON_ON;
					}
					else if( Err_Buzz_cnt <= 499) BUZZER_CON_OFF;
					else Err_Buzz_cnt = 0;
				}
			}
			break;
	}
}

/*******************************************************************************
* FUNCTION : startFunc
* DESCRIPTION : 스타트 시그널이 들어오면 스타트 전 초기화 등 설정 작업을 진행한다.
*******************************************************************************/
void startFunc()
{
	eventCtrl.nTimeEventCnt=0;
	eventCtrl.btTimeCnt50m=0;

	gUVD_STATE.fElapseTime=0;
	gUVD_STATE.nEventStep=0;

	detStt.runstop=RUN_START;
	detStt.nowStart = YES;

	detStt.oldState = gUVD_STATE.btState;
	gUVD_STATE.btState = UVD_STATE_RUN_MODE;
	sendSelfMsgState(UVD_STATE_RUN_MODE);

	remoteAction(ON, OFF , gUVD_SETUP.btDefaultMarkOut);	// start out

	if(gUVD_SETUP.ChannelSetA.btAutoZero == UVD_CHANNEL_AUTOZERO_START
		|| gUVD_SETUP.ChannelSetA.btAutoZero == UVD_CHANNEL_AUTOZERO_BOTH) detStt.bAZstartA = YES;
	if(gUVD_SETUP.ChannelSetB.btAutoZero == UVD_CHANNEL_AUTOZERO_START
		|| gUVD_SETUP.ChannelSetB.btAutoZero == UVD_CHANNEL_AUTOZERO_BOTH) detStt.bAZstartB = YES;

	eventAction();

	LEDStateChange = 0;
//	ledReadyRun(LED_BLINK);

}

/*******************************************************************************
* FUNCTION : stopFunc
* DESCRIPTION : 스톱 시그널이 들어오면 스타트 전 초기화 등 설정 작업을 진행한다.
*******************************************************************************/
void stopFunc()
{
	//DEBUG_I(DBG_LEVEL_INFO, "() state[%d, %s]\r\n", gUVD_STATE.btState, uvd_state_str(gUVD_STATE.btState));

	switch(gUVD_STATE.btState) {
		case UVD_STATE_RUN_MODE://			2	// run  ==> single , dual mode
			detStt.runstop=RUN_STOP;
			remoteChkEn(YES);
			break;
		case UVD_STATE_SCANNING_MODE://		3
			detStt.oldState = gUVD_STATE.btState;
			readyInitSet();
			sendSelfMsgState(gUVD_STATE.btState);
			break;
		case UVD_STATE_DIAGNOSIS_MODE://	4
			detStt.oldState = gUVD_STATE.btState;
			readyInitSet();
			sendSelfMsgState(gUVD_STATE.btState);
			break;
		default:
			break;
	}
	
	LEDStateChange = 0;
//	ledReadyRun(LED_ON);
}

/*******************************************************************************
* FUNCTION : programMode
* DESCRIPTION : 이벤트 프로그램 모드에서 실행된다.
*	스타트가 되면 0.25초마다 실행된다.
*
*******************************************************************************/
void programMode()
{
	if(eventCtrl.bTableExist == YES) {	// 이벤트 테이블 존재
		if(gUVD_STATE.fElapseTime >= gUVD_EVENT[gUVD_STATE.nEventStep + 1].fTime) {	// 시간이
			gUVD_STATE.nEventStep++;
			eventAction();
		}
	}
	else {	// 이벤트 테이블이 없음 --
	}
}

/*******************************************************************************
* FUNCTION : eventAction
* DESCRIPTION : 어떤 이벤트를 실행할 것인지?
*	파장 , ... , 외부 트리거 ...
*******************************************************************************/
void eventAction()
{
	unsigned char curTable;
	//unsigned char beforeTable;
	unsigned char nextTable;

	curTable = gUVD_STATE.nEventStep;
	nextTable = gUVD_STATE.nEventStep + 1;

	if(gUVD_SETUP.nProgramCount > gUVD_STATE.nEventStep && gUVD_EVENT[nextTable].fTime > gUVD_EVENT[curTable].fTime) {
		eventCtrl.bTableExist = YES;
	}
	else {
		eventCtrl.bTableExist = NO;
	}

// 이벤트 실행
	if(gUVD_STATE.nEventStep > 0) {
		eventExecution();
	}
}

void eventExecution()
{
	bool waveChangeA=NO;
	bool waveChangeB=NO;
	bool cutFilterChange=NO;

	unsigned int beforeStep;
	unsigned int curStep;

	beforeStep = gUVD_STATE.nEventStep - 1;
	curStep = gUVD_STATE.nEventStep;
// wavelength
	if(gUVD_EVENT[curStep].sWavelengthA != gUVD_EVENT[beforeStep].sWavelengthA) waveChangeA = YES;
	if(gUVD_EVENT[curStep].sWavelengthB != gUVD_EVENT[beforeStep].sWavelengthB) waveChangeB = YES;
	if(gUVD_EVENT[curStep].btCutFilter != gUVD_EVENT[beforeStep].btCutFilter) cutFilterChange = YES;

	if(waveChangeA || waveChangeB || cutFilterChange) {
		motorInitSet(gUVD_EVENT[curStep].sWavelengthA,gUVD_EVENT[curStep].sWavelengthB,gUVD_EVENT[curStep].btCutFilter);
	}

// lamp	컨트롤은 InitEvent설정에서만 컨트롤한다.
	if(gUVD_EVENT[curStep].btPolarity != gUVD_EVENT[beforeStep].btPolarity) {
		if(gUVD_SETUP.ChannelSetA.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_POLAR
			|| gUVD_SETUP.ChannelSetA.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_BOTH) {
			if(waveChangeA) detStt.bAOwaveA = YES;	// 파장도 변하엿다. 파장변경과 같은 방식으로 동작해야 한다.
			else detStt.bAOpolaA = YES;
		}

		if(gUVD_SETUP.ChannelSetB.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_POLAR
			|| gUVD_SETUP.ChannelSetB.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_BOTH) {
			if(waveChangeB) detStt.bAOwaveB = YES;	// 파장도 변하엿다. 파장변경과 같은 방식으로 동작해야 한다.
			else detStt.bAOpolaB = YES;
		}
		detStt.bPolarity = gUVD_EVENT[curStep].btPolarity;		// 0: Positive, 1:Negative
	}

// AutoZero
	if(waveChangeA)	{
		if(gUVD_SETUP.ChannelSetA.btAutoZero == UVD_CHANNEL_AUTOZERO_WAVE
			|| gUVD_SETUP.ChannelSetA.btAutoZero == UVD_CHANNEL_AUTOZERO_BOTH ) detStt.bAZwaveA = YES;

		if(gUVD_SETUP.ChannelSetA.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_WAVE
			|| gUVD_SETUP.ChannelSetA.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_BOTH ) detStt.bAOwaveA = YES;
	}

	if(waveChangeB)	{
		if(gUVD_SETUP.ChannelSetB.btAutoZero == UVD_CHANNEL_AUTOZERO_WAVE
			|| gUVD_SETUP.ChannelSetB.btAutoZero == UVD_CHANNEL_AUTOZERO_BOTH ) detStt.bAZwaveB = YES;

		if(gUVD_SETUP.ChannelSetB.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_WAVE
			|| gUVD_SETUP.ChannelSetB.btAutoOffset == UVD_CHANNEL_AUTOOFFSET_BOTH ) detStt.bAOwaveB = YES;
	}

	detStt.bAutozeroA = detStt.bAutozeroB = gUVD_EVENT[curStep].btAutoZero;

// mark out	// 출력 - 지금동작
	if( gUVD_EVENT[curStep].btMarkOut != CONTACT_IGNORE )
	{
		remoteAction(OFF, OFF , gUVD_EVENT[curStep].btMarkOut); // 기존 구문임.
	}
}

int diagFunc()
{

	static unsigned char seq = DIAG_SEQ_MOVE_WAVE;
	static unsigned char diag_seq = UVD_DIAG_POWER_CHECK;
	static float fOffEnergy=0,fOnEnergy=0;
	static short timeCnt=0;

	static unsigned char oldDiagSeq = -1;


	if(detStt.diagStart == YES) {
		detStt.diagStart = NO;
		timeCnt = 0;
		seq = DIAG_SEQ_MOVE_WAVE;
		diag_seq = UVD_DIAG_POWER_CHECK;
	}

	if (oldDiagSeq != diag_seq) {
		DEBUG_I(DBG_LEVEL_INFO, "() Seq [%d, %s]\r\n", diag_seq, diagnostic_mode_str(diag_seq));
		oldDiagSeq = diag_seq;
	}
	
	switch(diag_seq) {
		case UVD_DIAG_POWER_CHECK:
			if(gUVD_SETUP.DiagnosisSet.btVCCCheck == YES) {
				gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_POWER_V5D_CHECK;
				gUVD_DIAGDATA.btPass = OK;	// detDiag.checkVolt[4];
				gUVD_DIAGDATA.fValue = 5.0;	// detDiag.sysVolt[4];
				sendDiagdata();
				gUVD_SETUP.DiagnosisSet.btVCCCheck = NO;
			}
			if(gUVD_SETUP.DiagnosisSet.btVDDCheck == YES) {
				gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_POWER_V15V_CHECK;
				gUVD_DIAGDATA.btPass = OK;	// detDiag.checkVolt[5];
				gUVD_DIAGDATA.fValue = 15;	// detDiag.sysVolt[5];
				sendDiagdata();
				gUVD_SETUP.DiagnosisSet.btVDDCheck = NO;
			}
			if(gUVD_SETUP.DiagnosisSet.btVSSCheck == YES) {
				gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_POWER_NV15D_CHECK;
				gUVD_DIAGDATA.btPass = OK;	// detDiag.checkVolt[6];
				gUVD_DIAGDATA.fValue = 15;	// detDiag.sysVolt[6];
				sendDiagdata();
				gUVD_SETUP.DiagnosisSet.btVSSCheck = NO;
			}
			if(gUVD_SETUP.DiagnosisSet.btDCCheck == YES) {
				gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_POWER_V12D_CHECK;
				gUVD_DIAGDATA.btPass = OK;	// detDiag.checkVolt[7];
				gUVD_DIAGDATA.fValue = 12;	// detDiag.sysVolt[7];
				sendDiagdata();
				gUVD_SETUP.DiagnosisSet.btDCCheck = NO;
			}
			if(gUVD_SETUP.DiagnosisSet.btWPowerCheck == YES) {		// Tung
				gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_POWER_WPOWER_CHECK;
				gUVD_DIAGDATA.btPass = OK;	// detDiag.checkVolt[5];	// 수정
				gUVD_DIAGDATA.fValue = 5.0;	// detDiag.sysVolt[5];		// 수정
				sendDiagdata();
				gUVD_SETUP.DiagnosisSet.btWPowerCheck = NO;
			}
			diag_seq = UVD_DIAG_SIGNAL1_CHECK;
			break;
		case UVD_DIAG_SIGNAL1_CHECK:
			if(gUVD_SETUP.DiagnosisSet.btSignal1Check == YES) {
				gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_SIGNAL1_CHECK;
				gUVD_DIAGDATA.btPass = 1;
				gUVD_DIAGDATA.fValue = 0.0;
				sendDiagdata();
				gUVD_SETUP.DiagnosisSet.btSignal1Check = NO;
			}
			else diag_seq = UVD_DIAG_SIGNAL2_CHECK;
			break;
		case UVD_DIAG_SIGNAL2_CHECK:
			if(gUVD_SETUP.DiagnosisSet.btSignal2Check == YES) {
				gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_SIGNAL2_CHECK;
				gUVD_DIAGDATA.btPass = 1;
				gUVD_DIAGDATA.fValue = 0.0;
				sendDiagdata();
				gUVD_SETUP.DiagnosisSet.btSignal2Check = NO;
			}
			else diag_seq = UVD_DIAG_D2LAMP_CHECK;
			break;
		case UVD_DIAG_D2LAMP_CHECK:
// lamp off --> 대기1초(완전히 오프시킴) --> 파장 254로 이동 --> 광량 측정(ref) --> lamp on --> 광량 측정(ref)
			if(gUVD_SETUP.DiagnosisSet.btD2LampON == YES) {
				switch(seq) {
					case DIAG_SEQ_MOVE_WAVE:
						motorInitSet(DIAG_D2LAMP_CHECK_WAVE, DIAG_D2LAMP_CHECK_WAVE, DIAG_LAMP_CHECK_CUTFILTER);
						D2lampCon(OFF);
//						sleep(2);
						seq = DIAG_SEQ_LAMP_OFF;
						timeCnt = 0;
						break;
					case DIAG_SEQ_LAMP_OFF:
						if(++timeCnt > DIAG_LAMP_CHECK_WAIT_CNT) {	// 1초 대기
							if(detMotorState.sGMCurStep == GM_wave[DIAG_D2LAMP_CHECK_WAVE]) {
								fOffEnergy = gUVD_STATE.SignalA.fReferenceE;
								timeCnt = 0;
								D2lampCon(ON);
								seq = DIAG_SEQ_LAMP_ON;
							}
						}
						break;
					case DIAG_SEQ_LAMP_ON:
						if(gUVD_STATE.btD2Lamp == UVDMSG_VALUE_D2LAMP_ON) {
							if(++timeCnt > DIAG_LAMP_CHECK_WAIT_CNT) {
								fOnEnergy = gUVD_STATE.SignalA.fReferenceE;
								seq = DIAG_SEQ_ENERGY_CHK;
								timeCnt = 0;
							}
						}
						break;
					case DIAG_SEQ_ENERGY_CHK:
						if(fOnEnergy > fOffEnergy) {
							gUVD_DIAGDATA.btPass = PASS;
						}
						else {
							gUVD_DIAGDATA.btPass = REJECT;
						}
						gUVD_DIAGDATA.fValue = 0.0;
						gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_D2LAMP_CHECK;
						sendDiagdata();
						gUVD_SETUP.DiagnosisSet.btD2LampON = NO;
						diag_seq = UVD_DIAG_WLAMP_CHECK;
						seq = DIAG_SEQ_MOVE_WAVE;
						break;
				}
			}
			else diag_seq = UVD_DIAG_WLAMP_CHECK;
			break;
		case UVD_DIAG_WLAMP_CHECK:
// lamp off --> 파장 720로 이동 --> 광량 측정 --> lamp on --> 광량 측정
			if(gUVD_SETUP.DiagnosisSet.btWLampON == YES) {
				switch(seq) {
					case DIAG_SEQ_MOVE_WAVE:
						motorInitSet(DIAG_WLAMP_CHECK_WAVE, DIAG_WLAMP_CHECK_WAVE, DIAG_LAMP_CHECK_CUTFILTER);
						WlampCon(UVDMSG_VALUE_WLAMP_OFF);
						seq = DIAG_SEQ_LAMP_OFF;
						timeCnt = 0;
						break;
					case DIAG_SEQ_LAMP_OFF:
						if(++timeCnt > DIAG_LAMP_CHECK_WAIT_CNT) {
							if(detMotorState.sGMCurStep == GM_wave[DIAG_WLAMP_CHECK_WAVE]) {
								fOffEnergy = gUVD_STATE.SignalA.fReferenceE;
								timeCnt = 0;
								WlampCon(UVDMSG_VALUE_WLAMP_ON);
								seq = DIAG_SEQ_LAMP_ON;
							}
						}
						break;
					case DIAG_SEQ_LAMP_ON:
						if(++timeCnt > DIAG_LAMP_CHECK_WAIT_CNT) {
							fOnEnergy = gUVD_STATE.SignalA.fReferenceE;
							timeCnt = 0;
							seq = DIAG_SEQ_ENERGY_CHK;
						}
						break;
					case DIAG_SEQ_ENERGY_CHK:
						if(fOnEnergy > fOffEnergy) {
							gUVD_DIAGDATA.btPass = PASS;
						}
						else {
							gUVD_DIAGDATA.btPass = REJECT;
						}
						gUVD_DIAGDATA.fValue = 0.0;
						gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_WLAMP_CHECK;
						sendDiagdata();
						gUVD_SETUP.DiagnosisSet.btWLampON = NO;
						diag_seq = UVD_DIAG_CUTFILTER_CHECK;
						seq = DIAG_SEQ_MOVE_WAVE;
						break;
				}
			}
			else diag_seq = UVD_DIAG_CUTFILTER_CHECK;
			break;
		case UVD_DIAG_CUTFILTER_CHECK:
// cutfilter 0번,파장 254로 이동 --> 광량 측정 --> cutfilter 1번 --> 광량 측정 --> cutfilter 0번,파장 486로 이동
// --> cutfilter 2번 --> 광량 측정
			if(gUVD_SETUP.DiagnosisSet.btCutFilterCheck == YES) {
				switch(seq) {
					case DIAG_SEQ_MOVE_CUTFILTER_NONE:
//cutfilter 0번,파장 254로 이동
						motorInitSet(DIAG_CUTFILTER_CHECK_WAVE_1ST, DIAG_CUTFILTER_CHECK_WAVE_1ST, CUTFILTER_NONE);
						if(gUVD_STATE.btD2Lamp == UVDMSG_VALUE_D2LAMP_ON) {
							seq = DIAG_SEQ_MOVE_CUTFILTER_1ST;
						}
						break;
					case DIAG_SEQ_MOVE_CUTFILTER_1ST:
// 광량 측정 (NONE) --> move 1st
						if(++timeCnt > 500) {
							if(detMotorState.sGMCurStep == GM_wave[DIAG_CUTFILTER_CHECK_WAVE_1ST] && detMotorState.btFMCurStep == FM_POS_NONE) {

								//DEBUG_I(DBG_LEVEL_INFO, "() DIAG_SEQ_MOVE_CUTFILTER_1ST\n");
								
								fOffEnergy = gUVD_STATE.SignalA.fReferenceE;
								motorInitSet(DIAG_CUTFILTER_CHECK_WAVE_1ST, DIAG_CUTFILTER_CHECK_WAVE_1ST, CUTFILTER_1ST);
								timeCnt = 0;
								seq = DIAG_SEQ_ENERGY_CHK_1ST;
							}
						}
						break;
					case DIAG_SEQ_ENERGY_CHK_1ST:
// 광량 측정 (1st) --> move None
						if(++timeCnt > 500) {
							if(detMotorState.sGMCurStep == GM_wave[DIAG_CUTFILTER_CHECK_WAVE_1ST] && detMotorState.btFMCurStep == FM_POS_1ST) {
								fOnEnergy = gUVD_STATE.SignalA.fReferenceE;
								if(fOnEnergy < fOffEnergy) { // 정상 다음으로 : 컷필터를 사용하면 광량이 낮아져야 한다
									motorInitSet(DIAG_CUTFILTER_CHECK_WAVE_2ND, DIAG_CUTFILTER_CHECK_WAVE_2ND, CUTFILTER_NONE);
									timeCnt = 0;
									seq = DIAG_SEQ_MOVE_CUTFILTER_2ND;
								}
								else { // error
									gUVD_DIAGDATA.btPass = REJECT;
									gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_CUTFILTER_CHECK;
									gUVD_DIAGDATA.fValue = 0.0;
									sendDiagdata();
									timeCnt = 0;
									seq = DIAG_SEQ_MOVE_CUTFILTER_NONE;
									diag_seq = UVD_DIAG_CALIBRATION_CHECK;
									WlampCon(UVDMSG_VALUE_WLAMP_OFF);
								}
							}
						}
						break;
					case DIAG_SEQ_MOVE_CUTFILTER_2ND:
						if(++timeCnt > 500) {
							if(detMotorState.sGMCurStep == GM_wave[DIAG_CUTFILTER_CHECK_WAVE_2ND] && detMotorState.btFMCurStep == FM_POS_NONE) {
								fOffEnergy = gUVD_STATE.SignalA.fReferenceE;
								motorInitSet(DIAG_CUTFILTER_CHECK_WAVE_2ND, DIAG_CUTFILTER_CHECK_WAVE_2ND, CUTFILTER_2ND);
								timeCnt = 0;
								seq = DIAG_SEQ_ENERGY_CHK_2ND;
							}
						}
						break;
					case DIAG_SEQ_ENERGY_CHK_2ND:
						if(++timeCnt > 500) {
							if(detMotorState.sGMCurStep == GM_wave[DIAG_CUTFILTER_CHECK_WAVE_2ND] && detMotorState.btFMCurStep == FM_POS_2ND) {
								fOnEnergy = gUVD_STATE.SignalA.fReferenceE;
								if(fOnEnergy < fOffEnergy) { // 정상 : 컷필터를 사용하면 광량이 낮아져야 한다.
									gUVD_DIAGDATA.btPass = PASS;
								}
								else { // error
									gUVD_DIAGDATA.btPass = REJECT;
								}
								gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_CUTFILTER_CHECK;
								gUVD_DIAGDATA.fValue = 0.0;
								sendDiagdata();
								timeCnt = 0;
								seq = DIAG_SEQ_MOVE_CUTFILTER_NONE;
								diag_seq = UVD_DIAG_CALIBRATION_CHECK;
								WlampCon(UVDMSG_VALUE_WLAMP_OFF);
							}
						}
						break;
				}
			}
			else diag_seq = UVD_DIAG_CALIBRATION_CHECK;
			break;
		case UVD_DIAG_CALIBRATION_CHECK:
			if(gUVD_SETUP.DiagnosisSet.btCalibration == YES) {
				switch(seq) {
					case UVD_DIAG_CAL_DIAG_SEQ:
						if( Diagnostic(DIAG_ALL) ) {
							seq = UVD_DIAG_CAL_CAL_SEQ ;
						}
						break;
					case UVD_DIAG_CAL_CAL_SEQ:
						if( Calibrate() ) {
							gUVD_DIAGDATA.uDiagnosis = UVD_DIAG_CALIBRATION_CHECK;
							gUVD_DIAGDATA.btPass = PASS;
							gUVD_DIAGDATA.fValue = 0.0;
							sendDiagdata();
							
							//DEBUG_I(DBG_LEVEL_INFO, "() Calibrate() Pass!! \n");
							
							seq = UVD_DIAG_CAL_DIAG_SEQ ;
							diag_seq = UVD_DIAG_POWER_CHECK;
							readyInitSet();
						}
						break;
				}
			}
			else {
				seq = UVD_DIAG_CAL_DIAG_SEQ ;
				diag_seq = UVD_DIAG_POWER_CHECK;
				readyInitSet();
			}

			return 1;
			break;
	}
	return 0;
}

