/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ pinmux.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <string.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>

#include "beaglebone.hpp"
#include "macro.hpp"

BBG_err pinmux_set_pinmode(const char *pin_name, const char *mode);
