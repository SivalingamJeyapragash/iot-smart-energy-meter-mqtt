## System Architecture

This project implements an IoT-based smart energy metering system using ESP32 and MQTT.

### Hardware Components
- ESP32 microcontroller
- ACS712 current sensor
- ZMPT101B voltage sensor
- Relay module for power control

### Software Components
- MQTT Broker
- IoT Dashboard
- Embedded firmware (ESP32)

### Data Flow
1. ESP32 reads voltage and current values
2. Power and energy (kWh) are calculated
3. Data is published to MQTT topics
4. Dashboard subscribes to energy data
5. Relay control commands are sent back to ESP32 via MQTT

### Use Case
Designed to eliminate manual meter reading and enable remote billing and power management for electricity providers.
