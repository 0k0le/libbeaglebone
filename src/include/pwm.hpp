/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ pwm.hpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#pragma once

#include "beaglebone.hpp"
#include "pinmux.hpp"
#include "common.hpp"

typedef struct pwm_t {
  const char *module;
  const int sysfs;
  const int index;
  const int muxmode;
  const char *path;
  const char *name;
  const char *chip;
  const char *addr;
  const char *pin;  // Pin name eg P9_21
} pwm_t;

typedef struct pwm_object {
	int dutycycle_fd;
	int enable_fd;
	int period_fd;
	int polarity_fd;

	int period; // nanoseconds
	int dutycycle; // nanoseconds
	int polarity;
} pwm_object;

BBG_err pwm_init(const char *pin, int frequency, int duty_cycle, pwm_object *pwmobj);
BBG_err pwm_set_frequency(pwm_object *pwmobj, int frequency);
BBG_err pwm_set_duty_cycle(pwm_object *pwmobj, int dutycycle);
void pwm_cleanup(pwm_object *pwmobj);

