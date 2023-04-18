#pragma once

#define ENCODER_ON 1
#define ENCODER_OFF 0

typedef struct encoder_t {
    char channel[256];
    char pina[32];
    char pinb[32];
    char path[256];
} encoder_t;

encoder_t *encoder_open(const char *encoder);
bool encoder_enable(encoder_t *enc, int state);
int encoder_position(encoder_t *enc);