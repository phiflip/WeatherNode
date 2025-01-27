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
     https://api.open-meteo.com/v1/forecast?latitude=46.874&longitude=7.283&hourly=temperature_2m,relativehumidity_2m,apparent_temperature,precipitation_probability,rain,cloudcover,windspeed_10m&current_weather=true&forecast_days=1&timezone=Europe%2FBerlin
     ```
   - **Return**: A parsed JSON object.

---

## **4. Function Node**

1. Drag a **Function Node** to process the API response.
2. Use the following example code to extract weather data:
   ```javascript
   var currentTemp = {};
   var humidity = {};
   var windSpeed = {};

   if (msg.payload.current_weather) {
       currentTemp.payload = msg.payload.current_weather.temperature; // Temperature
       currentTemp.topic = "Current Temperature";

       humidity.payload = msg.payload.current_weather.relativehumidity_2m; // Humidity
       humidity.topic = "Humidity";

       windSpeed.payload = msg.payload.current_weather.windspeed; // Wind Speed
       windSpeed.topic = "Wind Speed";
   }

   return [currentTemp, humidity, windSpeed];
   ```

3. Configure the Function Node outputs to match the extracted metrics.

---

## **5. Visualization Nodes**

1. Drag Gauge nodes or other visualization nodes into your flow.
2. Configure each node:
   - **Temperature Gauge**: Min: `-20`, Max: `50`, Unit: `°C`.
   - **Humidity Gauge**: Min: `0`, Max: `100`, Unit: `%`.
   - **Wind Speed Gauge**: Min: `0`, Max: `50`, Unit: `km/h`.
3. Connect each Function Node output to a respective gauge node.

---

## **6. Benefits of Open-Meteo API**
- Free and easy to use.
- No API key required.
- Provides a wide range of weather metrics.

---

Enjoy using the Open-Meteo API in your Node-RED projects!
