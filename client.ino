


// חיפוש מנוי לפי תג RFID
int findSubscriberIndexByRFID(const String& rfidTag) {
  for (int i = 0; i < currentCount; i++) {
    if (subscribers[i].rfidTag == rfidTag) {
      return i;
    }
  }
  return -1;
}

int findSubscriberIndexByCode(const String& code) {
  for (int i = 0; i < currentCount; i++) {
    if (subscribers[i].accessCode == code) {
      return i;
    }
  }
  return -1;
}

bool isCodeRegistered(const String& code) {
  for (int i = 0; i < currentCount; i++) {
    if (subscribers[i].accessCode == code) {
      return true;
    }
  }
  return false;
}

// הוספת מנוי חדש
bool addSubscriber(const String& name, String& rfid, String& subType, int visit=0) {
  if (currentCount >= MAX_SUBSCRIBERS) return false;
  Subscriber newSubscriber = { name, rfid, subType, visit };
  if (findSubscriberIndexByRFID(newSubscriber.rfidTag) != -1) return false;

  subscribers[currentCount++] = newSubscriber;
  return true;
}

void resetVisitCountsIfNewMonth() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  int currentDay = timeinfo.tm_mday;
  int currentMonth = timeinfo.tm_mon; // 0 = ינואר

  // אם זה היום הראשון של החודש וזה חודש חדש מאז האיפוס האחרון
  if (currentDay == 1 && currentMonth != lastResetMonth) {
    Serial.println("Resetting visit counts for new month");
    for (int i = 0; i < currentCount; i++) {
      if (subscribers[i].subscriptionType == "month") {
        subscribers[i].visitCount = 0;
        Serial.print("Reset for: ");
        Serial.println(subscribers[i].name);
      }
    }
    lastResetMonth = currentMonth;
  }
}
