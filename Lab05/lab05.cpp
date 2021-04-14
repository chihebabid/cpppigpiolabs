//============================================================================
// Name        : lab05.cpp
// Author      : CA ABID
// Version     : 0.1
// Copyright   : 2021
// Description : Reading Temperature and humidity from a DHT11 sensor by using a callback function
//============================================================================
#include <iostream>
#include <unistd.h>
#include <pigpio.h>
#include <thread>
#include <chrono>
#include <tuple>
#include "CDht11Cb.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (gpioInitialise() < 0) {
		cout << "Failure..." << endl;
		exit(-1);
	}
	cout<<"Create Dht11"<<endl;
	CDht11Cb Dht11(16);
	for (int i = 0; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		tuple<int,int> data = Dht11.Measure();
		std::cout << "Temperature: " << get<0>(data) << " °C" << std::endl;
		std::cout << "Humidity: " << std::get<1>(data) << "%" << std::endl;
		std::cout << std::endl;

	}

	gpioTerminate();
	return 0;
}

