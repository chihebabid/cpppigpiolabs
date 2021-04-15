#include <SPI.h>
volatile int rSize = 0;
volatile bool finish = false;
char myArray[100];
void setup() {  
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  pinMode(SCK, INPUT);
  // turn on SPI in slave mode
  SPCR |= bit(SPE);
  // turn on interrupts
  SPI.attachInterrupt();
  Serial.println("Initialization done..."); 
}

ISR (SPI_STC_vect) // SPI interrupt routine
{
  myArray[rSize] = SPDR;  
  if (myArray[rSize] == '!') finish = true;
  rSize++;

}  // end of interrupt service routine (ISR) for SPI


void loop () {  
  if (finish) {
    Serial.print("Received message : ");
    for (int i = 0; i < rSize; i++)
      Serial.print((char)myArray[i]);
    Serial.println("\n=============================================");
    rSize = 0; finish = false;
  }
}
