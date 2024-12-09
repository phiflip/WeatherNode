#include "LoRaWan_APP.h"
#include "Arduino.h"

//----------------------------------------------------------------
// BM680
#include "BME680.h" // Include the BME680 Sensor library
#include <BH1750.h> // Include the light Sensor library
#include <Wire.h> //

BME680_Class BME680; //< Create an instance of the BME680
BH1750 lightMeter;
//----------------------------------------------------------------

/*
   set LoraWan_RGB to Active,the RGB active in loraWan
   RGB red means sending;
   RGB purple means joined done;
   RGB blue means RxWindow1;
   RGB yellow means RxWindow2;
   RGB green means received done;
*/

/* OTAA para*/
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x**}; // change here
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0x00, 0x**}; // change here
uint8_t appKey[] = { 0xF2, 0x2D, 0xF6, 0x49, 0x6E, 0x88, 0xA0, 0x94, 0x60, 0xAD, 0x0F, 0x4E, 0x00, 0x00, 0x00, 0x** }; // change here


/* ABP para*/
uint8_t nwkSKey[] = { 0xE8, 0x12, 0xC1, 0xC6, 0x7F, 0x59, 0xA6, 0x61, 0xCE, 0xF4, 0x07, 0x43, 0xAA, 0x85, 0xFA, 0x00 };
uint8_t appSKey[] = { 0x7B, 0x88, 0x2E, 0x17, 0x3B, 0x72, 0xDF, 0x87, 0xA3, 0xE7, 0x98, 0x5C, 0x08, 0x5E, 0xAA, 0x00 };
uint32_t devAddr =  ( uint32_t )0x26013E00;
/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 90000; //change here for sending frequency

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
  Number of trials to transmit the frame, if the LoRaMAC layer did not
  receive an acknowledgment. The MAC performs a datarate adaptation,
  according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
  to the following table:

  Transmission nb | Data Rate
  ----------------|-----------
  1 (first)       | DR
  2               | DR
  3               | max(DR-1,0)
  4               | max(DR-1,0)
  5               | max(DR-2,0)
  6               | max(DR-2,0)
  7               | max(DR-3,0)
  8               | max(DR-3,0)

  Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
  the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/

uint8_t confirmedNbTrials = 4;
uint16_t baseline;
int count;
int maxtry = 10;

float Temperature, Humidity, Pressure, lux, VOC_gases, Airquality;

/*!
   \brief   Prepares the payload of the frame
*/

float CalculateIAQ()
{
  float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
  float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

  float hum_score, gas_score;
  float gas_reference = VOC_gases;
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
  float gas_lower_limit = 50000;  // 50000 Bad air quality limit
  float gas_upper_limit = 150000; // 150000 Good air quality limit
  if (gas_reference > gas_upper_limit)
    gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit)
    gas_reference = gas_lower_limit;
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100;

  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = hum_score + gas_score;

  return air_quality_score;
}


static void prepareTxFrame(uint8_t port)
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);
  pinMode(GPIO0, OUTPUT);
  digitalWrite(GPIO0, LOW);

  //-- BME680
  Wire.begin();
  BME680.begin(I2C_STANDARD_MODE);
  delay(1000);
  BME680.setOversampling(TemperatureSensor, Oversample16);
  BME680.setOversampling(HumiditySensor, Oversample16);
  BME680.setOversampling(PressureSensor, Oversample16);
  BME680.setIIRFilter(IIR4);
  BME680.setGas(320, 150); // 320C for 150 milliseconds

  delay(3000); //* New BME680 sensors requires burn in (about 48 hours!) . Once burned in a sensor requires at least 5 minutes of run in before gas resistance readings are considered good.

  static int32_t  bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings;  // BME readings

  BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);
  delay(500);
  BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);
  while ((bme680_temp_readings > 6000 || bme680_humidity_readings == 100 || bme680_humidity_readings == 0) && count < maxtry)
  {
    BME680.getSensorData(bme680_temp_readings, bme680_humidity_readings, bme680_pressure_readings, bme680_gas_readings);
    delay(250);
  }

  Temperature = bme680_temp_readings / 100.0;
  Humidity = bme680_humidity_readings / 1000.0;
  Pressure = bme680_pressure_readings / 100.0;
  VOC_gases = bme680_gas_readings;
  Airquality = CalculateIAQ();

  Wire.end();

  //-- BH1750 Light
  pinMode(GPIO0, OUTPUT);
  digitalWrite(GPIO0, HIGH);

  Wire.begin();

  count = 0;
  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2);
  delay(100);
  lux = lightMeter.readLightLevel();
  delay(100);
  lux = lightMeter.readLightLevel();
  lightMeter.end();
  Wire.end();
  digitalWrite(GPIO0, LOW);
  pinMode(GPIO0, ANALOG);

  //-- Battery
  unsigned int batteryVoltage = getBatteryVoltage(); // Measured voltage at ADC pin in mV
  appDataSize = 12; // change here number of total bytes

  int Temperature_payload = Temperature * 100;
  unsigned int Humidity_payload = Humidity * 100;
  unsigned int Pressure_payload = Pressure / 2;
  unsigned int Airquality_payload = Airquality * 100;
  unsigned int Light_payload =  lightMeter.readLightLevel();
  /*--------------------------------------------------------*/
  // convert and encode temperature data
  appData[0] = highByte(Temperature_payload);
  appData[1] = lowByte(Temperature_payload);
  /*--------------------------------------------------------*/
  // convert and encode humidity data
  appData[2] = highByte(Humidity_payload);
  appData[3] = lowByte(Humidity_payload);
  /*--------------------------------------------------------*/
  // encode atmospheric pressure data


  /*--------------------------------------------------------*/
  // encode air quality  data

  /*--------------------------------------------------------*/
  // encode light data

  /*--------------------------------------------------------*/
  // encode battery data

  /*--------------------------------------------------------*/

  // Print sensor data to serial monitor
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
  Serial.begin(115200);
#if(AT_SUPPORT)
  enableAt();
#endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
}


void loop()
{
  switch (deviceState)
  {
    case DEVICE_STATE_INIT:
      {
#if(LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
#endif
#if (AT_SUPPORT == 1)
        getDevParam();
#endif
        printDevParam();
        LoRaWAN.init(loraWanClass, loraWanRegion);
        deviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
#if (ACTIVE_REGION == 1)
        setSubBand2();
#endif
        LoRaWAN.join();
        break;
      }
    case DEVICE_STATE_SEND:
      {
        prepareTxFrame(appPort);
        //      if (checkcopyprotect() == true)
        {
          LoRaWAN.send();
        }
        deviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
      }
    case DEVICE_STATE_SLEEP:
      {
#if (TriggerInt == 1)
        if (accelWoke) {
          if (IsLoRaMacNetworkJoined) {
            prepareTxFrame(appPort);
            if (checkcopyprotect() == true)
            {
              LoRaWAN.send();
            }
            //} else {
            //  LoRaWAN.join();
            //  prepareTxFrame(appPort);
            //  LoRaWAN.send();
          }
          accelWoke = false;
        }
#endif
        LoRaWAN.sleep();
        break;
      }
    default:
      {
        deviceState = DEVICE_STATE_INIT;
        break;
      }
  }
}
