F_CPU=16000000
CC=avr-gcc
CFLAGS=-std=c99 -mmcu=atmega324a -Os -Wl,-u,vfprintf -lprintf_flt -lm -Wall -I. -Iusbdrv -DF_CPU=$(F_CPU) -DDEBUG_LEVEL=0
OBJS=main.o usart.o buzzer.o matrix.o rtc.o i2chw/twimastertimeout.o #rtc/rtc.o rtc/twi.o rtc/twi-lowlevel.o

all: main.hex

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@
	avr-size main.elf

main.elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: upload
upload: main.hex
	sudo /opt/bootloadHID.2012-12-08/commandline/bootloadHID main.hex

.PHONY: clean
clean:
	rm -rf *.o *.elf *.hex
