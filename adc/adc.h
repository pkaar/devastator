/* adc.h - Microchip ATmega328P ADC library
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#ifndef ADC_H_
#define ADC_H_


#include <stdint.h>


typedef enum {
    ADC_INPUT_CH0,
    ADC_INPUT_CH1,
    ADC_INPUT_CH2,
    ADC_INPUT_CH3,
    ADC_INPUT_CH4,
    ADC_INPUT_CH5,
    ADC_INPUT_CH6,
    ADC_INPUT_CH7
} adc_input_t;

typedef enum {
    ADC_ERROR_OK,
    ADC_ERROR_INPUT
} adc_error_t;


void adc_init(void);
adc_error_t adc_read(adc_input_t input, uint8_t* value);


#endif /* ADC_H_ */
