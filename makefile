# Matthew Todd Geiger <mgeiger@newtonlabs.com>
# makefile
#
# beaglebone-firmware

CC=arm-linux-gnueabihf-g++
AR=ar

I2CTOOLSBDIR=i2c-tools/lib

BUILDOPTS=-Wall -Wextra -pedantic -fpic -c -O2 -Isrc/include -D_DEBUG
LDOPTS=-L$(I2CTOOLSBDIR) -l:libi2c.a
AROPTS=rcs

BINDIR=bin
BIN=$(BINDIR)/libbeaglebone.so

INTDIR=bin/int
SRCDIR=src

pinmux.o=$(INTDIR)/pinmux.o
pinmux.cpp=$(SRCDIR)/pinmux.cpp

pwm.o=$(INTDIR)/pwm.o
pwm.cpp=$(SRCDIR)/pwm.cpp

common.o=$(INTDIR)/common.o
common.cpp=$(SRCDIR)/common.cpp

gpio.o=$(INTDIR)/gpio.o
gpio.cpp=$(SRCDIR)/gpio.cpp

adc.o=$(INTDIR)/adc.o
adc.cpp=$(SRCDIR)/adc.cpp

i2c.o=$(INTDIR)/i2c.o
i2c.cpp=$(SRCDIR)/i2c.cpp

encoder.o=$(INTDIR)/encoder.o
encoder.cpp=$(SRCDIR)/encoder.cpp

libi2c.a=$(I2CTOOLSBDIR)/libi2c.a

$(BIN): init $(pinmux.o) $(pwm.o) $(common.o) $(gpio.o) $(adc.o) $(i2c.o) $(encoder.o)
	$(CC) -shared $(pinmux.o) $(pwm.o) $(common.o) $(gpio.o) $(adc.o) $(i2c.o) $(encoder.o) $(LDOPTS) -o $(BIN)
	$(AR) $(AROPTS) $(BINDIR)/libbeaglebone.a $(pinmux.o) $(pwm.o) $(common.o) $(gpio.o) $(adc.o) $(i2c.o) $(encoder.o) $(libi2c.a)

$(pinmux.o): $(pinmux.cpp)
	$(CC) $(pinmux.cpp) $(BUILDOPTS) -o $(pinmux.o)

$(pwm.o): $(pwm.cpp)
	$(CC) $(pwm.cpp) $(BUILDOPTS) -o $(pwm.o)

$(common.o): $(common.cpp)
	$(CC) $(common.cpp) $(BUILDOPTS) -o $(common.o)

$(gpio.o): $(gpio.cpp)
	$(CC) $(gpio.cpp) $(BUILDOPTS) -o $(gpio.o)

$(adc.o): $(adc.cpp)
	$(CC) $(adc.cpp) $(BUILDOPTS) -o $(adc.o)

$(i2c.o): $(i2c.cpp)
	$(CC) $(i2c.cpp) $(BUILDOPTS) -o $(i2c.o)

$(encoder.o): $(encoder.cpp)
	$(CC) $(encoder.cpp) $(BUILDOPTS) -o $(encoder.o)

init:
	mkdir -p bin/int

clean:
	rm -rf bin

install:
	mkdir -p /opt/libbeaglebone/
	cp bin/libbeaglebone.so /opt/libbeaglebone
