
# TTN Integration Guide for Node-RED

This guide explains how to integrate **The Things Network (TTN)** with Node-RED using MQTT. Follow these steps to connect your Node device to TTN and process data in Node-RED.

---

## Table of Contents

1. [Overview of TTN Integration](#overview-of-ttn-integration)
2. [Creating an MQTT API Key](#creating-an-mqtt-api-key)
3. [Connecting Node-RED to TTN](#connecting-node-red-to-ttn)
4. [Configuring Payload Formatters](#configuring-payload-formatters)

---

## 1. Overview of TTN Integration


### Prerequisites:
- If no gateway is nearby, use a simulated payload via TTN.
- Alternatively, send data from the Node device to TTN via a gateway.

---

## 2. Creating an MQTT API Key



1. Log in to the TTN Console.
2. Navigate to **Integrations > MQTT** under your application.
3. Generate a new API Key by clicking **"Generate new API Key"**.
4. Copy and securely store the API Key for later use.

---

## 3. Connecting Node-RED to TTN

### Step 1: Add an MQTT Input Node in Node-RED

1. Drag the **`mqtt in`** node to your Node-RED workspace.
2. Set the following properties:
   - **Topic**: Use `#` to subscribe to all topics.
   - **QoS (Quality of Service)**: Set to `2`.
   - **Output**: Choose **a parsed JSON object**.

---

### Step 2: Configure the MQTT Broker


1. Open the MQTT node properties.
2. Click **"Add new mqtt-broker"** and configure the following:
   - **Server**: `eu1.cloud.thethings.network`
   - **Port**: `1883`
   - **Enable Clean Session**: Checked
   - **Use MQTT 3.1**: Checked

---

### Step 3: Add Security Credentials

![Add Security Credentials](grafik5.png)

1. Under the **Security** tab:
   - **Username**: Your TTN Application ID (e.g., `your-username@ttn`).
   - **Password**: The API Key you generated earlier.

2. Save and deploy your flow.

---

## 4. Configuring Payload Formatters


1. In the TTN Console, navigate to **Payload Formatters**.
2. Select **Uplink** and choose **Custom JavaScript formatter**.
3. Use the following example code to decode incoming payloads:
   ```javascript
   function decodeUplink(input) {
       var data = {};
       var bytes = input.bytes;

       data.temp = ((bytes[0] << 24) >> 16) + bytes[1] / 100;
       data.humidity = bytes[2];
       data.pressure = ((bytes[3] << 8) | bytes[4]) / 10;

       return {
           data: data
       };
   }
   ```
4. Save your changes and test the decoder with a sample payload.

---

## Notes:
- Ensure your Node device is correctly configured to send data to TTN.
- For more details, refer to [The Things Stack Documentation](https://www.thethingsindustries.com/docs/integrations/other-integrations/node-red/).

