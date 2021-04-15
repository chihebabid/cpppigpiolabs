//============================================================================
// Name        : pigpioLED.cpp
// Author      : CA ABID
// Version     :
// Copyright   : 2021
// Description : Reading Temperature and humidity from a DHT11 sensor by using a callback function
//============================================================================
#include <iostream>
#include <pigpio.h>
#include <cstring>
using namespace std;

int main(int argc, char *argv[]) {
	if (gpioInitialise() < 0) {
		cout << "Error initializing pigpio library..." << endl;
		exit(-1);
	}
	uint32_t flags=0x00;
	int hSpi=spiOpen(0,100000,flags);
	if (hSpi>0) {
		cout << "Error SPI initialization..."<<endl;
		gpioTerminate();
		exit(-1);
	}
	const char *sBuffer="Hello world SPI!";
	char rBuffer[100];
	//spiWrite(hSpi,(char *)buffer,3);
	spiXfer(hSpi,(char *)sBuffer,(char *)rBuffer,strlen(sBuffer));
	cout<<"Received : ";
	for (size_t j=0;j<strlen(sBuffer);j++) {
		cout<<rBuffer[j];
	}
	cout<<endl;
	spiClose(hSpi);
	gpioTerminate();
	return 0;
}

