
#include "encoder.hpp"
#include "common.hpp"
#include "pinmux.hpp"

#include <cstring>
#include <cstdio>

#include <fcntl.h>
#include <unistd.h>

#define MAX_ENCODERS 4

encoder_t encoders[MAX_ENCODERS] = {
    {"eQEP0", "P9_92", "P9_27", "48300000.epwmss/48300180.eqep"},
    {"eQEP1", "P8_35", "P8_33", "48302000.epwmss/48302180.eqep"},
    {"eQEP2", "P8_12", "P8_11", "48304000.epwmss/48304180.eqep"},
    {"eQEP2b", "P8_41", "P8_42", "48304000.epwmss/48304180.eqep"}
};

encoder_t *encoder_open(const char *encoder) {
    for(int i = 0; i < MAX_ENCODERS; i++) {
        if(strcmp(encoders[i].channel, encoder) == 0) {
            pinmux_set_pinmode(encoders[i].pina, "qep");
            pinmux_set_pinmode(encoders[1].pinb, "qep");
            return encoders+i;
        }
    }

    DEBUG("Failed to find encoder");
    return nullptr;
}

bool encoder_enable(encoder_t *enc, int state) {
    if(!enc)
        return false;

    char path[512];
    snprintf(path, 512, "%s%s/enabled", BBG_SYSFS_DIR, enc->path);

    DEBUG("Encoder path: %s", path);

    int fd = open(path, O_WRONLY);
    if(fd == -1) {
        DEBUG("Failed to open encoder enable path");
        close(fd);
        return false;
    }

    char writec[2];
    snprintf(writec, 2, "%d", state);

    if(write(fd, writec, 1) != 1) {
        DEBUG("Failed to write write enable encoder!");
        close(fd);
        return false;
    }

    close(fd);

    return true;
}

int encoder_position(encoder_t *enc) {
    if(!enc)
        return false;

    char path[512];
    snprintf(path, 512, "%s%s/position", BBG_SYSFS_DIR, enc->path);

    DEBUG("Encoder path: %s", path);

    int fd = open(path, O_RDONLY);
    if(fd == -1) {
        DEBUG("Failed to open encoder position path");
        close(fd);
        return false;
    }

    char buffer[16] = {0};
    if(read(fd, buffer, 15) == -1) {
        DEBUG("Failed to read from encoder position driver!");
        return 0;
    }

    close(fd);

    return atoi(buffer);
}