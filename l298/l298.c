/* l298.c - STMicroelectronics L298 dual full-bridge DC motor driver library
 *
 * STMicroelectronics L298 dual full-bridge DC motor driver library.
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#include "l298.h"

#include <avr/io.h>
#include <stdlib.h>

#include "gpio.h"
#include "pwm.h"


#define L298_INPUT1             GPIO_PIN_PC2
#define L298_INPUT2             GPIO_PIN_PC3
#define L298_INPUT3             GPIO_PIN_PC4
#define L298_INPUT4             GPIO_PIN_PC5

#define L298_ENABLE_A           PWM_OUTPUT_CH0
#define L298_ENABLE_B           PWM_OUTPUT_CH1


typedef struct {
    gpio_pin_t pin;
    gpio_level_t level;
} l298_input_t;


const uint8_t l298_pwm_output[] = {
        255,  25,  50,  75, 100, 125, 150, 175, 200, 225, 255
};


/*******************************************************************************
 *
 * l298_init() - initialize L298 DC motor driver module
 *
 * Description:
 *  Initialize L298 DC motor driver module. L298 outputs are generated on two
 *  independent channels.
 *
 * Parameters:
 *
 * Returns:
 *
 ******************************************************************************/
void l298_init(void)
{
    /* Initialize GPIO pins for motor direction control. */
    gpio_init(L298_INPUT1, GPIO_MODE_OUTPUT);
    gpio_init(L298_INPUT2, GPIO_MODE_OUTPUT);
    gpio_init(L298_INPUT3, GPIO_MODE_OUTPUT);
    gpio_init(L298_INPUT4, GPIO_MODE_OUTPUT);

    /* Initialize PWM module for motor speed control. */
    pwm_init();
}


/*******************************************************************************
 *
 * l298_set() - set L298 DC motor speed
 *
 * Description:
 *  Set L298 DC motor speed. For each rotation direction 10 different speed
 *  values can be set. The sign of the speed value defines rotation direction.
 *
 * Parameters:
 *  motor                   L298 motor output
 *  speed                   motor speed (0 - ±10)
 *
 * Returns:
 *  L298_ERROR_OK           - success
 *  L298_ERROR_MOTOR        - failure, invalid motor output
 *  L298_ERROR_SPEED        - failure, invalid motor speed
 *
 ******************************************************************************/
l298_error_t l298_set(l298_motor_t motor, int8_t speed)
{
    l298_input_t input1;
    l298_input_t input2;

    pwm_output_t enable;

    /* Check for valid motor speed value. */
    if (abs(speed) > L298_SPEED_MAX)
    {
        return L298_ERROR_SPEED;
    }

    /* Get motor control pins and signals. */
    switch (motor)
    {
        case L298_MOTOR_A:
            input1.pin = L298_INPUT1;
            input2.pin = L298_INPUT2;

            enable = PWM_OUTPUT_CH0;
            break;

        case L298_MOTOR_B:
            input1.pin = L298_INPUT3;
            input2.pin = L298_INPUT4;

            enable = PWM_OUTPUT_CH1;
            break;

        default:
            return L298_ERROR_MOTOR;
    }

    /* Set motor control pin levels. */
    if (speed > 0)
    {
        input1.level = GPIO_LEVEL_HIGH;
        input2.level = GPIO_LEVEL_LOW;
    }
    else if (speed < 0)
    {
        input1.level = GPIO_LEVEL_LOW;
        input2.level = GPIO_LEVEL_HIGH;
    }
    else
    {
        input1.level = GPIO_LEVEL_LOW;
        input2.level = GPIO_LEVEL_LOW;
    }

    /* Set motor rotation direction. */
    gpio_set(input1.pin, input1.level);
    gpio_set(input2.pin, input2.level);

    /* Set motor speed. */
    pwm_set(enable, l298_pwm_output[abs(speed)]);

    return L298_ERROR_OK;
}
