//רמקול
void MP3(){
  mySerial.begin(9600, SERIAL_8N1, 14, 27); // RX, TX
  mySerial.write(arrCommandMp3, sizeof(arrCommandMp3));
  isPlaying = true;
}

void MP3_Stop() {
  mySerial.begin(9600, SERIAL_8N1, 14, 27); // RX, TX
  byte stopCommand[] = {0x7E, 0xFF, 0x06, 0x16, 0x00, 0x00, 0x00, 0xEF}; // פקודת עצירה 
  mySerial.write(stopCommand, sizeof(stopCommand));
  isPlaying = false;
}
