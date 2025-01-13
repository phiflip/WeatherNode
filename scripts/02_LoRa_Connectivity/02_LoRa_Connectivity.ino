#include "LoRaWan_APP.h"
#include "Arduino.h"

/*
 * LoRaWAN RGB LED Indicators:
 * - Red: Sending data
 * - Purple: Join successful
 * - Blue: RxWindow1 (receiving)
 * - Yellow: RxWindow2 (receiving)
 * - Green: Data received
 */

/* OTAA Parameters */
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xA6, 0x6F };
uint8_t appKey[] = { 0xDE, 0x74, 0xAE, 0x44, 0x56, 0x1F, 0xF4, 0x8A, 0xD2, 0x45, 0x0E, 0x87, 0x8C, 0xA0, 0x24, 0x41 };

/* ABP Parameters */
uint8_t nwkSKey[16] = {0};
uint8_t appSKey[16] = {0};
uint32_t devAddr = 0;

/* LoRaWAN channels mask, default channels 0-7 are enabled */
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

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

/* Application data transmission duty cycle in milliseconds */
uint32_t appTxDutyCycle = 20000;


/* Application port number, typically set between 1 and 200 */
uint8_t appPort = 1; // Adjust according to the application needs

/*!
* Number of trials to transmit the frame if no acknowledgment is received.
* The data rate will be adjusted according to LoRaWAN specifications:
* - 1st and 2nd trials: Same Data Rate (DR)
* - 3rd and 4th trials: Lower DR by 1
* - 5th and 6th trials: Lower DR by 2
* - 7th and 8th trials: Lower DR by 3
*/
uint8_t confirmedNbTrials = 6; // Number of trials before stopping, adjusting DR

/* ---------------------------------------------------- */
/* Prepare the payload for transmission */
static void prepareTxFrame(uint8_t port) {
    appDataSize = 2;  // Payload size in bytes (e.g., 2 bytes)

    unsigned int humidity = 50;  // Example humidity value (e.g., 50%)
    unsigned int temp = 24;      // Example temperature value (e.g., 24°C)
    
    appData[0] = humidity;  // Store humidity in payload
    appData[1] = temp;      // Store temperature in payload
}
/* ---------------------------------------------------- */


void setup() {
	boardInitMcu();        // Initialize the board
	Serial.begin(115200);  // Start serial communication for debugging
#if(AT_SUPPORT)
	enableAt();            // Enable AT commands if supported
#endif
	deviceState = DEVICE_STATE_INIT;  // Set initial device state
	LoRaWAN.ifskipjoin();  // Check if network join can be skipped (using saved state)
}

void loop() {
	switch (deviceState) {
		case DEVICE_STATE_INIT:
		{
#if(AT_SUPPORT)
			getDevParam();  // Get device parameters if AT support is enabled
#endif
			printDevParam();  // Print device parameters to the serial monitor
			LoRaWAN.init(loraWanClass, loraWanRegion);  // Initialize LoRaWAN with the selected class and region
			deviceState = DEVICE_STATE_JOIN;  // Move to the join state
			break;
		}
		case DEVICE_STATE_JOIN:
		{
			LoRaWAN.join();  // Attempt to join the LoRaWAN network
			break;
		}
		case DEVICE_STATE_SEND:
		{ 
			prepareTxFrame(appPort);  // Prepare the data payload for transmission
			LoRaWAN.send();           // Send the data over LoRaWAN
			deviceState = DEVICE_STATE_CYCLE;  // Move to the cycle state
			break;
		}
		case DEVICE_STATE_CYCLE:
		{
			// Schedule the next transmission cycle with a random delay
			txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);
			LoRaWAN.cycle(txDutyCycleTime);  // Start the cycle timer
			deviceState = DEVICE_STATE_SLEEP;  // Move to the sleep state
			break;
		}
		case DEVICE_STATE_SLEEP:
		{
			LoRaWAN.sleep();  // Put the device to sleep to save power
			break;
		}
		default:
		{
			deviceState = DEVICE_STATE_INIT;  // Reinitialize if the state is unknown
			break;
		}
	}
}