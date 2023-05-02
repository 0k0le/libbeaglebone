/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\
 *  \--\ gpio.cpp \--\
 *   \\\\\\\\\\\\\\\\\\
 */

#include "gpio.hpp"
#include "common.hpp"

#include <pthread.h>

static pins_t table[] = {
  	{ "USR0", "USR0", 53, -1, -1},
  	{ "USR1", "USR1", 54, -1, -1},
  	{ "USR2", "USR2", 55, -1, -1},
  	{ "USR3", "USR3", 56, -1, -1},
  	{ "DGND", "P8_1", 0, -1, -1},
  	{ "DGND", "P8_2", 0, -1, -1},
  	{ "GPIO1_6", "P8_3", 38, -1, -1},
  	{ "GPIO1_7", "P8_4", 39, -1, -1},
  	{ "GPIO1_2", "P8_5", 34, -1, -1},
  	{ "GPIO1_3", "P8_6", 35, -1, -1},
  	{ "TIMER4", "P8_7", 66, 2, -1},
  	{ "TIMER7", "P8_8", 67, 2, -1},
  	{ "TIMER5", "P8_9", 69, 2, -1},
  	{ "TIMER6", "P8_10", 68, 2, -1},
  	{ "GPIO1_13", "P8_11", 45, -1, -1},
  	{ "GPIO1_12", "P8_12", 44, -1, -1},
  	{ "EHRPWM2B", "P8_13", 23, 4, -1},
  	{ "GPIO0_26", "P8_14", 26, -1, -1},
  	{ "GPIO1_15", "P8_15", 47, -1, -1},
  	{ "GPIO1_14", "P8_16", 46, -1, -1},
  	{ "GPIO0_27", "P8_17", 27, -1, -1},
  	{ "GPIO2_1", "P8_18", 65, -1, -1},
  	{ "EHRPWM2A", "P8_19", 22, 4, -1},
  	{ "GPIO1_31", "P8_20", 63, -1, -1},
  	{ "GPIO1_30", "P8_21", 62, -1, -1},
  	{ "GPIO1_5", "P8_22", 37, -1, -1},
  	{ "GPIO1_4", "P8_23", 36, -1, -1},
  	{ "GPIO1_1", "P8_24", 33, -1, -1},
  	{ "GPIO1_0", "P8_25", 32, -1, -1},
  	{ "GPIO1_29", "P8_26", 61, -1, -1},
  	{ "GPIO2_22", "P8_27", 86, -1, -1},
  	{ "GPIO2_24", "P8_28", 88, -1, -1},
  	{ "GPIO2_23", "P8_29", 87, -1, -1},
  	{ "GPIO2_25", "P8_30", 89, -1, -1},
  	{ "UART5_CTSN", "P8_31", 10, -1, -1},
  	{ "UART5_RTSN", "P8_32", 11, -1, -1},
  	{ "UART4_RTSN", "P8_33", 9, -1, -1},
  	{ "UART3_RTSN", "P8_34", 81, 2, -1},
  	{ "UART4_CTSN", "P8_35", 8, -1, -1},
  	{ "UART3_CTSN", "P8_36", 80, 2, -1},
  	{ "UART5_TXD", "P8_37", 78, -1, -1},
  	{ "UART5_RXD", "P8_38", 79, -1, -1},
  	{ "GPIO2_12", "P8_39", 76, -1, -1},
  	{ "GPIO2_13", "P8_40", 77, -1, -1},
  	{ "GPIO2_10", "P8_41", 74, -1, -1},
  	{ "GPIO2_11", "P8_42", 75, -1, -1},
  	{ "GPIO2_8", "P8_43", 72, -1, -1},
  	{ "GPIO2_9", "P8_44", 73, -1, -1},
  	{ "GPIO2_6", "P8_45", 70, 3, -1},
  	{ "GPIO2_7", "P8_46", 71, 3, -1},
  	{ "DGND", "P9_1", 0, -1, -1},
  	{ "DGND", "P9_2", 0, -1, -1},
  	{ "VDD_3V3", "P9_3", 0, -1, -1},
  	{ "VDD_3V3", "P9_4", 0, -1, -1},
  	{ "VDD_5V", "P9_5", 0, -1, -1},
  	{ "VDD_5V", "P9_6", 0, -1, -1},
  	{ "SYS_5V", "P9_7", 0, -1, -1},
  	{ "SYS_5V", "P9_8", 0, -1, -1},
  	{ "PWR_BUT", "P9_9", 0, -1, -1},
  	{ "SYS_RESETn", "P9_10", 0, -1, -1},
  	{ "UART4_RXD", "P9_11", 30, -1, -1},
  	{ "GPIO1_28", "P9_12", 60, -1, -1},
  	{ "UART4_TXD", "P9_13", 31, -1, -1},
  	{ "EHRPWM1A", "P9_14", 50, 6, -1},
  	{ "GPIO1_16", "P9_15", 48, -1, -1},
  	{ "EHRPWM1B", "P9_16", 51, 6, -1},
  	{ "I2C1_SCL", "P9_17", 5, -1, -1},
  	{ "I2C1_SDA", "P9_18", 4, -1, -1},
  	{ "I2C2_SCL", "P9_19", 13, -1, -1},
  	{ "I2C2_SDA", "P9_20", 12, -1, -1},
  	{ "UART2_TXD", "P9_21", 3, 3, -1},
  	{ "UART2_RXD", "P9_22", 2, 3, -1},
  	{ "GPIO1_17", "P9_23", 49, -1, -1},
  	{ "UART1_TXD", "P9_24", 15, -1, -1},
  	{ "GPIO3_21", "P9_25", 117, -1, -1},
  	{ "UART1_RXD", "P9_26", 14, -1, -1},
	{ "GPIO3_19", "P9_27", 115, -1, -1},
	{ "SPI1_CS0", "P9_28", 113, 4, -1},
	{ "SPI1_D0", "P9_29", 111, 1, -1},
	{ "SPI1_D1", "P9_30", 112, -1, -1},
	{ "SPI1_SCLK", "P9_31", 110, 1, -1},
	{ "VDD_ADC", "P9_32", 0, -1, -1},
	{ "AIN4", "P9_33", 0, -1, 4},
	{ "GNDA_ADC", "P9_34", 0, -1, -1},
	{ "AIN6", "P9_35", 0, -1, 6},
	{ "AIN5", "P9_36", 0, -1, 5},
	{ "AIN2", "P9_37", 0, -1, 2},
	{ "AIN3", "P9_38", 0, -1, 3},
	{ "AIN0", "P9_39", 0, -1, 0},
	{ "AIN1", "P9_40", 0, -1, 1},
	{ "CLKOUT2", "P9_41", 20, -1, -1},
	{ "GPIO0_7", "P9_42", 7, 0, -1},
	{ "DGND", "P9_43", 0, -1, -1},
	{ "DGND", "P9_44", 0, -1, -1},
	{ "DGND", "P9_45", 0, -1, -1},
	{ "DGND", "P9_46", 0, -1, -1},
	{ nullptr, nullptr, 0, 0, 0}
};


// Gets gpio number for interfacing with driver
static int get_gpio_by_pin(const char *pin) {
	if(pin == nullptr)
		return -1;

	for(pins_t *pins = table; pins->name != nullptr; pins++) {
		if(strcmp(pins->pin, pin) == 0) {
			DEBUG("GPIO Pin found: %d", pins->gpio);	
			return pins->gpio;
		}
	}

	WARN("No gpio pin was found for: %s", pin);
	return -1;
}


static BBG_err gpio_is_exported(int gpio_num) {
	char gpio_path[64];
	snprintf(gpio_path, 64, GPIO_SYSFS_DIR "gpio%d", gpio_num);

	if(access(gpio_path, R_OK|W_OK|X_OK) == F_OK) {
		return BBG_ERR_SUCCESS;
	}

	return BBG_ERR_FAILED;
}

BBG_err gpio_export(const char *pin) {
	DEBUG("gpio_export()");
	int gpio_number = 0;

	if((gpio_number = get_gpio_by_pin(pin)) == -1) {
		ERR("Failed to get gpio_number");
		return BBG_ERR_FAILED;
	}

	/*if(gpio_is_exported(gpio_number) == BBG_ERR_SUCCESS) {
		DEBUG("GPIO is already exported");
		goto SKIP;
	}*/

	if(export_device(GPIO_SYSFS_DIR, gpio_number) == BBG_ERR_FAILED) {
		ERR("export_device() returned an error");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

BBG_err gpio_set_direction(const char *pin, const char *direction) {
	DEBUG("gpio_set_direction()");
	BBG_err ret = BBG_ERR_SUCCESS;
	char gpio_driver_file[64];
	int driver_fd = -1;
	int gpio_number;

	if(strcmp(direction, "in") != 0 && strcmp(direction, "out") != 0) {
		ERR("Invalid direction: %s", direction);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if((gpio_number = get_gpio_by_pin(pin)) == -1) {
		ERR("Failed to get GPIO number");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	snprintf(gpio_driver_file, 64, GPIO_SYSFS_DIR "gpio%d/direction", gpio_number);

	if((driver_fd = open(gpio_driver_file, O_WRONLY)) == -1) {
		ERR("Failed to open GPIO driver file: %s", gpio_driver_file);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if(write(driver_fd, direction, strlen(direction)) == -1) {
		ERR("Failed to write GPIO direction");
		ret = BBG_ERR_FAILED;
		goto END;
	}

END:
	if(driver_fd != -1)
		close(driver_fd);

	return ret;
}

BBG_err gpio_write(const char *pin, const int value) {
	DEBUG("gpio_write()");
	BBG_err ret = BBG_ERR_SUCCESS;
	int driver_fd = -1;
	int gpio_number;
	char gpio_driver_file[64];
	char gpio_value[8];

	if((gpio_number = get_gpio_by_pin(pin)) == -1) {
		ERR("Failed to get gpio number");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if(value)
		snprintf(gpio_value, 8, "1");
	else
		snprintf(gpio_value, 8, "0");

	snprintf(gpio_driver_file, 64, GPIO_SYSFS_DIR "gpio%d/value", gpio_number);

	if((driver_fd = open(gpio_driver_file, O_WRONLY)) == -1) {
		ERR("Failed to open driver file: %s", gpio_driver_file);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if(write(driver_fd, gpio_value, 1) == -1) {
		ERR("Failed to write \"%s\" to %s", gpio_value, gpio_driver_file);
		ret = BBG_ERR_FAILED;
		goto END;
	}

END:
	if(driver_fd != -1)
		close(driver_fd);

	return ret;
}

BBG_err gpio_init(const char *pin, const char *direction, const int value) {
	DEBUG("gpio_init()");
	if(pinmux_set_pinmode(pin, "gpio") == BBG_ERR_FAILED) {
		ERR("Failed to set pinmode for pin: %s", pin);
		return BBG_ERR_FAILED;
	}
	
	if(gpio_export(pin) == BBG_ERR_FAILED) {
		ERR("Failed to export GPIO for pin: %s", pin);
		return BBG_ERR_FAILED;
	}

	if(gpio_set_direction(pin, direction) == BBG_ERR_FAILED) {
		ERR("Failed to set GPIO direction");
		return BBG_ERR_FAILED;
	}

	if(gpio_write(pin, value) == BBG_ERR_FAILED) {
		ERR("Failed to write GPIO value");
		return BBG_ERR_FAILED;
	}
	
	return BBG_ERR_SUCCESS;
}

BBG_err gpio_unexport(const char *pin) {
	DEBUG("gpio_unexport()");
	int gpio_number = 0;

	if((gpio_number = get_gpio_by_pin(pin)) == -1) {
		ERR("Failed to get gpio_number");
		return BBG_ERR_FAILED;
	}

	if(unexport_device(GPIO_SYSFS_DIR, gpio_number) == BBG_ERR_FAILED) {
		ERR("unexport_device() returned an error");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

BBG_err gpio_close(const char *pin) {
	DEBUG("gpio_close()");
	if(gpio_write(pin, 0) == BBG_ERR_FAILED) {
		ERR("Failed to gpio_write()");
		return BBG_ERR_FAILED;
	}

	if(gpio_unexport(pin) == BBG_ERR_FAILED) {
		ERR("Failed to gpio_unexport()");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}
