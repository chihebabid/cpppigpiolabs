/*
 * CDht11.cpp
 *
 *  Created on: 13 avr. 2021
 *      Author: chiheb
 */

#include "CDht11Cb.h"

#include <pigpio.h>
#include <thread>
#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <tuple>


CDht11Cb::CDht11Cb(const uint8_t gpioPin) {
	_gpioPin = gpioPin;
	_highTime = 0;
	_lowTime = 0;
	_data = 0;
	_counter = 0;
	gpioSetMode(gpioPin, PI_OUTPUT);
	gpioWrite(gpioPin, 1);
}

/*
 * Extract fields from acquired data and check parity
 */
auto CDht11Cb::ProcessData(uint64_t Data) {
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
	return std::make_tuple(TemperatureHigh, HumidityHigh);
}

/*
 * Make a measure of temperature & humidity
 */
std::tuple<uint8_t, uint8_t> CDht11Cb::Measure() {
	SendStartSignal();
	gpioSetMode(_gpioPin, PI_INPUT);
	gpioSetAlertFunc(static_cast<unsigned>(_gpioPin),CDht11Cb::CBFunction);
	_highTime = gpioTick();
	_data = 0;
	_counter = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	gpioSetAlertFunc(static_cast<unsigned>(_gpioPin), nullptr);
	gpioSetMode(_gpioPin, PI_OUTPUT);
	return ProcessData(_data);
}

/*
 * Callback function, here data from DHT11 are acquired
 */
void CDht11Cb::CBFunction(int gpio, int level, uint32_t tick) {
	if (level == 1) { // low edge
		_highTime = _highTime - tick;
		if (_counter - 3 > 0) {
			_data <<= 1;
			if (_lowTime < _highTime)
				_data |= 1;
		}
		_lowTime = tick;
	} else { // high edge
		_highTime = tick;
		_lowTime = _lowTime - tick;
	}
	_counter++;
}

/*
 * Request from a DHT11
 */
void CDht11Cb::SendStartSignal() {
	gpioWrite(_gpioPin, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	gpioWrite(_gpioPin, 1);
}

/*
 * Calculate parity
 */
uint8_t CDht11Cb::CalculateParity(uint8_t HumidityHigh, uint8_t HumidityLow,
		uint8_t TemperatureHigh, uint8_t TemperatureLow) {
	return static_cast<uint8_t>(HumidityHigh + HumidityLow + TemperatureHigh + TemperatureLow);
}

uint8_t CDht11Cb::_gpioPin ;
uint32_t CDht11Cb::_highTime ;
uint32_t CDht11Cb::_lowTime ;
uint64_t CDht11Cb::_data ;
uint32_t CDht11Cb::_counter;

