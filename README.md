# Multi-Functional Robot Car Project

## Overview
This repository contains the development of a **multi-functional robotic car** that evolves through four versions, each introducing new features and improvements. The car is controlled via a **custom mobile app** and integrates **Bluetooth communication, obstacle avoidance, robotic arm functionality, and live camera streaming**.

## Features
- **Versatile Movement**: Supports manual and autonomous driving modes.
- **Interactive Control**: Operates via an Android app for seamless user interaction.
- **Smart Navigation**: Uses sensors to detect and avoid obstacles efficiently.
- **Expandable Design**: Can be modified for additional functionalities.



## Version Summaries

### **V1: Basic Infrared Remote-Controlled Car**
- **Chassis**: Plywood with a toy car powertrain.
- **Control System**: Infrared remote control.
- **Basic Movement**: Forward, backward, left, right.

### **V2: Bluetooth-Controlled Car with Obstacle Avoidance**
- **Chassis**: Acrylic with TT motor kit.
- **Microcontroller**: Arduino UNO.
- **Motor Driver**: L293D.
- **Wireless Communication**: HC05 Bluetooth module.
- **Sensors**: Ultrasonic sensor mounted on a servo motor.
- **Shift Register**: Expands digital output pins.
- **Mobile App**: Developed using MIT App Inventor for stable control.

### **V3: Robotic Arm Integration**
- **All features of V2**, plus:
- **Robotic Arm**: Controlled via MG995 servo motors.
- **Advanced Mobile App**: Includes slider controls for six servo motors.

### **V4: Camera Streaming and Multi-Microcontroller System**
- **All features of V3**, plus:
- **ESP32 AI Thinker Camera Module**: Streams live video.
- **ESP8266 OLED Display**: Shows debug info and camera IP.
- **Multi-Microcontroller System**:
  - **MC0 (Arduino UNO)**: Main controller.
  - **MC1 (Arduino Nano)**: Handles sensors.
  - **MC2 (ESP8266)**: Manages OLED display.
  - **MC3 (ESP32CAM)**: Handles video streaming and timelapsing.
- **Vero Board Shield**: Custom header pins for motor outputs.

## Installation & Usage
1. Clone the repository:
   ```sh
   git clone https://github.com/Simplified-Shanto/Multi-Functional-Robot-Car-Project.git

