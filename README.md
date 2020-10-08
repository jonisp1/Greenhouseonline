# Greenhouseonline
Greenhouseonline.ga Arduino code 

The "Server" module handles the comunication of the system with the web Server and connects with each intivijual unit

-GSM/GPRS Communication for communicationg with the web server
-Cable comunication with Greenhouse Units 1 and 2
-Nrf24 comunication with the Refrigeration unit

The Units 1 and 2 modules collects the sensor data and sends them to the Server module via cable connection, The server module gives them inctraction about 
the control of ventelation system 
-Temprature sensor
-Humidity sensor 
-Soid humidity sensor 
-Door magnetic sensor
-4 relays controling the ventelation system 

The Refrigeration unit collect the sensor data and sends them to the Server module via wireless Nrf24 connection, The server module gives them inctraction about 
the control of watering system 
-Temprature sensor
-Humidity sensor
-Door magnetic sensor
-2 relays controling the watering system 
