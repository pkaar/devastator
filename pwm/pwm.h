/* pwm.h - Microchip ATmega328P PWM library
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#ifndef PWM_H_
#define PWM_H_


#include <stdint.h>


typedef enum {
    PWM_OUTPUT_CH0,
    PWM_OUTPUT_CH1
} pwm_output_t;

typedef enum {
    PWM_ERROR_OK,
    PWM_ERROR_OUTPUT
} pwm_error_t;


void pwm_init(void);
pwm_error_t pwm_set(pwm_output_t output, uint8_t duty);


#endif /* PWM_H_ */
