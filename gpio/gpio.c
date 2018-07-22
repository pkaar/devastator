/* gpio.c - Microchip ATmega328P GPIO library
 *
 * Microchip ATmega328P GPIO library.
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#include "gpio.h"

#include <avr/io.h>


typedef struct {
    gpio_pin_t pin;
    uint8_t index;
    volatile uint8_t* reg_ddr;
    volatile uint8_t* reg_port;
    volatile uint8_t* reg_pin;
} gpio_config_t;


const gpio_config_t gpio_config[] = {
        {GPIO_PIN_PB0, 0, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PB1, 1, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PB2, 2, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PB3, 3, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PB4, 4, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PB5, 5, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PB6, 6, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PB7, 7, &DDRB, &PORTB, &PINB},
        {GPIO_PIN_PC0, 0, &DDRC, &PORTC, &PINC},
        {GPIO_PIN_PC1, 1, &DDRC, &PORTC, &PINC},
        {GPIO_PIN_PC2, 2, &DDRC, &PORTC, &PINC},
        {GPIO_PIN_PC3, 3, &DDRC, &PORTC, &PINC},
        {GPIO_PIN_PC4, 4, &DDRC, &PORTC, &PINC},
        {GPIO_PIN_PC5, 5, &DDRC, &PORTC, &PINC},
        {GPIO_PIN_PC6, 6, &DDRC, &PORTC, &PINC},
        {GPIO_PIN_PD0, 0, &DDRD, &PORTD, &PIND},
        {GPIO_PIN_PD1, 1, &DDRD, &PORTD, &PIND},
        {GPIO_PIN_PD2, 2, &DDRD, &PORTD, &PIND},
        {GPIO_PIN_PD3, 3, &DDRD, &PORTD, &PIND},
        {GPIO_PIN_PD4, 4, &DDRD, &PORTD, &PIND},
        {GPIO_PIN_PD5, 5, &DDRD, &PORTD, &PIND},
        {GPIO_PIN_PD6, 6, &DDRD, &PORTD, &PIND},
        {GPIO_PIN_PD7, 7, &DDRD, &PORTD, &PIND}
};


/*******************************************************************************
 *
 * gpio_init() - initialize GPIO pin
 *
 * Description:
 *  Initialize GPIO pin. GPIO pins can be initialized to operate as logical
 *  inputs or outputs.
 *
 * Parameters:
 *  pin                     GPIO pin
 *  mode                    operation mode
 *
 * Returns:
 *  GPIO_ERROR_OK           - success
 *  GPIO_ERROR_PIN          - failure, invalid GPIO pin
 *  GPIO_ERROR_MODE         - failure, invalid operation mode
 *
 ******************************************************************************/
gpio_error_t gpio_init(gpio_pin_t pin, gpio_mode_t mode)
{
    uint8_t idx;
    uint8_t gpio_config_size;

    volatile uint8_t* reg_ddr;
    volatile uint8_t* reg_port;

    gpio_config_size = sizeof(gpio_config) / sizeof(gpio_config[0]);

    /* Check for valid GPIO pin. */
    for (idx = 0; idx < gpio_config_size; idx++)
    {
        if (gpio_config[idx].pin == pin)
        {
            break;
        }
    }

    /* Invalid GPIO pin when loop index matches GPIO config table size. */
    if (idx == gpio_config_size)
    {
        return GPIO_ERROR_PIN;
    }

    reg_ddr = gpio_config[idx].reg_ddr;
    reg_port = gpio_config[idx].reg_port;

    /* Set GPIO operation mode. */
    switch (mode)
    {
        case GPIO_MODE_INPUT:
            *reg_ddr &= ~(1 << gpio_config[idx].index);
            break;

        case GPIO_MODE_OUTPUT:
            *reg_ddr |= (1 << gpio_config[idx].index);
            break;

        default:
            return GPIO_ERROR_MODE;
    }

    *reg_port &= ~(1 << gpio_config[idx].index);

    return GPIO_ERROR_OK;
}


/*******************************************************************************
 *
 * gpio_get() - get logical GPIO pin level
 *
 * Description:
 *  Get logical GPIO pin level.
 *
 * Parameters:
 *  pin                     GPIO pin
 *  level                   pin level
 *
 * Returns:
 *  GPIO_ERROR_OK           - success
 *  GPIO_ERROR_PIN          - failure, invalid GPIO pin
 *
 ******************************************************************************/
gpio_error_t gpio_get(gpio_pin_t pin, gpio_level_t* level)
{
    uint8_t idx;
    uint8_t gpio_config_size;

    volatile uint8_t* reg_pin;

    gpio_config_size = sizeof(gpio_config) / sizeof(gpio_config[0]);

    /* Check for valid GPIO pin. */
    for (idx = 0; idx < gpio_config_size; idx++)
    {
        if (gpio_config[idx].pin == pin)
        {
            break;
        }
    }

    /* Invalid GPIO pin when loop index matches GPIO config table size. */
    if (idx == gpio_config_size)
    {
        return GPIO_ERROR_PIN;
    }

    reg_pin = gpio_config[idx].reg_pin;

    /* Get logical GPIO pin level. */
    if (*reg_pin & (1 << gpio_config[idx].index))
    {
        *level = GPIO_LEVEL_HIGH;
    }
    else
    {
        *level = GPIO_LEVEL_LOW;
    }

    return GPIO_ERROR_OK;
}


/*******************************************************************************
 *
 * gpio_set() - set logical GPIO pin level
 *
 * Description:
 *  Set logical GPIO pin level.
 *
 * Parameters:
 *  pin                     GPIO pin
 *  level                   pin level
 *
 * Returns:
 *  GPIO_ERROR_OK           - success
 *  GPIO_ERROR_PIN          - failure, invalid GPIO pin
 *  GPIO_ERROR_MODE         - failure, GPIO pin configured as input
 *  GPIO_ERROR_LEVEL        - failure, invalid GPIO pin level
 *
 ******************************************************************************/
gpio_error_t gpio_set(gpio_pin_t pin, gpio_level_t level)
{
    uint8_t idx;
    uint8_t gpio_config_size;

    volatile uint8_t* reg_ddr;
    volatile uint8_t* reg_port;

    gpio_config_size = sizeof(gpio_config) / sizeof(gpio_config[0]);

    /* Check for valid GPIO pin. */
    for (idx = 0; idx < gpio_config_size; idx++)
    {
        if (gpio_config[idx].pin == pin)
        {
            break;
        }
    }

    /* Invalid GPIO pin when loop index matches GPIO config table size. */
    if (idx == gpio_config_size)
    {
        return GPIO_ERROR_PIN;
    }

    reg_ddr = gpio_config[idx].reg_ddr;
    reg_port = gpio_config[idx].reg_port;

    /* Check if GPIO pin is initialized as input. */
    if (!(*reg_ddr & (1 << gpio_config[idx].index)))
    {
        return GPIO_ERROR_MODE;
    }

    /* Set logical GPIO level. */
    switch (level)
    {
        case GPIO_LEVEL_LOW:
            *reg_port &= ~(1 << gpio_config[idx].index);
            break;

        case GPIO_LEVEL_HIGH:
            *reg_port |= (1 << gpio_config[idx].index);
            break;

        case GPIO_LEVEL_INVERT:
            *reg_port ^= (1 << gpio_config[idx].index);
            break;

        default:
            return GPIO_ERROR_LEVEL;
    }

    return GPIO_ERROR_OK;
}


/*******************************************************************************
 * gpio_config_input() - configure GPIO pin pull-up resistor
 *
 * Description:
 *  Configure GPIO pin pull-up resistor.
 *
 * Parameters:
 *  pin                     GPIO pin
 *  pullup                  pull-up configuration
 *
 * Returns:
 *  GPIO_ERROR_OK           - success
 *  GPIO_ERROR_PIN          - failure, invalid GPIO pin
 *  GPIO_ERROR_MODE         - failure, GPIO pin configured as output
 *  GPIO_ERROR_PULLUP       - failure, invalid GPIO pin pull-up configuration
 *
 ******************************************************************************/
gpio_error_t gpio_config_input(gpio_pin_t pin, gpio_pullup_t pullup)
{
    uint8_t idx;
    uint8_t gpio_config_size;

    volatile uint8_t* reg_ddr;
    volatile uint8_t* reg_port;

    gpio_config_size = sizeof(gpio_config) / sizeof(gpio_config[0]);

    /* Check for valid GPIO pin. */
    for (idx = 0; idx < gpio_config_size; idx++)
    {
        if (gpio_config[idx].pin == pin)
        {
            break;
        }
    }

    /* Invalid GPIO pin when loop index matches GPIO config table size. */
    if (idx == gpio_config_size)
    {
        return GPIO_ERROR_PIN;
    }

    reg_ddr = gpio_config[idx].reg_ddr;
    reg_port = gpio_config[idx].reg_port;

    /* Check if GPIO pin is initialized as output. */
    if (*reg_ddr & (1 << gpio_config[idx].index))
    {
        return GPIO_ERROR_MODE;
    }

    /* Configure GPIO pin pull-up resistor.*/
    switch (pullup)
    {
        case GPIO_PULLUP_DISABLE:
            *reg_port &= ~(1 << gpio_config[idx].index);
            break;

        case GPIO_PULLUP_ENABLE:
            *reg_port |= (1 << gpio_config[idx].index);
            break;

        default:
            return GPIO_ERROR_PULLUP;
    }

    return GPIO_ERROR_OK;
}


/*******************************************************************************
 *
 * gpio_reg_ddr() - get PORT data direction register address
 *
 * Description:
 *  Get PORT data direction register address.
 *
 * Parameters:
 *  pin                     GPIO pin
 *
 * Returns:
 *  > 0                     - PORT data direction register address
 *  NULL                    - failure, invalid GPIO pin
 *
 ******************************************************************************/
volatile uint8_t* gpio_reg_ddr(gpio_pin_t pin)
{
    uint8_t idx;
    uint8_t gpio_config_size;

    gpio_config_size = sizeof(gpio_config) / sizeof(gpio_config[0]);

    /* Check for valid GPIO pin. */
    for (idx = 0; idx < gpio_config_size; idx++)
    {
        if (gpio_config[idx].pin == pin)
        {
            break;
        }
    }

    /* Invalid GPIO pin when loop index matches GPIO config table size. */
    if (idx == gpio_config_size)
    {
        return NULL;
    }

    return gpio_config[idx].reg_ddr;
}


/*******************************************************************************
 *
 * gpio_reg_port() - get PORT data register address
 *
 * Description:
 *  Get PORT data register address.
 *
 * Parameters:
 *  pin                     GPIO pin
 *
 * Returns:
 *  > 0                     - PORT data register address
 *  NULL                    - failure, invalid GPIO pin
 *
 ******************************************************************************/
volatile uint8_t* gpio_reg_port(gpio_pin_t pin)
{
    uint8_t idx;
    uint8_t gpio_config_size;

    gpio_config_size = sizeof(gpio_config) / sizeof(gpio_config[0]);

    /* Check for valid GPIO pin. */
    for (idx = 0; idx < gpio_config_size; idx++)
    {
        if (gpio_config[idx].pin == pin)
        {
            break;
        }
    }

    /* Invalid GPIO pin when loop index matches GPIO config table size. */
    if (idx == gpio_config_size)
    {
        return NULL;
    }

    return gpio_config[idx].reg_port;
}


/*******************************************************************************
 *
 * gpio_reg_pin() - get PORT input pin register address
 *
 * Description:
 *  Get PORT input pin register address.
 *
 * Parameters:
 *  pin                     GPIO pin
 *
 * Returns:
 *  > 0                     - PORT input pin register address
 *  NULL                    - failure, invalid GPIO pin
 *
 ******************************************************************************/
volatile uint8_t* gpio_reg_pin(gpio_pin_t pin)
{
    uint8_t idx;
    uint8_t gpio_config_size;

    gpio_config_size = sizeof(gpio_config) / sizeof(gpio_config[0]);

    /* Check for valid GPIO pin. */
    for (idx = 0; idx < gpio_config_size; idx++)
    {
        if (gpio_config[idx].pin == pin)
        {
            break;
        }
    }

    /* Invalid GPIO pin when loop index matches GPIO config table size. */
    if (idx == gpio_config_size)
    {
        return NULL;
    }

    return gpio_config[idx].reg_pin;
}
