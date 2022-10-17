/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ adc.cpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#include "adc.hpp"

static adc_info_t table[] {
	{"P9_33", 4},
	{"P9_35", 6},
	{"P9_36", 5},
	{"P9_37", 2},
	{"P9_38", 3},
	{"P9_39", 0},
	{"P9_40", 1},
	{nullptr, 0}
};

static int get_ain_from_pin(const char *pin) {
	for(adc_info_t *adc = table; adc->pin != nullptr; adc++) {
		if(strcmp(adc->pin, pin) == 0) {
			DEBUG("Found adc pin: %d", adc->ain);
			return adc->ain;
		}
	}

	DEBUG("No ADC ain found from: %s", pin);
	return -1;
}

BBG_err adc_init(const char *pin) {
	if(pinmux_set_pinmode(pin, "adc") == BBG_ERR_FAILED) {
		ERR("Failed to set %s to adc", pin);
		return BBG_ERR_FAILED;
	}

	//int fd = open(ADC_SYSFS_DIR "in_voltage"

	return BBG_ERR_SUCCESS;
}

BBG_err adc_read(const char *pin, float *ret_value) {
	BBG_err ret = BBG_ERR_SUCCESS;
	char adc_driver_path[64];
	char adc_driver_read[64];
	int driver_fd = -1;
	int ain_num;

	if((ain_num = get_ain_from_pin(pin)) == -1) {
		ERR("Failed to get ain number");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	DEBUG("AIN PIN: %d", ain_num);

	snprintf(adc_driver_path, 64, ADC_SYSFS_DIR "in_voltage%d_raw", ain_num);

	DEBUG("ADC Driver: %s", adc_driver_path);

	if((driver_fd = open(adc_driver_path, O_RDONLY)) == -1) {
		ERR("Failed to open: %s", adc_driver_path);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	lseek(driver_fd, 0, SEEK_SET);
	if(read(driver_fd, adc_driver_read, 64) == -1) {
		ERR("Failed to read() %s", adc_driver_path);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	*ret_value = atof(adc_driver_read); 
	*ret_value /= 4095.0;
	*ret_value *= 1.8;

END:
	if(driver_fd != -1)
		close(driver_fd);

	return ret;
}
