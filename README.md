# ESP32-Smart-Seismograph-with-Emergency-Alert-System

📖 Project Overview

The ESP32 Smart Seismograph is an IoT-based earthquake monitoring system that detects ground vibrations using an MPU6050 accelerometer and displays real-time seismic data on a 1.3-inch OLED (SH1106) display.

When vibration intensity crosses predefined thresholds, the system automatically triggers emergency alerts:

📩 SMS alerts for moderate seismic activity (1st & 2nd events)

📞 Automatic phone call for high-risk activity (3rd event)

This project is ideal for academic mini-projects, IoT demonstrations, disaster management prototypes, and smart safety systems.

⚙️ Features

📟 Real-time vibration monitoring

📊 Displays current vibration, maximum vibration, and event count

🚨 Three alert levels: Normal, Minor, Moderate, High

🔔 Buzzer & LED alert indication

📩 SMS alert using Twilio (1st & 2nd hit)

📞 Automatic phone call using Twilio (3rd hit)

🌐 WiFi-enabled ESP32

📉 Noise reduction using moving average filter

🧰 Hardware Requirements
Component	Quantity
ESP32 Dev Board	1
MPU6050 Accelerometer	1
1.3-inch OLED Display (SH1106, I2C)	1
Buzzer	1
LED	1
Breadboard	1
Jumper Wires	As required
🖼️ Schematic Diagram


<img width="958" height="534" alt="seismograph" src="https://github.com/user-attachments/assets/da3374b7-4b8c-409f-ba5a-9747e1a28a19" />



📌 Note: The schematic diagram shows the complete wiring of ESP32, MPU6050, OLED (1.3-inch SH1106), buzzer, and LED. Make sure the I2C lines (SDA & SCL) are shared correctly.

🔌 Pin Configuration
I2C Connections
ESP32	MPU6050	OLED
GPIO21	SDA	SDA
GPIO22	SCL	SCL
3.3V	VCC	VCC
GND	GND	GND
Output Devices
Device	ESP32 Pin
Buzzer	GPIO25
LED	GPIO26
🧠 Working Principle

The MPU6050 accelerometer measures acceleration along X, Y, and Z axes.

Resultant vibration magnitude is calculated using vector magnitude.

A moving average filter smoothens noise.

Vibration intensity is classified into alert levels:

Normal

Minor

Moderate

High

Based on alert escalation:

1st & 2nd Moderate/High → SMS sent

3rd Moderate/High → Phone call placed

Alerts are rate-limited using cooldown timers.

📟 OLED Display Format
SEISMOGRAPH
Vib: 1.24 g
Max: 2.01
Events: 3
!! MODERATE !!
🌐 Software & Libraries Used

Arduino IDE

ESP32 Board Package

Libraries:

Wire.h

WiFi.h

HTTPClient.h

MPU6050.h

Adafruit_GFX.h

Adafruit_SH110X.h

base64.h

📡 Twilio Integration

This project uses Twilio REST APIs for:

Sending SMS alerts

Making emergency phone calls

⚠️ Note: You must have an active Twilio account with verified phone numbers.

🚀 How to Run

Install Arduino IDE and ESP32 board support

Install required libraries via Library Manager

Connect hardware as per pin diagram

Update WiFi & Twilio credentials in code

Upload code to ESP32

Open Serial Monitor (115200 baud)

Observe OLED and alerts during vibration

🧪 Applications

Earthquake early-warning prototype

Smart building safety system

Disaster management research

IoT-based monitoring systems

Engineering mini/major projects

📌 Future Enhancements

Richter scale estimation

Cloud data logging (Firebase / Thingspeak)

Mobile app integration

GPS-based location alerts

Battery backup support

👨‍💻 Author

Manideep
SRM University – Computing Technology
IoT & Full Stack Developer

⭐ Acknowledgements

Twilio API

Adafruit Libraries

Arduino & ESP32 Community

📜 License

This project is licensed for educational and academic use.
