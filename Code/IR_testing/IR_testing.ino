

#include <Arduino.h>
#include <IRremote.hpp> // include the library

bool signalReceived = false;

void setup() {
    Serial.begin(9600);
    pinMode(A4, INPUT_PULLUP); //IR signal
    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(A4, ENABLE_LED_FEEDBACK);

}

void loop() {
  Serial.println("Ready for input");
   
    do {
      
      Serial.println("waiting for input");
      signalReceived = false;
      if(IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.command == 0x15 && !IrReceiver.decodedIRData.flags) {
          signalReceived = true;
        }
        IrReceiver.resume();
      }
      delay(200);
      
    }
    while (signalReceived == false);
        
    Serial.println("Signal Received");
    delay(5000);
    
    
    
    
}
