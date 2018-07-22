/* timer.c - Microchip ATmega328P timer library
 *
 * Microchip ATmega328P timer library.
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>


#define TIMER_PRESCALER_VALUE       256


static uint8_t tccr1b;

static timer_callback_t timer_callback;


/*******************************************************************************
 *
 * timer_init() - initialize timer module
 *
 * Description:
 *  Initialize timer module. Periodic interrupts are generated according to the
 *  specified period. A callback function is executed when a timer overflow is
 *  detected.
 *
 * Parameters:
 *  period                  timer period
 *  callback                timer callback function
 *
 * Returns:
 *  TIMER_ERROR_OK          - success
 *  TIMER_ERROR_PERIOD      - failure, invalid timer period
 *  TIMER_ERROR_CALLBACK    - failure, invalid timer callback function
 *
 ******************************************************************************/
timer_error_t timer_init(uint16_t period, timer_callback_t callback)
{
    uint32_t ocr1a;

    /* Check for valid timer period. */
    if ((period < TIMER_PERIOD_MIN) || (period > TIMER_PERIOD_MAX))
    {
        return TIMER_ERROR_PERIOD;
    }

    /* Check for valid timer callback function. */
    if (callback == NULL)
    {
        return TIMER_ERROR_CALLBACK;
    }

    timer_callback = callback;

    /* Calculate output compare register value. */
    ocr1a = F_CPU / TIMER_PRESCALER_VALUE * period / 1000 - 1;

    OCR1A = (uint16_t) ocr1a;

    tccr1b = 0x00;

    /* Configure timer mode and prescaler. */
    tccr1b |= (1 << WGM12) | (1 << CS12);

    /* Enable output compare match interrupt. */
    TIMSK1 |= (1 << OCIE1A);

    return TIMER_ERROR_OK;
}


/*******************************************************************************
 *
 * timer_start() - start timer module
 *
 * Description:
 *  Start timer module.
 *
 * Parameters:
 *
 * Returns:
 *
 ******************************************************************************/
void timer_start(void)
{
    TCCR1B = tccr1b;
}


/*******************************************************************************
 *
 * timer_stop() - stop timer module
 *
 * Description:
 *  Stop timer module.
 *
 * Parameters:
 *
 * Returns:
 *
 ******************************************************************************/
void timer_stop(void)
{
    TCCR1B = 0;

    /* Clear timer counter value. */
    TCNT1 = 0;
}


/*******************************************************************************
 *
 * timer_restart() - restart timer module
 *
 * Description:
 *  Restart timer module.
 *
 * Parameters:
 *
 * Returns:
 *
 ******************************************************************************/
void timer_restart(void)
{
    TCCR1B = 0;

    /* Clear timer counter value. */
    TCNT1 = 0;

    TCCR1B = tccr1b;
}


/*******************************************************************************
 *
 * ISR() - timer compare match interrupt service routine
 *
 * Description:
 *  timer compare match interrupt service routine.
 *
 * Parameters:
 *  n/a
 *
 * Returns:
 *  n/a
 *
 ******************************************************************************/
ISR(TIMER1_COMPA_vect)
{
    timer_callback();
}
