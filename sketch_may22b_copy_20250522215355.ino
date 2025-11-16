#include <Servo.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <MFRC522.h> 
#include <SPI.h>
#include <time.h> 
//רמקול
#include <HardwareSerial.h>

#define SENSOR_ADDR 0x48 
#define IN_A 25
#define IN_B 33
#define TRIG_PIN 2
#define ECHO_PIN 15
#define NEOPIXEL_PIN 18
#define NUM_PIXELS 16
#define BUFFER_SIZE       14
#define DATA_SIZE         10
#define DATA_VERSION_SIZE 2 
#define DATA_TAG_SIZE     8
#define CHECKSUM_SIZE     2
#include<Adafruit_NeoPixel.h>

#define PIN 18
#define NUMPIXELSSECOND   16

#define WLAN_SSID "U80_584B0"
#define WLAN_PASS "12345678"

#define NUMPIXELS 8
#define LED_PIN  4

// הגדרות ל-RFID
#define BUFFER_SIZE       14
#define DATA_SIZE         10
#define DATA_VERSION_SIZE 2 
#define DATA_TAG_SIZE     8
#define CHECKSUM_SIZE     2

#define RXD2 5
#define TXD2 23

WiFiServer server(80);

uint8_t buffer[BUFFER_SIZE];
int buffer_index = 0;


String currentTag = "";     // התג הנוכחי שנקרא
unsigned long lastReadTime = 0;  // הזמן שבו התג נקרא לאחרונה

// מערך לצ'יפים רשומים
String registeredTags[10];
int registeredCount = 0;

// Adafruit_NeoPixel pixels3 =Adafruit_NeoPixel(NUMPIXELSSECOND, PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
int delayval = 100; 


uint8_t baseR = 0, baseG = 255, baseB = 0; // צבע ברירת מחדל: ירוק
unsigned long lastUpdate = 0;
const int speed = 20; // ככל שקטן יותר → יותר מהיר

int offset = 0;
Servo myServo;  // יצירת אובייקט למנוע הסרוו

int lastResetMonth = -1;

HardwareSerial mySerial(2);

byte arrCommandMp3[] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x01, 0xEF}; 
bool isPlaying = false;
bool isPlayingToClose = false;

const int micPin = 34; // פין ADC של ה-ESP32
Adafruit_NeoPixel pixels2(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentRoomCount=0;
String currentCode = "";



void setup() {

  Serial.begin(115200);  
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);  // rfid

  Serial.println("Start setup");

  connectToWifi();
  server.begin();

  myServo.attach(13); 
  Wire.begin(); // SDA = 21, SCL = 22 ב־ESP32
  pinMode(IN_A, OUTPUT);
  pinMode(IN_B, OUTPUT);
  // pinMode(TRIG_PIN, OUTPUT);
  // pinMode(ECHO_PIN, INPUT);
  pixels.begin();
  pixels.clear();
  pixels.show();
  pixels2.begin();
  pixels2.clear();
  pixels2.show();
  analogReadResolution(12); // רזולוציה 12 ביט

  Serial.println("End setup");
  

}

struct Subscriber {
  String name;        // שם מנוי
  String rfidTag;     // תג RFID
  String subscriptionType; // סוג מנוי
  int visitCount;     // מספר ביקורים
  unsigned long enterTime;
  String accessCode;

};

// יצירת מערך של מנויים
const int MAX_SUBSCRIBERS = 10;
Subscriber subscribers[MAX_SUBSCRIBERS] = {
  {"danny cohen", "003DB9DA", "month", 5, 0, "123"},
  {"sara levi", "003A208F", "year", 12, 0, "456"},
  {"hagila shlomov", "0031AF44", "day", 1, 0, "987"},
};

int currentCount = 3; // מספר המנויים הנוכחי

void loop(){
  handleWeb();
      // בדיקה כמה פעמים ביקר
      // בדיקה כמה יש כרגע בחדר
  if (currentTag != "") {
    int indexC = findSubscriberIndexByRFID(currentTag);
    Serial.print("indexC: ");
    Serial.println(indexC);
    currentTag ="";

    if (indexC != -1) {
      String sub = subscribers[indexC].subscriptionType;
      int subVisit = subscribers[indexC].visitCount;
      Serial.print("subVisit: ");
      Serial.println(subVisit);
      int maxVisits = 0;
      if (sub == "year") maxVisits = 150;
      else if (sub == "month") maxVisits = 13;
      else if (sub == "day") maxVisits = 1;
      Serial.print("maxVisits: ");
      Serial.println(maxVisits);
      Serial.print("currentRoomCount: ");
      Serial.println(currentRoomCount);

      if (subVisit < maxVisits && currentRoomCount < 10 && subscribers[indexC].enterTime == 0) {
        Serial.println("open door!!!!!");
        openDoor();
        currentRoomCount++;
        subscribers[indexC].enterTime = millis();
        subscribers[indexC].visitCount += 1;
        Serial.print("currentRoomCount: ");
        Serial.println(currentRoomCount); 
      }
    }
  }
  else if(isCodeRegistered(currentCode)){
    int indexcode=findSubscriberIndexByCode(currentCode);
    currentCode = "";

    if (indexcode != -1) {
      String sub = subscribers[indexcode].subscriptionType;
      int subVisit = subscribers[indexcode].visitCount;
      Serial.print("subVisit: ");
      Serial.println(subVisit);
      int maxVisits = 0;
      if (sub == "year") maxVisits = 150;
      else if (sub == "month") maxVisits = 13;
      else if (sub == "day") maxVisits = 1;
      Serial.print("maxVisits: ");
      Serial.println(maxVisits);
      Serial.print("currentRoomCount: ");
      Serial.println(currentRoomCount);

      if (subVisit < maxVisits && currentRoomCount < 10 && subscribers[indexcode].enterTime == 0) {
        Serial.println("open door!!!!!");
        openDoor(); 
        currentRoomCount++;
        subscribers[indexcode].enterTime = millis();
        subscribers[indexcode].visitCount += 1;
        Serial.print("currentRoomCount: ");
        Serial.println(currentRoomCount); 
      }
    }
  }

  exit_sub();
  if(currentRoomCount>=1){
    closeDoor();
    MP3();
    int voice=micInfo();
    turnOnLeds(voice);
    double temp=getTemperature();
    if(temp > 30){
      turnOnFan();
    }
    else{
      turnOffFan();
    }
  }
  else{
    turnOffLeds();
    isPlayingToClose = true;
    if(isPlayingToClose){
      byte stopCommand[] = {0x7E, 0xFF, 0x06, 0x16, 0x00, 0x00, 0x00, 0xEF};
      mySerial.write(stopCommand, sizeof(stopCommand));
      isPlayingToClose = false;
      isPlaying = false;
    }
  }

  // resetVisitCountsIfNewMonth();

}

void exit_sub(){
  for (int i = 0; i < currentCount; i++) {
    if (subscribers[i].enterTime > 0 && millis() - subscribers[i].enterTime >= 30000) {
      // מנוי נמצא כבר בפנים, ועברו 30 שניות
      Serial.println("run time for: ");
      Serial.println(subscribers[i].name);
      if (currentRoomCount > 0) {
        currentRoomCount--;
      }      
      subscribers[i].enterTime = 0; // אפס כדי שלא ייצא שוב

      Serial.print("currentRoomCount after exit: ");
      Serial.println(currentRoomCount);
    }
  }
}

