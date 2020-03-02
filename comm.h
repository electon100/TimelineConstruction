#include "SoftwareSerial.h"
SoftwareSerial stack(2, 5); // RX, TX

String serialReadStr() {
  String buf = "";
  long timeout = 3000;
    long previousMillis = millis();
    while (true)
    {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > timeout) break;
      if (stack.available() > 0)
      {
          char chr = (char)stack.read();
          if (chr == '\n'){
              break;
          } 
          else
          {
              buf += chr;
          }
      }
    }

    return buf;
}

void serialWriteStr(String str) {
  Serial.print(str);
  Serial.print('\n');
}
