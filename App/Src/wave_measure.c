/*
 * wave_measure.c
 *
 *  Created on: Apr 5, 2026
 *      Author: evgeny
 */

#include "wave_measure.h"
#include "process_adc.h"
#include "adc.h"
#include "adc_driver.h"
#include "math.h"
#include "arm_math.h"
#include "usbd_cdc_if.h"

#define VREFINT_CAL_VREF_MV                   ( 3300UL)
#define VREFINT_CAL_ADDR_MV                   ((uint16_t*) (0x1FFF7A2AU))
#define ADC_RESOLUTION pow(2, 12)

typedef enum{
	ADC_Channel_1,
	ADC_Channel_2,
	ADC_NumbOfCnannels
}wave_measure_channels_e;

typedef struct{
	float32_t main_freq_Hz;
	float32_t main_phase_deg;
}waveMeasureFFT_result;

#define ADC_DMA_BUFFER_SIZE 	(ADC_NumbOfCnannels * ADC_DMA_STEPS * ADC_DMA_CYCLES)
#define FFT_BUF_SIZE			(ADC_DMA_STEPS * ADC_DMA_CYCLES)

uint16_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];
float32_t fftBufIn[FFT_BUF_SIZE], fftBufOut[FFT_BUF_SIZE];
float32_t fftBufPhases[FFT_BUF_SIZE/2];
arm_rfft_fast_instance_f32 fftHandler;
volatile uint8_t adc_data_ready_flag = 0;

static uint8_t fft_buffer(waveMeasureConfig_s* wave_measure_config, uint8_t channel, waveMeasureFFT_result* result);

void wave_measure_adc_callback(void){
	adc_data_ready_flag = 1;
}

uint8_t check_adc_result(waveMeasureConfig_s* wave_measure_config){
	waveMeasureFFT_result result_ch1, result_ch2;
	if(adc_data_ready_flag){
		adc_data_ready_flag = 0;
		fft_buffer(wave_measure_config, ADC_Channel_1, &result_ch1);
		fft_buffer(wave_measure_config, ADC_Channel_2, &result_ch2);

		printf("F1: %6.2f Hz, F2: %6.2f Hz\t", result_ch1.main_freq_Hz, result_ch2.main_freq_Hz);
		if(result_ch1.main_freq_Hz == result_ch2.main_freq_Hz){
			printf("delta Phase, deg: %3.2f\n\r", result_ch1.main_phase_deg - result_ch2.main_phase_deg);
			return 0;
		}
		else{
			printf("\n\r");
		}
	}
	return -1;
}

int wave_measure_init(waveMeasureConfig_s* wave_measure_config){
	memset(adc_dma_buffer, 0x00, sizeof(adc_dma_buffer[0])*ADC_DMA_BUFFER_SIZE);
	wave_measure_config->buf_adc_in = adc_dma_buffer;
	wave_measure_config->buf_adc_in_size = sizeof(adc_dma_buffer);
	wave_measure_config->numb_of_channels = ADC_NumbOfCnannels;
	wave_measure_config->adc_callback = wave_measure_adc_callback;
	wave_measure_config->adc_sample_rate = wave_measure_config->main_freqency*
			wave_measure_config->time_resolution/wave_measure_config->numb_of_channels;

	arm_rfft_fast_init_f32(&fftHandler, FFT_BUF_SIZE );
	return 0;
}

int wave_measure_run(void){
	return adc_driver_start(adc_dma_buffer, ADC_DMA_BUFFER_SIZE);
}

uint8_t fft_buffer(waveMeasureConfig_s* wave_measure_config, uint8_t channel, waveMeasureFFT_result* result){
	float32_t freq = 0, main_freq = 0, tmp_max = 0;
	uint16_t main_bin = 0;
	if(channel >= ADC_NumbOfCnannels){
		return -1;
	}
	for(int i = 0; i < FFT_BUF_SIZE; i++){
		fftBufIn[i] = (float32_t)wave_measure_config->buf_adc_in[i*2 + channel];
	}
	arm_rfft_fast_f32(&fftHandler, fftBufIn, fftBufOut, 0);

	for(int i = 1; i < FFT_BUF_SIZE/2; i++){
		float32_t re = fftBufOut[2*i];
		float32_t im = fftBufOut[2*i + 1];
		fftBufPhases[i] = atan2f(im, re);
	}

	arm_cmplx_mag_f32(fftBufOut, fftBufOut, FFT_BUF_SIZE/2);
	for(int i = 1; i < FFT_BUF_SIZE/2; i++){
		freq = (float32_t)(i * wave_measure_config->adc_sample_rate * 2) / FFT_BUF_SIZE;
		if(tmp_max < fftBufOut[i]){
			tmp_max = fftBufOut[i];
			main_freq = freq;
			main_bin = i;
		}
	}

	result->main_phase_deg = fftBufPhases[main_bin]*180.0f / M_PI;
	int neibour = (main_bin > 1) ? (main_bin - 1) : (main_bin + 1);
	if(fftBufOut[neibour]*100 < fftBufOut[main_bin]){
		result->main_freq_Hz = main_freq;
		return 0;
	}
	else{
		result->main_freq_Hz = 0;
		return -1;
	}
}

/*double process_buffer(uint16_t *buffer, uint8_t numb_of_channels) {
	double phot1 = 0.0;
	double phot2 = 0.0;
    uint32_t phot1_sum = 0, phot2_sum = 0;
    uint16_t phot1_avg, phot2_avg;

    for (int i = 0; i < ADC_DMA_SAMPLES; ++i) {
    	phot1_sum += buffer[0];
    	phot2_sum += buffer[1];
        buffer += 2;
    }

    phot1_avg = phot1_sum / ADC_DMA_SAMPLES;
    phot2_avg = phot2_sum / ADC_DMA_SAMPLES;

    phot1 = (float) VREFINT_CAL_VREF_MV * phot1_avg / ADC_RESOLUTION / 1000;
    phot2 = (float) VREFINT_CAL_VREF_MV * phot2_avg / ADC_RESOLUTION / 1000;

    usbTxBufLen2 = sprintf((char*)usbTxBuf2,"ADC: %1u\r\n",  HAL_GetTick());
    CDC_Transmit_FS(usbTxBuf2, usbTxBufLen2);
    return phot2;
}*/
