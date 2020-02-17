void initialDisplay(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Add the first event to your timeline by pressing the middle button...");
}

void askQuestion(const char* question){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Is this event before or after the first event you picked:");
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.printf("%s", question);
}

void displayEvent(const char* question){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("%s", question);
}

void displayCorrect(){
  M5.Lcd.fillScreen(GREEN);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(115, 110);
  M5.Lcd.printf("CORRECT!");
  delay(3000);
  M5.Lcd.setTextColor(WHITE);
}

void displayIncorrect(){
  M5.Lcd.fillScreen(RED);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(105, 110);
  M5.Lcd.printf("INCORRECT!");
  delay(3000);
  M5.Lcd.setTextColor(WHITE);
}
