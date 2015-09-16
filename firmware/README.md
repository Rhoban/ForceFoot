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

TODO
