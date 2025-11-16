//טמפרטורה

double getTemperature() {
  Wire.beginTransmission(SENSOR_ADDR);//הכתובת של החיישן
  Wire.write(0x00); // רגיסטר קריאת טמפרטורה
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR, 2);//קריאת 2 בתים
  if (Wire.available() == 2) {
    uint8_t msb = Wire.read();//בית גבוה
    uint8_t lsb = Wire.read();//בית נמוך

    int16_t rawTemp = ((msb << 8) | lsb) >> 4;//מקבל את הערך הממשי 
    float temperature = rawTemp * 0.0625;//הכפלה לקבלת ערך אמיתי

    // Serial.print("טמפרטורה: ");
    // Serial.print(temperature);
    // Serial.println(" מעלות צלזיוס");
    return temperature;

  }

}
