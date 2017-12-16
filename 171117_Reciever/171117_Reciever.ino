#include <SoftwareSerial.h>

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3 ); // RX, TX

String message = Serial.readString();

unsigned long previousShort = 0;
unsigned long previousLong = 0;
char readChar   = Serial.read(); 

int interval = 1000;
void setup() {
  Serial.begin(9600);
}

void loop()
{


if (readChar == '.') {
  Serial.write("short");
} else if (readChar == '-') {
  Serial.write("long");
}
  
 for (int i = 0; i < message.length(); i++)
  {
    if (message.charAt(i) == '.')
    {
      Serial.println("SHORT");
    }
    else if (message.charAt(i)  == '-')
    {
      Serial.println("LONG");
    }
  } // for*/
  
}




