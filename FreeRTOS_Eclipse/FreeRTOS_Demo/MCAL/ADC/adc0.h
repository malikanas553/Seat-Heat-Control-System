/*
 * adc.h
 *
 *  Created on: Jul 13, 2025
 *      Author: LightStore
 */

#ifndef ADC0_H_
#define ADC0_H_

#include "std_types.h"

typedef enum
{
  ADC_PE1 = 2,ADC_PE2 = 1
}ADC_PINS;

#define ADC_REF_VOLT_VALUE            3.3
#define ADC_MAXIMUM_VALUE             4095

void ADC0_Init(void);

uint16 ADC0_ReadChannel(ADC_PINS Channel);

#endif /* ADC0_H_ */
