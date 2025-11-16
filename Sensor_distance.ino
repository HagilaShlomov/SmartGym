// מרחק

void checkDistance() {
  // קריאת מרחק מהחיישן
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // קביעת צבע בסיס לפי מרחק
  if (distance < 20) {
    baseR = 255; baseG = 0; baseB = 0;   // אדום
  } else if (distance < 40) {
    baseR = 0; baseG = 0; baseB = 255;   // כחול
  } else {
    baseR = 0; baseG = 255; baseB = 0;   // ירוק
  }

  // כל 'speed' מילי־שניות נעדכן את אפקט השרשרת
  if (millis() - lastUpdate >= speed) {
    lastUpdate = millis();
    offset = (offset + 1) % (NUM_PIXELS * 2); // הלולאה מתחדשת כל סיבוב

    for (int i = 0; i < NUM_PIXELS; i++) {
      // חישוב הבהירות לפי מיקום השרשרת
      int distanceFromWave = abs(offset - i);
      int brightness = 255 - min(distanceFromWave * 40, 255); // דעיכה הדרגתית

      uint32_t color = pixels.Color(
        (baseR * brightness) / 255,
        (baseG * brightness) / 255,
        (baseB * brightness) / 255
      );

      pixels.setPixelColor(i, color);
    }

    pixels.show();
  }

  // delay(10); // עיכוב קטן ללולאה
}
