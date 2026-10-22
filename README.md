![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

# SmartGym

## Overview

SmartGym is an ESP32-based smart gym access and automation system that combines RFID authentication, a web-based interface, and sensor-driven hardware control. The system allows gym members to register and log in using an RFID card or a numeric access code, while automating door access and environmental controls.

## Features
- Member Management: Web-based registration and login, subscriber records, and visit counting.
- Access Control: RFID and numeric-code authentication with servo-controlled door opening and closing.
- Sensor Integration: Ultrasonic distance sensing, microphone input, and temperature monitoring.
- Automated Hardware Control: NeoPixel LED effects, temperature-based fan control, and MP3 audio playback.
- Web Interface: HTTP server hosted on the ESP32 for member registration and login.
- Embedded Development: Integration of sensors, actuators, communication interfaces, and control logic within a microcontroller-based system.

## Quick start (build & upload)
1. Open this project with the Arduino IDE or PlatformIO.
2. Select the appropriate ESP32 board and COM port.
3. Include required libraries: Servo, Wire, Adafruit_NeoPixel, WiFi, SPIFFS, MFRC522, SPI.
4. Upload the sketch file [sketch_may22b_copy_20250522215355.ino](sketch_may22b_copy_20250522215355.ino).

## Wiring (high level)
- RFID serial -> Serial1 pins (RXD2/TXD2).
- Servo -> GPIO 13 (attached in setup).
- Ultrasonic: TRIG_PIN (2), ECHO_PIN (15).
- NeoPixels: NEOPIXEL_PIN (18) and LED_PIN (4).
- Temperature sensor -> I2C (SDA=21, SCL=22).
- Fan driver -> IN_A (25), IN_B (33).
- Microphone ADC -> micPin (34).
- MP3 module -> specified RX/TX pins (see [Sensor_mp3.ino](Sensor_mp3.ino)).

## Web UI
- The ESP32 runs an HTTP server on port 80. Use the main page to choose "sign up" or "log in".
- Registration flow:
  - Open [GET /register handled by sendRegisterPage](handle_web.ino) and place RFID tag on the reader before submitting the form.
- Login options:
  - RFID: place tag then use [GET /login handled by sendLoginPageWithOptions](handle_web.ino).
  - Code: use the on-screen keypad which posts to [POST /check_code handled in handle_web.ino](handle_web.ino).

## Important functions & symbols
- [`checkDistance`](Sensor_distance.ino) — distance sensor & NeoPixel wave/color logic
- [`handleWeb`](handle_web.ino) — main HTTP request handler
- [`readRFID`](handle_web.ino) / [`extract_tag`](handle_web.ino) — RFID parsing
- [`sendRegisterPage`](handle_web.ino) / [`sendLoginPageWithOptions`](handle_web.ino) / [`sendHomePage`](handle_web.ino)
- [`connectToWifi`](handle_web.ino)
- [`MP3`](Sensor_mp3.ino) / [`MP3_Stop`](Sensor_mp3.ino)
- [`getTemperature`](Sensor_temperature.ino)
- [`turnOnFan`](Sensor_fan.ino) / [`turnOffFan`](Sensor_fan.ino)
- [`turnOnLeds`](Sensor_leds.ino) / [`turnOffLeds`](Sensor_leds.ino)
- [`findSubscriberIndexByRFID`](client.ino) / [`findSubscriberIndexByCode`](client.ino) / [`addSubscriber`](client.ino)
- [`resetVisitCountsIfNewMonth`](client.ino)
- [`openDoor`](Sensor_servo.ino) / [`closeDoor`](Sensor_servo.ino)
- [`Subscriber` struct and main loop/setup](sketch_may22b_copy_20250522215355.ino)
- [`Servo` class header`](Servo.h)

## Technologies

- ESP32
- Arduino / C++
- RFID (MFRC522)
- Wi-Fi and HTTP
- Ultrasonic distance sensor
- Temperature sensor and microphone
- Servo motor, NeoPixel LEDs, fan, and MP3 module

## Workspace files
- [client.ino](client.ino)
- [handle_web.ino](handle_web.ino)
- [README.md](README.md)
- [Sensor_distance.ino](Sensor_distance.ino)
- [Sensor_fan.ino](Sensor_fan.ino)
- [Sensor_leds.ino](Sensor_leds.ino)
- [Sensor_mic.ino](Sensor_mic.ino)
- [Sensor_mp3.ino](Sensor_mp3.ino)
- [Sensor_rfid.ino](Sensor_rfid.ino)
- [Sensor_servo.ino](Sensor_servo.ino)
- [Sensor_temperature.ino](Sensor_temperature.ino)
- [Servo.h](Servo.h)
- [sketch_may22b_copy_20250522215355.ino](sketch_may22b_copy_20250522215355.ino)

## Notes & TODO
- Validate and normalize subscriptionType strings between UI and logic (UI uses "Monthly/Yearly/One-time" while logic checks "month/year/day").
- Add persistent storage for subscribers (SPIFFS or LittleFS) to survive reboots.
- Improve concurrency/clients handling and HTTP request parsing for larger bodies.
- Consider debouncing RFID reads and securing the web UI (simple token or captive portal).

License
