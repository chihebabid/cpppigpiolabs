//============================================================================
// Name        : lab02.cpp
// Author      : CA ABID
// Version     :
// Copyright   : 2021
// Description : Control a LED using a callback function to detect the state change a a Button
//============================================================================

#include <iostream>
#include <unistd.h>
#include <pigpio.h>

#define O_LED 17    // Using GPIO17
#define I_PUSH 2    // Using GPIO2
using namespace std;
void cbPushButton(int gpio, int level, uint32_t tick) {
	if (level==0) gpioWrite(O_LED,!gpioRead(O_LED));
}
int main(int argc, char *argv[]) {
	cout << "Running pigpio program" << endl;
	if (gpioInitialise()<0) {
		cout<<"Failure..."<<endl;
		exit(-1);
	}
	gpioSetMode(O_LED, PI_OUTPUT);
	gpioSetMode(I_PUSH,PI_INPUT);
	gpioSetPullUpDown(I_PUSH,PI_PUD_UP);
	gpioSetAlertFunc(I_PUSH,cbPushButton);
	while (1);
	gpioTerminate();
	return 0;
}



