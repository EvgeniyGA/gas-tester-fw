
#ifdef TEST

#ifdef __cplusplus
extern "C" {
#endif
#include "unity.h"
#include "wave_gen.h"
#include <string.h>
#ifdef __cplusplus
}
#endif
void setUp(void)
{
}

void tearDown(void)
{
}

void test_wave_gen_low_frew(void){
	const uint16_t DAC_TIME_RESOLUTION = 128;
	uint16_t dac_buf[DAC_TIME_RESOLUTION];
	memset(dac_buf, 0x00, DAC_TIME_RESOLUTION);
	waveGenConfig_s wave_config = {
		.amplitude = 1.5,
		.midpoint = 1.5,
		.freq = 0,
		.fun = cosf,
		.buf = dac_buf,
		.dac_resolution = 12,
		.dac_reference = 3,
		.numb_of_steps = DAC_TIME_RESOLUTION,
		.timer_frequency = DAC_TIMER_FREQENCY_MHZ*1000000
	};
	wave_config.freq = 1;
	uint8_t ret = initWaveMas(&wave_config);
	TEST_ASSERT_NOT_EQUAL_UINT8(ret, 0);
}

void test_wave_gen_wrongs(void){
	const uint16_t DAC_TIME_RESOLUTION = 128;
	uint16_t dac_buf[DAC_TIME_RESOLUTION];
	memset(dac_buf, 0x00, DAC_TIME_RESOLUTION);
	waveGenConfig_s wave_config = {
		.amplitude = 1.5,
		.midpoint = 1.5,
		.freq = 0,
		.fun = cosf,
		.buf = dac_buf,
		.dac_resolution = 12,
		.dac_reference = 3,
		.numb_of_steps = DAC_TIME_RESOLUTION,
		.timer_frequency = DAC_TIMER_FREQENCY_MHZ*1000000
	};

	uint8_t ret = initWaveMas(&wave_config);
	TEST_ASSERT_NOT_EQUAL_UINT8(ret, 0);

	wave_config.freq = 1000;
	wave_config.numb_of_steps = 0;
	ret = initWaveMas(&wave_config);
	TEST_ASSERT_NOT_EQUAL_UINT8(ret, 0);

	wave_config.numb_of_steps = DAC_TIME_RESOLUTION;
	wave_config.timer_frequency = 0;
	ret = initWaveMas(&wave_config);
	TEST_ASSERT_NOT_EQUAL_UINT8(ret, 0);
}

void test_wave_gen_amplitudes(void)
{
	const uint16_t DAC_TIME_RESOLUTION = 128;
	uint16_t dac_buf[DAC_TIME_RESOLUTION];
	memset(dac_buf, 0x00, DAC_TIME_RESOLUTION);
	waveGenConfig_s wave_config = {
		.amplitude = 1.5,
		.midpoint = 1.5,
		.freq = 5000,
		.fun = cosf,
		.buf = dac_buf,
		.dac_resolution = 12,
		.dac_reference = 3,
		.numb_of_steps = DAC_TIME_RESOLUTION,
		.timer_frequency = DAC_TIMER_FREQENCY_MHZ*1000000
	};

	wave_config.amplitude = 1.6;
	uint8_t ret = initWaveMas(&wave_config);
	TEST_ASSERT_NOT_EQUAL_UINT8(ret, 0);

	wave_config.amplitude = 1.5;
	wave_config.midpoint = 1.4;
	ret = initWaveMas(&wave_config);
	TEST_ASSERT_NOT_EQUAL_UINT8(ret, 0);
}

void test_wave_gen_simple(void)
{
	const uint16_t DAC_TIME_RESOLUTION = 128;
	uint16_t dac_buf[DAC_TIME_RESOLUTION];
	memset(dac_buf, 0x00, DAC_TIME_RESOLUTION);
	waveGenConfig_s wave_config = {
		.amplitude = 1.5,
		.midpoint = 1.5,
		.freq = 5000,
		.fun = cosf,
		.buf = dac_buf,
		.dac_resolution = 12,
		.dac_reference = 3,
		.numb_of_steps = DAC_TIME_RESOLUTION,
		.timer_frequency = DAC_TIMER_FREQENCY_MHZ*1000000
	};

	initWaveMas(&wave_config);

	FILE* f = fopen("sine_output.csv", "w");
	for (int i = 0; i < wave_config.numb_of_steps; i++) {
		fprintf(f, "%d\n", dac_buf[i]);
	}
	fclose(f);

	TEST_ASSERT_NOT_EQUAL_UINT16(dac_buf[0], 0);
}

void test_wave_gen_periodic(void)
{
	uint16_t DAC_TIME_RESOLUTION = 128;
	uint16_t dac_buf[DAC_TIME_RESOLUTION];
	memset(dac_buf, 0x00, DAC_TIME_RESOLUTION);
	waveGenConfig_s wave_config = {
		.amplitude = 1.5,
		.midpoint = 1.5,
		.freq = 5000,
		.fun = cosf,
		.buf = dac_buf,
		.dac_resolution = 12,
		.dac_reference = 3,
		.numb_of_steps = DAC_TIME_RESOLUTION,
		.timer_frequency = DAC_TIMER_FREQENCY_MHZ*1000000
	};
	initWaveMas(&wave_config);

	TEST_ASSERT_NOT_EQUAL_UINT16(dac_buf[0], dac_buf[10]);
	TEST_ASSERT_UINT16_WITHIN(50, dac_buf[0], dac_buf[wave_config.numb_of_steps - 1]);
}

#endif // TEST
