#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SLAVE 4
RF24 radio(7, 8);
const byte address[6] = "00001";
int a = 0;

 int text = 0;
void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE);
  Wire.onRequest(loop);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);

  radio.startListening();
  
}
 
void loop () {
  if (radio.available()) {
     
     radio.read(&text, sizeof(text));
    Serial.println(text);
    if(text == 11){  
      Wire.write(11);
      Serial.println("화장실");
    }

    if(text == 22){  
      Wire.write(22);
      Serial.println("회의실");
    }

    if(text == 33){  
      Wire.write(33);
      Serial.println("식당");
    }
    
  }
}

   
  

