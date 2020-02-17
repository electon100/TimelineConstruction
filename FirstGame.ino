#include <M5Stack.h>
#include "displayScreens.h" 
#include "buttons.h"

const int EVENTS = 4;

char* question1 = "In this year, Charles I became the second Stuart King, after the death of his father James I";
char* question2 = "Soon after his coronation as king, Parliament prevented Charles I from collecting Tonnage and Poundage for more than one year";
char* question3 = "Charles I needed money before he could fight with France and Spain, so he used a Forced Loan to raise money from the nobility";
char* question4 = "Five members of the nobility took the king to court over their imprisonment for refusing to pay the Forced Loan, but the king won";

const char* QUESTIONS[EVENTS] = {question1, question2, question3, question4};

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
}

void loop() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Add the first event to your timeline by pressing the middle button...");

  waitOnButton('b');

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Your first event is: ");
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("%s", question1);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.printf("Press the middle button to continue...");

  waitOnButton('b');

  for (int i = 1; i < EVENTS; i++){
    displayEvent(QUESTIONS[i]);
    bool correct = buttonC();
    if (correct){
      displayCorrect();
    }
    else{
      displayIncorrect();
    }
  }
}
