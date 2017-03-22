include Makefile.config

CC := avr-gcc
LD := avr-gcc
OBJCOPY := avr-objcopy

SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

DEFS	  += -DF_CPU=$(F_CPU)
LIBS	  :=

SRC       :=
OBJ       :=
INCLUDES  := -Isrc/
add-src   = $(eval include src/$1/Makefile.sources); \
    $(eval SRC += $(addprefix src/$1/,$(SOURCES))); \
    $(eval OBJ += $(addprefix build/$1/,$(subst .S,.o,$(subst .cpp,.o,$(subst .c,.o,$(SOURCES)))))); \

$(foreach module,$(MODULES),$(call add-src,$(module)))

override CFLAGS  := -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -O2
override LDFLAGS := -Wl,-Map,$(PRG).map

vpath %.c $(SRC_DIR)


all: checkdirs build/flash.bin

build/flash.bin: build/flash
	$(OBJCOPY) -j .text -j .data -O binary $< $@

build/flash: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

checkdirs: $(BUILD_DIR)

define make-goal
$1/%.o: $2/%.cpp
	$(CPP) $(CFLAGS) $(DEFS) $(INCLUDES) -c $$< -o $$@

$1/%.o: $2/%.c
	$(CC) $(CFLAGS) $(DEFS) $(INCLUDES) -c $$< -o $$@

$1/%.o: $2/%.S
	$(CC) $(CFLAGS) $(DEFS) $(INCLUDES) -c $$< -o $$@
endef

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf build/*

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir),$(patsubst build/%,src/%,$(bdir)))))

sendisp:
	avrdude -c usbtiny -p $(MCU_TARGET) -U flash:w:build/flash.bin:r

sendu:
	avrdude -c arduino -p $(MCU_TARGET) -P /dev/ttyUSB0 -U flash:w:build/flash.bin:r

send:
	avrdude -c arduino -p $(MCU_TARGET) -P /dev/ttyACM0 -U flash:w:build/flash.bin:r

sendi: erasei
	sudo avrdude -c avrisp2 -p $(MCU_TARGET) -P usb -U flash:w:build/flash.bin:r

sendd:
	./dynamixel/flash /dev/ttyUSB0 build/flash.bin

erasei:
	sudo avrdude -c avrisp2 -p $(MCU_TARGET) -P usb -e
