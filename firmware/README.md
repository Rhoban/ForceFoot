# Firmware

You will need an ICSP/ISP tool that uses the the 6-pin AVR header. We recommend
that you use the AVR ISP MkII. You can also use Arduino Uno as ISP.

## Fuses

The first step is configuring the fuses, in order to configure it to use the
external 16Mhz quartz. You can run the fuse.sh script:

    ./fuse-m328p.sh

Alternatively, you can use:

    ./fuse-m328pb.sh

For the ATmega328PB

This assumes you use the AVR ISP MkII, you can however change this script according
to your hardware.

## Building

First, edit `Makefile.config` to set `MCU_TARGET` to either `atmega328p` or `atmega328pb`.

To build the firmware, simply run:

    make

In the firmware directory

## Flashing

You can flash the firmware using the:

    make sendisp

Command, this still assumes AVR ISP MkII, you can edit the `Makefile` if you're
using other hardware.

## Protocol

The protocol is very straightforward, the board will appear exactly like a dynamixel 
servo, with a fancy model number and its own ID.

Model ID, ID and baudrate are not configurable on-the-fly with the current version, it
can be changed in the header of `src/main/main.c`. (Default model number 5004, id 123 and baudrate
1000000).

Each gauge value is a signed 24 bit value (least significant byte first), resulting in a total
of 3 bytes. Thus, the values of the four gauges are 12 bytes. These bytes can be read at the 
present position address of the dynamixel registers (``0x24`` / 36).

0x24 &rarr; 0x26 | 0x27 &rarr; 0x29 | 0x2a &rarr; 0x2c | 0x2d &rarr; 0x2f
-------|--------|-------|--------
Jauge 1 | Jauge 2 | Jauge 3 | Jauge 4

Here is an example of C code to read the values from one jauge:

```C
#include <stdint.h>

// The bytes are here LSB, the same order that it appear in the dynamixel
// (For instance, jauge 1 values are at dynamixel addresses 0x24 for b0, 0x25 
// for b1 and 0x26 for b2)
int32_t decode24bits(uint8_t b0, uint8_t b1, uint8_t b2)
{
    int32_t result = (
          (b2 << 24)
        | (b1 << 16)
        | (b0 << 8)
        ) >> 8;

    return result;
}
```

## How to use the values

### Calibration

Note that the values will not be trimmed, you'll have to write some code to get the 0-offset
and save it somewhere.

Then, you may want to calibrate each gauge using objects of known mass and applying linear 
regression on some values. You'll be then able to get the force, corresponding statically
to the weight applied to the robot on each gauge.

### Weight

An interresting value is the total weight of the robot, this can for instance tell you if your 
robot is on the floor or being handled.

### Weight ratio

The weight ratio is the distribution of the weight over the two foots. This can be used to know
what is the actual support phase during the walk (i.e which foot is actually on the ground supporting
the robot).

### Center of pressure

The center of pressure can be computed using barycenter of the points (the x,y of your cleats) weighted
with the force applied on it.

This result can be found using the sum of moments equation (a simimar problem is solved in [this
thesis, page 34](http://ir.library.oregonstate.edu/xmlui/bitstream/handle/1957/29797/SchaferChristopherA2005.pdf)).

