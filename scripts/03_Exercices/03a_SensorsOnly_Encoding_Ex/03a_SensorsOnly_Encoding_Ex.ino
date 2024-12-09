/*! @file I2CDemo.ino

  @section I2CDemoauthor Author

  Written by Arnd <Arnd@Zanduino.Com> at https://www.github.com/SV-Zanshin

  @section I2CDemoversions Changelog

  | Version | Date       | Developer  | Comments
  | ------- | ---------- | ---------- | ------------------------------------------------------------ |
  | 1.0.3   | 2020-07-04 | SV-Zanshin | Issue #25 implement clang-formatting                         |
  | 1.0.2   | 2020-05-09 | SV-Zanshin | Issue #8  clean up comments and code                         |
  | 1.0.1   | 2019-01-30 | SV-Zanshin |           Removed old comments                               |
  | 1.0.1   | 2019-01-26 | SV-Zanshin | Issue #3  convert documentation to Doxygen                   |
  | 1.0.0b  | 2018-06-30 | SV-Zanshin |           Cloned from original BME280 program                |
*/
#include <Arduino.h>
#include <LoRaWan_APP.h>

#include <Wire.h>
#include <BH1750.h>
#include <BME680.h>// Include the BME680 Sensor library

BH1750 lightMeter; // Create an instance of the BH1750
BME680_Class BME680; // Create an instance of the BME680

/**************************************************************************************************
** Declare all program constants                                                                 **
**************************************************************************************************/
const uint32_t SERIAL_SPEED {
  115200
};  ///< Set the baud rate for Serial I/O

/**************************************************************************************************
** Declare global variables, instantiate classes                                                **
**************************************************************************************************/

static int32_t  bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings, VOC_gases;  // BME readings
float Temperature, Humidity, Pressure, lux, Airquality;

float CalculateIAQ()
{
  float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
  float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

  float hum_score, gas_score;
  float gas_reference = bme680_gas_readings;
  float hum_reference = 40;
  int getgasreference_count = 0;

  //Calculate humidity contribution to IAQ index
  if (Humidity >= 38 && Humidity <= 42)
    hum_score = 0.25 * 100; // Humidity +/-5% around optimum
  else
  { //sub-optimal
    if (Humidity < 38)
      hum_score = 0.25 / hum_reference * Humidity * 100;
    else
    {
      hum_score = ((-0.25 / (100 - hum_reference) * Humidity) + 0.416666) * 100;
    }
  }

  //Calculate gas contribution to IAQ index
  float gas_lower_limit = 100000;  // Bad air quality limit
  float gas_upper_limit = 200000; // Good air quality limit
  if (gas_reference > gas_upper_limit)
    gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit)
    gas_reference = gas_lower_limit;
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100;

  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = hum_score + gas_score;

  return air_quality_score;
}

void setup() {
  //Vext ON
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

  //-- Light Meter BH1750

  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));

  Serial.begin(SERIAL_SPEED);  // Start serial port at Baud rate

  Serial.print(F("Starting I2CDemo example program for BME680\n"));
  Serial.print(F("- Initializing BME680 sensor\n"));
  while (!BME680.begin(I2C_STANDARD_MODE)) {  // Start BME680 using I2C, use first device found
    Serial.print(F("-  Unable to find BME680. Trying again in 5 seconds.\n"));
    delay(5000);
  }  // of loop until device is located
  Serial.print(F("- Setting 16x oversampling for all sensors\n"));
  BME680.setOversampling(TemperatureSensor, Oversample16);  // Use enumerated type values
  BME680.setOversampling(HumiditySensor, Oversample16);     // Use enumerated type values
  BME680.setOversampling(PressureSensor, Oversample16);     // Use enumerated type values
  Serial.print(F("- Setting IIR filter to a value of 4 samples\n"));
  BME680.setIIRFilter(IIR4);  // Use enumerated type values
  Serial.print(F("- Setting gas measurement to 320\xC2\xB0\x43 for 150ms\n"));  // "�C" symbols
  BME680.setGas(320, 150);  // 320�c for 150 milliseconds
}


void loop() {

  //-- BME680
  BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);  // Get readings
  Temperature = bme680_temp_readings / 100.0;
  Humidity = bme680_humidity_readings / 1000.0;
  Pressure = bme680_pressure_readings;
  VOC_gases = bme680_gas_readings; // volatile organic compounds readings
  Airquality = CalculateIAQ();

    //-- Light Meter BH1750
  float lux = lightMeter.readLightLevel();

    //-- Battery
  unsigned int batteryVoltage = getBatteryVoltage(); // Measured voltage at ADC pin in mV

  Serial.print("Temperature:");
  Serial.print(Temperature);   // Temp readings in degrees
  Serial.print("C ");

  Serial.print("Humidity:");
  Serial.print(Humidity); // Readings in relative humidity
  Serial.print("% ");

  Serial.print("Pressure:");
  Serial.print(Pressure);//Pressure Pascals
  Serial.print("Pa ");

  Serial.print("Gas:");
  Serial.print(VOC_gases / 1000.0); // Resistance  kOhm
  Serial.print("kOhm ");

  Serial.print("AirQ:");
  Serial.print(Airquality); // final IAQ index value (0-100% where 100% is good quality air)
  Serial.print("% ");

  Serial.print("Light:");
  Serial.print(lux);
  Serial.print("lx ");

  Serial.print("BatteryVoltage:");
  Serial.print(batteryVoltage);
  Serial.println("mV ");

  /* Write encoded payload here*/

  int Temperature_payload = Temperature * 100;
  unsigned int Humidity_payload = Humidity*100 ;
  unsigned int Pressure_payload = Pressure;

  /*--------------------------------------------------------*/
  // encode temperature data
  appData[0] = highByte(Temperature_payload);
  appData[1] = lowByte(Temperature_payload);
  Serial.print("Payload temperature: ");
  Serial.print(appData[0], HEX);
  Serial.print(" ");
  Serial.println(appData[1], HEX);
  /*--------------------------------------------------------*/
  // encode humidity data
  appData[2] = Humidity_payload >> 8;
  appData[3] = Humidity_payload;
  Serial.print("Payload humidity: ");
  Serial.print(appData[2], HEX);
  Serial.print(" ");
  Serial.println(appData[3], HEX);
  /*--------------------------------------------------------*/
  // encode atmospheric pressure data
  appData[4] = Pressure_payload >> 16; 
  appData[5]  = Pressure_payload >> 8;        
  appData[6]  = Pressure_payload;       
  Serial.print("Payload pressure: ");
  Serial.print(appData[4], HEX);
  Serial.print(" ");
  Serial.print(appData[5], HEX);
  Serial.print(" ");
  Serial.println(appData[6], HEX);


  /*--------------------------------------------------------*/
  // encode Air Quality data


  /*--------------------------------------------------------*/
  // encode light meter data



  /*--------------------------------------------------------*/
  // encode battery data

  
  /*--------------------------------------------------------*/
   /*--------------------------------------------------------*/
  
  // Print the full payload in a single line
  Serial.print("Encoded Payload: ");
  for (int i = 0; i < 12; i++) {
    if (appData[i] < 0x10) Serial.print("0");  // Add leading zero for single-digit hex values
    Serial.print(appData[i], HEX);
  }
  Serial.println();  // Move to the next line
  Serial.println("");

  delay(30000);

}
