# Week 5: Storing CubeCell Data in Google Spreadsheet via Node-RED

In this week, you will learn how to capture data from your CubeCell device and store it in a Google Spreadsheet using Node-RED. This exercise is designed to give you hands-on experience with IoT data flows, cloud integration, and real-time data storage.

## Table of Contents

1. [Objective](#objective)
2. [Prerequisites](#prerequisites)
3. [Steps Overview](#steps-overview)
4. [Detailed Instructions](#detailed-instructions)
5. [Useful Resources](#useful-resources)

## Objective

The goal of this exercise is to set up a flow in Node-RED that collects data from your CubeCell device and stores it in a Google Spreadsheet. This is a crucial step in building IoT applications where data persistence and accessibility are important.

## Prerequisites

Before you start, ensure that you have the following:

- A configured CubeCell device that is able to collect and transmit data.
- Node-RED installed either locally or in the cloud (e.g., Oracle Cloud or AWS as described in previous exercises).
- A Google account with access to Google Spreadsheets.
- Basic understanding of Node-RED flows and how to work with nodes.

## Steps Overview

1. **Set Up Google Spreadsheet:**
   - Create a new Google Spreadsheet where the data will be stored.
   - Share the spreadsheet and obtain the necessary API credentials to access it programmatically from Node-RED.

2. **Configure Node-RED:**
   - Set up the necessary nodes in Node-RED to capture data from the CubeCell device.
   - Use the Google Sheets node to send data to the spreadsheet.

3. **Test the Setup:**
   - Verify that the data from CubeCell is being correctly transmitted and stored in the Google Spreadsheet.

## Detailed Instructions

### 1. Set Up Google Spreadsheet

- Follow the [Google Spreadsheet Setup Video](https://www.youtube.com/watch?v=Cn20aQp9Syw) to create your spreadsheet and get the necessary API credentials.
- Ensure that the API credentials are saved securely as they will be needed in Node-RED.

### 2. Configure Node-RED

- Open Node-RED and create a new flow.
- Add the necessary nodes to capture the data from CubeCell, process it if necessary, and then send it to the Google Spreadsheet.
- Use the "Google Sheets" node in Node-RED to connect to your spreadsheet using the API credentials.

### 3. Test the Setup

- Inject test data through Node-RED to ensure that everything is working.
- Check your Google Spreadsheet to confirm that the data is being recorded correctly.

## Useful Resources

- [Google Spreadsheet Setup Video](https://www.youtube.com/watch?v=Cn20aQp9Syw)
- [Node-RED Google Sheets Integration Guide](https://nodered.org/docs/user-guide/nodes)

---

**Note:** This exercise builds on knowledge from previous weeks, so make sure your Node-RED environment is correctly set up and functioning before starting.

