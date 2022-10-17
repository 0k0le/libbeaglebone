/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ pwm.cpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#include "beaglebone.hpp"
#include "common.hpp"
#include "pinmux.hpp"
#include "pwm.hpp"

#include <string.h>

static pwm_t pwm_table[] = {
  { "ehrpwm2", 6, 1, 4, "ehrpwm.2:1", "EHRPWM2B", "48304000", "48304200", "P8_13"},
  { "ehrpwm2", 5, 0, 4, "ehrpwm.2:0", "EHRPWM2A", "48304000", "48304200", "P8_19"},
  { "ehrpwm1", 4, 1, 2, "ehrpwm.1:1", "EHRPWM1B", "48302000", "48302200", "P8_34"},
  { "ehrpwm1", 3, 0, 2, "ehrpwm.1:0", "EHRPWM1A", "48302000", "48302200", "P8_36"},
  { "ehrpwm2", 5, 0, 3, "ehrpwm.2:0", "EHRPWM2A", "48304000", "48304200", "P8_45"},
  { "ehrpwm2", 6, 1, 3, "ehrpwm.2:1", "EHRPWM2B", "48304000", "48304200", "P8_46"},
  { "ehrpwm1", 3, 0, 6, "ehrpwm.1:0", "EHRPWM1A", "48302000", "48302200", "P9_14"},
  { "ehrpwm1", 4, 1, 6, "ehrpwm.1:1", "EHRPWM1B", "48302000", "48302200", "P9_16"},
  { "ehrpwm0", 1, 1, 3, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P9_21"},
  { "ehrpwm0", 0, 0, 3, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P9_22"},
  {   "ecap2", 7, 0, 4, "ecap.2",     "ECAPPWM2", "48304000", "48304100", "P9_28"},
  { "ehrpwm0", 1, 1, 1, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P9_29"},
  { "ehrpwm0", 0, 0, 1, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P9_31"},
  {   "ecap0", 2, 0, 0, "ecap.0",     "ECAPPWM0", "48300000", "48300100", "P9_42"},
  { "timer4", 0, 0, 2, "", "", "", "dmtimer-pwm-4", "P8_7" },
  { "timer7", 0, 0, 2, "", "", "", "dmtimer-pwm-7", "P8_8" },
  { "timer5", 0, 0, 2, "", "", "", "dmtimer-pwm-5", "P8_9" },
  { "timer6", 0, 0, 2, "", "", "", "dmtimer-pwm-6", "P8_10" },
  { "ehrpwm0", 0, 0, 1, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P1_8"},
  { "ehrpwm0", 0, 0, 1, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P1_36"},
  { "ehrpwm0", 1, 1, 1, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P1_10"},
  { "ehrpwm0", 1, 1, 1, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P1_33"},
  { "ehrpwm1", 3, 0, 6, "ehrpwm.1:0", "EHRPWM1A", "48302000", "48302200", "P2_1"},
  { "ehrpwm2", 6, 1, 3, "ehrpwm.2:1", "EHRPWM2B", "48304000", "48304200", "P2_3"},
  { "timer7", 0, 0, 4, "", "", "", "dmtimer-pwm-7", "P1_20" },
  { "timer6", 0, 0, 1, "", "", "", "dmtimer-pwm-6", "P1_26" },
  { "timer5", 0, 0, 1, "", "", "", "dmtimer-pwm-5", "P1_28" },
  { "timer7", 0, 0, 5, "", "", "", "dmtimer-pwm-7", "P2_27" },
  { "timer4", 0, 0, 2, "", "", "", "dmtimer-pwm-4", "P2_31" },
  { NULL, 0, 0, 0, NULL, NULL, NULL, NULL, NULL }
};

static pwm_t *get_pwm_by_pin(const char *pin) {
	for(pwm_t *pwm = pwm_table; pwm->module != NULL; pwm++) {
		if(strcmp(pwm->pin, pin) == 0) {
			DEBUG("PWM Info\n"
			"\tModule: %s\n"
			"\tSysfs: %d\n"
			"\tIndex: %d\n"
			"\tMuxmode: %d\n"
			"\tPath: %s\n"
			"\tName: %s\n"
			"\tChip: %s\n"
			"\tAddr: %s\n"
			"\tPin: %s\n", pwm->module, pwm->sysfs, pwm->index, pwm->muxmode,
			pwm->path, pwm->name, pwm->chip, pwm->addr, pwm->pin);
			return pwm;
		}
	}

	return nullptr;
}

static int is_dmtimer_pin(pwm_t *pwm) {
	char temp[6];
	
	if(pwm == nullptr)
		return 0;

	memcpy(temp, pwm->module, 5);
	temp[5] = 0;

	return (strcmp(temp, "timer") == 0);
}

static void get_pwm_info(pwm_object *pwmobj) {
	char period[32];
	char enable[32];
	char polarity[32];
	char duty_cycle[32];

	memset(period, 0, 32);
	memset(enable, 0, 32);
	memset(polarity, 0, 32);
	memset(duty_cycle, 0, 32);

	lseek(pwmobj->enable_fd, 0, SEEK_SET);
	lseek(pwmobj->dutycycle_fd, 0, SEEK_SET);
	lseek(pwmobj->polarity_fd, 0, SEEK_SET);
	lseek(pwmobj->period_fd, 0, SEEK_SET);

	(void)read(pwmobj->enable_fd, enable, 31);
	(void)read(pwmobj->dutycycle_fd, duty_cycle, 31);
	(void)read(pwmobj->polarity_fd, polarity, 31);
	(void)read(pwmobj->period_fd, period, 31);

	printf("[PWMINFO]\n"
			"\tEnable: %s"
			"\tDutycycle: %s"
			"\tPolarity: %s"
			"\tPeriod: %s", enable, duty_cycle, polarity, period);
}

static BBG_err write_enable(pwm_object *pwmobj, const char *state) {
	lseek(pwmobj->enable_fd, 0, SEEK_SET);

	if(write(pwmobj->enable_fd, state, strlen(state)) == -1) {
		ERR("Failed to set enable: %s", state);
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

static BBG_err write_period(pwm_object *pwmobj, const char *period) {
	lseek(pwmobj->period_fd, 0, SEEK_SET);

	if(write(pwmobj->period_fd, period, strlen(period)) == -1) {
		ERR("Failed to set period: %s", period);
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

static BBG_err write_duty_cycle(pwm_object *pwmobj, const char *dutycycle) {
	lseek(pwmobj->dutycycle_fd, 0, SEEK_SET);

	if(write(pwmobj->dutycycle_fd, dutycycle, strlen(dutycycle)) == -1) {
		ERR("Failed to set duty cycle: %s", dutycycle);
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

/*
 * This functions parses the device tree opening all the correct file descriptors
 */
BBG_err pwm_init(const char *pin, int frequency, int duty_cycle, pwm_object *pwmobj) {
	int ret = BBG_ERR_SUCCESS;
	char mode[16] = "pwm";
	char pwm_pin_str[256];
	char pwm_pin_str_full[256];
	char pwm_chip_path[256];
	char pwm_chip_path_full[256];
	char pwm_actual_path[256];
	char pwm_dutycycle_path[256];
	char pwm_enable_path[256];
	char pwm_period_path[256];
	char pwm_polarity_path[256];
	char temp[32];
	pwm_t *pwm = nullptr;

	// Sanatize
	if(pin == nullptr) {
		ret = BBG_ERR_FAILED;
		goto END;
	}

	// Check if special pin
	if(strcmp(pin, "P9_28") == 0)
		strncpy(mode, "pwm2", 16);

	// Set pinmode
	if(pinmux_set_pinmode(pin, mode) == BBG_ERR_FAILED) {
		ERR("pinmux_set_pinmode(); Are you setting the pin to a valid mode?");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	// Get pin info
	pwm = get_pwm_by_pin(pin);
	if(pwm == nullptr) {
		ERR("get_pwm_by_pin()");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if(!is_dmtimer_pin(pwm)) {
		if(create_dir_from_device_address(BBG_SYSFS_DIR, pwm->chip, pwm_pin_str, 256) == BBG_ERR_FAILED) {
			ERR("create_dir_from_device_address()");
			ret = BBG_ERR_FAILED;
			goto END;
		}

		DEBUG("pwm_pin_str: %s", pwm_pin_str);

		if(create_dir_from_device_address(pwm_pin_str, pwm->addr, pwm_pin_str_full, 256) == BBG_ERR_FAILED) {
			ERR("create_dir_from_device_address() 2");
			ret = BBG_ERR_FAILED;
			goto END;
		}

		DEBUG("pwm_pin_str_full: %s", pwm_pin_str_full);

	} else { // This is setup in a way were adding support later will be easier
		ERR("Timer pins are currently unsupported");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if(create_dir_from_device_address(pwm_pin_str_full, "pwm", pwm_chip_path, 256) == BBG_ERR_FAILED) {
		ERR("create_dir_from_device_address() 3");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	DEBUG("pwm_chip_path: %s", pwm_chip_path);

	if(create_dir_from_device_address(pwm_chip_path, "pwmchip", pwm_chip_path_full, 256) == BBG_ERR_FAILED) {
		ERR("create_dir_from_device_address() 4");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	DEBUG("pwm_chip_path_full: %s", pwm_chip_path_full);

	// A "device export" needs to happen for PWM to become available
	if(export_device(pwm_chip_path_full, pwm->index) == BBG_ERR_FAILED) {
		ERR("export_device()");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	snprintf(temp, 32, ":%d", pwm->index);

	if(create_dir_from_device_address(pwm_chip_path_full, "pwm", pwm_actual_path, 256, temp) == BBG_ERR_FAILED) {
		ERR("create_dir_from_device_address() 5");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	DEBUG("pwm_actual_path: %s", pwm_actual_path);

	if(create_dir_from_device_address(pwm_actual_path, "duty_cycle", pwm_dutycycle_path, 256) == BBG_ERR_FAILED) {
		ERR("create_dir_from_device_address() 6");
		ret = BBG_ERR_FAILED;
		goto END;
	}
	pwm_dutycycle_path[strlen(pwm_dutycycle_path)-1] = 0;

	DEBUG("pwm_dutycycle_path: %s", pwm_dutycycle_path);

	if(create_dir_from_device_address(pwm_actual_path, "enable", pwm_enable_path, 256) == BBG_ERR_FAILED) {
		ERR("create_dir_from_device_address() 7");
		ret = BBG_ERR_FAILED;
		goto END;
	}
	pwm_enable_path[strlen(pwm_enable_path)-1] = 0;

	DEBUG("pwm_enable_path: %s", pwm_enable_path);

	if(create_dir_from_device_address(pwm_actual_path, "period", pwm_period_path, 256) == BBG_ERR_FAILED) {
		ERR("create_dir_from_device_address() 8");
		ret = BBG_ERR_FAILED;
		goto END;
	}
	pwm_period_path[strlen(pwm_period_path)-1] = 0;

	DEBUG("pwm_period_path: %s", pwm_period_path);

	if(create_dir_from_device_address(pwm_actual_path, "polarity", pwm_polarity_path, 256) == BBG_ERR_FAILED) {
		ERR("create_dir_from_device_address() 9");
		ret = BBG_ERR_FAILED;
		goto END;
	}
	pwm_polarity_path[strlen(pwm_polarity_path)-1] = 0;

	DEBUG("pwm_polarity_path: %s", pwm_polarity_path);

	if((pwmobj->polarity_fd = open(pwm_polarity_path, O_RDWR)) == -1) {
		ERR("Failed to open polarity path");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if((pwmobj->period_fd = open(pwm_period_path, O_RDWR)) == -1) {
		ERR("Failed to open period path");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if((pwmobj->enable_fd = open(pwm_enable_path, O_RDWR)) == -1) {
		ERR("Failed to open enable path");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if((pwmobj->dutycycle_fd = open(pwm_dutycycle_path, O_RDWR)) == -1) {
		ERR("Failed to open dutycycle path");
		ret = BBG_ERR_FAILED;
		goto END;
	}

	// This is test - will remove later
	pwm_set_frequency(pwmobj, frequency);
	pwm_set_duty_cycle(pwmobj, duty_cycle);
	write_enable(pwmobj, "1");
	get_pwm_info(pwmobj);

END:
	return ret;
}

BBG_err pwm_set_duty_cycle(pwm_object *pwmobj, int dutycycle) {
	if(pwmobj == nullptr) {
		ERR("pwmobj cannot be null");
		return BBG_ERR_FAILED;
	}

	float f_dutycycle = static_cast<float>(dutycycle);
	float f_period = static_cast<float>(pwmobj->period);

	pwmobj->dutycycle = static_cast<int>(f_period * (f_dutycycle / 100.0));

	char temp[32];
	snprintf(temp, 32, "%d", pwmobj->dutycycle);

	if(write_duty_cycle(pwmobj, temp) == BBG_ERR_FAILED) {
		ERR("write_duty_cycle()");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

BBG_err pwm_set_frequency(pwm_object *pwmobj, int frequency) {
	if(pwmobj == nullptr) { 
		ERR("pwmobj cannot be null");
		return BBG_ERR_FAILED;
	}

	if(frequency <= 0) {
		ERR("Invalid frequency: %d", frequency);
		return BBG_ERR_FAILED;
	}

	pwmobj->period = (1e9/frequency); // Convert to nanoseconds
	
	char temp[32];
	snprintf(temp, 32, "%d", pwmobj->period);

	if(write_period(pwmobj, temp) == BBG_ERR_FAILED) {
		ERR("write_period");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

void pwm_cleanup(pwm_object *pwmobj) {
	write_enable(pwmobj, "0");

	close(pwmobj->dutycycle_fd);
	close(pwmobj->period_fd);
	close(pwmobj->enable_fd);
	close(pwmobj->polarity_fd);
}
