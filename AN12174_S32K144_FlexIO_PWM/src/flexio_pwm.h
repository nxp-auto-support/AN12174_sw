/*
 * flexio_pwm.h
 *
 *  Created on: Apr 30, 2018
 *      Author: nxa19513
 */

#ifndef FLEXIO_PWM_H_
#define FLEXIO_PWM_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define FLEXIO_CLK (48000000)
#define MAX_FREQ   (FLEXIO_CLK/2)   // 1MHz 2MHz
#define MIN_FREQ   (FLEXIO_CLK/256) // 8KHz 187.5KHz

 void flexio_pwm_init(uint32_t freq, uint8_t duty);
 void flexio_init(void);
 void flexio_pwm_start(void);
 void flexio_pwm_stop(void);



#endif /* FLEXIO_PWM_H_ */
