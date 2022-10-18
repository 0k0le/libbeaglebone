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

/*
 * pin_number get_ain_from_pin(string pin)
 * 
 * Input: Beaglebone pin, eg: "P9_33"
 * Output: Beaglebone ain number.
 *
 * A beaglebone ain number is used to locate the correct
 * adc driver file
 * */
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

/*
 * adc_init(string pin)
 *
 * Input: Beaglebone pin, eg: "P9_33"
 *
 * This will initialize the pin as an ADC
 */
BBG_err adc_init(const char *pin) {
	if(pinmux_set_pinmode(pin, "adc") == BBG_ERR_FAILED) {
		ERR("Failed to set %s to adc", pin);
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

/*
 * adc_read(string pin, float *ret_value)
 *
 * Input1: Beaglebone pin, eg: "P9_33"
 * Input2: Address to float in memory to write result
 *
 * Convert analog to digital on "pin" then write it to *ret_value
 */
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
