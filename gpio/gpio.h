/* gpio.h - Microchip ATmega328P GPIO library
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#ifndef GPIO_H_
#define GPIO_H_


#include <stddef.h>
#include <stdint.h>


typedef enum {
    GPIO_PIN_PB0,
    GPIO_PIN_PB1,
    GPIO_PIN_PB2,
    GPIO_PIN_PB3,
    GPIO_PIN_PB4,
    GPIO_PIN_PB5,
    GPIO_PIN_PB6,
    GPIO_PIN_PB7,
    GPIO_PIN_PC0,
    GPIO_PIN_PC1,
    GPIO_PIN_PC2,
    GPIO_PIN_PC3,
    GPIO_PIN_PC4,
    GPIO_PIN_PC5,
    GPIO_PIN_PC6,
    GPIO_PIN_PD0,
    GPIO_PIN_PD1,
    GPIO_PIN_PD2,
    GPIO_PIN_PD3,
    GPIO_PIN_PD4,
    GPIO_PIN_PD5,
    GPIO_PIN_PD6,
    GPIO_PIN_PD7
} gpio_pin_t;

typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT
} gpio_mode_t;

typedef enum {
    GPIO_LEVEL_LOW,
    GPIO_LEVEL_HIGH,
    GPIO_LEVEL_INVERT
} gpio_level_t;

typedef enum {
    GPIO_PULLUP_DISABLE,
    GPIO_PULLUP_ENABLE
} gpio_pullup_t;

typedef enum {
    GPIO_ERROR_OK,
    GPIO_ERROR_PIN,
    GPIO_ERROR_MODE,
    GPIO_ERROR_LEVEL,
    GPIO_ERROR_PULLUP
} gpio_error_t;


gpio_error_t gpio_init(gpio_pin_t pin, gpio_mode_t mode);
gpio_error_t gpio_get(gpio_pin_t pin, gpio_level_t* level);
gpio_error_t gpio_set(gpio_pin_t pin, gpio_level_t level);
gpio_error_t gpio_config_input(gpio_pin_t pin, gpio_pullup_t pullup);
volatile uint8_t* gpio_reg_ddr(gpio_pin_t pin);
volatile uint8_t* gpio_reg_port(gpio_pin_t pin);
volatile uint8_t* gpio_reg_pin(gpio_pin_t pin);


#endif /* GPIO_H_ */
