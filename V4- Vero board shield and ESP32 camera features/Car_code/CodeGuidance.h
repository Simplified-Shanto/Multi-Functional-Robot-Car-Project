/*Written by: Nayeem Islam Shanto (islamshafiul283@gmail.com)
 *Intended board: Arduino Uno.
 *Controller: MI2 AI2 built custom Mobile App. 
 * 
 *Modification start:  1-24-2024
 *Modification end:  . 
 *
 * What's new in this version? 
 * 1.Debug prints will be removed from the main microcontroller to reduce memory load. 
      An OLED display will be attached with the second microcontroller for the purpose. 
   2. Existing LCD display will be removed and line follower functionalities will be introduced. 
         
 */



 /*
Hardware connections. Board; Arduino Uno. 
Analog Pins. 
-------------------------------------
A0   echoPin of sonar sensor. 
A1   
A2   txPin of softwareSerial.
A3   rxPin of softwareSerial.  
A4   
A5   




Digital Pins (PWM 3, 5, 6, 9, 10, 11)
-------------------------------------
D0        TX of the Bluetooth module. 
D1        RX of the Bluetooth module. 
D2        dataPin of shift register. 
~D3       common enable pins of the wheel motors. 
D4        clockPin of shift register. 
~D5        Sensor shield power Transistor switching pin.
~D6       armServo4
D7        
D8        latchPin of shift register. 
~D9       armServo1. 
~D10      armServo2Left. 
~D11      armServo2Right. 
D12       Sonar sensor carrying servo. 
D13       trigPin of sonar sensor. 


New Digital Pins from shift register. 
14        GripperIN1
15        WristMotor(in3)
16        in1 of wheel motor. 
17        in3 of wheel motor. 
18        GripperIN2
19        in2 of wheel motor. 
20        in4 of wheel motor. 
21        WristMotor(in4)


*/




/*
 * Control Characters
 "f:x." Move forward with pwm x (0<=x<=255)
 "b:x." Move backward with pwm x (0<=x<=255)
 "l:x." Move left with pwm x (0<=x<=255)
 "r:x." Move right with pwm x (0<=x<=255)
 "u:1." Move the base continuous rotation servo in forward direction
 "u:-1." Move the base continous rotation servo in backward direction
 "v:1."  Move the shoulder joint complementary servo pair in forward direction 
 "v:-1." Move the shoulder joint complementary servo pair in backward direction 
 "w:1."  Move the elbow joint CRS motor in forward direction 
 "w:-1." Move the elbow joint CRS motor in backward direction 
 "y:1." Rotate the wrist clockwise. 
 "y:-1." Rotate the wrist anticlockwise. 
 "z:1." Open up the gripper. 
 "z:-1." Close the gripper. 
 "m:1." Set car to Driving mode. 
 "m:2." Set car to Obstacle avoidance mode. 
 "m:3." Set car to Sensor reading mode and keep sending 
        sensor readings to controller app. 
 "m:4." Set the car to Line follower mode. 
 
 "index:value." Change the Obstacle avoidance settings value at 
                the mentioned index with the given value. This was
                introduced to calibrate obstacle avoidance performa
                nce realtime by adjusting variables like safe distance, 
                forward velocity, right-left velocity etc. 

 */
 