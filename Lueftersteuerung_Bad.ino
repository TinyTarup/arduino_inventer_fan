/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
const int forwardPin = 5; // D1 
const int reversePin = 4; // D2
const int taster = 14; // D5
const int runs = 700;
const int cooldown = 100;

// 0 = warte auf taster
// 1 = forward
// 2 = forward_cooldown
// 3 = reverse
// 4 = reverse_cooldown
int modus = 0;

int pressed = 0;
int runsForward = 0;
int runsReverse = 0;
int runsCooldown = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  Serial.println("Starte Lüftersteuerung setup");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(forwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);
  pinMode(taster, INPUT);
  Serial.println("Lüftersteuerung setup DONE!");
}

void resetRuns() {  
  runsForward = 0;
  runsReverse = 0;
  runsCooldown = 0;
}

void stopFans() {
  digitalWrite(forwardPin, LOW);
  digitalWrite(reversePin, LOW);
}

void goForward() {
  digitalWrite(forwardPin, HIGH);
  digitalWrite(reversePin, LOW);  
}

void goReverse() {
  digitalWrite(forwardPin, LOW);
  digitalWrite(reversePin, HIGH);  
}

// the loop function runs over and over again forever
void loop() {
  pressed = digitalRead(taster);
  if (pressed == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);    
    // if the button is pressed
    if (modus == 0) {
      // we are going forward now!
      goForward();
      modus = 1;
    } else {
      // we are stopping now!
      stopFans();    
      modus = 0;
    }
    resetRuns();
    delay(1000);
  } else {
    digitalWrite(LED_BUILTIN, LOW);  
    // if button wasn't pressed, we continue normal operation
    if (modus == 1) {
      if (runsForward >= runs) {
        // we are cooling down now!
        stopFans();
        modus = 2;
        resetRuns();
      }
      runsForward++;
    }

    if (modus == 2) {
      if (runsCooldown >= cooldown) {
        goReverse();
        modus = 3;
        resetRuns();
      }
      runsCooldown++;
    }

    if (modus == 3) {
      if (runsReverse >= runs) {
        stopFans();
        modus = 4;
        resetRuns();
      }
      runsReverse++;
    }

    if (modus == 4) {
      if (runsCooldown >= cooldown) {
        goForward();
        modus = 1;
        resetRuns();
      }
      runsCooldown++;
    }
  }
  delay(100);
}
