# LazyOrchids
This project uses Arduino to control the terrarium environment for growing orchids

# Workflow
## Supervisor 
Arduino Nano 33 IoT is used as the supervisor to measure and upload the readings of the terrarium to ThingSpeak so iPhone can see it through Scriptable widget
* In addition to Arduino Nano 33 IoT, a commercial hygrometer-thermometer 
  is put in the terrarium as a 3rd party supervisor. It also acts as a quick 
  raw-eye check 
## Worker
Arduino Uno R3 is used as the worker to read terrarium environment factors 
(temperature, humidity, light)
and make actions directly (activates either fan or mister)

# Setup 
## General terrarium
### Requirements 
* 1 × big plastic bottle
* Some water gel beads 
* Some Sphagnum moss 
* 1 × small glass as water tank for mister
* 3D designed holders 
  * Fan motor holder
  * Mister disc holder

## Supervisor
### Requirements
* Hardware 
  * 1 × Arduino Nano 33 IoT
  * 1 × SHT31 temperature humidity sensor
  * 1 × Light dependent resistor (LDR) / photoresistor (photocell)
  * 1 × 4.7 kΩ resistor 
* Software 
  * Arduino IDE
  * ThingSpeak 
  * Scriptable 
* Access to Wi-Fi

### Wiring 
<div style="text-align: center;">
  <img src="readme_img/supervisor.png" width="500"/>
</div>

## Worker
### Requirements
* Hardware 
  * 1 × Arduino Uno R3 
  * 1 × SHT31 temperature humidity sensor
  * 1 × LDR
  * 1 × 4.7 kΩ resistor
  * 1 × Fan blade + 3-6 V motor
  * 1 × water atomizer / mister + control board
  * 2 × Diode rectifier
  * 2 × NPN transistor PN2222 
  * 2 × 330 Ω resistor
* Software 
  * Arduino IDE

### Wiring
<div style="text-align: center;">
  <img src="readme_img/worker.png" width="500"/>
</div>



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

