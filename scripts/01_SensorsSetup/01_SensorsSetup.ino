#include <Wire.h>
#include <BH1750.h>
#include <BME680.h> // Include the BME680 sensor library

BH1750 lightMeter; // Create an instance of the BH1750 light sensor
BME680_Class BME680; // Create an instance of the BME680 environmental sensor

/**************************************************************************************************
** Declare global variables, instantiate classes                                                **
**************************************************************************************************/

static int32_t temp, humidity, pressure, gas;  // Variables to store sensor readings
float Temperature, Humidity, Pressure, Gas, Air_Quality, Light; // Variables for processed data

// Function to calculate the Indoor Air Quality (IAQ) index
float CalculateIAQ() {
  float hum_weighting = 0.25; // Humidity contributes 25% to the air quality score
  float gas_weighting = 0.75; // Gas resistance contributes 75% to the air quality score

  float hum_score, gas_score;
  float gas_reference = gas; // Current gas resistance
  float hum_reference = 40.0; // Optimal humidity reference point

  // Calculate humidity contribution to the IAQ index
  if (Humidity >= 38 && Humidity <= 42) {
    hum_score = hum_weighting * 100; // Humidity is optimal
  } else { 
    // Humidity is sub-optimal
    if (Humidity < 38) {
      hum_score = hum_weighting / hum_reference * Humidity * 100;
    } else {
      hum_score = ((-hum_weighting / (100 - hum_reference) * Humidity) + 0.416666) * 100;
    }
  }

  // Calculate gas resistance contribution to the IAQ index
  float gas_lower_limit = 35000;  // Lower limit for poor air quality (in Ohms)
  float gas_upper_limit = 70000;  // Upper limit for good air quality (in Ohms)
  
  if (gas_reference > gas_upper_limit) gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit) gas_reference = gas_lower_limit;
  
  gas_score = (gas_weighting / (gas_upper_limit - gas_lower_limit) * gas_reference - 
              (gas_lower_limit * (gas_weighting / (gas_upper_limit - gas_lower_limit)))) * 100;

  // Combine humidity and gas scores for the final IAQ value (0-100%, where 100% is good air quality)
  float air_quality_score = hum_score + gas_score;

  return air_quality_score;
}

void setup() {
  // Turn on Vext (external power)
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

  // Initialize the BH1750 light sensor
  lightMeter.begin();
  Serial.begin(115200);  // Start serial communication at 115200 baud rate
  Serial.println(F("BH1750 Test begin"));

  Serial.println(F("Starting BME680 sensor..."));
  while (!BME680.begin(I2C_STANDARD_MODE)) {  // Initialize BME680 using I2C
    Serial.println(F("Unable to find BME680. Retrying in 5 seconds..."));
    delay(5000);
  }

  // Configure BME680 sensor settings
  BME680.setOversampling(TemperatureSensor, Oversample16);  // 16x oversampling for temperature
  BME680.setOversampling(HumiditySensor, Oversample16);     // 16x oversampling for humidity
  BME680.setOversampling(PressureSensor, Oversample16);     // 16x oversampling for pressure
  BME680.setIIRFilter(IIR4);  // Set IIR filter to 4 samples
  BME680.setGas(320, 150);    // Set gas heater to 320°C for 150ms
}

void loop() {
  // Read light level from BH1750
  float Light = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(Light);
  Serial.print(" lx | ");

  // Read data from BME680 sensor
  BME680.getSensorData(temp, humidity, pressure, gas);  
  Temperature = temp / 100.0;      // Convert temperature to °C
  Humidity = humidity / 1000.0;    // Convert humidity to %
  Pressure = pressure;             // Pressure in Pascals
  Gas = gas;                       // Gas resistance in Ohms
  Air_Quality = CalculateIAQ();    // Calculate IAQ based on sensor data

  // Print sensor readings to serial
  Serial.print("Temperature: ");
  Serial.print(Temperature);
  Serial.print(" °C | ");

  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.print(" % | ");

  Serial.print("Pressure: ");
  Serial.print(Pressure);
  Serial.print(" Pa | ");

  Serial.print("Gas: ");
  Serial.print(Gas);  // Ohms
  Serial.print(" kOhms | ");

  Serial.print("Air Quality: ");
  Serial.print(Air_Quality);  // Print IAQ value
  Serial.println(" %");

  delay(12000);  // Wait 12 seconds before the next loop iteration
}
