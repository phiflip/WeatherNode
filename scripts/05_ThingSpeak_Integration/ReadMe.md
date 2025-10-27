## 1. Create a Free ThingSpeak Account

1. Go to [https://thingspeak.com/](https://thingspeak.com/)
2. Sign up for a **free account** (only email required, no credit card)
3. After registration, create a new **Channel**

## 2. Channel Configuration
Activate the required measurement fields in your ThingSpeak channel, such as temperature, air pressure, humidity, battery voltage, rainfall, and wind speed, according to the data your WeatherNode transmits.

## 3. TTN Payload Decoder

In the **TTN Console**, go to  

**Applications → Payload Formatters → Uplink → Custom JavaScript**,  
and paste the following decoder:

```js
function Decoder(bytes, port) {
var var1 = ((bytes[0] << 24) >> 16 | bytes[1]) / 100;
var var2 = (bytes[2] << 8 | bytes[3]) / 1000; 
var var3 = (bytes[4] << 8 | bytes[5]) * 2;
var var4 = (bytes[6] << 8 | bytes[7]) / 1000;
var var5 = bytes[8]; 
var var6 = (bytes[9] << 8 | bytes[10]) * 2 * 3.1428 * 0.08;
var var7 = bytes[11] * 10;

return {
  field1: var1,
  field2: var2,
  field3: var3,
  field4: var4,
  field5: var5 * 0.1,
  field6: (var6 / var7) * 3.6,
  field7: var7
};
}

```
## 4. Add a Webhook Integration

In your **TTN Console**:

1. Go to your application → **Integrations → Webhooks**
2. Select **ThingSpeak** from the template list
3. Enter your **ThingSpeak Write API Key** from the channel settings
4. Save and test your uplink

Ensure that the field mapping (`field1–field7`) matches your channel configuration above.

## 5. Create the ThingSpeak Dashboard

In your ThingSpeak channel:
1. Go to **Channel View → Charts**
2. Enable charts for each field you want to visualize
3. To make your data public, enable **Public View** under *Channel Settings → Sharing*
4. Copy your public dashboard URL, e.g.  

https://thingspeak.com/channels/<YOUR_CHANNEL_ID>/charts/1

⚠️ By enabling public sharing, you agree that your data and logs are **visible to everyone**, including anonymous users and external aggregators.

## 6. Optional: Data Export
- You can periodically export data via **Download → Export Data (CSV)** in the ThingSpeak channel view

ThingSpeak automatically stores **up to 8,000 data entries per field** for free accounts.  
Each new data upload replaces the oldest entry once the limit is reached.








