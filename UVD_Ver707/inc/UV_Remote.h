/*
 * UV_Remote.h
 *
 *  Created on: 2020. 8. 6.
 *      Author: ksyi
 */

#ifndef UV_REMOTE_H_
#define UV_REMOTE_H_

void remoteCheckClearInit(unsigned char Enable);
void remoteCheck(void);
void Remote_Command(char Comm, unsigned long arg);

#endif /* UV_REMOTE_H_ */
