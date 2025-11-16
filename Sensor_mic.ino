
int micInfo(){
  int micValue = analogRead(micPin);
  // Serial.println(micValue);
  return micValue;
}