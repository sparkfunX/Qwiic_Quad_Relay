SparkX Qwiic Quad Relay
========================================

-> [![SparkX Qwiic Quad Relay](https://cdn.sparkfun.com/assets/parts/1/3/3/6/3/15032-Qwiic_Quad_Relay-01.jpg)](https://cdn.sparkfun.com/assets/parts/1/3/3/6/3/15032-Qwiic_Quad_Relay-01.jpg) <-

[*SparkX Qwiic Quad Relay (SPX-15032)*](https://www.sparkfun.com/products/15032)
The Qwiic Quad Relay is a product designed for switching not one, but **four** high powered devices, from your Arduino or other low powered microcontroller using I<sup>2</sup>C. It's rocking four [relays](https://www.sparkfun.com/products/100) on its' PCB rated up to 5 Amps per channel at 250VAC. Each channel is sporting its own colored LED and silk for easy identification, and has screw terminals for easy connection. 

The board is made to integrate easily into the [Qwiic environment](https://www.sparkfun.com/qwiic) with Qwiic connectors on each side for easy daisy chaining. At the heart of the product is an ATtiny84 with its own I<sup>2</sup>C address awaiting for I<sup>2</sup>C commands that are designated to turn on and off individual channels or _all_ the channels at once! All of this is detailed nicely in provided example code, which also walks you through changing the boards single I<sup>2</sup>C address. There is a four pin header broken out for those of you not taking advantage of the Qwiic connector.

The onboard barrel jack is rated for wall warts in the range, 7-15V but there is a jumper on the underside in the case that your wall wart is 5V. Messing with such high voltage can be dangerous! We've included many safety precautions onto the PCB including, wide traces designed for high amperage, ground isolation between the relay and other circuitry, and a milled out area around the common pin of the relay.

Repository Contents
-------------------

* **/Firmware** - Latest firmware and examples. 
* **/Arduino Example** - Examples on how to use the product with buttons and switches, with hardware interrupts and without.
* **/Hardware** - Eagle design files (.brd, .sch)

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact techsupport@sparkfun.com.

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release under the same license.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
