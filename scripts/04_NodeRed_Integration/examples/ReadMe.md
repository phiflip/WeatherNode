
# Node-RED Examples

This folder contains example flows for various integrations and use cases in Node-RED. Each subfolder focuses on a specific integration or functionality, providing ready-to-use flows and documentation.

## Subfolders

### 1. **`ttn/`**
   - Contains examples for integrating **The Things Network (TTN)** with Node-RED.
   - Includes MQTT flows for receiving and processing IoT device data.

### 2. **`open_meteo/`**
   - Demonstrates how to fetch weather data using the **Open-Meteo API**.
   - Includes flows for retrieving and visualizing weather forecasts.

### 3. **`google_sheets/`**
   - Provides examples for integrating **Google Sheets** with Node-RED.
   - Includes flows for reading from and writing data to Google Sheets.

## How to Use

1. Navigate to the subfolder relevant to your use case.
2. Import the `.json` flow files into Node-RED:
   - Open the Node-RED editor.
   - Click on the menu button in the top-right corner.
   - Select **Import > Clipboard**.
   - Paste the content of the `.json` file and click **Import**.
3. Customize the flows as needed for your specific project.

## Prerequisites

- Ensure you have Node-RED installed and running.
- Install any required Node-RED nodes for the respective flows (e.g., `node-red-dashboard`, `node-red-contrib-mqtt`, etc.).
- Refer to the documentation in each subfolder for additional setup instructions.
