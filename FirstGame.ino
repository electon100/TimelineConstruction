#include <M5Stack.h>
#include "displayScreens.h" 
#include "buttons.h"
#include "eventList.h"
#include "comm.h"

// TODO Fix error when 2 then 0 is scanned and when 3 then 1 is scanned and when you scan before it says scan event

// Number of events required to win
#define WINNUM 10

SoftwareSerial rfid(3, 1); // RX, TX

// Id of the device, 0 = MASTER, 1 = SLAVE
int deviceID = 0;
// Keeps track of the events the player has
EventList eventList = EventList(WINNUM);
int loopCount = 0;

char tagValue[11];

// Code that can be removed at a later date
// ------------------------------------------
// TODO this needs to be the number of lines in the read in text file
const int EVENTS = 4;

const char* question1 = "In this year, Charles I became the second Stuart King, after the death of his father James I";
const char* question2 = "Soon after his coronation as king, Parliament prevented Charles I from collecting Tonnage and Poundage for more than one year";
const char* question3 = "Charles I needed money before he could fight with France and Spain, so he used a Forced Loan to raise money from the nobility";
const char* question4 = "Five members of the nobility took the king to court over their imprisonment for refusing to pay the Forced Loan, but the king won";

const char* QUESTIONS[EVENTS] = {question1, question2, question3, question4};
const int YEARS[EVENTS] = {1625, 1626, 1627, 1627};

// ------------------------------------------

const char* allowedTags[10] = {
  "0600B48BC4\0",
  "0600B3E375\0",
  "0600B498C4\0",
  "0600B3BAC2\0",
  "0600B40130\0",
  "0600B4485B\0",
  "0600B63DBB\0",
  "0600B3C469\0",
  "0600B4C521\0",
  "0600B3CC6D\0"
};

int tagInEvents(char tag[]){
  tag[10] = '\0';
  for (int i = 0; i < EVENTS; i++){
    if (strncmp(allowedTags[i], tag, 10) == 0){
      return i;
    }
  }
  return -1;
}

int waitForEvent(){
  Serial.println("Waiting for event");
  char val = 0;
  int readCount = 0;
  while (readCount < 16){
    if (rfid.available() > 0){
      val = rfid.read();
      if (0 < readCount && readCount < 11){
        tagValue[readCount-1] = val;
      }
      readCount++;
    }
  }
  
  Serial.println("Got event:");
  Serial.println(tagValue);

  return tagInEvents(tagValue);
}

int gameEnded(){
  Serial.println("Checking if game has ended");
  // Check if master device has 10 events
  if (eventList.getSize() == WINNUM) return deviceID;
  // Check if slave device has 10 events
  Serial.println("Waiting for slave response");
  String slaveAnswer = "";
  slaveAnswer = serialReadStr();
  int slaveSize = slaveAnswer.toInt();
  Serial.println(slaveAnswer);
  if (slaveSize == WINNUM) return 1;
  // -1 return indicates game has not ended
  return -1;
}

void endGame(int winner){
  if (winner == deviceID){
    winScreen();
    serialWriteStr("lose");
  }
  else {
    loseScreen();
    serialWriteStr("win");
  }
  delay(20000);
}

String alertMaster(){
  Serial.println("Sending information to master");
  // Send master device the number of events in list
  serialWriteStr(String(eventList.getSize()));
  String ending = serialReadStr();
  return ending;
}

bool isNearby(int event, int neighbours[]){
  if (event == neighbours[0] || event == neighbours[1]) return true;
  else if (YEARS[event] == YEARS[neighbours[0]] || YEARS[event] == YEARS[neighbours[1]]) return true;
  else return false;
}

char buttonToPress(int event, int nearbyEvent){
  if (YEARS[event] < YEARS[nearbyEvent]) return 'a';
  else if (YEARS[event] > YEARS[nearbyEvent]) return 'c';
  else if (YEARS[event] == YEARS[nearbyEvent]) return 'b';
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);

  Serial.begin(9600);
  rfid.begin(9600);
  stack.begin(57600);
  // TODO start menu here
}

void loop() {
  // If it's the first loop
  if (loopCount == 0){
    Serial.println("First time loop");
    
    // Ask user to scan first event
    initialDisplay();
    int event = waitForEvent();
    if (event > -1){
      Serial.println("Valid event");
      eventList.addEvent(event);
      displayEvent(QUESTIONS[event]);
    }
    else{
      Serial.println("Invalid event");
      invalidEvent();
    }

    Serial.println("Waiting on button B press");
    waitOnButton('b');
  }
  else{
    Serial.println("Normal loop");
    displayWait();
    // Check if anyone has won the game, if they have, end the game (only do this if master device)
    if (deviceID == 0){
      int winner = gameEnded();
      if (winner > -1){
        endGame(winner);
      }
    }
    // Tell the master device the current size of the event list (only do this if slave device)
    else if (deviceID == 1){
      String ending = alertMaster();
      if (ending == "win") winScreen();
      else if (ending == "lose") loseScreen();
    }

    nextEvent();
    
    // Wait for new event
    int event = waitForEvent();    
    if (event > -1){
      Serial.println("Before");
      const char* question = QUESTIONS[event];
      Serial.println("HERE");
      Serial.println(question);
      Serial.println("Middle");
      displayEvent(QUESTIONS[event]);
      Serial.println("After");
    }
    else M5.Lcd.printf("Event does not exist");
    waitOnButton('b');

    // When a new event is detected, ask the player to select an event closest to the new event
    askNearbyEvent();
    int nearbyEvent = waitForEvent();
    bool valid = false;
    if (nearbyEvent > -1){
      int neighbours[2];
      eventList.getNeighbours(event, neighbours);
      Serial.println(neighbours[0]);
      Serial.println(neighbours[1]);
      valid = isNearby(nearbyEvent, neighbours);
    }

    if (valid){
      // The user must then input whether the new event is before or after the chosen close event
      higherOrLower();
      char correctButton = buttonToPress(event, nearbyEvent);
      bool correct = false;
      if (correctButton == 'b') correct = anyButton();
      else correct = mustPressButton(correctButton);

      // If correct, add event to events list
      if (correct){
        eventList.addEvent(event);
        displayCorrect();
        delay(2000);
      }
      // If incorrect, play a minigame
      else{
        displayIncorrect();
        delay(2000);
        // If minigame succeeds, add event to list
        // If minigame fails, change event associated with tag
      }
    }
    else{
      // Tell the user the event they chose was not close enough to the new event
      notClose();
      waitOnButton('b');
    }
    
    // With a random probability, choose a new order for the list (only do this if master device)
    //    Wait for all events in the list to be scanned through in the new order
    //    Events that are scanned through wrong now change and are no longer part of the event list
    // Receieve whether master device has decided to reorder the list (only do this if slave device)
    //    Wait for all events in the list to be scanned through in the new order
    //    Events that are scanned through wrong now change and are no longer part of the event list
  }
  
  loopCount++;
}
