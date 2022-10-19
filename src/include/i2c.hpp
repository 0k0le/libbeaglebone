/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ i2c.hpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

#pragma once

#include "beaglebone.hpp"
#include "macro.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
//#include <i2c/smbus.h>

#define DEFAULT_BBG_I2C_DEVICE 2
#define I2C_DEVICE_PREFIX "/dev/i2c-"

typedef struct i2cdevice {
	int fd;
	int addr;
	int index;
} i2cdevice;

extern "C" __s32 i2c_smbus_access(int file, char read_write, __u8 command,
			      int size, union i2c_smbus_data *data);
extern "C" __s32 i2c_smbus_read_byte(int file);

BBG_err i2c_read_block(i2cdevice *i2cdev, char *buffer);
BBG_err i2c_open_device(i2cdevice *i2cdev, int device_index = DEFAULT_BBG_I2C_DEVICE);
BBG_err i2c_close_device(i2cdevice *i2cdev);
BBG_err i2c_set_addr(i2cdevice *i2cdev, int addr);
BBG_err i2c_read_byte(i2cdevice *i2cdev, char *byte);
