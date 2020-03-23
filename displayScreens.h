void initialDisplay(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Add the first event to\nyour timeline by scanning it...");
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
  M5.Lcd.printf("Please scan an event from your timeline that is\nclose to the one you just selected...");
}

void notClose(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("That event is not close\nenough. Return it to the\ngroup pile.");
  M5.Lcd.setCursor(0, 200);
  M5.Lcd.printf("Press the middle button\nwhen ready to continue...");
}

void higherOrLower(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Is your new event before\nor after the event in yourtimeline?");
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
  M5.Lcd.printf("Waiting for other player\n...");
}

void displayEvent(const char* question){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print(question);
  M5.Lcd.setCursor(0, 200);
  M5.Lcd.printf("Press the middle button\nwhen ready to continue...");
}

void displayCorrect(){
  M5.Lcd.fillScreen(GREEN);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(115, 110);
  M5.Lcd.printf("CORRECT!");
  M5.Lcd.setCursor(30, 150);
  M5.Lcd.printf("Add the event to your");
  M5.Lcd.setCursor(110, 170);
  M5.Lcd.printf("timeline!");
  delay(3000);
  M5.Lcd.setTextColor(WHITE);
}

void displayIncorrect(){
  M5.Lcd.fillScreen(RED);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(105, 110);
  M5.Lcd.printf("INCORRECT!");
  M5.Lcd.setCursor(20, 150);
  M5.Lcd.printf("Return the event to the");
  M5.Lcd.setCursor(100, 170);
  M5.Lcd.printf("group pile!");
  delay(3000);
  M5.Lcd.setTextColor(WHITE);
}

void noEvents(int reorder){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  switch (reorder){
    case 0:
      M5.Lcd.printf("You have no events from\nthe Personal Rule. Waitingon your opponent to scan\ntheir events...");
      break;
    case 1:
      M5.Lcd.printf("You have no events relating to finance. Waiting on your opponent to scan their events...");
      break;
    case 2:
      M5.Lcd.printf("You have no events relating to war. Waiting on your opponent to scan their events...");
      break;
    default:
      M5.Lcd.printf("You have no events from\nthe Personal Rule. Waitingon your opponent to scan\ntheir events...");
      break;
  }
}

void scanEvents(int numToScan, int reorder){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  switch (reorder){
    case 0:
      M5.Lcd.printf("Scan all events from your timeline relating to the\nPersonal Rule. You have %i events left...", numToScan);
      break;
    case 1:
      M5.Lcd.printf("Scan all events from your timeline relating to\nfinance. You have %i eventsleft...", numToScan);
      break;
    case 2:
      M5.Lcd.printf("Scan all events from your timeline relating to war. You have %i events left...", numToScan);
      break;
    default:
      M5.Lcd.printf("Scan all events from your timeline relating to the\nPersonal Rule. You have %i events left...", numToScan);
      break;
  }
}

void removingEvent(int reorder){
  M5.Lcd.fillScreen(RED);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(0, 0);
  switch (reorder){
    case 0:
      M5.Lcd.printf("That event is not part of the Personal Rule. It has been removed from your\ntimeline. Please return itto the group pile.");
      break;
    case 1:
      M5.Lcd.printf("That event is not related to finance. It has been\nremoved from your time-\nline. Please return it to the group pile.");
      break;
    case 2:
      M5.Lcd.printf("That event is not related to war. It has been re-\nmoved from your timeline. Please return it to the\ngroup pile.");
      break;
    default:
      M5.Lcd.printf("That event is not part of the Personal Rule. It has been removed from your\ntimeline. Please return itto the group pile.");
      break;
  }
  M5.Lcd.setCursor(0, 200);
  M5.Lcd.printf("Press the middle button\nwhen ready to continue...");
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
