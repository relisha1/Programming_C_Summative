#define NUM_LIGHTS 2

// ================= STRUCT =================
struct TrafficLight {
  int redPin, yellowPin, greenPin, buttonPin;
  int vehicleCount;
  String state;
  unsigned long lastChangeTime;
  int phase; // 0=Green, 1=Yellow, 2=Red
};

// Pointer for dynamic memory
TrafficLight *lights;

// Timing (in milliseconds)
unsigned long greenTime = 5000;
unsigned long yellowTime = 2000;
unsigned long redTime = 5000;

// SETUP 
void setup() {
  Serial.begin(9600);

  // Allocate memory
  lights = (TrafficLight*) malloc(NUM_LIGHTS * sizeof(TrafficLight));

  // Intersection 1
  lights[0] = {2, 3, 4, 8, 0, "RED", 0, 2};

  // Intersection 2
  lights[1] = {5, 6, 7, 9, 0, "RED", 0, 0};

  // Set pin modes
  for (int i = 0; i < NUM_LIGHTS; i++) {
    pinMode(lights[i].redPin, OUTPUT);
    pinMode(lights[i].yellowPin, OUTPUT);
    pinMode(lights[i].greenPin, OUTPUT);
    pinMode(lights[i].buttonPin, INPUT_PULLUP);
    lights[i].lastChangeTime = millis();
  }

  Serial.println("System Started...");
  Serial.println("Commands: 1=All Red | 2=Reset");
}

void loop() {
  unsigned long currentTime = millis();

  for (int i = 0; i < NUM_LIGHTS; i++) {

    // VEHICLE DETECTION 
    if (digitalRead(lights[i].buttonPin) == LOW) {
      lights[i].vehicleCount++;
      Serial.print("Vehicle detected at Intersection ");
      Serial.println(i);
      delay(200); // small debounce
    }

    // TRAFFIC CONTROL
    switch (lights[i].phase) {

      case 0: // GREEN
        setLight(i, LOW, LOW, HIGH);
        lights[i].state = "GREEN";

        if (currentTime - lights[i].lastChangeTime >
            greenTime + lights[i].vehicleCount * 1000) {
          lights[i].phase = 1;
          lights[i].lastChangeTime = currentTime;
        }
        break;

      case 1: // YELLOW
        setLight(i, LOW, HIGH, LOW);
        lights[i].state = "YELLOW";

        if (currentTime - lights[i].lastChangeTime > yellowTime) {
          lights[i].phase = 2;
          lights[i].lastChangeTime = currentTime;
        }
        break;

      case 2: // RED
        setLight(i, HIGH, LOW, LOW);
        lights[i].state = "RED";

        if (currentTime - lights[i].lastChangeTime > redTime) {
          lights[i].phase = 0;
          lights[i].vehicleCount = 0; // reset count
          lights[i].lastChangeTime = currentTime;
        }
        break;

      default: // ERROR HANDLING
        Serial.println("Error: Invalid State");
        lights[i].phase = 2;
        break;
    }

    logData(i);
  }

  serialMenu();
}


// Control LEDs
void setLight(int i, int r, int y, int g) {
  digitalWrite(lights[i].redPin, r);
  digitalWrite(lights[i].yellowPin, y);
  digitalWrite(lights[i].greenPin, g);
}

// Display data
void logData(int i) {
  Serial.print("Intersection ");
  Serial.print(i);
  Serial.print(" | State: ");
  Serial.print(lights[i].state);
  Serial.print(" | Cars: ");
  Serial.println(lights[i].vehicleCount);
}

// Serial menu
void serialMenu() {
  if (Serial.available()) {
    char input = Serial.read();

    if (input == '1') {
      Serial.println("Manual Override: All RED");
      for (int i = 0; i < NUM_LIGHTS; i++) {
        setLight(i, HIGH, LOW, LOW);
      }
    }

    else if (input == '2') {
      Serial.println("System Reset");
      for (int i = 0; i < NUM_LIGHTS; i++) {
        lights[i].vehicleCount = 0;
        lights[i].phase = 0;
      }
    }

    else {
      Serial.println("Invalid Command");
    }
  }
}