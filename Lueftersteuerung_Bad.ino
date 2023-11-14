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
