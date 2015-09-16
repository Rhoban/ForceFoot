# Foot firmware

## Presentation

The foot sensing board is based on a maple mini board which features a STM32
processor.

## Building

To build the firmware, you'll need some tools:

    sudo aptitude install build-essential git-core wget screen dfu-util \
        openocd python python-serial

Then, you'll have to clone the following repository:

    git clone https://github.com/Rhoban/Maple.git

In your HOME (you can also edit the `Makefile` to change this path)

You can then type:

    make
    make install

This will send the firmware on the board

## Protocol

The protocol is very straightforward, the board will appear exactly like a dynamixel 
servo, with a fancy model number and its own ID.

Model ID, ID and baudrate are not configurable on-the-fly with the current version, it
can be changed in the header of `main.cpp`. (Default model number 5000, id 123 and baudrate
1000000).

Each gauge value is a signed 24 bit value, resulting in a total of 3 bytes. Thus, the values
of the four gauges are 12 bytes. These bytes can be read at the present position address
of the dynamixel registers (0x24).

Note that the values will not be trimmed, you'll have to write some code to get the 0-offset
and save it somewhere.
