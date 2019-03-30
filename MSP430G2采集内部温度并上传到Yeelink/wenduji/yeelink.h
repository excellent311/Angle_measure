/*
 * yeelink.h
 *
 *  Created on: 2015-11-5
 *      Author: Administrator
 */

#ifndef YEELINK_H_
#define YEELINK_H_
#include"msp430g2553.h"
#include"stdio.h"
#include"string.h"
#include"usart.h"
#include"delay.h"

void yeelink_send(const char *device_id,const char *sensors_id,float value);

#endif /* YEELINK_H_ */
