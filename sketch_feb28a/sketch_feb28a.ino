const int redLED = 13;
const int yellowLED = 12;
const int greenLED = 11;
const int buzzer = 9; // Buzzer on pin 9

float sodiumLevel;
float potassiumLevel;
String userInput;

void setup() {
  Serial.begin(9600);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Buzzer test on startup
  Serial.println("Testing buzzer...");
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  Serial.println("System Ready! Type 'exit' anytime to stop the program.");
}

float readLevel(const char* ion, float minVal, float maxVal) {
  Serial.print("Enter ");
  Serial.print(ion);
  Serial.print(" level (in mEq/L) [");
  Serial.print(minVal);
  Serial.print(" - ");
  Serial.print(maxVal);
  Serial.println(" preferred]: ");
  
  while (!Serial.available());
  String input = Serial.readStringUntil('\n');
  input.trim();
  return input.toFloat();
}

void loop() {
  // Check for 'exit' command
  if (Serial.available()) {
    userInput = Serial.readStringUntil('\n');
    userInput.trim();

    if (userInput == "exit") {
      Serial.println("Exiting the program...");
      while (true); // Infinite loop to stop further execution
    }
  }

  // Read Sodium and Potassium levels
  sodiumLevel = readLevel("Sodium", 135, 145);
  potassiumLevel = readLevel("Potassium", 3.5, 5.0);

  Serial.print("Sodium Level: ");
  Serial.println(sodiumLevel);
  Serial.print("Potassium Level: ");
  Serial.println(potassiumLevel);

  // Reset LEDs and buzzer
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(buzzer, LOW);

  // Determine hydration status
  if (sodiumLevel > 160 && potassiumLevel < 2.5) {
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
    Serial.println("Severe Dehydration! [Red LED + Buzzer ON]");
    playAlertTune();
  } else if (sodiumLevel > 145 && potassiumLevel < 3.0) {
    digitalWrite(redLED, HIGH);
    Serial.println("Acute Dehydration. [Red LED]");
  } else if ((sodiumLevel >= 145 && sodiumLevel <= 160) && (potassiumLevel >= 3.0 && potassiumLevel < 3.5)) {
    digitalWrite(yellowLED, HIGH);
    Serial.println("Borderline Hydration. [Yellow LED ON]");
  } else if (sodiumLevel >= 135 && sodiumLevel <= 145 && potassiumLevel >= 3.5 && potassiumLevel <= 5.0) {
    digitalWrite(greenLED, HIGH);
    Serial.println("Hydrated. [Green LED ON]");
  } else {
    Serial.println("Invalid input. Please check the entered levels.");
  }

  delay(1000); // Wait 1 second before next input
}
void playAlertTune() {
  for (int i = 0; i < 3; i++) { // Repeat 3 times
    tone(buzzer, 2000, 200); // High-pitched beep
    delay(300); // Short pause between beeps
    tone(buzzer, 1500, 200); // Lower-pitched beep
    delay(300); // Short pause between beeps
  }
  noTone(buzzer); // Ensure buzzer is off after the tune
}
