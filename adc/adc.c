/* adc.c - Microchip ATmega328P ADC library
 *
 * Microchip ATmega328P ADC library.
 *
 * Copyright (C) 2018 Philipp Kaar
 * All rights reserved.
 *
 * Author(s): Philipp Kaar      <philipp.kaar@gmail.com>
 */


#include "adc.h"

#include <avr/io.h>


typedef struct {
    adc_input_t input;
    uint8_t mux;
} adc_config_t;


const adc_config_t adc_config[] = {
        {ADC_INPUT_CH0, 0x00},
        {ADC_INPUT_CH1, 0x01},
        {ADC_INPUT_CH2, 0x02},
        {ADC_INPUT_CH3, 0x03},
        {ADC_INPUT_CH4, 0x04},
        {ADC_INPUT_CH5, 0x05},
        {ADC_INPUT_CH6, 0x06},
        {ADC_INPUT_CH7, 0x07}
};


/*******************************************************************************
 *
 * adc_init() - initialize ADC module
 *
 * Description:
 *  Initialize ADC module. ADC input channels are single ended voltage inputs.
 *
 * Parameters:
 *
 * Returns:
 *
 ******************************************************************************/
void adc_init(void)
{
    /* Set external ADC reference voltage and conversion result adjustment. */
    ADMUX |= (1 << REFS0) | (1 << ADLAR);

    /* Enable ADC module and set clock prescaler. */
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    /* Start ADC dummy conversion. */
    ADCSRA |= (1 << ADSC);

    /* Wait for ADC conversion to complete. */
    while (!(ADCSRA & (1 << ADIF)));
}


/*******************************************************************************
 *
 * adc_read() - read ADC input channel value
 *
 * Description:
 *  Read ADC input channel value. ADC input channel voltages are converted to a
 *  8-bit digital value.
 *
 * Parameter:
 *  input                   ADC input channel
 *  value                   conversion value
 *
 * Returns:
 *  ADC_ERROR_OK            - success
 *  ADC_ERROR_INPUT         - failure, invalid ADC input channel
 *
 ******************************************************************************/
adc_error_t adc_read(adc_input_t input, uint8_t* value)
{
    uint8_t idx;
    uint8_t adc_config_size;

    /* Clear ADC input selection bits. */
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

    adc_config_size = sizeof(adc_config) / sizeof(adc_config[0]);

    /* Check for valid ADC input channel. */
    for (idx = 0; idx < adc_config_size; idx++)
    {
        if (adc_config[idx].input == input)
        {
            break;
        }
    }

    /* Invalid ADC input when loop index matches ADC config table size. */
    if (idx == adc_config_size)
    {
        return ADC_ERROR_INPUT;
    }

    /* Set ADC input channel. */
    ADMUX |= adc_config[idx].mux;

    /* Start ADC conversion. */
    ADCSRA |= (1 << ADSC);

    /* Wait for ADC conversion to complete. */
    while (!(ADCSRA & (1 << ADIF)));

    *value = ADCH;

    return ADC_ERROR_OK;
}
