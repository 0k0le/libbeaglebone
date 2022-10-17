/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ pinmux.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#include "pinmux.hpp"

BBG_err pinmux_set_pinmode(const char *pin_name, const char *mode) {
	char ocp_dir[BBG_PATH_STR_SIZE] = BBG_SYSFS_DIR;
	char ocp_pin_string[64];
	int fd = -1;
	BBG_err ret = BBG_ERR_SUCCESS;

	// Sanatize input
	if(pin_name == nullptr || mode == nullptr) {
		ret = BBG_ERR_FAILED;
		goto END;
	}

	// Create sysfs string
	snprintf(ocp_pin_string, 64, "ocp:%s_pinmux/state", pin_name);
	strncat(ocp_dir, ocp_pin_string, BBG_PATH_STR_SIZE-1);

	DEBUG("ocp_dir: %s", ocp_dir);
	DEBUG("mode: %s", mode);

	// Write pinmux
	fd = open(ocp_dir, O_WRONLY);
	if(fd == -1) {
		ERR("Failed to open \"%s\"", ocp_dir);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if(write(fd, mode, strlen(mode)) == -1) {
		ERR("Failed to write mode");
		ret = BBG_ERR_FAILED;
		goto END;
	}

END:
	if(fd != -1)
		close(fd);

	return ret;	
}
