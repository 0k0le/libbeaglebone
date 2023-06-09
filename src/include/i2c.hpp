/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\
 *  \--\ i2c.hpp \--\
 *   \\\\\\\\\\\\\\\\\
 */

/*
 * Helpful documentation:
 * http://nilhcem.com/android-things/arduino-as-an-i2c-slave
 * https://www.ti.com/lit/an/slva704/slva704.pdf?ts=1666163475515&ref_url=https%253A%252F%252Fwww.google.com%252F
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

#define I2C_BLOCK_SIZE 32

typedef struct i2cdevice {
	int fd;
	int addr;
	int index;
} i2cdevice;

extern "C" __s32 i2c_smbus_read_block_data(int file, __u8 command, __u8 *values);
extern "C" __s32 i2c_smbus_access(int file, char read_write, __u8 command,
			      int size, union i2c_smbus_data *data);
extern "C" __s32 i2c_smbus_read_byte(int file);
extern "C" __s32 i2c_smbus_read_byte_data(int file, __u8 command);
extern "C" __s32 i2c_smbus_write_i2c_block_data(int file, __u8 command, __u8 length,
					    const __u8 *values);
extern "C" __s32 i2c_smbus_read_i2c_block_data(int file, __u8 command, __u8 length,
					   __u8 *values);

BBG_err i2c_read_block(i2cdevice *i2cdev, char *buffer, __u8 maxlen, __u8 cmd);
BBG_err i2c_open_device(i2cdevice *i2cdev, int device_index = DEFAULT_BBG_I2C_DEVICE);
BBG_err i2c_close_device(i2cdevice *i2cdev);
BBG_err i2c_set_addr(i2cdevice *i2cdev, int addr);
BBG_err i2c_read_byte_data(i2cdevice *i2cdev, char *byte, __u8 cmd);
BBG_err i2c_write_block(i2cdevice *i2cdev, char *buffer, __u8 maxlen, __u8 cmd);
