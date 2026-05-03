/*
 * app.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: evgeny
 */


#include <stdio.h>
#include <string.h>
#include "main.h"
#include "app.h"
#include "arm_math.h"
#include "wave_starter.h"
#include "wave_measure.h"
#include "usbd_cdc_if.h"
#include "SEGGER_RTT.h"

#define dbg(s)                        SEGGER_RTT_WriteString( 0, s )//; HAL_Delay(1)
#define dbgln(s)                      dbg( s "\n" )
#define dbgf( format, ... )           SEGGER_RTT_printf( 0, ( const char * ) ( format ), ##__VA_ARGS__ ); HAL_Delay(1)

waveGenConfig_s 	wave_gen_config;
waveMeasureConfig_s wave_measure_config;

void init(void){
	SEGGER_RTT_ConfigUpBuffer( 0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM );
    SEGGER_RTT_WriteString( 0, "SEGGER Real-Time-Terminal Sample\n" );
}

void setup(void){
	wave_measure_config.main_freqency = MAIN_FREQENCY_HZ;
	wave_measure_config.time_resolution = MAIN_TIME_RESOLUTION;
	wave_measure_init(&wave_measure_config);
	wave_measure_run();

	wave_gen_config.freq = MAIN_FREQENCY_HZ;
	wave_gen_config.numb_of_steps = MAIN_TIME_RESOLUTION;
	wave_gen_config.fun = arm_cos_f32;
	wave_starter_init(&wave_gen_config);
	wave_starter_run(&wave_gen_config);
}

void loop(void){
	int res = check_adc_result(&wave_measure_config);
	if(!res){
		dbgln("OK!");
	}
}
