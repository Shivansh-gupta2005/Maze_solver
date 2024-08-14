

// Constants
const int numSensors = 3;  // Number of Sharp IR sensors
const int sensorPins[numSensors] = {A0, A1, A2}; // Analog pins for IR sensors
const int mapWidth = 10;
const int mapHeight = 10;
const int maxDistance = 50;  // Maximum distance in cm
const int obstacleThreshold = 10;  // Threshold distance to detect obstacles

// Global variables
int mazeMap[mapWidth][mapHeight] = {0}; // 0: free, 1: obstacle
int robotX = 5; // Example robot position X
int robotY = 5; // Example robot position Y

void setup() {
  Serial.begin(9600);
  // Initialize sensors
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  // Initialize map
  initializeMap();
}

void loop() {
  // Read sensor values
  int sensorValues[numSensors];
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Localize and update map
  updateMap(robotX, robotY, sensorValues);

  // Print the map
  printMap();

  delay(1000); // Delay for readability
}

void updateMap(int x, int y, int sensorValues[]) {
  // Simple approach to update the map
  for (int i = 0; i < numSensors; i++) {
    int distance = getDistance(sensorValues[i]);
    if (distance < obstacleThreshold) {
      int dx = 0;
      int dy = 0;

      // Simple mapping logic: Assume sensors are front, right, back, left
      switch (i) {
        case 0: // Front sensor
          dy = 1;
          break;
        case 1: // Right sensor
          dx = 1;
          break;
        case 2: // Left sensor
          dx = -1;
          break;
      }

      int mapX = x + dx;
      int mapY = y + dy;

      if (mapX >= 0 && mapX < mapWidth && mapY >= 0 && mapY < mapHeight) {
        mazeMap[mapX][mapY] = 1; // Mark as obstacle
      }
    }
  }
}

int getDistance(int sensorValue) {
  // Simple calibration: inverse relationship (adjust based on your sensors)
  return map(sensorValue, 0, 1023, maxDistance, 0);
}

void initializeMap() {
  for (int x = 0; x < mapWidth; x++) {
    for (int y = 0; y < mapHeight; y++) {
      mazeMap[x][y] = 0; // Initialize map as free
    }
  }
}

void printMap() {
  for (int y = 0; y < mapHeight; y++) {
    for (int x = 0; x < mapWidth; x++) {
      if (mazeMap[x][y] == 1) {
        Serial.print("X "); // Obstacle
      } else {
        Serial.print(". "); // Free space
      }
    }
    Serial.println();
  }
}
