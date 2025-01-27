# Using Google Forms and Parser to Save Data into Google Sheets

This guide explains how to send data from Node-RED to Google Sheets using Google Forms and a parser. This method is simple to set up and does not require complex OAuth authentication.

---

## **1. Prerequisites**
- A Google account.
- Access to Node-RED.
- A Google Spreadsheet where the data will be saved.

---

## **2. Create a Google Form**

1. **Create a new form**:
   - Open [Google Forms](https://forms.google.com).
   - Click on **Blank Form**.

2. **Add required fields**:
   - Add fields for the data you want to save (e.g., `Timestamp`, `Temperature`, `Humidity`).

3. **Link the form to Google Sheets**:
   - Go to **Responses** > **Select response destination**.
   - Choose **Create a new spreadsheet** to store the form responses in a Google Spreadsheet.

4. **Get the form URL**:
   - Go to **Send** > Click on the link icon and copy the URL.

---

## **3. Identify Form Parameters**

1. **Use the Pre-fill Feature**:
   - Click the three dots in the top right and select **Pre-fill form**.
   - Fill in any answer fields with sample values (e.g., 11, 22, 33).
   - Click **Get Link** to generate a pre-filled URL.

2. **Extract Parameters**:
   - The generated URL will look like:
     ```plaintext
     https://docs.google.com/forms/d/e/<FORM_ID>/formResponse?entry.123456=11&entry.654321=22&entry.789012=33
     ```
   - Each field (e.g., `Timestamp`, `Temperature`, `Humidity`) corresponds to a unique `entry.X` ID.

---

## **4. Configure Node-RED**

1. **Add an HTTP Request Node**:
   - Drag an **HTTP Request Node** into your flow.

2. **Configure the node**:
   - Method: `POST`
   - URL: Replace `/viewform` in the form URL with `/formResponse`.

3. **Prepare the payload**:
   - Use a Function Node to format the `msg.payload`:
     ```javascript
     msg.payload = {
         "entry.123456": new Date().toISOString(), // Timestamp
         "entry.654321": 24.5,                    // Temperature
         "entry.789012": 55.1                     // Humidity
     };
     return msg;
     ```

4. **Add a Debug Node**:
   - Connect a Debug Node to monitor the output.

---

## **5. Verify Data in Google Sheets**
- Open the Google Spreadsheet linked to your Google Form.
- Verify that the data appears in the respective columns.

---

## **6. Benefits of This Method**
- No OAuth setup or API keys required.
- Simple to implement and test.
- Data is directly stored in Google Sheets.

---

## **7. Limitations**
- You can only append new data (no updates or deletions).
- Limited speed for high-frequency data submissions.

---

Feel free to reach out if you need further assistance!
