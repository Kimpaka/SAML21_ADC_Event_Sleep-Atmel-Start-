//[*]------------------------------------------------------------------------------------------------[*]
#include <atmel_start.h>
//[*]------------------------------------------------------------------------------------------------[*]
#define ADC_value_size (2)
//[*]------------------------------------------------------------------------------------------------[*]
static uint16_t         adc_values;
static volatile uint8_t ready = 0;
//[*]------------------------------------------------------------------------------------------------[*]
//	Event
//[*]------------------------------------------------------------------------------------------------[*]
static void adc_cb(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
	ready = 1;
}
//[*]------------------------------------------------------------------------------------------------[*]
static void adc_m_cb(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
 	ready = 1;
}
//[*]------------------------------------------------------------------------------------------------[*]
//	ADC
//[*]------------------------------------------------------------------------------------------------[*]
void adc_init()
{
	_adc_async_set_irq_state(&ADC_0.device, 0, ADC_ASYNC_DEVICE_CONVERT_CB, false);
	_adc_async_set_irq_state(&ADC_0.device, 0, ADC_ASYNC_DEVICE_ERROR_CB, false);
	adc_async_register_callback(&ADC_0, 0, ADC_ASYNC_MONITOR_CB, adc_m_cb);
	adc_async_enable_channel(&ADC_0, 0);
}
//[*]------------------------------------------------------------------------------------------------[*]
void adc_read()
{
	_adc_async_set_irq_state(&ADC_0.device, 0, ADC_ASYNC_DEVICE_CONVERT_CB, true);
	adc_async_read_channel(&ADC_0, 0, (uint8_t *)&adc_values, 2);
	printf("Monitor!! ADC Vaule : %d\r\n", adc_values);
	_adc_async_set_irq_state(&ADC_0.device, 0, ADC_ASYNC_DEVICE_CONVERT_CB, false);
}
//[*]------------------------------------------------------------------------------------------------[*]
//	Main
//[*]------------------------------------------------------------------------------------------------[*]
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	adc_init();
	printf("Sleep tight...\r\n");
	timer_start(&TIMER_0);
	
	sleep(4);

	while (1) {
		if(ready)	{
			ready = 0;
			adc_read();
		}
		sleep(4);
	}
	
	return 0;
}
