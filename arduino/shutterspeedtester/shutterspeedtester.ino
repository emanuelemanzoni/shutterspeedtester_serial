
// shutter speed tester settings
#define RECEIVER_PIN A1  // pin to read the light sensor output.
#define TIMEOUT 8000000  // timeout value for analogPulsIn().
#define THRESHOLD 200  // analogPlusIn() thinks HI, if the light sensor value is higher than this value.
#define MIN_DURATION 500 // usecs. ignore the duration if it is lower than this value.

unsigned long analogPulseIn(uint8_t pin, uint8_t state, unsigned long timeoutInMicros, int threshold = THRESHOLD) {

      unsigned long startTime = micros();

      // Wait until the previous pulse stops.
      while (true) {
        unsigned int value = analogRead(pin);
        if ((state == HIGH && value < threshold) || (state == LOW && value >= threshold)) {
          break;
        }
        if (micros() - startTime >= timeoutInMicros) {
          return 0;
        }
      }

      // Wait until the pulse starts.
      while (true) {
        unsigned int value = analogRead(pin);
        if ((state == HIGH && value >= threshold) || (state == LOW && value < threshold)) {
          break;
        }
        if (micros() - startTime >= timeoutInMicros) {
          return 0;
        }
      }

      unsigned long time0 = micros();

      // Wait until the pulse stops.
      while (true) {
        unsigned int value = analogRead(pin);
        if ((state == HIGH && value < threshold) || (state == LOW && value >= threshold)) {
          break;
        }
        if (micros() - startTime >= timeoutInMicros) {
          return 0;
        }
      }

      unsigned long endTime = micros();
      return (endTime - time0);
}


void displayInitialScreen() {
  Serial.println("Shutter Speed Tester");
}

void displayDuration(unsigned long duration) {
  double msecs = duration / 1000.0;
  String text = String("Time: ") + msecs + String(" msecs");
  //display.println(text);
  Serial.println(text);
}

void displaySpeed(unsigned long duration) {
  String text;
  if (duration >= 1000000.0) {
    double secs = duration / 1000000.0;
    text = String("Speed: ") + secs + String(" secs");
  } else {
    double speeed = 1000000.0 / duration;
    text = String("Speed: 1/") + speeed + String(" secs");
  }
  //display.println(text);
  Serial.println(text);
}

void displayResult(unsigned long duration) {
  //display.clearDisplay();
  displayDuration(duration);
  displaySpeed(duration);
  //display.display();  
}

void setup() {
  pinMode(RECEIVER_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  displayInitialScreen();

  unsigned long duration = analogPulseIn(RECEIVER_PIN, HIGH, TIMEOUT);
  if (duration > MIN_DURATION) {
    displayResult(duration);
    delay(10000);
  }  
Serial.println();
}
