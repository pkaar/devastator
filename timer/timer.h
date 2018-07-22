/* timer.h - Microchip ATmega328P timer library
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#ifndef TIMER_H_
#define TIMER_H_


#include <stddef.h>
#include <stdint.h>


#define TIMER_PERIOD_MIN        1
#define TIMER_PERIOD_MAX        1000


typedef enum {
    TIMER_ERROR_OK,
    TIMER_ERROR_PERIOD,
    TIMER_ERROR_CALLBACK
} timer_error_t;


typedef void (*timer_callback_t)(void);


timer_error_t timer_init(uint16_t period, timer_callback_t callback);
void timer_start(void);
void timer_stop(void);
void timer_restart(void);


#endif /* TIMER_H_ */
