/*
 * ctrl_measure.h
 *
 *  Created on: Jan 6, 2025
 *      Author: x280
 */

#ifndef INC_CTRL_MEASURE_H_
#define INC_CTRL_MEASURE_H_

#include "main.h"
#include "adc.h"
#include "string.h"
#include "stdlib.h"

#define PUMP_SENSOR_SCALE_READOUT  1

#define ADC_NO_CONV 2
#define ADC_NO_CONV_END (ADC_NO_CONV-1)
#define ADC_FILTER_LEN 16			//no of samples
typedef struct CTRL_MEAS_T{

}CTRL_MEAS_t;

typedef enum {ADC_CONV_NOT_FINISHED=0, ADC_CONV_FISHED} isConvFinished_t;

typedef struct ADC_T{
	ADC_TypeDef *adc;
	DMA_TypeDef *dma;
	uint32_t dma_channel;
	uint16_t adc_conv_max;
	volatile isConvFinished_t isConvFinished;				//flag to 1 when string of channel conversion
	volatile uint16_t adc_buff[ADC_NO_CONV];	//in % 0-100%
}ADC_t;

extern ADC_t ADC_MEAS;

//#pragma message( "C Preprocessor got here!" )
#if ADC_NO_CONV > 0
	extern volatile uint16_t adc_median_buff1[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 1
	extern volatile uint16_t adc_median_buff2[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 2
	extern volatile uint16_t adc_median_buff3[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 3
	extern volatile uint16_t adc_median_buff4[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 4
	extern volatile uint16_t adc_median_buff5[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 5
	extern volatile uint16_t adc_median_buff6[ADC_FILTER_LEN];
#endif
//cannot be more than 4 adc median buffers becouse there is no more adc pins available on pcb

void ctrl_measure(void);				//in this function we interract with process that will get adc values

void adc_init(void);
//void adc_update(void);
void adc_dma_isr_handler(void);
void ctrl_measure_init(void);
uint16_t adc_median_filter(uint16_t input_val, uint16_t *buff);		//return filtered value
int compare_fcn(const void *a, const void *b);

#endif /* INC_CTRL_MEASURE_H_ */
