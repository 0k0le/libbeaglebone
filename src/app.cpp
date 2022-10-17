/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ app.cpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "pinmux.hpp"
#include "pwm.hpp"
#include "gpio.hpp"
#include "adc.hpp"

#define PAN_PWM_PIN 		"P9_22"
#define PAN_DIR_PIN 		"P8_26"
#define PAN_RESET_PIN		"P8_38"
#define MOTOR_ENABLE_PIN 	"P9_11"
#define MOTOR_SLEEP_PIN 	"P8_37"

#define FOCUS_PWM_PIN "P8_45"
#define FOCUS_DIR_PIN "P8_30"
#define FOCUS_POT_PIN "P9_37"

int main(int argc, char ** argv) {
	UNUSED(argc); UNUSED(argv);

	/*pwm_object pwmobj;

	gpio_init(PAN_DIR_PIN, "out", 0);
	gpio_init(MOTOR_SLEEP_PIN, "out", 1);
	gpio_init(PAN_RESET_PIN, "out", 1);
	gpio_init(MOTOR_ENABLE_PIN, "out", 0);

	pwm_init(PAN_PWM_PIN, 2000, 50, &pwmobj);	

	sleep(2);

	gpio_close(PAN_DIR_PIN);
	gpio_close(MOTOR_SLEEP_PIN);
	gpio_close(PAN_RESET_PIN);
	gpio_close(MOTOR_ENABLE_PIN);

	pwm_cleanup(&pwmobj);*/

	/*float value;
	adc_init("P9_40");
	adc_read("P9_40", &value);

	DEBUG("ADC Value: %f", value);*/ 

	// Move lense demo
	/*pwm_object focus_pwm;
	float value;

	gpio_init(FOCUS_DIR_PIN, "out", 1);
	pwm_init(FOCUS_PWM_PIN, 2000, 50, &focus_pwm);
	adc_init(FOCUS_POT_PIN);

	adc_read(FOCUS_POT_PIN, &value);
	DEBUG("Lense Value: %f", value);
	sleep(2);

	gpio_write(FOCUS_DIR_PIN, 0);
	sleep(2);

	adc_read(FOCUS_POT_PIN, &value);
	DEBUG("Lense Value: %f", value);

	gpio_close(FOCUS_POT_PIN);
	pwm_cleanup(&focus_pwm);*/

	return EXIT_SUCCESS;
}
