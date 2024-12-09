# WeatherNode Coding and Decoding Exercises

## Overview

This directory contains exercises aimed at helping you learn how to encode and decode sensor data using the BME680 and BH1750 sensors. These exercises will guide you through encoding sensor readings into a payload, transmitting them via LoRaWAN, and decoding the payload on The Things Network (TTN).

## Exercise 1: Encoding Sensor Data

### Objective

In this exercise, you will learn how to encode sensor data into a hexadecimal payload. You will work with the BME680 and BH1750 sensors to collect environmental data (temperature, humidity, pressure, and light levels) and encode these readings into a payload suitable for transmission.

### Instructions

1. Download the code `03a_SensorsOnly_Encoding_Ex.ino`.
2. Complete the code by encoding the remaining sensor readings into the payload.
3. Print the encoded payload as hexadecimal values to the Serial Monitor.

### Example Output

Your output should resemble this on the Serial Monitor:
`Encoded Payload: 09 3E 04 FF 00 .. .. .. ..`


### Submission

Copy the hexadecimal payload values and prepare them for the next exercise.

## Exercise 2: Decoding Sensor Data on TTN

### Objective

In this exercise, you will decode the sensor data encoded and transmitted in Exercise 1. You will write a decoder function on The Things Network (TTN) to parse the payload back into its original sensor readings.

### Instructions

1. Download the code `03b_Sensors_Encoding_WithTransmission_Ex.ino`.
2. Insert the encoded payload lines from `03a_SensorsOnly_Encoding_Ex.ino` into this code.
3. Ensure you update the EUIs at the top of the code to match your device.

### Example Decoder Function on TTN

Here's an example of a TTN decoder function you can use:

```javascript
function decodeUplink(input) {
    var data = {};
    var bytes = input.bytes;

    data.temp = (((bytes[0] << 24) >> 16) + bytes[1]) / 100;
    data.humidity = ((bytes[2] << 8) + bytes[3]) / 100;

    return {
        data: data
    };
}
```
### Testing Your Decoder

    Copy your encoded bytes (e.g., 09 3E 04 FF 00 .. .. .. ..) and use the Simulate Uplink feature on TTN to test your decoder.
    Verify that the decoded values match the original sensor readings.
