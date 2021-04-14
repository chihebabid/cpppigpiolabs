/*
 * CDht11.h
 *
 *  Created on: 13 avr. 2021
 *      Author: chiheb
 */

#ifndef CDHT11CB_H_
#define CDHT11CB_H_

#include <cstdint>
#include <tuple>
class CDht11Cb {
public:
	/*
	 * No need for copying objects of this class
	 */
	CDht11Cb(const CDht11Cb&) = delete;
	CDht11Cb& operator=(const CDht11Cb&) = delete;


	/*
	 * Constructor
	 * Provide an instance to a raspberry pi object and the GPIO pin for data transmission.
	 */
	explicit CDht11Cb(const uint8_t gpioPin);

	/*
	 * This method starts a measurement and returns the data from the last measurement.
	 * This behaviour is due to how the DHT11 sensor works. Make two consecutive readings
	 * to get the most current data. But wait at least 5 seconds between measurements,
	 * otherwise the data will be wrong.
	 */
	std::tuple<uint8_t,uint8_t> Measure();

private:
	static uint8_t _gpioPin;
	static uint32_t _highTime;
	static uint32_t _lowTime;
	static uint64_t _data;
	static uint32_t _counter;
	/*
	 * Signal the sensor that we want to read the data.
	 */
	void SendStartSignal();

	/*
	 * Wait for the GPIO pin to get low. Returns the microseconds waited until the pin got low.
	 */
	static void CBFunction(int gpio, int level, uint32_t tick);

	/*
	 * Wait for the GPIO pin to get high. Returns the microseconds waited until the pin git high.
	 */
	int WaitForHigh();

	/*
	 * Extracts the relevant data from the binary raw data.
	 */
	auto ProcessData(uint64_t Data);

	/*
	 * Calculates the checksum for the data to make sure there was no error during data transmission.
	 */
	uint8_t CalculateParity(uint8_t HumidityHigh, uint8_t HumidityLow,
			uint8_t TemperatureHigh, uint8_t TemperatureLow);



};

#endif /* CDHT11CB_H_ */
