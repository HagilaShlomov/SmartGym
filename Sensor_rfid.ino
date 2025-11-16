//rdif
void extract_tag2() {
  Serial.println("- - - - - - ");

  Serial.print("Message Head: ");
  Serial.println(buffer[0]);

  Serial.print("[Version] ");
  for (int i = 1; i < 1 + DATA_VERSION_SIZE; i++)
    Serial.print((char)buffer[i]);
  Serial.println();

  Serial.print("[Tag] ");
  for (int i = 3; i < 3 + DATA_TAG_SIZE; i++)
    Serial.print((char)buffer[i]);
  Serial.println();

  Serial.print("[Checksum] ");
  for (int i = 11; i < 11 + CHECKSUM_SIZE; i++)
    Serial.print((char)buffer[i]);
  Serial.println();

  Serial.print("Message Tail: ");
  Serial.println(buffer[13]);
}

void readRFID2() {
  int ssvalue;
  bool call_extract_tag = false;

  while (Serial1.available() > 0) {
    Serial.println("RFID is available");
    ssvalue = Serial1.read();
    if (ssvalue == -1) {
     Serial.println("entered RFID");

      return;
    }

    if (ssvalue == 2) {
      buffer_index = 0;
      Serial.println("Start of new message (2)");
    }

    if (ssvalue == 3) {
      call_extract_tag = true;
      Serial.println("End of message (3)");
    }

    if (buffer_index >= BUFFER_SIZE) {
      Serial.println("Buffer overflow");
      buffer_index = 0;
      return;
    }

    buffer[buffer_index++] = ssvalue;

    if (call_extract_tag && buffer_index == BUFFER_SIZE) {
      extract_tag2();
    }

    Serial.println(ssvalue);

  }
}

