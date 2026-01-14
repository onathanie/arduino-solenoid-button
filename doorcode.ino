const int relayPin = 8;
const int buttonPin = 2;

bool relayActive = false;
bool stableButtonState = LOW;
bool lastReading = LOW;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long unlockTime = 0;
const unsigned long holdDuration = 3000; // 3 seconds

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(relayPin, HIGH); // Locked by default (active LOW)
  Serial.begin(9600);
}

void loop() {
  bool reading = digitalRead(buttonPin);

  // Debounce
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // VALID button press (LOW → HIGH)
      if (stableButtonState == HIGH && !relayActive) {
        digitalWrite(relayPin, LOW); // Unlock
        relayActive = true;
        unlockTime = millis();
        Serial.println("Unlocked (3s virtual hold)");
      }
    }
  }

  // Auto-lock after 3 seconds
  if (relayActive && millis() - unlockTime >= holdDuration) {
    digitalWrite(relayPin, HIGH); // Lock
    relayActive = false;
    Serial.println("Auto-locked");
  }

  lastReading = reading;
}
