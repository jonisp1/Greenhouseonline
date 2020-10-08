# Greenhouseonline
[Greenhouseonline](Greenhouseonline.ga)  Arduino code 

The "Server" module handles the communication of the system with the web Server and connects with each individual unit

-GSM/GPRS Communication for communication with the web server  
-Cable communication with Greenhouse Units 1 and 2  
-Nrf24 communication with the Refrigeration unit  

The Units 1 and 2 modules collects the sensor data and sends them to the Server module via cable connection, The server module gives them interactions about 
the control of ventilation system  
-Temperature sensor  
-Humidity sensor  
-Soil humidity sensor  
-Door magnetic sensor  
-4 relays controlling the ventilation system   

The Refrigeration unit collect the sensor data and sends them to the Server module via wireless Nrf24 connection, The server module gives them interactions about the control of watering system  
-Temperature sensor  
-Humidity sensor   
-Door magnetic sensor   
-2 relays controlling the watering system  

