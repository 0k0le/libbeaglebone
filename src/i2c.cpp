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
#include <linux/i2c.h>

// Create I2C device from index
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

// Set slave address
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

// Read I2C data
BBG_err i2c_read_block(i2cdevice *i2cdev, char *buffer, __u8 maxlen, __u8 cmd) {
	if(i2cdev == nullptr) {
		ERR("i2cdev cannot be nullptr");
		return BBG_ERR_FAILED;
	}

	if(i2cdev->fd == -1) {
		ERR("i2cdev is uninitialized");
		return BBG_ERR_FAILED;
	}

	if(maxlen > I2C_BLOCK_SIZE) {
		ERR("maxlen cannot be greater than 32");
		return BBG_ERR_FAILED;
	}

	__s32 len = 0;
	if((len = i2c_smbus_read_i2c_block_data(i2cdev->fd, cmd, maxlen, (__u8 *)buffer)) == -1) {
		ERR("Failed to write block data");
		return BBG_ERR_FAILED;
	}

	DEBUG("%d bytes in i2c block read", len);

	return BBG_ERR_SUCCESS;
}


// Write I2C data
BBG_err i2c_write_block(i2cdevice *i2cdev, char *buffer, __u8 maxlen, __u8 cmd) {
	if(i2cdev == nullptr) {
		ERR("i2cdev cannot be nullptr");
		return BBG_ERR_FAILED;
	}

	if(i2cdev->fd == -1) {
		ERR("i2cdev is uninitialized");
		return BBG_ERR_FAILED;
	}

	if(i2c_smbus_write_i2c_block_data(i2cdev->fd, cmd, maxlen, (const __u8 *)buffer) == -1) {
		ERR("Failed to write block data");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}

BBG_err i2c_read_byte_data(i2cdevice *i2cdev, char *data, __u8 cmd) {
	if(i2cdev == nullptr) {
		ERR("i2cdev cannot be nullptr");
		return BBG_ERR_FAILED;
	}

	if(i2cdev->fd == -1) {
		ERR("i2cdev is uninitialized");
		return BBG_ERR_FAILED;
	}

	*data = i2c_smbus_read_byte_data(i2cdev->fd, cmd);
	if(*data == -1) {
		ERR("Failed to smbus_read");
		return BBG_ERR_FAILED;
	}

	return BBG_ERR_SUCCESS;
}





