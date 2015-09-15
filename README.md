# Force foot

Here is a design of a robot foot using strain gauges as force sensors.
It was used by the Rhoban Football Club (France) during the RoboCup 2015.

![force foot](docs/sigmaban_foot.png)

## Repository contents

This repository contains:

* A **mechanical design** suitable for Rhoban robots, that can be a good base
  to be adapted to fit your needs;
* Schematics for an **electronics board** containing all the components to get
  the values from the sensors and communicate them through dynamixel;
* A [**firmware**](firmware) compliant with the electronics;
* **Documentation and examples**.

## Sourcing the gauges

The design is based on [Strain gauges](https://en.wikipedia.org/wiki/Strain_gauge).
You can buy the sensor itself separately, and also find it with mechanical
(mostly aluminium) shaft, we recommend this last option since it's not expansive
and already guarantee to have a correct bending.

The first thing you'll need is to get gauges fitting your needs, we used 5kg
nominal, which is about the maximum weight that they should have to hold
separately.

You can buy low-cost ones from [AliExpress](http://www.aliexpress.com/wholesale?SearchText=strain+gauge+5kg),
eBay or any general purpose market place.

There is different gauges out there, this design is intended to work with
differential ones (featuring wheatstone bridge arrangement, 4 wires). 

## Mechanical considerations


