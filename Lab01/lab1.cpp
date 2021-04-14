//============================================================================
// Name        : pigpioPUSHLED.cpp
// Author      : CA ABID
// Version     :
// Copyright   : 2021
// Description : Control a LED, Ansi-style
//============================================================================

#include <iostream>
#include <pigpio.h>

#define O_LED 17    // Using GPIO17
#define I_PUSH 2    // Using GPIO2
using namespace std;
int main(int argc, char *argv[]) {
	cout << "Running pigpio program" << endl;
	if (gpioInitialise()<0) {
		cout<<"Failure..."<<endl;
		exit(-1);
	}
	gpioSetMode(O_LED, PI_OUTPUT);
	gpioSetMode(I_PUSH,PI_INPUT);
	gpioSetPullUpDown(I_PUSH,PI_PUD_UP);
	while (1) {
		if (gpioRead(I_PUSH)==0) {
			while (gpioRead(I_PUSH)==0);
			gpioWrite(O_LED,!gpioRead(O_LED));
		}

	}
	cout << "gpioTerminate()..." << endl;
	gpioTerminate(); // call this when done with library
	return 0;
}

