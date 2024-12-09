#include "LoRaWan_APP.h"
#include "Arduino.h"

//----------------------------------------------------------------
// BME680 Sensor and BH1750 Light Sensor Libraries
#include "BME680.h"  // Include the BME680 Sensor library
#include <BH1750.h>  // Include the BH1750 light sensor library
#include <Wire.h>    // Include the Wire library for I2C communication

BME680_Class BME680;  //< Create an instance of the BME680 sensor
BH1750 lightMeter;    //< Create an instance of the BH1750 light sensor
//----------------------------------------------------------------

/*
   Configuration for LoRaWAN RGB LED indicators:
   - Red: Sending data
   - Purple: Successfully joined the network
   - Blue: Receiving data in RxWindow1
   - Yellow: Receiving data in RxWindow2
   - Green: Successfully received data
*/

/* OTAA (Over-The-Air Activation) parameters */
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x** }; // Application EUI - identifier for the application
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0xA3, 0x** }; // Device EUI - unique identifier for the device
uint8_t appKey[] = { 0xF2, 0x2D, 0xF6, 0x49, 0x6E, 0x88, 0xA0, 0x94, 0x60, 0xAD, 0x0F, 0x4E, 0xA9, 0xD1, 0x94, 0x** }; // Application key for OTAA

/* ABP (Activation By Personalization) parameters */
uint8_t nwkSKey[] = { 0xE8, 0x12, 0xC1, 0xC6, 0x7F, 0x59, 0xA6, 0x61, 0xCE, 0xF4, 0x07, 0x43, 0xAA, 0x85, 0xFA, 0x00 }; // Network session key
uint8_t appSKey[] = { 0x7B, 0x88, 0x2E, 0x17, 0x3B, 0x72, 0xDF, 0x87, 0xA3, 0xE7, 0x98, 0x5C, 0x08, 0x5E, 0xAA, 0x00 }; // Application session key
uint32_t devAddr = (uint32_t)0x26013E5D; // Device address for ABP

/* LoRaWAN channel mask, default channels 0-7 */
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/* LoRaWAN region, selected in the Arduino IDE tools */
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/* LoRaWAN class, supported classes are A and C */
DeviceClass_t loraWanClass = LORAWAN_CLASS;

/* Application data transmission duty cycle in milliseconds */
uint32_t appTxDutyCycle = 600000;  // Change this value to adjust the sending frequency

/* OTAA or ABP activation mode */
bool overTheAirActivation = LORAWAN_NETMODE;

/* ADR (Adaptive Data Rate) enable/disable */
bool loraWanAdr = LORAWAN_ADR;

/* Network reservation for keeping network info in flash memory */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port for data transmission */
uint8_t appPort = 2;

/* Number of transmission trials if acknowledgment is not received */
uint8_t confirmedNbTrials = 4;

/* Baseline variable for sensor data */
uint16_t baseline;

/* Counters and maximum retries for sensor readings */
int count;
int maxtry = 10;

/* Variables to hold sensor data */
float Temperature, Humidity, Pressure, lux, VOC_gases, Airquality;

/*!
   \brief   Calculates the Indoor Air Quality (IAQ) index
*/
float CalculateIAQ() {
  float hum_weighting = 0.25;  // Humidity effect contributes 25% to the IAQ score
  float gas_weighting = 0.75;  // Gas effect contributes 75% to the IAQ score

  float hum_score, gas_score;
  float gas_reference = VOC_gases;
  float hum_reference = 40;
  
  // Calculate humidity contribution to IAQ index
  if (Humidity >= 38 && Humidity <= 42)
    hum_score = 0.25 * 100;  // Humidity is optimal
  else {  // Sub-optimal humidity
    if (Humidity < 38)
      hum_score = 0.25 / hum_reference * Humidity * 100;
    else {
      hum_score = ((-0.25 / (100 - hum_reference) * Humidity) + 0.416666) * 100;
    }
  }

  // Calculate gas contribution to IAQ index
  float gas_lower_limit = 27200;  // Bad air quality limit
  float gas_upper_limit = 32000;  // Good air quality limit
  if (gas_reference > gas_upper_limit)
    gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit)
    gas_reference = gas_lower_limit;
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100;

  // Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = hum_score + gas_score;

  return air_quality_score;
}

/*!
   \brief   Prepares the payload of the frame for transmission
*/
static void prepareTxFrame(uint8_t port) {
  pinMode(Vext, OUTPUT);  // Configure Vext pin as output
  digitalWrite(Vext, LOW); // Set Vext pin low
  delay(500);  // Wait for 500 ms

  pinMode(GPIO0, OUTPUT); // Configure GPIO0 pin as output
  digitalWrite(GPIO0, LOW); // Set GPIO0 pin low

  //-- BME680 Sensor
  Wire.begin();  // Initialize I2C communication
  BME680.begin(I2C_STANDARD_MODE);  // Initialize BME680 sensor in standard I2C mode
  delay(1000);  // Wait for 1 second
  BME680.setOversampling(TemperatureSensor, Oversample16);  // Set oversampling for temperature sensor
  BME680.setOversampling(HumiditySensor, Oversample16);  // Set oversampling for humidity sensor
  BME680.setOversampling(PressureSensor, Oversample16);  // Set oversampling for pressure sensor
  BME680.setIIRFilter(IIR4);  // Set IIR filter for BME680 sensor
  BME680.setGas(320, 150);  // Set gas measurement parameters

  delay(3000);  // Wait for 3 seconds for sensor warm-up

  // Read sensor data
  static int32_t bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings;
  BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);
  delay(500);  // Wait for 500 ms
  BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);
  
  // Retry reading sensor data if values are not valid
  while ((bme680_temp_readings > 6000 || bme680_humidity_readings == 100 || bme680_humidity_readings == 0) && count < maxtry) {
    BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);
    delay(250);  // Wait for 250 ms before retrying
  }

  // Convert sensor readings to appropriate units
  Temperature = bme680_temp_readings / 100.0;
  Humidity = bme680_humidity_readings / 1000.0;
  Pressure = bme680_pressure_readings / 100.0;
  VOC_gases = bme680_gas_readings;
  Airquality = CalculateIAQ();  // Calculate IAQ index

  Wire.end();  // End I2C communication

  //-- BH1750 Light Sensor
  pinMode(GPIO0, OUTPUT); // Configure GPIO0 pin as output
  digitalWrite(GPIO0, HIGH); // Set GPIO0 pin high

  Wire.begin();  // Initialize I2C communication

  count = 0;
  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2);  // Initialize BH1750 sensor in high resolution mode
  delay(100);  // Wait for 100 ms
  lux = lightMeter.readLightLevel();  // Read light level
  delay(100);  // Wait for 100 ms
  lux = lightMeter.readLightLevel();  // Read light level again
  lightMeter.end();  // End BH1750 sensor
  Wire.end();  // End I2C communication
  digitalWrite(GPIO0, LOW); // Set GPIO0 pin low
  pinMode(GPIO0, ANALOG); // Configure GPIO0 pin as analog input

  //-- Battery Voltage
  unsigned int batteryVoltage = getBatteryVoltage();  // Read battery voltage from ADC pin
  appDataSize = 12;  // Set the payload size to 12 bytes

  // Prepare data payload for transmission
  int Temperature_payload = Temperature * 100;
  unsigned int Humidity_payload = Humidity * 100;
  unsigned int Pressure_payload = Pressure / 2;
  unsigned int Airquality_payload = Airquality * 100;
  unsigned int Light_payload = lightMeter.readLightLevel();
  
  // Encode temperature data
  appData[0] = highByte(Temperature_payload);
  appData[1] = lowByte(Temperature_payload);
  // Encode humidity data
  appData[2] = highByte(Humidity_payload);
  appData[3] = lowByte(Humidity_payload);
  // Encode atmospheric pressure data
  appData[4] = highByte(Pressure_payload);
  appData[5] = lowByte(Pressure_payload);
  // Encode air quality data
  appData[6] = highByte(Airquality_payload);
  appData[7] = lowByte(Airquality_payload);
  // Encode light data
  appData[8] = highByte(Light_payload);
  appData[9] = lowByte(Light_payload);
  // Encode battery voltage data
  appData[10] = highByte(batteryVoltage);
  appData[11] = lowByte(batteryVoltage);

  // Print sensor data to the serial monitor
  Serial.print(" Temp = ");
  Serial.print(Temperature);
  Serial.print(" C, RelHum = ");
  Serial.print(Humidity);
  Serial.print("%, ");
  Serial.print("Pressure = ");
  Serial.print(Pressure);
  Serial.print("hPa, ");
  Serial.print(" Gas Resistance = ");
  Serial.print(VOC_gases);
  Serial.print(" Ohm, ");
  Serial.print("Air quality score = ");
  Serial.print(Airquality);
  Serial.print("%, ");
  Serial.print("Light = ");
  Serial.print(lux);
  Serial.print("lx, ");
  Serial.print("BatteryVoltage = ");
  Serial.print(batteryVoltage);
  Serial.println("mV");
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate
#if (AT_SUPPORT)
  enableAt();  // Enable AT commands if supported
#endif
  deviceState = DEVICE_STATE_INIT;  // Set initial device state
  LoRaWAN.ifskipjoin();  // Check if device should skip join
}

void loop() {
  switch (deviceState) {
    case DEVICE_STATE_INIT:
      {
#if (LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();  // Automatically generate DevEUI from chip ID if enabled
#endif
#if (AT_SUPPORT == 1)
        getDevParam();  // Retrieve device parameters if AT commands are supported
#endif
        printDevParam();  // Print device parameters to serial monitor
        LoRaWAN.init(loraWanClass, loraWanRegion);  // Initialize LoRaWAN with specified class and region
        deviceState = DEVICE_STATE_JOIN;  // Change state to join
        break;
      }
    case DEVICE_STATE_JOIN:
      {
#if (ACTIVE_REGION == 1)
        setSubBand2();  // Set sub-band for the region if required
#endif
        LoRaWAN.join();  // Attempt to join the LoRaWAN network
        break;
      }
    case DEVICE_STATE_SEND:
      {
        prepareTxFrame(appPort);  // Prepare data frame for transmission
        // if (checkcopyprotect() == true)
        {
          LoRaWAN.send();  // Send data frame
        }
        deviceState = DEVICE_STATE_CYCLE;  // Change state to cycle
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission with a random duty cycle
        txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);
        LoRaWAN.cycle(txDutyCycleTime);  // Set the next transmission cycle
        deviceState = DEVICE_STATE_SLEEP;  // Change state to sleep
        break;
      }
    case DEVICE_STATE_SLEEP:
      {
#if (TriggerInt == 1)
        if (accelWoke) {
          if (IsLoRaMacNetworkJoined) {
            prepareTxFrame(appPort);  // Prepare data frame if network is joined
            if (checkcopyprotect() == true) {
              LoRaWAN.send();  // Send data frame
            }
            //} else {
            //  LoRaWAN.join();
            //  prepareTxFrame(appPort);
            //  LoRaWAN.send();
          }
          accelWoke = false;  // Reset acceleration wake flag
        }
#endif
        LoRaWAN.sleep();  // Put the device to sleep
        break;
      }
    default:
      {
        deviceState = DEVICE_STATE_INIT;  // Reset to initial state on default case
        break;
      }
  }
}
