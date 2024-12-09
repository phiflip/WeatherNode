#include <Arduino.h>
#include <LoRaWan_APP.h>
#include <Wire.h>
#include <BH1750.h>    // Include the BH1750 light sensor library
#include <BME680.h>    // Include the BME680 environmental sensor library

BH1750 lightMeter;     // Create an instance of the BH1750 light sensor
BME680_Class BME680;   // Create an instance of the BME680 environmental sensor

/**************************************************************************************************
** Declare all program constants                                                                 **
**************************************************************************************************/
const uint32_t SERIAL_SPEED{
  115200
};  ///< Set the baud rate for Serial I/O

/**************************************************************************************************
** Declare global variables, instantiate classes                                                **
**************************************************************************************************/

// Variables to hold raw sensor data from BME680
static int32_t bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings;

// Variables to hold processed sensor data
float Temperature, Humidity, Pressure, lux, VOC_gases, Airquality;

/**
 * Function to calculate the Indoor Air Quality (IAQ) index based on humidity and gas readings.
 * Combines humidity and gas resistance to produce a score between 0 and 100, where 100 indicates good air quality.
 */
float CalculateIAQ() {
  float hum_weighting = 0.25;  // Humidity contributes 25% to the IAQ score
  float gas_weighting = 0.75;  // Gas resistance contributes 75% to the IAQ score
  float hum_score, gas_score;
  float gas_reference = bme680_gas_readings;
  float hum_reference = 40;  // Ideal humidity reference value

  // Calculate humidity contribution to IAQ index
  if (Humidity >= 38 && Humidity <= 42) {
    hum_score = 0.25 * 100;  // Humidity within optimal range
  } else {                     
    // Calculate sub-optimal humidity score
    if (Humidity < 38) {
      hum_score = 0.25 / hum_reference * Humidity * 100;
    } else {
      hum_score = ((-0.25 / (100 - hum_reference) * Humidity) + 0.416666) * 100;
    }
  }

  // Calculate gas contribution to IAQ index
  float gas_lower_limit = 10000;  // Lower limit for bad air quality
  float gas_upper_limit = 20000;  // Upper limit for good air quality
  if (gas_reference > gas_upper_limit) gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit) gas_reference = gas_lower_limit;

  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - 
              (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100;

  // Combine humidity and gas scores for final IAQ score (0-100%)
  float air_quality_score = hum_score + gas_score;
  return air_quality_score;
}

void setup() {
  // Initialize external power for sensors
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);  // Allow sensors to stabilize after power-up

  // Initialize the BH1750 light sensor
  lightMeter.begin();
  Serial.begin(SERIAL_SPEED);  // Start serial communication for debugging

  // Initialize the BME680 sensor
  Serial.println(F("Starting I2CDemo example program for BME680"));
  Serial.println(F("- Initializing BME680 sensor"));
  while (!BME680.begin(I2C_STANDARD_MODE)) {  // Start BME680 using I2C
    Serial.println(F("-  Unable to find BME680. Trying again in 5 seconds."));
    delay(5000);
  }

  // Configure BME680 sensor settings
  Serial.println(F("- Setting 16x oversampling for all sensors"));
  BME680.setOversampling(TemperatureSensor, Oversample16);  // Set temperature oversampling
  BME680.setOversampling(HumiditySensor, Oversample16);     // Set humidity oversampling
  BME680.setOversampling(PressureSensor, Oversample16);     // Set pressure oversampling
  Serial.println(F("- Setting IIR filter to a value of 4 samples"));
  BME680.setIIRFilter(IIR4);  // Set IIR filter to 4 samples
  Serial.println(F("- Setting gas measurement to 320°C for 150ms"));
  BME680.setGas(320, 150);    // Set gas heater temperature to 320°C for 150ms
}

void loop() {
  // Read data from BME680 sensor
  BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);
  Temperature = bme680_temp_readings / 100.0;  // Convert temperature to °C
  Humidity = bme680_humidity_readings / 1000.0;  // Convert humidity to %
  Pressure = bme680_pressure_readings;  // Pressure in Pascals
  VOC_gases = bme680_gas_readings;  // Volatile Organic Compounds (VOC) resistance
  Airquality = CalculateIAQ();  // Calculate IAQ score

  // Read light level from BH1750 sensor
  lux = lightMeter.readLightLevel();

  // Read battery voltage (in millivolts)
  unsigned int batteryVoltage = getBatteryVoltage();

  // Print sensor readings to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(Temperature);  // Print temperature
  Serial.print(" °C ");

  Serial.print("Humidity: ");
  Serial.print(Humidity);  // Print humidity
  Serial.print(" % ");

  Serial.print("Pressure: ");
  Serial.print(Pressure);  // Print pressure
  Serial.print(" Pa ");

  Serial.print("Gas: ");
  Serial.print(VOC_gases);  // Print gas resistance
  Serial.print(" Ohm ");

  Serial.print("AirQ: ");
  Serial.print(Airquality);  // Print IAQ score
  Serial.print(" % ");

  Serial.print("Light: ");
  Serial.print(lux);  // Print light level
  Serial.print(" lx ");

  Serial.print("BatteryVoltage: ");
  Serial.print(batteryVoltage);  // Print battery voltage
  Serial.println(" mV ");

  // Encode the sensor data into a payload for transmission
  int Temperature_payload = Temperature * 100;  // Convert temperature to integer format (2 decimal places)
  unsigned int Humidity_payload = Humidity * 100;  // Convert humidity to integer format (2 decimal places)
  unsigned int Pressure_payload = Pressure / 2;  // Simplify pressure value
  unsigned int Airquality_payload = Airquality * 100;  // Convert IAQ to integer format (2 decimal places)
  unsigned int Light_payload = lux;  // Light level in lux

  /*--------------------------------------------------------*/
  // Encode temperature data
  appData[0] = highByte(Temperature_payload);
  appData[1] = lowByte(Temperature_payload);
  Serial.print("Payload temperature: ");
  Serial.print(appData[0], HEX);
  Serial.print(" ");
  Serial.println(appData[1], HEX);

  /*--------------------------------------------------------*/
  // Encode humidity data
  appData[2] = highByte(Humidity_payload);
  appData[3] = lowByte(Humidity_payload);
  Serial.print("Payload humidity: ");
  Serial.print(appData[2], HEX);
  Serial.print(" ");
  Serial.println(appData[3], HEX);

  /*--------------------------------------------------------*/
  // Encode atmospheric pressure data
  appData[4] = highByte(Pressure_payload);
  appData[5] = lowByte(Pressure_payload);
  Serial.print("Payload pressure: ");
  Serial.print(appData[4], HEX);
  Serial.print(" ");
  Serial.println(appData[5], HEX);

  /*--------------------------------------------------------*/
  // Encode Air Quality data
  appData[6] = highByte(Airquality_payload);
  appData[7] = lowByte(Airquality_payload);
  Serial.print("Payload air quality: ");
  Serial.print(appData[6], HEX);
  Serial.print(" ");
  Serial.println(appData[7], HEX);

  /*--------------------------------------------------------*/
  // Encode light meter data
  appData[8] = highByte(Light_payload);
  appData[9] = lowByte(Light_payload);
  Serial.print("Payload light: ");
  Serial.print(appData[8], HEX);
  Serial.print(" ");
  Serial.println(appData[9], HEX);

  /*--------------------------------------------------------*/
  // Encode battery voltage data
  appData[10] = highByte(batteryVoltage);
  appData[11] = lowByte(batteryVoltage);
  Serial.print("Payload battery: ");
  Serial.print(appData[10], HEX);
  Serial.print(" ");
  Serial.println(appData[11], HEX);


  /*--------------------------------------------------------*/
  
  // Print the full payload in a single line
  Serial.print("Encoded Payload: ");
  for (int i = 0; i < 12; i++) {
    if (appData[i] < 0x10) Serial.print("0");  // Add leading zero for single-digit hex values
    Serial.print(appData[i], HEX);
  }
  Serial.println();  // Move to the next line


  delay(5000);  // Wait 5 seconds before the next measurement cycle
}
