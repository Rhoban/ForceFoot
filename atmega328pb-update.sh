echo "deb http://archive.ubuntu.com/ubuntu/ bionic main restricted universe multiverse" > /etc/apt/sources.list.d/tmp.list
apt-get update
apt-get install -y gcc-avr avrdude
rm /etc/apt/sources.list.d/tmp.list
apt-get update
cp /etc/avrdude.conf /etc/avrdude_save.conf
cp misc/avrdude.conf /etc/
