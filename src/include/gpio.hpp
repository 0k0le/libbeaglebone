/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\
 *  \--\ gpio.hpp \--\
 *   \\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>

#include "beaglebone.hpp"
#include "macro.hpp"
#include "pinmux.hpp"
#include "common.hpp"

#define GPIO_SYSFS_DIR "/sys/class/gpio/"
#define GPIO_SYSFS_EXPORT GPIO_SYSFS_DIR "export"
#define GPIO_SYSFS_UNEXPORT GPIO_SYSFS_DIR "unexport"

typedef struct pins_t { 
    const char *name; 
    const char *pin; 
    int gpio;
    int pwm_mux_mode;
    int ain;
} pins_t;

BBG_err gpio_export(const char *pin);
BBG_err gpio_unexport(const char *pin);
BBG_err gpio_set_direction(const char *pin, const char *direction);
BBG_err gpio_write(const char *pin, const int value);
BBG_err gpio_init(const char *pin, const char *direction, const int value);
BBG_err gpio_close(const char *pin);
int gpio_read(const char *pin);