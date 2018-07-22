# makefile - Microchip ATmega328P makefile
#
# Makefile for Microchip ATmega328P controllers.
#
# Copyright (C) 2018 Philipp Kaar
# All rights reserved.
#
# Author(s): Philipp Kaar   <philipp.kaar@gmail.com>
#


# AVR GCC toolchain.
TCPATH  := /usr/local/gcc-avr/bin/

# Output file name.
TARGET  := $(notdir $(shell pwd))

# Target controller.
DEVICE  := atmega328p

# Controller clock speed.
F_CPU   := 16000000UL

# --- DO NOT EDIT MAKEFILE BELOW THIS LINE ---

# Set command and flags for compiler.
CC      = $(TCPATH)avr-gcc -c
CCFLAGS = -mmcu=$(DEVICE) -Wall -gdwarf-2 -Os -funsigned-char \
          -funsigned-bitfields -fpack-struct -fshort-enums -DF_CPU=$(F_CPU)

# Set command and flags for assembler.
AS      = $(TCPATH)avr-gcc -c -x assemble-with-cpp
ASFLAGS = -mmcu=$(DEVICE) -Wa,-gdwarf2 -DF_CPU=$(F_CPU)

# Set command and flags for dependency.
DP      = $(TCPATH)avr-gcc -MM
DPFLAGS = -MM -MD -MP -MF $(basename $<).d

# Set command and flags for linker.
LD      = $(TCPATH)avr-gcc
LDFLAGS = -mmcu=$(DEVICE) -Wl,-Map=$(TARGET).map

# Set command and flags for hex-file creator.
HX      = $(TCPATH)avr-objcopy
HXFLAGS = -O ihex -R .eeprom -R .fuse -R .lock -R .signature

# Set command and flags for listing.
LS      = $(TCPATH)avr-objdump
LSFLAGS = -D -s

# Set command for size.
SIZE    = $(TCPATH)avr-size

# Set command and flags for AVRDUDE.
AD      = avrdude
ADFLAGS = -p atmega328p -e -c avr911 -P /dev/ttyUSB0 -U flash:w:$(TARGET).hex:i

# Create lists of all existing source, header and assembler files.
CSRC    := $(shell find -name '*.c')
HSRC    := $(shell find -name '*.h')
SSRC    := $(shell find -name '*.s')

# Create lists of object, dependencies and assembler files to be generated.
OBJS    := $(CSRC:.c=.o) $(SSRC:.s=.o)
ASMS    := $(CSRC:.c=.asm)
DEPS    := $(CSRC:.c=.d)

# Create lists of directories containing source, header and assembler files.
CDIR    := $(sort $(dir $(CSRC)))
HDIR    := $(sort $(dir $(HSRC)))
SDIR    := $(sort $(dir $(SSRC)))
IDIR    := $(sort $(CDIR) $(HDIR))

# Attach -I at the beginning of all include directories.
INCS    = $(foreach dir, $(IDIR),-I$(dir))

# Search path for targets and prerequisites.
VPATH = $(IDIR)

# Set only the needed suffixes
.SUFFIXES:
.SUFFIXES: .c .s .d .o .hex .elf

# Create object, dependency and assembler files from source files.
%.o: %.c
	@echo ' '
	@echo 'Building file: $(@D)/$(@F)'
	@echo 'Invoking: AVR GCC C Compiler'
	$(CC) $(CCFLAGS) $(INCS) -o $@ $<
	$(CC) $(CCFLAGS) $(INCS) -S -o $(basename $@).asm $<
	$(DP) $(DPFLAGS) $(INCS) -o $(basename $@).d $<
	@echo 'Finished building: $(@D)/$(@F)'
	
# Create object files from assembler files.
%.o: %.s
	@echo ' '
	@echo 'Building file: $(@D)/$(@F)'
	@echo 'Invoking: AVR GCC C Compiler'
	$(AS) $(ASFLAGS) -o $@ $<
	@echo 'Finished building: $(@D)/$(@F)'

# Create executable from object files.
%.elf: $(OBJS)
	@echo ' '
	@echo 'Building target: $(TARGET).elf'
	@echo 'Invoking: AVR GCC C Linker'
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET).elf
	$(LS) $(LSFLAGS) $@ > $(basename $@).lst
	$(SIZE) $(TARGET).elf
	@echo 'Finished building target: $(TARGET).elf'

# Create Intel hex-file from executable.
%.hex: %.elf
	@echo ' '
	@echo 'Creating .hex file: $(TARGET).hex'
	$(HX) $(HXFLAGS) $< $@
	@echo 'Finished creating .hex file: $(TARGET).hex'
	@echo ' '

all: $(TARGET).hex

clean:
	@echo ' '
	@echo 'Cleaning project: $(TARGET)'
	@rm -f $(OBJS)          # remove .o files
	@rm -f $(ASMS)          # remove .asm files
	@rm -f $(DEPS)          # remove .d files
	@rm -f $(TARGET).*      # remove .* files
	@echo 'Finished cleaning project: $(TARGET)'
	@echo ' '

flash: $(TARGET).hex
	@echo ' '
	@echo 'Flashing target: $(TARGET).hex'
	$(AD) $(ADFLAGS)
	@echo 'Finished flashing target: $(TARGET).hex'
	@echo ' '

.PHONY: all clean flash

.SECONDARY: $(OBJS) $(TARGET).elf