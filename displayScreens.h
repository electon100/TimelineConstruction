void initialDisplay(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Add the first event to your timeline by scanning it...");
}

void invalidEvent(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Event does not exist");
  delay(3000);
}

void nextEvent(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Scan your next event...");
}

void askNearbyEvent(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Please scan an event from your timeline that is close to the one you just selected...");
}

void notClose(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("That event is not close enough. Return the new event to the group pile and try a different one. Press the middle button to continue.");
}

void higherOrLower(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Is your new event before or after the event in your timeline?");
  M5.Lcd.setCursor(0, 220);
  M5.Lcd.printf("Before");
  M5.Lcd.setCursor(135, 220);
  M5.Lcd.printf("Same");
  M5.Lcd.setCursor(260, 220);
  M5.Lcd.printf("After");
}

void displayWait(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Waiting for other player...");
}

void displayEvent(const char* question){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("%s", question);
  M5.Lcd.setCursor(0, 200);
  M5.Lcd.printf("Press the middle button when ready to continue...");
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

void winScreen(){
  M5.Lcd.fillScreen(GREEN);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(115, 110);
  M5.Lcd.printf("YOU WIN!");
  delay(3000);
  M5.Lcd.setTextColor(WHITE);
}

void loseScreen(){
  M5.Lcd.fillScreen(RED);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(115, 110);
  M5.Lcd.printf("YOU LOSE!");
  delay(3000);
  M5.Lcd.setTextColor(WHITE);
}
