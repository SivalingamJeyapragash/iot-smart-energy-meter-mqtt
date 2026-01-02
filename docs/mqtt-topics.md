## MQTT Topics Structure

This project uses structured MQTT topics for scalable energy monitoring.

### Topics
- energy/house_01/voltage
- energy/house_01/current
- energy/house_01/power
- energy/house_01/units
- energy/house_01/relay_control

### Description
- Sensor data is published periodically
- Relay control topic is subscribed by ESP32
- Supports multiple houses by changing house ID
