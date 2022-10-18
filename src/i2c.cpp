/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ i2c.cpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#include "i2c.hpp"

BBG_err i2c_open_device(i2cdevice *i2cdev, int device_index) {
	char driver_path[64];

	if(i2cdev == nullptr) {
		ERR("i2cdev cannot be nullptr");
		return BBG_ERR_FAILED;
	}

	i2cdev->fd = -1;
	i2cdev->addr = -1;
	i2cdev->index = device_index;

	snprintf(driver_path, 64, I2C_DEVICE_PREFIX "%d", device_index);

	DEBUG("i2c Driver Path: %s", driver_path);

	if((i2cdev->fd = open(driver_path, O_RDWR)) == -1) {
		ERR("Failed to open i2c driver: %s", driver_path);
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

BBG_err i2c_close_device(i2cdevice *i2cdev) {
	if(i2cdev == nullptr) {
		ERR("i2cdev cannot be nullptr");
		return BBG_ERR_FAILED;
	}

	if(i2cdev->fd == -1) {
		ERR("Cannot deinitialize a device that hasnt been initialized");
		return BBG_ERR_FAILED;
	}

	close(i2cdev->fd);

	return BBG_ERR_SUCCESS;
}

BBG_err i2c_set_addr(i2cdevice *i2cdev, int addr) {
	if(i2cdev == nullptr) {
		ERR("i2cdev cannot be nullptr");
		return BBG_ERR_FAILED;
	}

	if(i2cdev->fd == -1) {
		ERR("i2cdev is uninitialized");
		return BBG_ERR_FAILED;
	}

	if(i2cdev->addr == addr)
		return BBG_ERR_SUCCESS;

	if(ioctl(i2cdev->fd, I2C_SLAVE, addr) == -1) {
		ERR("ioctl failed");
		return BBG_ERR_FAILED;
	}

	i2cdev->addr = addr;

	return BBG_ERR_SUCCESS;
}

BBG_err i2c_read_byte(i2cdevice *i2cdev, __s32 *data) {
	if(i2cdev == nullptr) {
		ERR("i2cdev cannot be nullptr");
		return BBG_ERR_FAILED;
	}

	if(i2cdev->fd == -1) {
		ERR("i2cdev is uninitialized");
		return BBG_ERR_FAILED;
	}

	*data = i2c_smbus_read_byte(i2cdev->fd);
	if(*data == -1) {
		ERR("Failed to smbus_read");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}




