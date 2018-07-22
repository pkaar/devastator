/* pwm.c - Microchip ATmega328P PWM library
 *
 * Microchip ATmega328P PWM library.
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#include "pwm.h"

#include <avr/io.h>


/*******************************************************************************
 *
 * pwm_init() - initialize PWM module
 *
 * Description:
 *  Initialize PWM module. PWM outputs are generated on two channels, where
 *  output channel 0 is pin PD5 and output channel 1 is pin PD6. PWM base
 *  frequency is given by f = F_CPU / 32640.
 *
 * Parameters:
 *
 * Returns:
 *
 ******************************************************************************/
void pwm_init(void)
{
    /* Set output on compare match and enable phase correct PWM. */
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);

    /* Disable interrupt generation. */
    TIMSK0 &= ~((1 << OCIE0B) | (1 << OCIE0A) | (1 << TOIE0));

    /* Clear initial counter value. */
    TCNT0 = 0x00;

    /* Set PWM duty cycle on all outputs to zero. */
    OCR0A = 0x00;
    OCR0B = 0x00;

    /* Set PWM channels to output. */
    DDRD |= (1 << DDD6) | (1 << DDD5);

    /* Set PWM clock source. */
    TCCR0B |= (1 << CS01) | (1 << CS00);
}


/*******************************************************************************
 *
 * pwm_set() - set PWM duty cycle
 *
 * Description:
 *  Set PWM duty cycle. Input values from 0 - 255 are mapped to PWM duty cycles
 *  of 0 - 100 %.
 *
 * Parameters:
 *  output                  PWM output channel
 *  duty                    duty cycle (0 - 255)
 *
 * Returns:
 *  PWM_ERROR_OK            - success
 *  PWM_ERROR_OUTPUT        - failure, invalid PWM output channel
 *
 ******************************************************************************/
pwm_error_t pwm_set(pwm_output_t output, uint8_t duty)
{
    /* Set PWM duty cycle. */
    switch (output)
    {
        case PWM_OUTPUT_CH0:
            OCR0B = duty;
            break;

        case PWM_OUTPUT_CH1:
            OCR0A = duty;
            break;

        default:
            return PWM_ERROR_OUTPUT;
    }

    return PWM_ERROR_OK;
}
