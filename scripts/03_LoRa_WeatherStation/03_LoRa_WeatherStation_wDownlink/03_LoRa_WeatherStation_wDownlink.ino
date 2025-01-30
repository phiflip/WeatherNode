#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "innerWdt.h"  // Inner Watchdog

//----------------------------------------------------------------
// BME680 Sensor Library
#include "BME680.h"
#include <Wire.h>

// Define GPIO3 as the interrupt pin for wind speed measurement
#define Speed_PIN GPIO3
// Define GPIO5 as the interrupt pin for the rain gauge
#define Rain_PIN GPIO5


// Create an instance for the BME680 sensor
BME680_Class BME680;
//----------------------------------------------------------------

/*
   LoRaWAN RGB status indicators:
   - Red: sending
   - Purple: joined successfully
   - Blue: RxWindow1
   - Yellow: RxWindow2
   - Green: data received
*/

/* OTAA Parameters */
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xA6, 0x6F };
uint8_t appKey[] = { 0x00, 0x00, 0xAE, 0x44, 0x56, 0x1F, 0xF4, 0x8A, 0xD2, 0x45, 0x0E, 0x87, 0x8C, 0xA0, 0x00, 0x00 };

/* ABP Parameters */
uint8_t nwkSKey[16] = { 0 };
uint8_t appSKey[16] = { 0 };
uint32_t devAddr = 0;

/* Transmission duty cycle in milliseconds */
uint32_t appTxDutyCycle = (30 * 1000);  // 30 seconds (in ms)

/* LoRaWAN Channel Mask (default channels 0-7) */
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

bool accelWoke = false;

/* LoRaWAN Region (EU868) */
LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_EU868;

/* LoRaWAN Device Class (A or C) */
DeviceClass_t loraWanClass = CLASS_A;

/* OTAA or ABP Mode */
bool overTheAirActivation = true;  // OTAA is enabled

/* Adaptive Data Rate (ADR) */
bool loraWanAdr = true;  // ADR is enabled

/* Save network info to flash (network reservation) */
bool keepNet = false;  // Network reservation is disabled

/* Confirmed or Unconfirmed Messages */
bool isTxConfirmed = false;  // Unconfirmed uplink messages

/* Application port */
uint8_t appPort = 2;
/* Number of trials to transmit the frame */
uint8_t confirmedNbTrials = 4;

/* Rain gauge counter */
uint8_t rain_total = 0;
/* Wind speed counter */
uint16_t windspeed_total = 0;

/* Sensor measurements */
float Temperature, Humidity, Pressure;


/* Function to prepare the payload for transmission */
static bool prepareTxFrame(uint8_t port, uint8_t restart) {
  // Power up sensors
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);
  pinMode(GPIO0, OUTPUT);
  digitalWrite(GPIO0, LOW);

  // Prepare local copies of counters to avoid concurrency issues
  int offset = 0;
  appPort = port;

  uint16_t rain_total_copy = rain_total;
  uint16_t windspeed_total_copy = windspeed_total;

  // Initialize BME680 sensor and collect data
  Wire.begin();
  BME680.begin(I2C_STANDARD_MODE);
  delay(1000);
  BME680.setOversampling(TemperatureSensor, Oversample16);
  BME680.setOversampling(HumiditySensor, Oversample16);
  BME680.setOversampling(PressureSensor, Oversample16);
  BME680.setIIRFilter(IIR4);

  delay(3000);  // Burn-in delay for new sensors

  static int32_t temperature, humidity, pressure, gas;
  BME680.getSensorData(temperature, humidity, pressure, gas);
  delay(500);
  BME680.getSensorData(temperature, humidity, pressure, gas);

  Temperature = temperature;
  Humidity = humidity;
  Pressure = pressure;
  Wire.end();

  // Measure battery voltage
  unsigned int batteryVoltage = getBatteryVoltage();
  appDataSize = 12;  // Total bytes in the payload

  // Prepare sensor data for transmission
  int Temperature_payload = Temperature;
  unsigned int Humidity_payload = Humidity;
  unsigned int Pressure_payload = Pressure / 2;
  unsigned int Duty_payload = appTxDutyCycle / 10000;

  // Encode temperature data
  appData[0] = highByte(Temperature_payload);
  appData[1] = lowByte(Temperature_payload);
  // Encode humidity data
  appData[2] = highByte(Humidity_payload);
  appData[3] = lowByte(Humidity_payload);
  // Encode atmospheric pressure data
  appData[4] = highByte(Pressure_payload);
  appData[5] = lowByte(Pressure_payload);
  // Encode battery voltage data
  appData[6] = highByte(batteryVoltage);
  appData[7] = lowByte(batteryVoltage);
  // Encode rain events data
  appData[8] = rain_total;
  // Encode wind events data
  appData[9] = highByte(windspeed_total);
  appData[10] = lowByte(windspeed_total);
  // Encode duty cycle time
  appData[11] = lowByte(Duty_payload);


  // Print sensor data to the serial monitor
  Serial.print("Temp = ");
  Serial.print(Temperature);
  Serial.print(" °C, Humidity = ");
  Serial.print(Humidity);
  Serial.print("%, Pressure = ");
  Serial.print(Pressure);
  Serial.print(" Pa, Battery Voltage = ");
  Serial.print(batteryVoltage);
  Serial.print(" mV, Rain Counter = ");
  Serial.println(rain_total);
  Serial.print("Windspeed Counter = ");
  Serial.println(windspeed_total);
  Serial.print("Duty cycle time ");
  Serial.println(appTxDutyCycle);
  return true;
}

extern bool IsLoRaMacNetworkJoined;

/* Interrupt service routine for accelerometer wakeup */
void accelWakeup() {
  accelWoke = true;
  delay(50);  // Debounce delay
}

void setup() {
  Serial.begin(115200);

  // Setup rain gauge interrupt
  pinMode(Rain_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Rain_PIN), increment_rain_meter, FALLING);

  // Setup wind speed interrupt
  pinMode(Speed_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Speed_PIN), increment_windspeed_meter, FALLING);

  Serial.println("Interrupts attached");

  // Enable the watchdog timer
  innerWdtEnable(true);
  delay(200);
}


volatile bool rainDetected = false;
volatile bool windDetected = false;

void increment_rain_meter() {
  rainDetected = true;  // Set a flag (instead of using delay)
}

void increment_windspeed_meter() {
  windDetected = true;
}

// Function to handle downlink messages
void downlinkCallback(uint8_t *payload, uint8_t size) {
  if (size > 0) {
    switch (payload[0]) {
      case 0x00:
        appTxDutyCycle = 30000;  // 30 seconds
        Serial.println("Duty cycle set to 30 seconds");
        break;
      case 0x30:
        appTxDutyCycle = 1800000;  // 30 minutes
        Serial.println("Duty cycle set to 30 minutes");
        break;
      case 0x60:
        appTxDutyCycle = 3600000;  // 1 hour
        Serial.println("Duty cycle set to 1 hour");
        break;
      default:
        Serial.println("Unknown downlink command");
        break;
    }
  }
}

void loop() {

  if (rainDetected) {
    rain_total++;
    Serial.println("Rain meter incremented");
    rainDetected = false;
  }

  if (windDetected) {
    windspeed_total++;
    Serial.println("Wind speed incremented");
    windDetected = false;
  }

  static uint8_t restart = 1;

  switch (deviceState) {
    case DEVICE_STATE_INIT:
      printDevParam();
      LoRaWAN.init(loraWanClass, loraWanRegion);
      deviceState = DEVICE_STATE_JOIN;
      break;

    case DEVICE_STATE_JOIN:
      LoRaWAN.join();
      break;

    case DEVICE_STATE_SEND:
      prepareTxFrame(appPort, restart);
      restart = 0;
      LoRaWAN.send();
      rain_total = 0;
      windspeed_total = 0;
      deviceState = DEVICE_STATE_CYCLE;
      break;

    case DEVICE_STATE_CYCLE:
      txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;

    case DEVICE_STATE_SLEEP:
      LoRaWAN.sleep();
      break;
  }
}

void downLinkDataHandle(McpsIndication_t *mcpsIndication) {
  if (mcpsIndication->RxData == true && mcpsIndication->BufferSize > 0) {
    downlinkCallback(mcpsIndication->Buffer, mcpsIndication->BufferSize);
  }
}

