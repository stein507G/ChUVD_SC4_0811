#ifndef _CHROGEN_LC_UVD_STATE_H_
#define _CHROGEN_LC_UVD_STATE_H_

#include "config.h"

const char* calibrate_str(int code);

const char* diagnostic_mode_str(int code);
const char* diagnostic_seq_str(int code);

const char* init_sequence_str(int code);

const char* motor_mode_str(int code);

const char* uvd_act_state_str(int code);
const char* uvd_error_str(int code);
const char* uvd_setup_str(int code);
const char* uvd_wave_mode_str(int code);


#endif	// _CHROGEN_LC_UVD_STATE_H_

