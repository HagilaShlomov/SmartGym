//סרוו

int servoPos = 0;

void setServo(int pos) {
  myServo.write(pos);
  servoPos = pos;  // לשמור את המיקום
}

// void turnServo() {
//   // הזזה מ־0 ל־180 מעלות
//   setServo(50);  // פותח
//   delay(3000);
//   setServo(0);   // סוגר

// }

bool doorRunning = false;
unsigned long doorStartTime = 0;

void openDoor() {
  if (!doorRunning) {
    myServo.write(60);  // פותח את הדלת ב-30 מעלות
    servoPos = 60;
    doorStartTime = millis();
    doorRunning = true;
  }
}

void closeDoor() {
  if (doorRunning && millis() - doorStartTime >= 4000) {  // אחרי 3 שניות
    myServo.write(0);  // סוגר את הדלת
    servoPos = 0;
    doorRunning = false;
  }
}


