# Matthew Todd Geiger <mgeiger@newtonlabs.com>
# makefile
#
# beaglebone-firmware

CC=g++
BUILDOPTS=-Wall -Wextra -pedantic -c -O2 -Isrc/include -D_DEBUG

BINDIR=bin/
BIN=bin/beaglebone-controller

INTDIR=bin/int
SRCDIR=src

app.o=$(INTDIR)/app.o
app.cpp=$(SRCDIR)/app.cpp

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

$(BIN): init $(app.o) $(pinmux.o) $(pwm.o) $(common.o) $(gpio.o) $(adc.o)
	$(CC) $(app.o) $(pinmux.o) $(pwm.o) $(common.o) $(gpio.o) $(adc.o) $(LDOPTS) -o $(BIN)

$(app.o): $(app.cpp)
	$(CC) $(app.cpp) $(BUILDOPTS) -o $(app.o)

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

init:
	mkdir -p bin/int

clean:
	rm -rf bin
