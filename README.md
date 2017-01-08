# ↕️ HomeKit-enabled Office Desk

> Making a height-adjustable office desk
> controllable by Siri using Apple's HomeKit framework.

<img src="https://cloud.githubusercontent.com/assets/198988/21662365/9be90a2c-d2d9-11e6-8f4c-d6f92b6cd230.gif" alt="Office Desk controlled by Siri" width="280" /> <img src="https://cloud.githubusercontent.com/assets/198988/21662454/022f88f6-d2da-11e6-80cb-79a3a32c0abf.gif" alt="Office Desk controlled using Home app" width="280" />

## Table of contents

* [Intro](#intro)
* [Prerequisites](#prerequisites)
* [Desk module](#desk-module)
  * [Module parts list](#module-parts-list)
  * [Module assembly](#module-assembly)
  * [Module software](#module-software)
* [HomeKit bridge](#homekit-bridge)
  * [Bridge parts list](#bridge-parts-list)
  * [Bridge assembly](#bridge-assembly)
  * [Bridge software](#bridge-software)
* [Attribution](#attribution)
* [License](#license)

## Intro

A while ago I bought a height-adjustable desk. It was supposed to make my often long hours sitting in front of my computer programming things a little bit healthier by allowing me to get up and stand from time to time. However, this really only lasted a couple of weeks until I almost forgot that I could change my desk's height.

Then, a couple of months ago I got the idea that the only way for me to stand up would be if my desk would force me to do it. I thought about ways to automate my desk's electronics and did some research. Luckily, [someone has already been experimenting with this idea](http://www.mikrocontroller.net/topic/373579) and a very similar desk, too. That person also [managed to analyze the protocol](http://www.mikrocontroller.net/topic/369941) between the manual hand switch and the motor controller with the help of some electrical engineers.

With this I went ahead and started building the devices and software necessary to make my desk controllable using Apple's HomeKit framework. That way I could set up scenes and automatically trigger them, for example changing my desk's position every two hours.

Additionally, I could now save my preferred sitting and standing heights and trigger them using Siri!

## Prerequisites

You'll need a height-adjustable desk with a [LogicData](http://www.logicdata.at)
Compact or a compatible motor controller. The motor controller is attached to the bottom of the desk. The hand switch should be connected to the controller with a DIN-7 connector.

<img src="https://cloud.githubusercontent.com/assets/198988/21726518/4b60a118-d43d-11e6-84fa-aa79e5bd5e02.JPG" alt="Motor controller DIN-7" width="560" />

Additionally, you'll need soldering equipment or a breadboard with jumper wires in order to interconnect all the components.

## Desk module

The desk module is linked between the hand switch and the motor controller. It passes through any signals from the hand switch, so you can still control the desk manually using buttons. It also adds an Arduino and XBee module, which allow you to send and receive signals remotely from the bridge and ultimately from HomeKit.

<img src="https://cloud.githubusercontent.com/assets/198988/21741728/8b722216-d4df-11e6-8eb1-3eb6fe2b2aec.gif" alt="Desk hardware" width="560" />

### Module parts list

||Part|Vendor|
|---|---|---|
|<img src="https://cdn-shop.adafruit.com/82x62/1086-08.jpg" alt="Arduino Micro with Headers - 5V 16MHz" />|**Arduino Micro with Headers - 5V 16MHz**<br />The perfect device to link between the motor controller and the hand switch, as it's 5V and very small.|[Adafruit](https://www.adafruit.com/products/1086)|
|<img src="https://cdn-shop.adafruit.com/82x62/128-01.jpg" alt="XBee Module - Series 1" />|**XBee Module - Series 1**<br />The wireless communicator.|[Adafruit](https://www.adafruit.com/products/128)|
|<img src="https://cdn-shop.adafruit.com/82x62/126-03.jpg" alt="XBee Adapter kit - v1.1" />|**XBee Adapter kit - v1.1**<br />The adapter that makes using the XBee module as simple as possible.|[Adafruit](https://www.adafruit.com/products/126)|
|<img src="https://cdn-shop.adafruit.com/82x62/266-04.jpg" alt="Premium Female/Female Jumper Wires" />|**Premium Female/Female Jumper Wires**<br />Great for connecting your circuit to the DIN 7-pole socket and plug.|[Adafruit](https://www.adafruit.com/products/266)|
|<img src="http://uk.farnell.com/productimages/thumbnail/en_GB/1814812-40.jpg" alt="Lumberg 7-pole DIN Circular Socket" />|**Lumberg 7-pole DIN Circular Socket (0122 07-1)**<br />The socket that will connect to the hand switch.|[Farnell](http://uk.farnell.com/lumberg/0122-07-1/socket-free-din-bayonet-7way/dp/1814817)|
|<img src="http://uk.farnell.com/productimages/thumbnail/en_GB/1814819-40.jpg" alt="Lumberg 7-pole DIN Circular Plug" />|**Lumberg 7-pole DIN Circular Plug (0131 07-1)**<br />The plug that will connect to the motor controller.|[Farnell](http://uk.farnell.com/lumberg/0131-07-1/plug-free-din-7way/dp/1814824)|

### Module assembly 

...

### Module software

...

## HomeKit bridge

The bridge is a Raspberry Pi device that connects to the local network and exposes at least one height-adjustable desk through HomeKit. It also contains a XBee module which is used to send and receive commands to and from the desk module.

<img src="https://cloud.githubusercontent.com/assets/198988/21696849/0d0362e6-d390-11e6-9af5-f057f5d6df14.gif" alt="HomeKit bridge" width="560" />

### Bridge parts list

||Part|Vendor|
|---|---|---|
|<img src="https://cdn-shop.adafruit.com/82x62/3055-06.jpg" alt="Raspberry Pi 3 - Model B" />|**Raspberry Pi 3 - Model B**<br />Probably the best device for the bridge, it's fast and comes with on-board wifi.|[Adafruit](https://www.adafruit.com/products/3055)|
|<img src="https://cdn-shop.adafruit.com/82x62/2310-07.jpg" alt="Adafruit Perma-Proto HAT for Pi Mini Kit" />|**Adafruit Perma-Proto HAT for Pi Mini Kit**<br />A prototyping HAT for soldering the components onto. If you prefer to use a breadboard, this is unnecessary.|[Adafruit](https://www.adafruit.com/products/2310)|
|<img src="https://cdn-shop.adafruit.com/82x62/128-01.jpg" alt="XBee Module - Series 1" />|**XBee Module - Series 1**<br />The wireless communicator.|[Adafruit](https://www.adafruit.com/products/128)|
|<img src="https://cdn-shop.adafruit.com/82x62/126-03.jpg" alt="XBee Adapter kit - v1.1" />|**XBee Adapter kit - v1.1**<br />The adapter that makes using the XBee module as simple as possible.|[Adafruit](https://www.adafruit.com/products/126)|
|<img src="https://cdn-shop.adafruit.com/82x62/2767-00.jpg" alt="8GB SD Card with Raspbian Jessie Operating System" />|**8GB SD Card with Raspbian Jessie Operating System**<br />For the Raspberry Pi. Can alternatively use an empty card and copy the OS onto it.|[Adafruit](https://www.adafruit.com/products/2767)|

### Bridge assembly 

...

### Bridge software

...

## Attribution

[Borislav Bertoldi](http://www.mikrocontroller.net/user/show/boris_b) has
made a similar project using a USB-connected remote. He has developed
the `DeskControl.ino` file.

[Joe F.](http://www.mikrocontroller.net/user/show/easylife) has helped
reverse-engineer the signals sent by the motor controller.

## License

This project is licensed under MIT (c) David Knezić unless
otherwise specified.
