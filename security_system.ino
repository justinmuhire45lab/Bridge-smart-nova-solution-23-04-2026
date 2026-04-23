const int RED_LED    = 2;
const int GREEN_LED  = 3;
const int BUZZER     = 6;
const int DOOR_BTN   = 7;
const int RESET_BTN  = 8;
const int LDR_PIN    = A0;

bool alarmActive  = false;
bool nightMode    = false;

unsigned long previousMillis = 0;
bool greenLedState = HIGH;

void setup() {
  pinMode(RED_LED,   OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER,    OUTPUT);
  pinMode(DOOR_BTN,  INPUT);
  pinMode(RESET_BTN, INPUT);

  Serial.begin(9600);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED,   LOW);
  digitalWrite(BUZZER,    LOW);

  Serial.println("MONITORING ACTIVE");
}

void loop() {
  int doorPressed  = digitalRead(DOOR_BTN);
  int resetPressed = digitalRead(RESET_BTN);
  int ldrValue     = analogRead(LDR_PIN);

  if (ldrValue > 600) {
    if (!nightMode) {
      nightMode = true;
      Serial.println("NIGHT MODE ON");
    }
  } else {
    nightMode = false;
  }

  if (doorPressed == HIGH && !alarmActive) {
    alarmActive = true;
    Serial.println("INTRUDER DETECTED");

    digitalWrite(GREEN_LED, LOW);

    for (int i = 0; i < 10; i++) {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BUZZER,  HIGH);
      delay(100);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER,  LOW);
      delay(100);
    }

    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER,  HIGH);
  }

  if (resetPressed == HIGH && alarmActive) {
    alarmActive = false;
    Serial.println("SYSTEM RESET");

    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER,  LOW);
    digitalWrite(GREEN_LED, HIGH);

    delay(300);

    Serial.println("MONITORING ACTIVE");
  }

  if (!alarmActive) {
    if (nightMode) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= 800) {
        previousMillis = currentMillis;
        greenLedState = !greenLedState;
        digitalWrite(GREEN_LED, greenLedState);
      }
    } else {
      digitalWrite(GREEN_LED, HIGH);
    }
  }

  delay(50);
}
