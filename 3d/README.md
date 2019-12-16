# 3D design

![3d](../docs/3d.png)

This is 3D designs for a foot. The files were created using Autodesk Inventor
2015 CAD software. If you are student, you can get a free version here:
http://www.autodesk.com/education/free-software/all

This design can be used as a reference or "as-is", if you decide to buy the
same gauges.

## Sourcing the gauges

The design is based on [Strain gauges](https://en.wikipedia.org/wiki/Strain_gauge).
You can buy the sensor itself separately, and also find it with mechanical
(mostly aluminium) shaft, we recommend this last option since it's not expansive
and already guarantee to have a correct bending.

The first thing you'll need is to get gauges fitting your needs, we used 5kg
nominal, which is about the maximum weight that they should have to hold
separately.

You can buy low-cost ones from AliExpress,
eBay or any general purpose market place.

There is different gauges out there, we recommend suing
differential ones (featuring wheatstone bridge arrangement, 4 wires). 

## Cells of this design

The cells in this design are 40 kg nominal, made of steel with 2 M4 holes with
27mm spacing

They can be found under the reference "HL703" or "TAL230A"

* [eBay](http://www.ebay.fr/itm/40kg-Aluminium-Alloy-Portable-Rectangle-Kitchen-Scale-Weight-Sensor-Load-Cell-/311507964552?hash=item4887525288:g:QxYAAOSwY3BZEuAr)
* [AliExpress](https://www.aliexpress.com/item/10pcs-40kg-weight-sensor-miniature-full-bridge-sensor-weighing-force-sensor-freeshipping/32498871281.html?spm=2114.01010208.3.126.DWqRNl&ws_ab_test=searchweb0_0,searchweb201602_5_10152_10065_10151_10068_5010015_10136_10137_10157_10060_10138_10155_10062_10156_437_10154_10056_10055_10054_10059_303_100031_10099_10103_10102_10101_10096_10147_10052_10053_10107_10050_10142_10051_5020016_10084_10083_10080_10082_10081_10177_10110_519_10111_10112_10113_10114_10180_10182_10185_10184_10078_10079_10073_10186_10123_142,searchweb201603_2,ppcSwitch_5&btsid=9c118a6d-5c25-40a6-ae97-44e3d203d671&algo_expid=1c5b80d4-c289-412d-ace7-c203356e1b79-17&algo_pvid=1c5b80d4-c289-412d-ace7-c203356e1b79)

## Foot part

### OnShape model

You can check out the Sigmaban humanoid model on OnShape to see the current mounting
of the Rhoban team foot force sensors:

* [OnShape Sigmaban public assembly](https://cad.onshape.com/documents/f3bdef32bffd81536fce83d1/v/779c691df8f135bba01eead1/e/a530b1889ee09acb5e1d7ff9)

### Autodesk (old)

The `foot.ipt` represents the foot plate, we recommend adjusting
the holes to fix it to the rest of your robot.

When printing it, you may fill this part with a reasonable amount of material.

[Continue through the docs with electronics »](../electronics)
