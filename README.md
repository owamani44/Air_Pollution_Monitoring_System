**🛰️ Air Pollution Monitoring System with GSM & ThingSpeak**

A real-time IoT-based air quality monitoring system that detects harmful gases in the environment using MQ-series sensors and transmits data via GSM (SIM800L) to a cloud platform (ThingSpeak) for visualization and alerting. This system is designed for deployment in regions affected by pollution to enable accessible, remote air quality monitoring.

**📋 Table of Contents**

Overview

System Features

Hardware Components

Circuit Diagram

How It Works

Installation & Setup

ThingSpeak Configuration

Expected Output

Troubleshooting

Use Cases

Contributing

License

**📖 Overview**

This system continuously monitors air quality using MQ-7 (Carbon Monoxide) and MQ-135 (Air Quality) gas sensors. Sensor data is processed by an Arduino board and transmitted over GSM to ThingSpeak. Users can remotely monitor air quality levels and receive alerts based on thresholds.

**⚙️ System Features**

Real-time air quality monitoring (CO, NH₃, NOₓ, Benzene, etc.)

Cloud-based logging and visualization using ThingSpeak

Remote deployment via GSM (SIM800L)

LED & buzzer alerts for unsafe air levels

Expandable design for additional sensors or IoT integration

**🔩 Hardware Components**

Component

Description

Arduino Uno/Nano

Main microcontroller

MQ-7

Carbon Monoxide gas sensor

MQ-135

Air quality sensor (multiple gases)

SIM800L

GSM module for cloud communication

LM2596

Step-down voltage regulator

Breadboard

Prototyping platform

Jumper wires

Circuit connections

LED

Visual alert

Buzzer

Audio alert

Power source

9V/12V battery or power supply

🖼️ Circuit Diagram
You can check out the circuit diagram  on " https://app.cirkitdesigner.com/project/330d60c6-4585-4c9c-b3df-c3cab3c22ea2 ".

**⚙️ How It Works**

Sensors: MQ-7 detects CO; MQ-135 detects harmful gases like NH₃, NOₓ, Benzene.

Arduino: Reads analog values from sensors and evaluates pollution level.

Thresholds: If values exceed a defined limit, the system triggers LED/buzzer alerts.

GSM Transmission: Arduino communicates with SIM800L to send data to ThingSpeak.

Cloud Dashboard: ThingSpeak plots data on graphs for remote monitoring.

**🛠 Installation & Setup**

Connect Hardware:

Wire MQ-7 and MQ-135 to analog pins A0 and A1.

Connect SIM800L to TX/RX (via logic level shifter if needed).

Connect buzzer and LED to digital pins.

Power Supply:

Use LM2596 to step down voltage to 4V for SIM800L.

Ensure clean power; SIM800L is sensitive to noise and current drops.

Upload Code:

Open AirPollutionMonitoring.ino in Arduino IDE.

Install required libraries: SoftwareSerial, ThingSpeak, etc.

Add your ThingSpeak channel details and SIM800L APN credentials.

Upload to Arduino.

🌐 ThingSpeak Configuration

Create a ThingSpeak account.

Create a new channel and enable fields (e.g., Field1 = CO, Field2 = AQI).

Get your Write API Key.

Replace in Arduino code:

String API = "YOUR_WRITE_API_KEY";

📊 Expected Output

Values printed on serial monitor

LED turns on + buzzer sounds when gas concentration exceeds safe limits

Data appears on ThingSpeak graphs

**🧪 Troubleshooting**

Problem and Solution

SIM800L not connecting

Check voltage (must be ~4V), strong signal
If it fails persistently replace the LM2596 with a shockty diode (IN4007 power diode)

GSM not sending data

Confirm APN, API key, network status

Sensors giving wrong readings

Warm up for 1-2 mins; calibrate if needed

Reboots when GSM sends data

Use capacitor (e.g., 1000µF) near SIM800L

**🌍 Use Cases**

Urban air quality monitoring

Smart city environmental systems

School science projects or IoT demos

Agricultural or industrial pollution detection
