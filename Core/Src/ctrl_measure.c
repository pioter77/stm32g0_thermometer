/*
 * ctrl_measure.c
 *
 *  Created on: Jan 6, 2025
 *      Author: x280
 */

#include "ctrl_measure.h"
#include "ctrl_device.h"

ADC_t ADC_MEAS={
	.adc=ADC1,
	.dma= DMA1,
	.dma_channel= LL_DMA_CHANNEL_2,
	.adc_conv_max= ADC_NO_CONV,
	.isConvFinished= ADC_CONV_NOT_FINISHED,
	.adc_buff[0 ... ADC_NO_CONV_END]=0,

};



#if ADC_NO_CONV > 0
	 volatile uint16_t adc_median_buff1[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 1
	 volatile uint16_t adc_median_buff2[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 2
	 volatile uint16_t adc_median_buff3[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 3
	 volatile uint16_t adc_median_buff4[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 4
	 volatile uint16_t adc_median_buff5[ADC_FILTER_LEN];
#endif
#if ADC_NO_CONV > 5
	 volatile uint16_t adc_median_buff6[ADC_FILTER_LEN];
#endif




void adc_init(void)
{
	  //Dma Adc enable
	  LL_ADC_Disable(ADC_MEAS.adc);
	  LL_DMA_DisableChannel(ADC_MEAS.dma, ADC_MEAS.dma_channel);
	  LL_DMA_ConfigAddresses(ADC_MEAS.dma,
			  	  	  	  	  ADC_MEAS.dma_channel,
							  LL_ADC_DMA_GetRegAddr(ADC_MEAS.adc, LL_ADC_DMA_REG_REGULAR_DATA),
							  (uint32_t)&ADC_MEAS.adc_buff,
							  LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	  LL_DMA_SetDataLength(ADC_MEAS.dma, ADC_MEAS.dma_channel, ADC_MEAS.adc_conv_max);
	  LL_DMA_EnableIT_TC(ADC_MEAS.dma, ADC_MEAS.dma_channel);
	  LL_DMA_EnableIT_TE(ADC_MEAS.dma, ADC_MEAS.dma_channel);
	  LL_DMA_EnableChannel(ADC_MEAS.dma, ADC_MEAS.dma_channel);
//	  LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_VBAT);
//	  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VBAT);
	  //cubemx misses this part when generating:
//	  LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_TEMPSENSOR);
//	  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
	  LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_VREFINT);
	  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
	  LL_ADC_Enable(ADC_MEAS.adc);
		//start adc meas
	  LL_ADC_REG_StartConversion(ADC_MEAS.adc);

}

void adc_dma_isr_handler(void)
{
	if(LL_DMA_IsActiveFlag_TC2(ADC_MEAS.dma))
	{
		LL_DMA_ClearFlag_TC2(ADC_MEAS.dma);
	}
	if(LL_DMA_IsActiveFlag_TE2(ADC_MEAS.dma))
	{
		LL_DMA_ClearFlag_TE2(ADC_MEAS.dma);
	}
}

void ctrl_measure_init(void)
{
	adc_init();
}

void ctrl_measure(void)
{

//	uint16_t sensor_in3= 			adc_median_filter(ADC_MEAS.adc_buff[0], (uint16_t *)adc_median_buff1);
//	uint16_t sensor_light= 			adc_median_filter(ADC_MEAS.adc_buff[1], (uint16_t *)adc_median_buff2);
	if(LL_ADC_IsActiveFlag_OVR(ADC1))
	{
//		  LL_ADC_Disable(ADC_MEAS.adc);
//		  LL_DMA_DisableChannel(ADC_MEAS.dma, ADC_MEAS.dma_channel);
		  LL_ADC_ClearFlag_OVR(ADC_MEAS.adc);
//		  LL_DMA_EnableChannel(ADC_MEAS.dma, ADC_MEAS.dma_channel);
//		  LL_ADC_Enable(ADC_MEAS.adc);
			//start adc meas
		  LL_ADC_REG_StartConversion(ADC_MEAS.adc);
	}

	CTRLdevice.temp_int_raw = adc_median_filter(ADC_MEAS.adc_buff[0], (uint16_t *)adc_median_buff1);
	 volatile uint16_t tempVal_raw = adc_median_filter(ADC_MEAS.adc_buff[1], (uint16_t *)adc_median_buff2);
//	 CTRLdevice.vBat_raw = calculate_temp_internal((uint16_t)tempVal_raw);
	 CTRLdevice.vBat_raw = calculate_vref_internal((uint16_t)tempVal_raw);
//	PLANT1.moisture_level= (uint16_t)((PLANT1.moisture_level_raw/4095.0)*100.0);
//	PLANT2.moisture_level= (uint16_t)((PLANT2.moisture_level_raw/4095.0)*100.0);

//	ADC_MEAS.adc_buff[2];
//	ADC_MEAS.adc_buff[3];
}

uint16_t adc_median_filter(uint16_t input_val, uint16_t *buff)
{
	volatile uint16_t buff_med[ADC_FILTER_LEN];
	memcpy((uint16_t *)buff_med, (uint16_t *)(buff)+1, ADC_FILTER_LEN*2-2);
	buff_med[ADC_FILTER_LEN-1]= input_val;
	qsort((uint16_t *)buff_med, ADC_FILTER_LEN, sizeof(uint16_t), compare_fcn);
	volatile uint16_t ret_val= (buff_med[ADC_FILTER_LEN/2-1]+buff_med[ADC_FILTER_LEN/2])/2;

	memcpy((uint16_t *)buff_med, (uint16_t *)(buff)+1, ADC_FILTER_LEN*2-2);
//	for(uint8_t i=0; i<(ADC_FILTER_LEN-1); i++) {
//		*(buff+i)=buff_med[i+1];
//	}
//	buff[ADC_FILTER_LEN-1]= input_val;
	buff_med[ADC_FILTER_LEN-1]= input_val;
	memcpy((uint16_t *)buff, (uint16_t *)buff_med, ADC_FILTER_LEN*2);

	return ret_val;
}

int compare_fcn(const void *a, const void *b)
{
	const uint16_t a_bak= *(const uint16_t *)a;
	const uint16_t b_bak= *(const uint16_t *)b;

	if(a_bak < b_bak) return -1;
	else if(a_bak > b_bak) return 1;
	else return 0;
}

int32_t calculate_temp_internal(uint16_t adc_raw)
{
	return __LL_ADC_CALC_TEMPERATURE((uint32_t)3300, adc_raw, LL_ADC_RESOLUTION_12B);
}

uint16_t calculate_vref_internal(uint16_t adc_raw)
{
	return __LL_ADC_CALC_VREFANALOG_VOLTAGE(adc_raw, LL_ADC_RESOLUTION_12B);
}

