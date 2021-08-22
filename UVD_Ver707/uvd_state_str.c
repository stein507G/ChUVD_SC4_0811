#include "uvd_state_str.h"

static char str[32] ={0};

const char* calibrate_str(int code)
{
	memset( str, 0 , strlen(str));
	
	switch(code) {
	case CAL_NONE :
		memcpy( str, "CAL_NONE", strlen("CAL_NONE"));
		break;
	case CAL_CHECK_CALIB :		
		memcpy( str, "CAL_CHECK_CALIB", strlen("CAL_CHECK_CALIB"));
		break;
	case CAL_FIND_SAM_ZERO	:	
		memcpy( str, "CAL_FIND_SAM_ZERO", strlen("CAL_FIND_SAM_ZERO"));
		break;
	case CAL_FIND_W486	:		
		memcpy( str, "CAL_FIND_W486", strlen("CAL_FIND_W486"));
		break;
	case CAL_FIND_W656	:		
		memcpy( str, "CAL_FIND_W656", strlen("CAL_FIND_W656"));
		break;
	case CAL_CAL_PARA	:
		memcpy( str, "CAL_CAL_PARA", strlen("CAL_CAL_PARA"));
		break;
	case CAL_FILE_OPEN_STEP	:
		memcpy( str, "CAL_FILE_OPEN_STEP", strlen("CAL_FILE_OPEN_STEP"));
		break;
	case CAL_SAVE_GRWAVE_STEP	:
		memcpy( str, "CAL_SAVE_GRWAVE_STEP", strlen("CAL_SAVE_GRWAVE_STEP"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;
}


const char* diagnostic_mode_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case UVD_DIAG_POWER_CHECK :
		memcpy( str, "UVD_DIAG_POWER_CHECK", strlen("UVD_DIAG_POWER_CHECK"));
		break;
//		case UVD_DIAG_POWER_V5D_CHECK :		
//			memcpy( str, "UVD_DIAG_POWER_V5D_CHECK", strlen("UVD_DIAG_POWER_V5D_CHECK"));
//			break;
//		case UVD_DIAG_POWER_V15V_CHECK	:	
//			memcpy( str, "UVD_DIAG_POWER_V15V_CHECK", strlen("UVD_DIAG_POWER_V15V_CHECK"));
//			break;
//		case UVD_DIAG_POWER_NV15D_CHECK	:		
//			memcpy( str, "UVD_DIAG_POWER_NV15D_CHECK", strlen("UVD_DIAG_POWER_NV15D_CHECK"));
//			break;
//		case UVD_DIAG_POWER_V12D_CHECK	:		
//			memcpy( str, "UVD_DIAG_POWER_V12D_CHECK", strlen("UVD_DIAG_POWER_V12D_CHECK"));
//			break;
//		case UVD_DIAG_POWER_WPOWER_CHECK	:
//			memcpy( str, "UVD_DIAG_POWER_WPOWER_CHECK", strlen("UVD_DIAG_POWER_WPOWER_CHECK"));
//			break;
	case UVD_DIAG_SIGNAL1_CHECK	:
		memcpy( str, "UVD_DIAG_SIGNAL1_CHECK", strlen("UVD_DIAG_SIGNAL1_CHECK"));
		break;
	case UVD_DIAG_SIGNAL2_CHECK	:
		memcpy( str, "UVD_DIAG_SIGNAL2_CHECK", strlen("UVD_DIAG_SIGNAL2_CHECK"));
		break;
	case UVD_DIAG_D2LAMP_CHECK	:
		memcpy( str, "UVD_DIAG_D2LAMP_CHECK", strlen("UVD_DIAG_D2LAMP_CHECK"));
		break;
	case UVD_DIAG_CALIBRATION_CHECK	:
		memcpy( str, "UVD_DIAG_CALIBRATION_CHECK", strlen("UVD_DIAG_CALIBRATION_CHECK"));
		break;
	case UVD_DIAG_WLAMP_CHECK	:
		memcpy( str, "UVD_DIAG_WLAMP_CHECK", strlen("UVD_DIAG_WLAMP_CHECK"));
		break;
	case UVD_DIAG_CUTFILTER_CHECK	:
		memcpy( str, "UVD_DIAG_CUTFILTER_CHECK", strlen("UVD_DIAG_CUTFILTER_CHECK"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}


const char* diagnostic_seq_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case SEQ_POWER_CHECK :
		memcpy( str, "SEQ_POWER_CHECK", strlen("SEQ_POWER_CHECK"));
		break;
	case SEQ_GRATING_HOME :		
		memcpy( str, "SEQ_GRATING_HOME", strlen("SEQ_GRATING_HOME"));
		break;
	case SEQ_FILTER_HOME	:	
		memcpy( str, "SEQ_FILTER_HOME", strlen("SEQ_FILTER_HOME"));
		break;
	case SEQ_D2_LAMP_ON	:		
		memcpy( str, "SEQ_D2_LAMP_ON", strlen("SEQ_D2_LAMP_ON"));
		break;
	case SEQ_D2_LAMP_CHECK	:		
		memcpy( str, "SEQ_D2_LAMP_CHECK", strlen("SEQ_D2_LAMP_CHECK"));
		break;
	case SEQ_GRATING_SCANNING	:
		memcpy( str, "SEQ_GRATING_SCANNING", strlen("SEQ_GRATING_SCANNING"));
		break;
	case SEQ_ERROR_OCCUR	:
		memcpy( str, "SEQ_ERROR_OCCUR", strlen("SEQ_ERROR_OCCUR"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}

const char* init_sequence_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case INIT_LED_BLINK :
		memcpy( str, "INIT_LED_BLINK", strlen("INIT_LED_BLINK"));
		break;
	case INIT_OFFSET_VALUE_SAVE :		
		memcpy( str, "INIT_OFFSET_VALUE_SAVE", strlen("INIT_OFFSET_VALUE_SAVE"));
		break;
	case INIT_DIAGNOSTIC	:	
		memcpy( str, "INIT_DIAGNOSTIC", strlen("INIT_DIAGNOSTIC"));
		break;
	case INIT_CALIBRATE	:		
		memcpy( str, "INIT_CALIBRATE", strlen("INIT_CALIBRATE"));
		break;
	case INIT_BACKLASH_TEST	:		
		memcpy( str, "INIT_BACKLASH_TEST", strlen("INIT_BACKLASH_TEST"));
		break;
	case INIT_W_LAMP_TEST	:
		memcpy( str, "INIT_W_LAMP_TEST", strlen("INIT_W_LAMP_TEST"));
		break;
	case INIT_CUTFILTER_TEST	:
		memcpy( str, "INIT_CUTFILTER_TEST", strlen("INIT_CUTFILTER_TEST"));
		break;
	case INIT_DA_AD_TEST	:
		memcpy( str, "INIT_DA_AD_TEST", strlen("INIT_DA_AD_TEST"));
		break;
	case INIT_HOLMIUM_TEST	:
		memcpy( str, "INIT_HOLMIUM_TEST", strlen("INIT_HOLMIUM_TEST"));
		break;
	case INIT_NOISE_DRIFT_TEST	:
		memcpy( str, "INIT_NOISE_DRIFT_TEST", strlen("INIT_NOISE_DRIFT_TEST"));
		break;
	case INIT_READYINIT_SET	:
		memcpy( str, "INIT_READYINIT_SET", strlen("INIT_READYINIT_SET"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}

const char* motor_mode_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case GM_OFF :
		memcpy( str, "GM_OFF", strlen("GM_OFF"));
		break;
	case GM_FREE_ACT :		
		memcpy( str, "GM_FREE_ACT", strlen("GM_FREE_ACT"));
		break;
	case GM_SINGLE_ACT	:	
		memcpy( str, "GM_SINGLE_ACT", strlen("GM_SINGLE_ACT"));
		break;
	case GM_DUAL_ACT	:		
		memcpy( str, "GM_DUAL_ACT", strlen("GM_DUAL_ACT"));
		break;
	case GM_SCAN_ACT	:		
		memcpy( str, "GM_SCAN_ACT", strlen("GM_SCAN_ACT"));
		break;
	case GM_HOME_ACT	:
		memcpy( str, "GM_HOME_ACT", strlen("GM_HOME_ACT"));
		break;
	case GM_CAL_SCAN_ACT	:
		memcpy( str, "GM_CAL_SCAN_ACT", strlen("GM_CAL_SCAN_ACT"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}


const char* uvd_act_state_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case UVD_STATE_INITIALIZING :
		memcpy( str, "UVD_STATE_INITIALIZING", strlen("UVD_STATE_INITIALIZING"));
		break;
	case UVD_STATE_READYINIT :		
		memcpy( str, "UVD_STATE_READYINIT", strlen("UVD_STATE_READYINIT"));
		break;
	case UVD_STATE_RUN_MODE	:	
		memcpy( str, "UVD_STATE_RUN_MODE", strlen("UVD_STATE_RUN_MODE"));
		break;
	case UVD_STATE_SCANNING_MODE	:		
		memcpy( str, "UVD_STATE_SCANNING_MODE", strlen("UVD_STATE_SCANNING_MODE"));
		break;
	case UVD_STATE_DIAGNOSIS_MODE	:		
		memcpy( str, "UVD_STATE_DIAGNOSIS_MODE", strlen("UVD_STATE_DIAGNOSIS_MODE"));
		break;
	case UVD_STATE_FAULT	:		
		memcpy( str, "UVD_STATE_FAULT", strlen("UVD_STATE_FAULT"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}

const char* uvd_error_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case UVD_ERR_NONE :
		memcpy( str, "UVD_ERR_NONE", strlen("UVD_ERR_NONE"));
		break;
	case UVD_ERR_INVALID_CONFIG :		
		memcpy( str, "UVD_ERR_INVALID_CONFIG", strlen("UVD_ERR_INVALID_CONFIG"));
		break;
	case UVD_ERR_INVALID_SETUP	:	
		memcpy( str, "UVD_ERR_INVALID_SETUP", strlen("UVD_ERR_INVALID_SETUP"));
		break;
	case UVD_ERR_INVALID_SERVICE	:		
		memcpy( str, "UVD_ERR_INVALID_SERVICE", strlen("UVD_ERR_INVALID_SERVICE"));
		break;
	case UVD_ERR_D2_LAMP	:		
		memcpy( str, "UVD_ERR_D2_LAMP", strlen("UVD_ERR_D2_LAMP"));
		break;
	case UVD_ERR_LEAK	:		
		memcpy( str, "UVD_ERR_LEAK", strlen("UVD_ERR_LEAK"));
		break;
	case UVD_ERR_POWER	:		
		memcpy( str, "UVD_ERR_POWER", strlen("UVD_ERR_POWER"));
		break;
	case UVD_ERR_INITIAL	:		
		memcpy( str, "UVD_ERR_INITIAL", strlen("UVD_ERR_INITIAL"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}

const char* uvd_setup_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case UVD_SETUP_NONE :
		memcpy( str, "UVD_SETUP_NONE", strlen("UVD_SETUP_NONE"));
		break;
	case UVD_SETUP_AUTO_ZERO :		
		memcpy( str, "UVD_SETUP_AUTO_ZERO", strlen("UVD_SETUP_AUTO_ZERO"));
		break;
	case UVD_SETUP_ACQUISITION_START	:	
		memcpy( str, "UVD_SETUP_ACQUISITION_START", strlen("UVD_SETUP_ACQUISITION_START"));
		break;
	case UVD_SETUP_SCAN_START	:		
		memcpy( str, "UVD_SETUP_SCAN_START", strlen("UVD_SETUP_SCAN_START"));
		break;
	case UVD_SETUP_DIAGNOSIS_START	:		
		memcpy( str, "UVD_SETUP_DIAGNOSIS_START", strlen("UVD_SETUP_DIAGNOSIS_START"));
		break;
	case UVD_SETUP_STOP	:		
		memcpy( str, "UVD_SETUP_STOP", strlen("UVD_SETUP_STOP"));
		break;
	case UVD_SETUP_RESET_ERROR	:		
		memcpy( str, "UVD_SETUP_RESET_ERROR", strlen("UVD_SETUP_RESET_ERROR"));
		break;
	case UVD_SETUP_READY_INIT	:		
		memcpy( str, "UVD_SETUP_READY_INIT", strlen("UVD_SETUP_READY_INIT"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}

const char* uvd_wave_mode_str(int code)
{

	memset( str, 0 , strlen(str));
	
	switch(code) {
	case UVD_SINGLE_WAVE_MODE :
		memcpy( str, "UVD_SINGLE_WAVE_MODE", strlen("UVD_SINGLE_WAVE_MODE"));
		break;
	case UVD_DUAL_WAVE_MODE :		
		memcpy( str, "UVD_DUAL_WAVE_MODE", strlen("UVD_DUAL_WAVE_MODE"));
		break;
	case UVD_SCAN_WAVE_MODE	:	
		memcpy( str, "UVD_SCAN_WAVE_MODE", strlen("UVD_SCAN_WAVE_MODE"));
		break;
	default:
		memcpy( str, "UNKNOWN", strlen("UNKNOWN"));
		break;
	}
	return str;

}



