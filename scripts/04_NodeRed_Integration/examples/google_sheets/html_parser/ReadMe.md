# Using Google Forms and Parser to Save Data into Google Sheets

This guide explains how to send data from Node-RED to Google Sheets using Google Forms and a parser. This method is simple to set up and does not require complex OAuth authentication.

---

## **1. Prerequisites**
- A Google account.
- Access to Node-RED.
- A Google Spreadsheet where the data will be saved.

---

## **2. Create a Google Form**

1. Open [Google Forms](https://forms.google.com) and create a blank form.
2. Add fields for the data you want to save (e.g., `Timestamp`, `Temperature`, `Humidity`).
3. Link the form to a Google Spreadsheet:
   - Go to **Responses** > **Select response destination**.
   - Choose **Create a new spreadsheet** to store the responses.
4. Copy the form URL by clicking **Send** and then the link icon.

---

## **3. Identify Form Parameters**

1. Use the pre-fill feature to get parameter mappings:
   - Click the three-dot menu in the top-right and select **Pre-fill form**.
   - Enter sample values (e.g., 11, 22, 33) and click **Get Link**.
2. Extract the parameters from the generated URL. Example:
   ```plaintext
   https://docs.google.com/forms/d/e/<FORM_ID>/formResponse?entry.123456=11&entry.654321=22&entry.789012=33
   ```
3. Replace `/viewform` with `/formResponse` and substitute placeholders (e.g., `{{payload.temperature}}`).

---

## **4. Configure Node-RED**

1. **Add an HTTP Request Node**:
   - Drag an **HTTP Request Node** into your flow and set the method to `GET`.


2. **Function Node Example**:
   For more detailed payload extraction:
     ```javascript
     msg.payload = {
         temperature: parseFloat(msg.payload.uplink_message.decoded_payload.temp),
         humidity: parseFloat(msg.payload.uplink_message.decoded_payload.humidity),
         pressure: parseFloat(msg.payload.uplink_message.decoded_payload.pressure),
     };
     return msg;
     ```

3. **Add a Debug Node**:
   - Use it to monitor the output and verify the data being sent.

---

## **4. Verify Data in Google Sheets**
- Open the Google Spreadsheet linked to your Google Form.
- Check that the data appears correctly in the respective columns.

---

## **5. Benefits of This Method**
- No OAuth setup or API keys required.
- Easy to implement and test.
- Data is directly stored in Google Sheets.

---

## **6. Limitations**
- Can only append new data (no updates or deletions).
- Limited speed for high-frequency data submissions.

---

Feel free to reach out if you need further assistance!
