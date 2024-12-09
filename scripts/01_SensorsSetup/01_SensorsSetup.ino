#include <Wire.h>
#include <BME680.h>  // Include the BME680 sensor library

// Create an instance of the BME680 environmental sensor
BME680_Class BME680;

/**************************************************************************************************
** Declare global variables, instantiate classes                                                  **
**************************************************************************************************/

static int32_t temp, humidity, pressure, gas;  // Add 'gas' variable to match the function signature
float Temperature, Humidity, Pressure;    // Variables for processed data

/* Define GPIO pins for rain gauge and wind speed sensors */
#define Rain_PIN GPIO5
#define Speed_PIN GPIO3

/* Rain gauge and wind speed counters */
uint16_t rain_total = 0;
uint16_t windspeed_total = 0;

/* Function to increment the rain gauge counter */
void increment_rain_meter() {
  rain_total++;
  Serial.println("Rain gauge incremented");
  delay(50);  // Debounce delay to prevent multiple rapid triggers
}

/* Function to increment the wind speed counter */
void increment_windspeed_meter() {
  windspeed_total++;
  Serial.println("Wind speed incremented");
  delay(5);  // Debounce delay for wind speed measurement
}

void setup() {
  // Turn on Vext (external power)
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

  // Initialize Serial communication
  Serial.begin(115200);
  Serial.println(F("Starting BME680 sensor..."));

  // Initialize the BME680 sensor
  while (!BME680.begin(I2C_STANDARD_MODE)) {
    Serial.println(F("Unable to find BME680. Retrying in 5 seconds..."));
    delay(5000);
  }

  // Configure BME680 sensor settings
  BME680.setOversampling(TemperatureSensor, Oversample16);  // 16x oversampling for temperature
  BME680.setOversampling(HumiditySensor, Oversample16);     // 16x oversampling for humidity
  BME680.setOversampling(PressureSensor, Oversample16);     // 16x oversampling for pressure
  BME680.setIIRFilter(IIR4);                                // Set IIR filter to 4 samples

  // Setup rain gauge interrupt
  pinMode(Rain_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Rain_PIN), increment_rain_meter, FALLING);

  // Setup wind speed interrupt
  pinMode(Speed_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Speed_PIN), increment_windspeed_meter, FALLING);

  Serial.println(F("Interrupts attached. Setup complete."));
}



void loop() {
  // Read data from the BME680 sensor

// Call getSensorData with the correct number of arguments
BME680.getSensorData(temp, humidity, pressure, gas, true);

  Temperature = temp / 100.0;     // Convert temperature to °C
  Humidity = humidity / 1000.0;   // Convert humidity to %
  Pressure = pressure / 100.0;    // Convert pressure to hPa

  // Print sensor readings to serial
  Serial.print("Temperature: ");
  Serial.print(Temperature);
  Serial.print(" °C | ");

  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.print(" % | ");

  Serial.print("Pressure: ");
  Serial.print(Pressure);
  Serial.print(" hPa | ");

  // Print rain gauge and wind speed counts
  Serial.print("Rain Counter: ");
  Serial.print(rain_total);
  Serial.print(" | ");

  Serial.print("Wind Speed Counter: ");
  Serial.println(windspeed_total);

  delay(12000);  // Wait 12 seconds before the next loop iteration
}
