sudo avrdude -c avrisp2 -p m328pb -P usb -U flash:w:build/$1:r
