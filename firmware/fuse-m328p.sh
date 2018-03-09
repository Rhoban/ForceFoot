# Configure the fuses of the ATMEGA to use the external quartz
sudo avrdude -p m328p -P usb -c avrisp2 -U lfuse:w:0b11101111:m
