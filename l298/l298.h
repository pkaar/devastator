/* l298.h - STMicroelectronics L298 dual full-bridge DC motor driver library
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#ifndef L298_H_
#define L298_H_


#include <stdint.h>


#define L298_SPEED_MAX          10


typedef enum {
    L298_MOTOR_A,
    L298_MOTOR_B
} l298_motor_t;

typedef enum {
    L298_ERROR_OK,
    L298_ERROR_MOTOR,
    L298_ERROR_SPEED
} l298_error_t;


void l298_init(void);
l298_error_t l298_set(l298_motor_t motor, int8_t speed);


#endif /* L298_H_ */
