/*
 * CDht11.cpp
 *
 *  Created on: 13 avr. 2021
 *      Author: chiheb
 */

#include "CDht11.h"
#include <pigpio.h>
#include <thread>
#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <tuple>
#define FREQUENCY 10000000



CDht11::CDht11(const uint8_t gpioPin) :
		_gpioPin(gpioPin) {
	gpioSetMode(gpioPin, PI_OUTPUT);
	gpioWrite(gpioPin, 1);
}

auto CDht11::ProcessData(uint64_t Data) {
	uint8_t HumidityHigh = (Data >> 32) & 0xFF;
	uint8_t HumidityLow = (Data >> 24) & 0xFF;
	uint8_t TemperatureHigh = (Data >> 16) & 0xFF;
	uint8_t TemperatureLow = (Data >> 8) & 0xFF;
	uint8_t Parity = Data & 0xFF;

	if (Parity
			!= CalculateParity(HumidityHigh, HumidityLow, TemperatureHigh,
					TemperatureLow)) {
		throw std::runtime_error("Parity checksum failed.");
	}


	return std::make_tuple(TemperatureHigh,HumidityHigh);
}

std::tuple<uint8_t,uint8_t> CDht11::Measure() {
	uint64_t data = 0;
	SendStartSignal();
	gpioSetMode(_gpioPin, PI_INPUT);
	try {
		WaitForLow();
		WaitForHigh();
		WaitForLow();
		for (int i = 0; i < 40; ++i) {
			data <<= 1;
			int LowTime = WaitForHigh();
			int HighTime = WaitForLow();
			if (LowTime < HighTime) {
				data |= 0x1;
			}
		}
		WaitForHigh();
	} catch (...) {
		gpioSetMode(_gpioPin, PI_OUTPUT);
		gpioWrite(_gpioPin, 1);
		throw;
	}
	gpioSetMode(_gpioPin, PI_OUTPUT);
	gpioWrite(_gpioPin, 1);
	return ProcessData(data);
}

int CDht11::WaitForLow() {
	auto StartTime = gpioTick();
	while (gpioRead(_gpioPin)) {
		if (FREQUENCY < gpioTick() - StartTime) {
			throw std::runtime_error(
					"Timeout while waiting for pin to get low.");
		}
	}
	return gpioTick() - StartTime;
}

int CDht11::WaitForHigh() {
	auto StartTime = gpioTick();
	while (!gpioRead(_gpioPin)) {
		if (FREQUENCY < gpioTick()- StartTime) {
			throw std::runtime_error(
					"Timeout while waiting for pin to get high.");
		}
	}
	return gpioTick() - StartTime;
}

void CDht11::SendStartSignal() {
	gpioWrite(_gpioPin, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	gpioWrite(_gpioPin, 1);
}



uint8_t CDht11::CalculateParity(uint8_t HumidityHigh, uint8_t HumidityLow,
		uint8_t TemperatureHigh, uint8_t TemperatureLow) {
	return static_cast<uint8_t>(HumidityHigh + HumidityLow + TemperatureHigh
			+ TemperatureLow);
}
