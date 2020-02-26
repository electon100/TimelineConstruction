#include <M5Stack.h>
#include "SoftwareSerial.h"
#include "displayScreens.h" 
#include "buttons.h"
#include "eventList.h"

// Number of events required to win
#define WINNUM 10

SoftwareSerial rfid(3, 1); // RX, TX

char* allowedTags[] = {
  "4300F9AA1F",
  "0000000000",
  "0000000000",
  "0000000000"
};

char tagValue[10];

// Code that can be removed at a later date
// ------------------------------------------
// TODO this needs to be the number of lines in the read in text file
const int EVENTS = 4;

char* question1 = "In this year, Charles I became the second Stuart King, after the death of his father James I";
char* question2 = "Soon after his coronation as king, Parliament prevented Charles I from collecting Tonnage and Poundage for more than one year";
char* question3 = "Charles I needed money before he could fight with France and Spain, so he used a Forced Loan to raise money from the nobility";
char* question4 = "Five members of the nobility took the king to court over their imprisonment for refusing to pay the Forced Loan, but the king won";

const char* QUESTIONS[EVENTS] = {question1, question2, question3, question4};
const int YEARS[EVENTS] = {1625, 1626, 1627, 1627};
// ------------------------------------------

// Id of the device, 0 = MASTER, 1 = SLAVE
int deviceID = 0;
// Keeps track of the events the player has
EventList eventList = EventList(WINNUM);
int loopCount = 0;

int tagInEvents(char tagValue[]){
  for (int i = 0; i < EVENTS; i++){
    if (strcmp(allowedTags[i], tagValue) == 0){
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
  // TODO Check if slave device has 10 events
  int slaveSize = 0;
  if (slaveSize == WINNUM) return 1;
  // -1 return indicates game has not ended
  return -1;
}

void endGame(int winner){
  Serial.println("Game has ended");
  //TODO display winner and loser screens correctly
  return;
}

void alertMaster(){
  Serial.println("Sending information to master");
  //TODO send master device the number of events in list
  return;
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
    // Check if anyone has won the game, if they have, end the game (only do this if master device)
    if (deviceID == 0){
      int winner = gameEnded();
      if (winner > -1){
        endGame(winner);
      }
    }
    // Tell the master device the current size of the event list (only do this if slave device)
    else if (deviceID == 1){
      alertMaster();
    }

    nextEvent();
    
    // Wait for new event
    int event = waitForEvent();
    if (event > -1){
      displayEvent(QUESTIONS[event]);
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
      valid = isNearby(nearbyEvent, neighbours);
    }

    if (valid){
      // The user must then input whether the new event is before or after the chosen close event
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
