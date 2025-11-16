void sendRegisterPage(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<!DOCTYPE html><html><head><title>Register</title>");
  client.println("<style>");
  client.println("body { font-family: Arial, sans-serif; text-align: center; padding: 20px; background: #f0f0f0; }");
  client.println("form { background: white; padding: 20px; display: inline-block; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }");
  client.println("input, select { margin: 10px 0; padding: 10px; width: 100%; font-size: 16px; }");
  client.println("button { padding: 10px 20px; font-size: 18px; background: #28a745; color: white; border: none; border-radius: 5px; cursor: pointer; }");
  client.println("button:hover { background: #218838; }");
  client.println("</style>");
  client.println("</head><body>");

  client.println("<h2>Register New Subscriber</h2>");
  client.println("<form action='/register_do' method='POST'>");
  client.println("Name:<br><input type='text' name='name' required><br>");
  client.println("Subscription Type:<br>");
  client.println("<select name='subscriptionType' required>");
  client.println("<option value='Monthly'>Monthly</option>");
  client.println("<option value='Yearly'>Yearly</option>");
  client.println("<option value='One-time'>One-time</option>");
  client.println("</select><br>");
  client.println("<p>Place RFID tag on reader then submit within 3 seconds.</p>");
  client.println("<button type='submit'>Register</button>");
  client.println("</form>");

  client.println("</body></html>");
}



void handleWeb() {
  readRFID();  // קריאת תג RFID

  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("New client connected");
  String request = client.readStringUntil('\r');
  Serial.println(request);

  // דפי כניסה ורישום
  if (request.indexOf("GET /register ") != -1) {
    sendRegisterPage(client);
  } 
  else if (request.indexOf("GET /login ") != -1) {
    // sendWaitingPage(client, "login");
    sendLoginPageWithOptions(client);

  }
  // טיפול POST ברישום
  else if (request.indexOf("POST /register_do") != -1) {
    // נקרא את שארית הבקשה (BODY) - נניח שזה קטן ונסרוק אותו במלואו
    String body = client.readStringUntil('\0'); 

    // מחלץ את name ו-subscriptionType מתוך body
    String name = getValue(body, "name");
    String subscriptionType = getValue(body, "subscriptionType");

    if (millis() - lastReadTime > 5000 || currentTag == "") {
      sendMessage(client, "No RFID tag detected. Please place tag and try again.");
    } else if (!isTagRegistered(currentTag)) {
      if (currentCount < MAX_SUBSCRIBERS) {
        subscribers[currentCount] = {name, currentTag, subscriptionType, 0};
        currentCount++;
        registeredCount++;
        sendMessage(client, "Registration successful for " + name);
      } else {
        sendMessage(client, "Subscriber list is full");//רשימת מנויים מלאה
      }
    } else {
      sendMessage(client, "Tag is already registered");//מנוי כבר רשום
    }
    currentTag = "";
  } 
  // התחברות באמצעות תג RFID בלבד
  else if (request.indexOf("GET /login_do") != -1) {
    if (millis() - lastReadTime > 3000 || currentTag == "") {
      sendMessage(client, "No RFID tag detected.");
    } else if (isTagRegistered(currentTag)) {
      sendMessage(client, "Login successful");
      // return currentTag;
    } else {
      sendMessage(client, "Tag not registered. Please sign up first.");
    }
    currentTag = "";
  }
  else if (request.indexOf("POST /check_code") != -1) {
    String body = client.readStringUntil('\0'); 
    String inputCode = getValue(body, "code");
    Serial.println(inputCode);

    currentCode = inputCode; // שומר במשתנה הגלובלי

    if (isCodeRegistered(inputCode)) {
      sendMessage(client, "Login successful");
    } else {
      sendMessage(client, "Incorrect code. Try again.");
    }
  }
  // דף הבית
  else {
    sendHomePage(client);
  }

  client.stop();
  Serial.println("Client disconnected.");
}

String getValue(String data, String key) {
  int keyIndex = data.indexOf(key + "=");
  if (keyIndex == -1) return "";

  int valueStart = keyIndex + key.length() + 1;
  int valueEnd = data.indexOf('&', valueStart);
  if (valueEnd == -1) valueEnd = data.length();

  String value = data.substring(valueStart, valueEnd);
  value.replace('+', ' ');  // החלפת + ברווח
  value = urlDecode(value);

  return value;
}

String urlDecode(String str) {
  String ret = "";
  char temp[] = "0x00";
  unsigned int len = str.length();
  unsigned int i = 0;

  while (i < len) {
    char c = str.charAt(i);
    if (c == '+') {
      ret += ' ';
    } else if (c == '%') {
      if (i + 2 < len) {
        temp[2] = str.charAt(i + 1);
        temp[3] = str.charAt(i + 2);
        ret += (char)strtol(temp, NULL, 16);
        i += 2;
      }
    } else {
      ret += c;
    }
    i++;
  }
  return ret;
}


bool isTagRegistered(String tag) {
  for (int i = 0; i < currentCount; i++) {
    if (subscribers[i].rfidTag == tag) {
      return true;
    }
  }
  return false;
}

void sendHomePage(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<!DOCTYPE html><html><head><title>sign in</title></head><body>");
  client.println("<h2 align=center>choose option</h2>");
  client.println("<p align=center><button style='width:200px;height:80px' onclick=\"location.href='/register'\"><h2>sign up</h2></button></p>");
  client.println("<p align=center><button style='width:200px;height:80px' onclick=\"location.href='/login'\"><h2>log in</h2></button></p>");
  client.println("</body></html>");
}

void sendMessage(WiFiClient client, String message) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<!DOCTYPE html><html><head><title>result</title></head><body>");
  client.println("<h2 align=center>" + message + "</h2>");
  client.println("<p align=center><a href='/'><button style='width:120px;height:60px'>back</button></a></p>");
  client.println("</body></html>");
}

void sendWaitingPage(WiFiClient client, String action) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE html><html><head><title>Waiting</title>");
  client.println("<meta http-equiv='refresh' content='2; url=/" + action + "_do'>");
  client.println("</head><body>");
  client.println("<h2 align=center>Waiting for chip...</h2>");
  client.println("</body></html>");
}

void connectToWifi() {
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// --- פונקציות RFID ---

void readRFID() {

  int ssvalue;
  bool call_extract_tag = false;

  if (Serial1.available() > 0) {
    ssvalue = Serial1.read();
    Serial.println("can read chip");
    Serial.println(ssvalue);

    if (ssvalue == -1) return;

    if (ssvalue == 2) {
      buffer_index = 0;
    }

    if (ssvalue == 3) {
      call_extract_tag = true;
    }

    if (buffer_index >= BUFFER_SIZE) {
      buffer_index = 0;
      return;
    }

    buffer[buffer_index++] = ssvalue;

    if (call_extract_tag && buffer_index == BUFFER_SIZE) {
      extract_tag();
    }
  }
}

void extract_tag() {
  Serial.println("extract_tag called");

  String tag = "";
  for (int i = 3; i < 3 + DATA_TAG_SIZE; i++) {
    tag += (char)buffer[i];
  }

  tag.trim(); // מנקה רווחים מיותרים

  if (tag.length() == DATA_TAG_SIZE) {
    currentTag = tag;
    lastReadTime = millis();
    Serial.print("current Tag: ");
    Serial.println(currentTag);
  }else {
    Serial.print("Tag length invalid: ");
    Serial.println(tag.length());
  }
}

void sendLoginPageWithOptions(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<!DOCTYPE html><html><head><title>Login</title>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<style>");
  client.println("body { font-family: sans-serif; text-align: center; background: #f0f0f0; padding: 20px; }");
  client.println(".keypad { display: grid; grid-template-columns: repeat(3, 80px); gap: 10px; justify-content: center; }");
  client.println("button { font-size: 24px; padding: 20px; }");
  client.println("#code { font-size: 28px; margin: 15px; letter-spacing: 10px; }");
  client.println("</style>");
  client.println("<script>");
  client.println("let code = '';");
  client.println("function addDigit(d) { code += d; document.getElementById('code').innerText = code; }");
  client.println("function clearCode() { code = ''; document.getElementById('code').innerText = ''; }");
  client.println("function submitCode() {");
  client.println(" fetch('/check_code', { method: 'POST', headers: {'Content-Type': 'application/x-www-form-urlencoded'}, body: 'code=' + code })");
  client.println(".then(response => response.text()).then(html => document.body.innerHTML = html); }");
  client.println("</script>");
  client.println("</head><body>");
  client.println("<h2>Login Options</h2>");
  client.println("<p>If you have a chip, please place it on the reader.</p>");
  client.println("<p>Or enter code manually:</p>");
  client.println("<div id='code'></div>");
  client.println("<div class='keypad'>");
  for (int i = 1; i <= 9; i++) {
    client.println("<button onclick='addDigit(" + String(i) + ")'>" + String(i) + "</button>");
  }
  client.println("<button onclick='clearCode()'>C</button>");
  client.println("<button onclick='addDigit(0)'>0</button>");
  client.println("<button onclick='submitCode()'>OK</button>");
  client.println("</div>");
  client.println("</body></html>");
}










// void handleWeb1() {
//   readRFID();  // קריאת תג RFID

//   WiFiClient client = server.available();
//   if (client) {
//     Serial.println("New client connected");

//     String request = client.readStringUntil('\r');
//     Serial.println(request);

//     // שליחה לדפי המתנה
//     if (request.indexOf("/register") != -1 && request.indexOf("/register_do") == -1) {
//       sendWaitingPage(client, "register");
//     } else if (request.indexOf("/login") != -1 && request.indexOf("/login_do") == -1) {
//       sendWaitingPage(client, "login");
//     }

//     // פעולת רישום
//     else if (request.indexOf("/register_do") != -1) {
//       if (millis() - lastReadTime > 3000 || currentTag == "") {
//         sendMessage(client, "no tag detected");
//       } else if (!isTagRegistered(currentTag)) {
//         if (registeredCount < 10) {
//           registeredTags[registeredCount++] = currentTag;
//           sendMessage(client, "sign up successfully");
//         } else {
//           sendMessage(client, "full array");
//         }
//       } else {
//         sendMessage(client, "already existing");
//       }
//       currentTag = "";
//     }

//     // פעולת התחברות
//     else if (request.indexOf("/login_do") != -1) {
//       if (millis() - lastReadTime > 3000 || currentTag == "") {
//         sendMessage(client, "no tag detected");
//       } else if (isTagRegistered(currentTag)) {
//         sendMessage(client, "log in successfully");
//       } else {
//         sendMessage(client, "need to sign up first");
//       }
//       currentTag = "";
//     }

//     // דף הבית
//     else {
//       sendHomePage(client);
//     }

//     delay(10);
//     client.stop();
//     Serial.println("Client disconnected.");
//   }
// }
