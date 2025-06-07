In this version of the car, you basically have two hardware units: the car, and the controller. 

1. The car 
It comprises of these major electro-mechanical components: 
a. Arduino UNO:
Onboard microcontroller

b. L293D motor driver:
Drives motors

c. HC05 bluetooth module:
Maintains wireless serial communication with controller device.

d. SG90 Servo motor: 
Ultrasonic sensor is mounted on it for changing the direction of ultrasonic beam

e. HCSR04 Ultrasonic Sensor:
Helps in measuring distance in a particular direction


2. The controller 
It comprises of these major electro-mechanical components:

a. ESP8266
Onboard microcontroller

b.KY023 joystick module:
Used as the main controlling sensor

c.HC05 bluetooth module:
Maintains wireless communication with the car

d.16x2 Liquid crystal display:
Helps changing the settings of the car and display values of different parameters.

In this version, the controller can send mode changing commands to the car, and all these processes can be monitored via the liquid crystal display. 

