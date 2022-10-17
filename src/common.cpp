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
		goto END;
	}

	if(write(fd, index_str, strlen(index_str)) == -1) {
		if(errno != EBUSY) {
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

BBG_err create_dir_from_device_address(const char *ocp_dir, const char *address, char *new_dir, size_t maxlen, const char *mustmatch) {
	BBG_err ret = BBG_ERR_SUCCESS;
	DIR *dir = nullptr;
	struct dirent *dirinfo = nullptr;
	char dirname[BBG_PATH_STR_SIZE];

	if(ocp_dir == nullptr || address == nullptr || new_dir == nullptr) {
		ret = BBG_ERR_FAILED;
		goto END;
	}

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
