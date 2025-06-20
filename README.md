# LazyOrchids
This project uses Arduino to control the terrarium environment for growing orchids

# Workflow
## Supervisor 
Arduino Nano 33 IoT is used as the supervisor to measure and upload the readings of the terrarium to ThingSpeak so iPhone can see it through Scriptable widget
## Worker

# Setup 
Requirements: 
* Supervisor 
  * Hardware 
    * Arduino Nano 33 IoT
    * SHT31 temperature humidity sensor
    * Light dependent resistor (LDR)
  * Software 
    * Arduino IDE
    * ThingSpeak 
    * Scriptable 
* Worker

# Notes 
Arduino IDE
* Changes made in Arduino IDE won't be automatically picked up in CLion 

ThingSpeak 
* For free tier users, you're allowed to upload data once per 15 seconds
* For the supervisor, you can technically split the data upload for sensor 
  and LDR, and set it like upload the sensor data every 15 seconds and LDR 
  data every 30 seconds. However, ThingSpeak will regard that per 15 second,
  you have 2 batches of data to upload and that exceeds the one upload per 
  15 seconds, resulting in the latter data upload always failed. This can 
  be seen in the branch `supervisor_individual_timer`. Because of that, 
  I'll always upload the data all at once

