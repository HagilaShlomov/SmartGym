//מאורר

void turnOnFan() {

  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, HIGH);
  // Serial.println("LEFT");


}
void turnOffFan() {

  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, LOW);
  // Serial.println("STOP");

}