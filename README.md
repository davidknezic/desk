# ↕️ HomeKit-enabled Office Desk

> Making a height-adjustable office desk
> controllable by Siri using Apple's HomeKit framework.

<img src="https://cloud.githubusercontent.com/assets/198988/21662365/9be90a2c-d2d9-11e6-8f4c-d6f92b6cd230.gif" alt="Office Desk controlled by Siri" width="280" /> <img src="https://cloud.githubusercontent.com/assets/198988/21662454/022f88f6-d2da-11e6-80cb-79a3a32c0abf.gif" alt="Office Desk controlled using Home app" width="280" />

## Intro

...

## Prerequisites

You'll need a height-adjustable desk with a [LogicData](http://www.logicdata.at)
Compact or a compatible motor controller. The motor controller is attached to the bottom of the desk. The hand switch should be connected to the controller with a DIN-7 connector.

<img src="https://cloud.githubusercontent.com/assets/198988/21726518/4b60a118-d43d-11e6-84fa-aa79e5bd5e02.JPG" alt="Motor controller DIN-7" width="560" />

Additionally, you'll need soldering equipment or a breadboard with jumper wires in order to interconnect all the components.

## Desk hardware

<img src="https://cloud.githubusercontent.com/assets/198988/21741728/8b722216-d4df-11e6-8eb1-3eb6fe2b2aec.gif" alt="Desk hardware" width="560" />

### Parts list

||Part|Vendor|
|---|---|---|
|<img src="https://cdn-shop.adafruit.com/82x62/1086-08.jpg" alt="Arduino Micro with Headers - 5V 16MHz" />|**Arduino Micro with Headers - 5V 16MHz**<br />The perfect device to link between the motor controller and the hand switch, as it's 5V and very small.|[Adafruit](https://www.adafruit.com/products/1086)|
|<img src="https://cdn-shop.adafruit.com/82x62/128-01.jpg" alt="XBee Module - Series 1" />|**XBee Module - Series 1**<br />The wireless communicator.|[Adafruit](https://www.adafruit.com/products/128)|
|<img src="https://cdn-shop.adafruit.com/82x62/126-03.jpg" alt="XBee Adapter kit - v1.1" />|**XBee Adapter kit - v1.1**<br />The adapter that makes using the XBee module as simple as possible.|[Adafruit](https://www.adafruit.com/products/126)|

## HomeKit bridge

The bridge is a Raspberry Pi device that connects to the local network and exposes at least one height-adjustable desk through HomeKit.

<img src="https://cloud.githubusercontent.com/assets/198988/21696849/0d0362e6-d390-11e6-9af5-f057f5d6df14.gif" alt="HomeKit bridge" width="560" />

### Parts list

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

## History

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
