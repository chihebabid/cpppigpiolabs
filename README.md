# cpppigpiolabs
C++ labs based on pigpio for Raspberry Pi


This repository contains labs developed with C++ and are based on the pigpio library. Mainly, the C++17 standard is used. Several labs contain class utilities that can be integrated into a library allowing to ease the development of complex projects. Maybe, in near future, I'll propose a complete library for different peripherals.

Programs are tested on a Raspberry Pi 3 Model B+, and using the latest distribution Rasperry OS 5.4.

## Description

* **Lab 01:** A simple program to read the state of a pushbutton. When pressed, the state a LED is inversed.
* **Lab 02:** Here, instead of reading the state of the pushbutton by polling, we use a callback function that is called every time the pushbutton state is modified.
* **Lab 03:** Reading the temperature from MCP9808 snesor connected via i2c bus
* **Lab 04:** Reading the temperature and the humidity from a DHT11 sensor. The communication with DHT11 is ensured through the class CDht11 which can be integrated to any other project. Reading is performed by polling.   
* **Lab 05:** As the Lab 04, we perform the the reading of temperature and humidity from a DHT11. However, reading is performed through a callback function instead of polling. Thus, it uses less CPU usage, but it can not be well suitable for several sensors simultaneously as it can produce conflicts.
* **Lab 06:** Sending a message from a RPI3 to an Artduino through SPI
