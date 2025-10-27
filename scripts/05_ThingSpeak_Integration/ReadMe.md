\## 🧾 1. Create a Free ThingSpeak Account



1\. Go to \[https://thingspeak.com/](https://thingspeak.com/)

2\. Sign up for a \*\*free account\*\* (only email required, no credit card)

3\. After registration, create a new \*\*Channel\*\*





\## ⚙️ 2. Channel Configuration



Enable the following fields:



| Field  | Parameter               | Unit      | Example |

|:-------|:------------------------|:----------|:--------|

| field1 | Temperature             | °C        | 12.3 |

| field2 | Air pressure 	   | hPa       | 102  |

| field3 | Humidity (rel.)         | %         | 45   |

| field4 | Battery voltage         | V         | 4.02 |

| field5 | Rain                    | mm        | 0.5 |

| field6 | Wind speed              | km/h      | 8.4 |

&nbsp;



\## 🧩 3. TTN Payload Decoder



In the \*\*TTN Console\*\*, go to  

\*\*Applications → Payload Formatters → Uplink → Custom JavaScript\*\*,  

and paste the following decoder:



```js

function Decoder(bytes, port) {

&nbsp; var var1 = ((bytes\[0] << 24) >> 16 | bytes\[1]) / 100;

&nbsp; var var2 = (bytes\[2] << 8 | bytes\[3]) / 1000; 

&nbsp; var var3 = (bytes\[4] << 8 | bytes\[5]) \* 2;

&nbsp; var var4 = (bytes\[6] << 8 | bytes\[7]) / 1000;

&nbsp; var var5 = bytes\[8]; 

&nbsp; var var6 = (bytes\[9] << 8 | bytes\[10]) \* 2 \* 3.1428 \* 0.08;

&nbsp; var var7 = bytes\[11] \* 10;



&nbsp; return {

&nbsp;   field1: var1,

&nbsp;   field2: var2,

&nbsp;   field3: var3,

&nbsp;   field4: var4,

&nbsp;   field5: var5 \* 0.1,

&nbsp;   field6: (var6 / var7) \* 3.6,

&nbsp;   field7: var7

&nbsp; };

}





---





\## 4. Add a Webhook Integration



In your \*\*TTN Console\*\*:

1\. Go to your application → \*\*Integrations → Webhooks\*\*

2\. Select \*\*ThingSpeak\*\* from the template list

3\. Enter your \*\*ThingSpeak Write API Key\*\* from the channel settings

4\. Save and test your uplink



Ensure that the field mapping (`field1–field7`) matches your channel configuration above.





\## 5. Create the ThingSpeak Dashboard



In your ThingSpeak channel:

1\. Go to \*\*Channel View → Charts\*\*

2\. Enable charts for each field you want to visualize

3\. To make your data public, enable \*\*Public View\*\* under \*Channel Settings → Sharing\*

4\. Copy your public dashboard URL, e.g.  

&nbsp;  https://thingspeak.com/channels/<YOUR\_CHANNEL\_ID>/charts/1



\## 6. Optional: Multiple Nodes or Data Export



\- For multiple WeatherNodes, use different channels or add a \*\*Node ID\*\* in `field8`

\- Export data periodically via:

&nbsp; - \*\*Download → CSV\*\*, or  

&nbsp; - \[ThingSpeak MATLAB Analysis Tool](https://thingspeak.com/apps/matlab\_analysis)



\## 7. Logs and Public Visibility



ThingSpeak automatically stores \*\*up to 8,000 data entries per field\*\* for free accounts.  

Each new data upload replaces the oldest entry once the limit is reached.



If you want others to view your station:

1\. Enable \*\*Public View\*\* under \*Channel Settings → Sharing\*

2\. Copy and share the \*\*public URL\*\* (starts with https://thingspeak.com/channels/…)

3\. Public channels can be indexed by ThingSpeak and may appear in the \*\*ThingSpeak Public Channel Explorer\*\* or other third-party dashboards.



⚠️ By enabling public sharing, you agree that your data and logs are \*\*visible to everyone\*\*, including anonymous users and external aggregators.



&nbsp;	



