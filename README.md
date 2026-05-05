![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

"# SmartGym" 

Short summary
- SmartGym is an ESP32-based gym access and environment controller using RFID and a simple web UI. It manages subscribers, opens/closes a servo door, plays audio, controls LEDs and a fan, and exposes a small web interface for registration and login.

Features
- RFID registration and login via web UI.
- Numeric code login via on-screen keypad.
- Per-subscriber visit counting and simple time-based exit.
- Door control using a servo.
- NeoPixel LED effects controlled by distance sensor and microphone level.
- MP3 playback control via serial to an audio module.
- Temperature-based fan control.

Quick start (build & upload)
1. Open this project with the Arduino IDE or PlatformIO.
2. Select the appropriate ESP32 board and COM port.
3. Include required libraries: Servo, Wire, Adafruit_NeoPixel, WiFi, SPIFFS, MFRC522, SPI.
4. Upload the sketch file [sketch_may22b_copy_20250522215355.ino](sketch_may22b_copy_20250522215355.ino).

Wiring (high level)
- RFID serial -> Serial1 pins (RXD2/TXD2).
- Servo -> GPIO 13 (attached in setup).
- Ultrasonic: TRIG_PIN (2), ECHO_PIN (15).
- NeoPixels: NEOPIXEL_PIN (18) and LED_PIN (4).
- Temperature sensor -> I2C (SDA=21, SCL=22).
- Fan driver -> IN_A (25), IN_B (33).
- Microphone ADC -> micPin (34).
- MP3 module -> specified RX/TX pins (see [Sensor_mp3.ino](Sensor_mp3.ino)).

Web UI
- The ESP32 runs an HTTP server on port 80. Use the main page to choose "sign up" or "log in".
- Registration flow:
  - Open [GET /register handled by sendRegisterPage](handle_web.ino) and place RFID tag on the reader before submitting the form.
- Login options:
  - RFID: place tag then use [GET /login handled by sendLoginPageWithOptions](handle_web.ino).
  - Code: use the on-screen keypad which posts to [POST /check_code handled in handle_web.ino](handle_web.ino).

Important functions & symbols
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

Workspace files
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

Notes & TODO
- Validate and normalize subscriptionType strings between UI and logic (UI uses "Monthly/Yearly/One-time" while logic checks "month/year/day").
- Add persistent storage for subscribers (SPIFFS or LittleFS) to survive reboots.
- Improve concurrency/clients handling and HTTP request parsing for larger bodies.
- Consider debouncing RFID reads and securing the web UI (simple token or captive portal).

License
