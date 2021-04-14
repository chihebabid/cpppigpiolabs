//============================================================================
// Name        : lab04.cpp
// Author      : CA ABID
// Version     :
// Copyright   : 2021
// Description : Temperature and humidity reading from DHT11 sensor by polling
//============================================================================
#include "CDht11.h"
#include <iostream>
#include <pigpio.h>
#include <thread>
#include <chrono>
#include <tuple>


int main(int argc, char *argv[]) {
	if (gpioInitialise() < 0) {
		std::cout << "Can't initialize pigpio..." << std::endl;
		exit(-1);
	}
	CDht11 Dht11(16);
	for (int i = 0; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::tuple<int,int> data = Dht11.Measure();
		std::cout << "Temperature: " << std::get<0>(data) << " °C" << std::endl;
		std::cout << "Humidity: " << std::get<1>(data) << "%" << std::endl;
		std::cout << std::endl;
	}

	gpioTerminate();
	return 0;
}

