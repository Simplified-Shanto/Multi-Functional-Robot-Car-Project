In this version of the car, you have two basic units: The car and the controller mobile app

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

f. 74HC595 shift register: 
Works as a digital output multiplexer, effectively increasing the digital output pins.

g. MG995 Servo motor: 
Rotates different joints of the robotic arm.

h. ESP32 AI thinker camera module
Streams video from the car to the controller device. 

Vero board shield header details: 
Header 3
_________
Pin                     Functionality
```                    `````````````````````````````````````````
1                       Gripper motor output 1(+)
2                       Gripper motor output 2(-)
3
4
5
6
7                       UnusedMotor output 1(+)
8                       Unused Motor output 2(-)


2. Controller Mobile App:
The mobile app is developed using MIT app inventor. Follow these instructions to connect the car with the app: 
a. Make sure the car's HC05 bluetooth module is flashing
b. When opening the app for the first time, make sure the app has nearby communication permission. 
c. Turn on bluetooth of your mobile device. 
d. Pair the car's bluetooth module with the device. The asked password for the bluetooth module is usually '1234'
e. After opening the app, press the 'Connect' button and you'll get a list of paired bluetooth devices. 
f. Select the item in the list that resembles the name of your bluetooth module. 
g. Once the app is connected to the car, the app will automatically take you to the main interface. 
h. Now, the controls in the interface can be used to control the car. 

This app features stable control system for forward, backward, left, right and slider control for 6 servo motors connected on the robotcar. 
