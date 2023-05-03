/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ common.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#include "common.hpp"

/*
 * export_device(string device_directory, int device_index)
 *
 * Input1: A string that is the directory that contains the driver files
 * Input2: The index that is written to the export driver file
 *
 * By writing the device number to the "export driver file" you can "enable"
 * the device you are exporting.
 */
BBG_err export_device(const char *device_path, int index) {
	BBG_err ret = BBG_ERR_SUCCESS;
	char export_device[512];
	char index_str[8];
	int fd = -1;

	if(device_path == nullptr) {
		ret = BBG_ERR_FAILED;
		goto END;
	}

	snprintf(index_str, 8, "%d", index);
	snprintf(export_device, 512, "%sexport", device_path);

	DEBUG("export_device: %s", export_device);

	fd = open(export_device, O_WRONLY);
	if(fd == -1) {
		ret = BBG_ERR_FAILED;
		ERR("Failed to open export device file!");
		goto END;
	}

	if(write(fd, index_str, strlen(index_str)) == -1) {
		ERR("Fatal error writing export");
		ret = BBG_ERR_FAILED;
		goto END;
	}
	
END:
	if(fd != -1)
		close(fd);

	return ret;
}

/*
 * unexport_device(string device_directory, int index)
 *
 * Input1: A string that is the directory that contains the driver files
 * Input2: The index that is written to the unexport driver file
 *
 * By writing a device index to the unexport driver file, you make a device
 * unavailable
 */
BBG_err unexport_device(const char *device_path, int index) {
	BBG_err ret = BBG_ERR_SUCCESS;
	char unexport_device[512];
	char index_str[8];
	int fd = -1;

	if(device_path == nullptr) {
		ret = BBG_ERR_FAILED;
		goto END;
	}

	snprintf(index_str, 8, "%d", index);
	snprintf(unexport_device, 512, "%sunexport", device_path);

	DEBUG("unexport_device: %s", unexport_device);

	fd = open(unexport_device, O_WRONLY);
	if(fd == -1) {
		ERR("Failed to open unexport file: %s", unexport_device);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	if(write(fd, index_str, strlen(index_str)) == -1) {
		if(errno != EBUSY) {
			ERR("Failed write to unexport file: %s", index_str);
			ret = BBG_ERR_FAILED;
			goto END;
		}

		WARN("Export write failed, shouldnt be catastrophic");
	}
	
END:
	if(fd != -1)
		close(fd);

	return ret;
}

/*
 * Dont use unless you know what you are doing...
 *
 * create_dir_from_device_address(string part1_string, string index_string, string new_string, maxlen, mustmatch_requirement)
 *
 */
BBG_err create_dir_from_device_address(const char *ocp_dir, const char *address, char *new_dir, size_t maxlen, const char *mustmatch) {
	BBG_err ret = BBG_ERR_SUCCESS;
	DIR *dir = nullptr;
	struct dirent *dirinfo = nullptr;
	char dirname[BBG_PATH_STR_SIZE];

	// Sanatize
	if(ocp_dir == nullptr || address == nullptr || new_dir == nullptr) {
		ret = BBG_ERR_FAILED;
		goto END;
	}

	// Open base as directory
	if((dir = opendir(ocp_dir)) == nullptr) {
		ERR("Failed to open ocp_dir: %s", ocp_dir);
		ret = BBG_ERR_FAILED;
		goto END;
	}

	ret = BBG_ERR_FAILED;
	dirinfo = readdir(dir);
	while(dirinfo != nullptr) {
		if(memcmp(dirinfo->d_name, address, strlen(address)) == 0) {
			if(mustmatch != nullptr) {
				if(strstr(dirinfo->d_name, mustmatch) != nullptr) {
					ret = BBG_ERR_SUCCESS;
					strncpy(dirname, dirinfo->d_name, BBG_PATH_STR_SIZE);
					break;
				}
			} else {
				ret = BBG_ERR_SUCCESS;
				strncpy(dirname, dirinfo->d_name, BBG_PATH_STR_SIZE);
				break;
			}
		}

		dirinfo = readdir(dir);
	}

	if(ret == BBG_ERR_FAILED)
		goto END;

	DEBUG("dirname: %s", dirname);

	snprintf(new_dir, maxlen, "%s%s/", ocp_dir, dirname);	

END:
	if(dir != nullptr)
		closedir(dir);

	return ret;
}
