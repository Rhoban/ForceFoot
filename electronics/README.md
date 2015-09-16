# Electronics

Here you'll find the schematics and (auto)routed board for sampling the gauge and communicate
through the dynamixel bus.

## Options

Some optional resistors on this board can be used to support dynamixel TTL/485 and 3/4 wire
gauges:

![optional features](/docs/board.png)


## Components

Here's a list of components you'll need to build the board with example links for buying them

### Resistors (all are 0805 package)

* 20 ohms
* 3 Kohms
* 8.2 Kohms
* 20 Kohms

### Capacitors (all are 0805 package)

* 1uF
* 0.2uF

### Polar capacitor, for decoupling

* EEE1CA470SP: 47uF
* EEEFK1C221XP: 220uF

### Power 

You can buy a 7805, but we recommend using a switching instead, like the TRACO TSR 1-2450
to avoid intensive heating

### ICs

You'll need a Maple Mini board and its sockets (you can also solder it directly on the board)
The amps are HX711, a really widespread chip that is use in weight scales
Small transistors are PNP S8550 (SOT-23 package)

