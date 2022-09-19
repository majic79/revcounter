# README

The source here is for a small project that I am experimenting with on the Raspberry Pi Pico W as an IoT monitor for a small wind turbine - but there are many other potential applications.

The idea is to monitor the shaft speed of a small experimental wind turbine, using an OptoInterrupter (e.g. something based around a L393 amplifier) and a disc to cut the beam.

During the course of this project, I also added a small OLED display (SSD1306) as an inexpensive monitor, a LiPo shim for power and coded some display information as well.

This is very much work in progress - feel free to learn from my mistakes, if I can't set an example, then at least let me serve as the horrible warning.

Credit to the Lexus2k project (https://github.com/lexus2k/ssd1306) for the font I've used for the display.

## Rationale

I like using PlatformIO, but there is no baremetal Pico SDK available there (at least not an officially supported one), and the idea of adding a library simply to be able to hook into the low level hardware timer seemed a waste of effort. Given that the Raspberry Pi Foundation have put a lot of time and effort into developing an official SDK, that seems the most prudent route of development.

Strictly speaking, I don't need the OLED panel, but it's helpful to be able to see something when you are playing with hardware to try and understand what's happening and get some instant feedback. Who knows, there may be other applications, and at least now I know there's some knowledge I've developed that I can call on.

As for the OptoInterrupter - there's more than one sensor that I may want to use, this just seemed the simplest and easiet to get going, while also not presenting any load on the unit being measured

## SDK

 Please refer to the getting start guide on the Pico SDK for instructions on how to configure you build environment

 https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html