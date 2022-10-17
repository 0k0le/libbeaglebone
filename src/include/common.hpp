/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\\\
 *  \--\ common.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <cstring>

#include "beaglebone.hpp"
#include "macro.hpp"

BBG_err create_dir_from_device_address(const char *ocp_dir, const char *address, char *new_dir, size_t maxlen, const char *mustmatch = nullptr);
BBG_err export_device(const char *device_path, int index);
