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

/* OTAA (Over-The-Air Activation) parameters */
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   // Application EUI
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x**, 0x** };  // Device EUI
uint8_t appKey[] = { 0xF2, 0x2D, 0xF6, 0x49, 0x6E, 0x88, 0xA0, 0x94, 0x60, 0xAD, 0x0F, 0x4E, 0xA9, 0xD1, 0x**, 0x** }; // Application Key

/* ABP (Activation By Personalization) parameters */
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x00 }; // Network Session Key
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x00 }; // Application Session Key
uint32_t devAddr =  ( uint32_t )0x007e6a00; // Device Address

/* LoRaWAN channels mask, default channels 0-7 are enabled */
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/* LoRaWAN region, set in the Arduino IDE tools menu */
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/* LoRaWAN class, either Class A or Class C are supported */
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/* Application data transmission duty cycle in milliseconds */
uint32_t appTxDutyCycle = 14000;

/* Select OTAA (true) or ABP (false) for network activation */
bool overTheAirActivation = LORAWAN_NETMODE;

/* Adaptive Data Rate (ADR) enable/disable */
bool loraWanAdr = LORAWAN_ADR;

/* Enable saving network information to flash memory */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed (true) or unconfirmed (false) messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

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
