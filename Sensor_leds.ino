// לדים חכמים

// void turnOnLeds(int micLevel) {
// //   // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
// // for(byte i=0;i< NUMPIXELSSECOND ;i++){
// //     // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
// //     pixels2.setPixelColor(i, random(255), random(255), random(255)); // Moderately bright green color.
// //     pixels2.show(); // This sends the updated pixel color to the hardware.
// //     delay(delayval); // Delay for a period of time (in milliseconds).
// //  }
// //     pixels2.clear();
// //     pixels2.show();
 
// //   //  delay(delayval); 

//  int brightness = map(micLevel, 0, 4095, 0, 255);

//   for(byte i=0; i<NUMPIXELS; i++){
//     // הפעלה של הלדים עם צבעים אקראיים ועם בהירות לפי עוצמת המיקרופון
//     uint8_t r = random(0, brightness);
//     uint8_t g = random(0, brightness);
//     uint8_t b = random(0, brightness);
//     pixels2.setPixelColor(i, pixels2.Color(r, g, b));
//   }
//   pixels2.show();

//   // אפשר לשנות את ההשהיה לפי עוצמה, כך שהקצב משתנה
//   int dynamicDelay = map(micLevel, 0, 4095, 100, 10);
//   delay(dynamicDelay);

//   pixels2.clear();
//   pixels2.show();
// }

void turnOnLeds(int micLevel) {
  static unsigned long previousBrightnessMillis = 0;
  static unsigned long previousColorMillis = 0;
  static bool ledsOn = false;

  static uint8_t currentR = 255;
  static uint8_t currentG = 0;
  static uint8_t currentB = 0;

  unsigned long currentMillis = millis();//שומר את הזמן שעבר מאז ההפעלה

  // שינוי צבע כל 2000 מילישניות
  if (currentMillis - previousColorMillis >= 2000) {//שינוי צבע כל 2 שניות
    currentR = random(0, 256);
    currentG = random(0, 256);
    currentB = random(0, 256);
    previousColorMillis = currentMillis;
  }

  // מיפוי העוצמה לפי טווח המיקרופון 2000-4000
  int brightness = map(constrain(micLevel, 2000, 4000), 2000, 4000, 10, 255);

  // כל כמה מילישניות מעדכנים את הלדים לפי רמת מיקרופון
  if (currentMillis - previousBrightnessMillis >= 50) {
    uint8_t r = (currentR * brightness) / 255;
    uint8_t g = (currentG * brightness) / 255;
    uint8_t b = (currentB * brightness) / 255;

    // מעדכן את כל 16 הלדים
    for (byte i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();

    // מעדכן את כל 8 הלדים
    for (byte i = 0; i < NUMPIXELS; i++) {
      pixels2.setPixelColor(i, pixels2.Color(r, g, b));
    }
    pixels2.show();
    previousBrightnessMillis = currentMillis;
  }
}

void turnOffLeds() {
  for (byte i = 0; i < NUMPIXELS; i++) {
    pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));  // כיבוי הצבע
  }
  pixels2.show();  // רענון הפיקסלים
  
  for (byte i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}



