# WeatherNode

<img src="https://github.com/phiflip/WeatherNode/blob/main/illustrations/logo_icon.png" width="150" alt="WeatherNode main logo type">

**WeatherNode** is a versatile and educational meteorological station project that uses the power of CubeCell microcontrollers, a range of environmental sensors, and Node-RED for real-time data processing and visualization. This project provides hands-on experience with IoT (Internet of Things) technologies, data acquisition, cloud integration, and real-time analytics for outdoor weather monitoring.

[//]: <> (<img src="https://github.com/phiflip/WeatherNode/blob/main/illustrations/NodeRed_Dashboard.PNG" width="50" alt="WeatherNode Node-RED Dashboard">
)
## Project Overview

The WeatherNode project allows you to build a fully functional weather station capable of measuring various environmental parameters such as temperature, humidity, air pressure, rainfall, and wind speed. Data collected by the sensors is transmitted via LoRaWAN to a central server where it is processed and visualized using Node-RED.

### Key Features

- **CubeCell Integration:** Uses CubeCell microcontrollers which are optimized for low-power IoT applications and provide seamless integration with LoRaWAN networks.
- **Multi-Sensor Setup:** Supports a variety of sensors to monitor outdoor environmental conditions, including:
  - **Temperature Sensor**
  - **Humidity Sensor**
  - **Air Pressure Sensor**
  - **Rainfall Sensor**
  - **Wind Speed Sensor**
- **Node-RED Interface:** Data is processed and visualized in real-time using a customizable Node-RED dashboard, providing intuitive insights into weather conditions.
- **Cloud Connectivity:** Demonstrates integration with cloud services like Google Spreadsheets, Oracle Cloud, and AWS for data storage and further processing.
- **Educational Focus:** Ideal for students and educators, the WeatherNode project is structured to guide users through the process of setting up IoT systems, from hardware assembly to software integration and cloud deployment.

## Getting Started

### Prerequisites

Before starting, ensure you have the following:

- **Hardware:**
  - Details about the hardware components, including the list of sensors and 3D models for enclosures, can be found in the [`hardware`](./hardware) folder.
  - **STL Files:** 3D models for the sensors connection clip are located in the [`stl_files`](./hardware/stl_files) subfolder.

- **Software:**
  - Arduino IDE (for programming the CubeCell)
  - Node-RED (installed locally or on a cloud platform like Oracle or AWS)
  - Google account (for spreadsheet integration)

### Setup Instructions

1. **Hardware Assembly:**
   - Connect the sensors (temperature, humidity, pressure, rainfall, and wind speed) to the CubeCell microcontroller.

2. **CubeCell Programming:**
   - Program the CubeCell with the provided `.ino` files using the Arduino IDE.

3. **Node-RED Configuration:**
   - Set up Node-RED on your preferred platform (local or cloud).

4. **Cloud Integration:**
   - Integrate with Google Spreadsheets or other cloud services to store and analyze the data.

## Documentation

- **[Troubleshooting](./troubleshoot/README.md):** Solutions for common issues encountered during the setup and operation of the WeatherNode.
