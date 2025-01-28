# Using Open-Meteo API with Node-RED

This guide explains how to use the Open-Meteo API with Node-RED to fetch and display weather data using a gauge or similar visualization nodes.

---

## **1. Prerequisites**
- A working Node-RED setup.
- Basic understanding of HTTP requests.
- Internet access to query the Open-Meteo API.

---

## **2. Setup Overview**
1. Use an HTTP Request node to fetch data from the Open-Meteo API.
2. Parse the response with a Function node.
3. Display the results on a gauge or similar visual nodes.

---

## **3. HTTP Request Node**

1. Drag an **HTTP Request Node** into your flow.
2. Configure it:
   - **Method**: `GET`
   - **URL**: Replace latitude and longitude with your desired location:
     ```
     https://api.open-meteo.com/v1/forecast?latitude=46.874&longitude=7.2773&current=temperature_2m,relative_humidity_2m,precipitation,rain,pressure_msl,wind_speed_10m
     ```
   - **Return**: A parsed JSON object.

---

## **4. Function Node**

1. Drag a **Function Node** to process the API response.
2. Use the following example code to extract the current temperature:
   ```javascript
      const openm_temperature = msg.payload.current.temperature_2m; // Temperature
      return { payload: openm_temperature };
   ```

3. Connect the output of the Function Node to a visualization node, such as a Gauge Node.

---

## **5. Visualization Nodes**

1. Drag Gauge nodes or other visualization nodes into your flow.
2. Configure each node:
   - **Temperature Gauge**: Min: `-20`, Max: `50`, Unit: `°C`.

---

## **6. Benefits of Open-Meteo API**
- Free and easy to use.
- No API key required.
- Provides a wide range of weather metrics.

---
