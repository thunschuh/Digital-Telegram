#include <SoftwareSerial.h>

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(7, 6); // RX, TX


int buttonState = 0;
int lastButtonState = 0;
int startPressed = 0;
int endPressed = 0;

int timeHold = 0;
int timeReleased = 0;

int pinNum = 2;
boolean boolPause = false;

boolean paused = true;
boolean messageW = false;
boolean morse = false;


int intervalLetter = 2000;
int intervalWord = 5000;
int intervalMessage = 10000;


int buzzer = 9; //buzzer to arduino pin 9
int ledBlue = 3;
int ledYellow = 5;
int ledRed = 4;

String morseCode = "";
String message = "";
String transMessage = "";

char LATIN_CHARACTERS[] = {
  // Numbers
  '0', '1', '2', '3', '4',
  '5', '6', '7', '8', '9',
  // Letters
  'a', 'b', 'c', 'd', 'e',
  'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z',
  // Special
  '.', '?', '@', ' '
};

String MORSE_CHARACTERS[] = {
  // Numbers
  "-----", ".----", "..---", "...--", "....-",
  ".....", "-....", "--...", "---..", "----.",
  // Letters
  ".-",    "-...",  "-.-.",  "-..", ".",
  "..-.",  "--.",   "....",  "..",  ".---",
  "-.-",   ".-..",  "--",    "-.",  "---",
  ".--.",  "--.-",  ".-.",   "...",  "-",
  "..-",   "...-",  ".--",   "-..-", "-.--",  "--..",
  // Special
  ".-.-.-", "..--..", ".--.-.", " "
};



void setup() {
  // put your setup code here, to run once:
  pinMode(pinMode, INPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(ledBlue, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);

  XBee.begin(9600);
  Serial.begin(9600);

}

//time variables for pause
unsigned long previousMillis = 0;
unsigned long previousWMillis = 0; // previous Word break pause
unsigned long previousMMillis = 0; // pervious  message break pause

unsigned long currentMillis;
unsigned long currentMMillis;
unsigned long currentWMillis;

void loop() {

  if (Serial.available())
  { // If data comes in from serial monitor, send it out to XBee
    XBee.write(Serial.read());
  }
  if (XBee.available())
  { // If data comes in from XBee, send it out to serial monitor
    Serial.write(XBee.read());


  }

  buttonState = digitalRead(pinNum);
  if (buttonState == LOW) {
    currentMillis = millis();
    currentMMillis = millis();
    currentWMillis = millis();

    ///// CHECKING FOR A MORSE CODE
    //AFTER 5 SECONDS BREAK
    if ((unsigned long)(currentMillis - previousMillis) >= intervalLetter && morse == false) {
      if (paused == false) {
        for (int i = 0; i < 40; i++) {
          if ( morseCode == MORSE_CHARACTERS[i]) {
            //Serial.println("MORSE CHARACTER:" + MORSE_CHARACTERS[i] + " ");
            message += morseCode;
            transMessage += LATIN_CHARACTERS[i];
            morse = true;
            digitalWrite(ledBlue, HIGH);
            delay(1000);
            digitalWrite(ledBlue, LOW);
          } // end if
        }// end for
      } // end  if paulse
      morseCode = "";
    } // end main if

    // NEXT WORD WORD WORD WORD
    else if ((currentWMillis - previousWMillis) >= intervalWord) {
      if (morse == true && messageW == false) {
        message += " ";
        transMessage += " ";
        //  Serial.println(message);
        messageW = true;
        // XBee.write("message");
        digitalWrite(ledYellow, HIGH);
        delay(1000);
        digitalWrite(ledYellow, LOW);
        delay(1000);
      }
      morse = false;
    } //END NEXT WORD

    //END MESSAGE
    if ((currentMMillis - previousMMillis) >= intervalMessage ) {
      if (messageW == true && paused == false) {
        // Serial.println("ENDE");
        // Serial.println(transMessage);
        message = "";
        transMessage = "";
        paused = true;
      }

    }
  }// end buttonstateLOW

  if (buttonState != lastButtonState) {
    paused = false;
    messageW = false;
    morse = false;
    if (buttonState == HIGH ) {
      paused = false;

      // the button was just pressed
      startPressed = millis();
      timeReleased = startPressed - endPressed;
      // the button was just released
    } else {
      endPressed = millis();
      timeHold = endPressed - startPressed;

      if (timeReleased >= 200 && timeHold <= 300 ) {
        Serial.println(".");
        XBee.write(".");
        morseCode += ".";

        tone(buzzer, 200); // Send 1KHz sound signal...
        delay(200);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
      }
      if (timeHold > 300 ) {
        Serial.println("-");
        XBee.write("-");


        morseCode += "-";

        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(800);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
      }
    } /// end else

    // save the current state as the last state,
    //for next time through the loop

    lastButtonState = buttonState;

    previousMillis = currentMillis;
    previousWMillis = currentWMillis;
    previousMMillis = currentMMillis;

  } // end if

}

