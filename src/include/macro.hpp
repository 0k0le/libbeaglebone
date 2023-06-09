/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, beaglebone-firmware
 *
 * \\\\\\\\\\\\\\\\\\\
 *  \--\ macro.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\
 */

/*
 * Chunk of useful macros
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#define FATAL(str, ...) { \
	fprintf(stderr, str "\n", ##__VA_ARGS__); \
	exit(EXIT_FAILURE); \
}

#ifdef _DEBUG
#define DEBUG(str, ...) printf("[DEBUG] " str "\n", ##__VA_ARGS__)
#define ERR(str, ...) fprintf(stderr, "[ERROR] " str "\n%s\n", ##__VA_ARGS__, strerror(errno))
#define WARN(str, ...) printf("[WARN] " str "\n", ##__VA_ARGS__)
#else
#define DEBUG(str, ...)
#define ERR(str, ...)
#define WARN(str, ...)
#endif

#define UNUSED(x)(void)(x)
