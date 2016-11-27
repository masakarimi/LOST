//Basic adafruitIO example
//post a random value every second



// edit the config.h tab and enter your Adafruit IO credentials
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include "config.h"

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();


int randVal;


int sendRate = 1000;
long lastSend = 0; // you can declair it as 0 or leave it.



// set up the 'myRandoVal' feed
//AdafruitIO_Feed *myRandoVal = io.feed("myRandoVal"); // start as an object and defining the name of the variable in this case "myRandoVal"
AdafruitIO_Feed *tempVal = io.feed("tempVal");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect(); //connects to your Adafruit account via the attached .h file.

  // wait for a connection
  while (io.status() < AIO_CONNECTED) { //AIO_Connected is an status, where 0 means no and 1 means yes.
    Serial.print(".");
    delay(500);

    if (!tempsensor.begin()) {
      Serial.println("Couldn't find MCP9808!");
      while (1);
    }

    // we are connected
    Serial.println();
    Serial.println(io.statusText());

  }
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run(); // this is handling the connection, persisted connection. IMPORTANT it has to be at the top of the loop.

  //pick a random number
  // randVal = random(0,255);

  tempsensor.shutdown_wake(0);

  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t");
  Serial.print(f); Serial.println("*F");
  delay(250);

  Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere

  delay(2000);


  if (millis() - lastSend >= sendRate) // this is a timer, millis is duration, how long has it been on eversinceit has been on.
  {
    // save randVal to the 'myRandoVal' feed on Adafruit IO
    tempVal->save(c); //syntax of pointing to it, that's how the library has been written

    //print the value to the serial monitor so you can check if its working
    Serial.print("sending -> ");
    Serial.println(c);

    lastSend = millis();
  }
}
