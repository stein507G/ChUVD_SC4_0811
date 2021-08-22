/*
 * UV_MotorCtrl.h
 *
 *  Created on: 2020. 8. 6.
 *      Author: ksyi
 */

#ifndef UV_MOTORCTRL_H_
#define UV_MOTORCTRL_H_

void Motor_InitValue(void);
void GM_step(unsigned char dir);
void FM_go_pos();
void FM_step();
void Motor_TimerCtrl(void);
void Motor_Ctrl_Mode(void);	// 0.125mS Ctrl
int Motor_Ctrl_Command(void);

#endif /* UV_MOTORCTRL_H_ */
