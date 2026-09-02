# Hardware Requirements for WeatherNode

## Overview

This document lists all the hardware components required to build the WeatherNode meteorological station, including sensors, enclosures, power supplies, and wiring.

## Components

1. **Microcontroller**
   - CubeCell HTCC-AB01 v2 (Antenna)
   - Mini-Breadboard

2. **Sensors**
   - BME680 Sensor
   - TFA Dostmann 30.3801.02 Windmesser
   - TFA Dostmann 30.3252.01 Regenmesser

3. **Enclosures**
   - Plastic Enclosure
   - 3D Printed Enclosures for the BME680:
     - STL files are available in the [`stl_files`](./stl_files) subfolder.

4. **Power Supply**
   - LiPo Battery (1000 mAh / 3.7 V)
   - Solar Panel (60x60 mm, 6V)
   - USB C Cable

5. **Cables and Wiring**
   - 1 × 70 cm cable: rain gauge reed switch to CubeCell
   - 1 × 30 cm ground cable: rain gauge to solar panel
   - 2 × 55 cm cables, yellow and black: solar panel to CubeCell
   - 1 × 10 cm 6-core cable:
     - 4 wires for the BME680 sensor
     - 2 wires for the wind sensor reed switch

6. **Tools**
   - **Soldering Station:**
     - A soldering station may be necessary to solder pins to the CubeCell, connect the solar panel, or secure the BME680 sensor connections. Ensure you have a soldering iron, solder wire, and basic soldering skills for this task.

## Estimated Cost

- Total: Approx. 100.- CHF per kit (excluding VAT)
- Suggested Suppliers:
  - bastelgarage.ch
  - conrad.ch
