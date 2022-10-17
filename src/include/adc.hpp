/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ adc.hpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "beaglebone.hpp"
#include "macro.hpp"
#include "common.hpp"
#include "pinmux.hpp"

#define ADC_SYSFS_DIR "/sys/bus/iio/devices/iio:device0/"

typedef struct adc_info_t {
	const char *pin;
	int ain;
} adc_info_t;

BBG_err adc_init(const char *pin);
BBG_err adc_read(const char *pin, float *ret_value);
