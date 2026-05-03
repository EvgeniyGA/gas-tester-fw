
#ifdef TEST

#ifdef __cplusplus
extern "C" {
#endif
#include "unity.h"
#include "wave_starter.h"
#include "mock_dac_driver.h"
#include "wave_gen.h"
#ifdef __cplusplus
}
#endif

waveGenConfig_s wave_config;

void setUp(void)
{
    memset(&wave_config, 0x00, sizeof(wave_config));
}

void tearDown(void)
{
}

int dac_start_StubCallback(uint16_t* buf, uint32_t size, uint16_t arr) {
    // Тут проверяешь, что пришло
    TEST_ASSERT_NOT_NULL(buf);
    TEST_ASSERT_GREATER_THAN(0, size);
    //TEST_ASSERT_EQUAL_INT(size,  1 / (wave_config.time_step * wave_config.freq));
    return 0;
}

void test_wave_starter_basic(void)
{
	wave_config.fun = cosf;
    wave_config.freq = 1000;
    wave_config.numb_of_steps = WAVE_TIME_RESOLUTION;
	wave_starter_init(&wave_config);

//	dac_start_ExpectAndReturn(wave_config.buf, 200, 0);
//	dac_start_ExpectAndReturn(NULL, NULL, 0);
//	dac_start_Expect(wave_config.buf, 200);
//	dac_start_Return(0);
//	dac_start_IgnoreArg_size();
//	dac_start_IgnoreAndReturn(0);
	dac_start_StubWithCallback(dac_start_StubCallback);

	TEST_ASSERT_EQUAL_INT8(0, wave_starter_run(&wave_config));
}

void test_wave_starting_wrong_initials(void){
    wave_config.fun = cosf;
    wave_config.freq = 1000;
    wave_config.numb_of_steps = 0;
	wave_starter_init(&wave_config);
    dac_start_StubWithCallback(dac_start_StubCallback);
    TEST_ASSERT_NOT_EQUAL_INT8(0, wave_starter_run(&wave_config));
}

#endif // TEST


