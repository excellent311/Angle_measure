/*
 * delay.h
 *
 *  Created on: 2015-11-5
 *      Author: Administrator
 */

#ifndef DELAY_H_
#define DELAY_H_

#define CPU          (16000000)
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))


#endif /* DELAY_H_ */
