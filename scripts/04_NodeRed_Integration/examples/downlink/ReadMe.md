# Downlink with Node-RED for TTN Devices

This guide explains how to configure a downlink message to send commands to a TTN device using Node-RED.

---

## **1. Prerequisites**
- A TTN application with a registered device.
- A generated MQTT API key in the TTN console.
- Node-RED installed and running.
- Basic understanding of Node-RED nodes.

---

## **2. Sending Downlink Messages via Node-RED**

### **2.1 MQTT Configuration**
1. **Generate an MQTT API Key**:
   - Go to **TTN Console > Integrations**.
   - Create a new MQTT API Key with the necessary permissions.

2. **Configure the MQTT Out Node**:
   - Add an **MQTT Out Node** in Node-RED.
   - Set the topic to:
     ```plaintext
     v3/<your-username>@ttn/devices/<device-id>/down/push
     ```
     Replace `<your-username>` and `<device-id>` with your TTN application username and device ID.

3. **Add the API Key**:
   - Enter the MQTT API Key in the MQTT Out Node configuration.

---

### **2.2 Creating the Downlink Payload**
1. **Inject Node**:
   - Add an **Inject Node** to trigger the downlink.
   - Configure it to send a 1-byte payload (e.g., `00`).

2. **Function Node**:
   - Add a **Function Node** to encode the payload into Base64.
   - Use the following code:
     ```javascript
     const hexString = msg.payload;
     const buffer = Buffer.from(hexString, 'hex');
     const base64Payload = buffer.toString('base64');

     msg.payload = {
         "downlinks": [{
             "f_port": 1,
             "frm_payload": base64Payload,
             "priority": "NORMAL"
         }]
     };
     return msg;
     ```

3. **Connect Nodes**:
   - Connect the **Inject Node** to the **Function Node**.
   - Connect the **Function Node** to the **MQTT Out Node**.

---

## **3. Testing the Downlink**
1. Click the **Inject Node** to send the downlink message.
2. Observe the device’s response:
   - The downlink will be marked as "scheduled" in the TTN console.
   - The downlink will be sent during the next uplink.

---

## **4. Notes**
- Downlink messages are delivered at the next uplink interval.
- The payload should be encoded in Base64, as shown in the Function Node example.

---

For detailed documentation, refer to the TTN Node-RED integration guide: [TTN Documentation](https://www.thethingsindustries.com/docs/integrations/node-red/send/).
