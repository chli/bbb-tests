# handle cross-compilation
CXX = $(CROSS_COMPILE)-g++
CC = $(CROSS_COMPILE)-gcc
AS = $(CROSS_COMPILE)-as
AR = $(CROSS_COMPILE)-ar
NM = $(CROSS_COMPILE)-nm
LD = $(CROSS_COMPILE)-ld
OBJDUMP = $(CROSS_COMPILE)-objdump
OBJCOPY = $(CROSS_COMPILE)-objcopy
RANLIB = $(CROSS_COMPILE)-ranlib
STRIP = $(CROSS_COMPILE)-strip

led-test : leds-demo.o spi-driver.o ws2801-driver.o
	${CXX} -o leds-test  leds-demo.o spi-driver.o ws2801-driver.o

lcd-test : lcd-demo.o gpio-driver.o spi-driver.o lcd-driver.o
	${CXX} -o lcd-test lcd-demo.o gpio-driver.o spi-driver.o lcd-driver.o

install : lcd-test leds-test
	scp lcd-test leds-test $(TARGET)

clean :
	rm -f lcd-demo.o leds-demo.o ws2801-driver.o gpio-driver.o spi-driver.o lcd-driver.o
	rm -f lcd-test
	rm -f leds-test

