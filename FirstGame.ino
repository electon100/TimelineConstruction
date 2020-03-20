#include <M5Stack.h>
#include "displayScreens.h" 
#include "buttons.h"
#include "eventList.h"
//#include "comm.h"
#include "SoftwareSerial.h"

// TODO Fix error when 2 then 0 is scanned and when 3 then 1 is scanned and when you scan before it says scan event

// Number of events required to win
#define WINNUM 10
#define deviceID 0
#define EVENTS 10

// Keeps track of the events the player has
EventList eventList = EventList(WINNUM);

SoftwareSerial rfid(3, 1); // RX, TX
SoftwareSerial stack(2, 5); // RX, TX

// Code that can be removed at a later date
// ------------------------------------------
// TODO this needs to be the number of lines in the read in text file

const PROGMEM char* question1 = "In this year, Charles I\nbecame the second Stuart\nKing, after the death of\nhis father James I";
const PROGMEM char* question2 = "Soon after his coronation as king, Parliament\nprevented Charles I from\ncollecting Tonnage and\nPoundage for more than oneyear";
const PROGMEM char* question3 = "Charles I needed money\nbefore he could fight withFrance and Spain, so he\nused a Forced Loan to\nraise money from the\nnobility";
const PROGMEM char* question4 = "Five members of the\nnobility took the king to court over their imprison-ment for refusing to pay\nthe Forced Loan, but the\nking won";
const PROGMEM char* question5 = "Parliament was dissolved\nafter it tried to intro-\nduce measures against\nTonnage and Poundage,\nstarting the Personal Rule";
const PROGMEM char* question6 = "Ship Money was introduced for inland towns to raise money during the Personal Rule";
const PROGMEM char* question7 = "The New Book of Common\nPrayer was introduced in\nScotland, causing riots\nthat led to the First\nBishops' War";
const PROGMEM char* question8 = "The Short Parliament was\ncalled to raise money for the Second Bishops' War,\nending the Personal Rule";
const PROGMEM char* question9 = "The Grand Remonstrance wasrejected by Charles duringthe Long Parliament, so\ncalled because it lasted\nlonger than the Short\nParliament";
const PROGMEM char* question10 = "The English Civil Wars\nbegan when Charles raised his standard in Nottingham";

const PROGMEM char* QUESTIONS[EVENTS] = {question1, question2, question3, question4, question5, question6, question7, question8, question9, question10};
const PROGMEM int YEARS[EVENTS] = {1625, 1626, 1627, 1627, 1629, 1635, 1637, 1640, 1641, 1642};
const PROGMEM bool PERSONALRULE[EVENTS] = {false, false, false, false, true, true, true, true, false, false};
const PROGMEM bool FINANCE[EVENTS] = {false, true, true, true, true, true, false, true, false, false};

// ------------------------------------------

const PROGMEM char* allowedTags[10] = {
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

int loopCount;

// Reads a string from the serial input from the other device
String serialReadStr() {
  String buf = "";
    while (true)
    {
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

// Writes a string to the serial output to the other device
void serialWriteStr(String str) {
  stack.print(str);
  stack.print('\n');
}

// If a given tag is in the list of allowedTags, returns its index, otherwise returns -1
int tagInEvents(char tag[]){
  tag[10] = '\0';
  for (int i = 0; i < EVENTS; i++){
    if (strncmp(allowedTags[i], tag, 10) == 0){
      return i;
    }
  }
  return -1;
}

// Waits for a tag to be scanned and then returns the index of the tag in allowedTags
int waitForEvent(){
  Serial.println(F("Waiting for event"));
  char val = 0;
  char tagValue[11];
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
  
  Serial.println(F("Got event:"));
  Serial.println(tagValue);

  return tagInEvents(tagValue);
}

// Checks if the game has ended and returns which device won if that is the case
int gameEnded(){
  Serial.println(F("Checking if game has ended"));
  // Check if master device has 10 events
  Serial.println(eventList.getSize());
  if (eventList.getSize() == WINNUM) return deviceID;
  
  // Check if slave device has enough events
//  Serial.println(F("Waiting for slave response"));
//  String slaveAnswer = serialReadStr();
//  int slaveSize = slaveAnswer.toInt();
//  Serial.println(slaveAnswer);
//  if (slaveSize == WINNUM) return 1;
  
  // -1 return indicates game has not ended
  return -1;
}

// Sends the slave the winner of the game and displays the correct end screen
void endGame(int winner){
  if (winner == -1) serialWriteStr("noone");
  else if (winner == deviceID){
    winScreen();
    serialWriteStr("lose");
    delay(20000);
  }
  else {
    loseScreen();
    serialWriteStr("win");
    delay(20000);
  }
}

// Tells the master how many events are in the slave's timeline
String alertMaster(){
  Serial.println(F("Sending information to master"));
  // Send master device the number of events in list
  serialWriteStr(String(eventList.getSize()));
  String ending = serialReadStr();
  return ending;
}

// Checks if the event from the timeline is a neighbour of the new event
bool isNearby(int event, int neighbours[]){
  if (event == neighbours[0] || event == neighbours[1]) return true;
  else if (YEARS[event] == YEARS[neighbours[0]] || YEARS[event] == YEARS[neighbours[1]]) return true;
  else return false;
}

// Calculates the correct button to press based on the event from the timeline that was scanned
char buttonToPress(int event, int nearbyEvent){
  if (YEARS[event] < YEARS[nearbyEvent]) return 'a';
  else if (YEARS[event] > YEARS[nearbyEvent]) return 'c';
  else if (YEARS[event] == YEARS[nearbyEvent]) return 'b';
  return 'b';
}

// Runs the Personal Rule scanning minigame
void reorderList(const bool list[EVENTS], int reorder){
  int numToScan = 0;
  for (int i = 0; i < EVENTS; i++){
    if (list[i] && eventList.itemInList(i)) numToScan++;
  }

  if (numToScan == 0) noEvents();
  else{
    while (numToScan > 0){
      scanEvents(numToScan, reorder);
      int event = waitForEvent();
      if (list[event] && eventList.itemInList(event)) numToScan--;
      else if (eventList.itemInList(event)){
        eventList.removeEvent(event);
        removingEvent(reorder);
        waitOnButton('b');
      }
    }
  }
  displayWait();
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);

  Serial.begin(9600);
  rfid.begin(9600);
  stack.begin(57600);

  randomSeed(analogRead(0));

  loopCount = 0;
  // TODO start menu here
}

void loop() {    
  eventList.showList();
  // If it's the first loop
  if (loopCount == 0){
    Serial.println(F("First time loop"));
    
    // Ask user to scan first event
    initialDisplay();
    int event = waitForEvent();
    if (event > -1){
      Serial.println(F("Valid event"));
      eventList.addEvent(event);
      displayEvent(QUESTIONS[event]);
    }
    else{
      Serial.println(F("Invalid event"));
      invalidEvent();
    }

    Serial.println(F("Waiting on button B press"));
    waitOnButton('b');
  }

  // If it's a normal loop
  else{
    Serial.println(F("Normal loop"));
    displayWait();
    // Check if anyone has won the game, if they have, end the game (only do this if master device)
    if (deviceID == 0){
      int winner = gameEnded();
      endGame(winner);
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
    while (eventList.itemInList(event)) event = waitForEvent();    
    if (event > -1){
      displayEvent(QUESTIONS[event]);
    }
    else M5.Lcd.printf("Event does not exist");
    waitOnButton('b');

    // When a new event is detected, ask the player to select an event closest to the new event
    askNearbyEvent();
    int nearbyEvent = waitForEvent();
    while (!eventList.itemInList(nearbyEvent)) nearbyEvent = waitForEvent();
    bool valid = false;
    if (nearbyEvent > -1){
      int neighbours[2];
      eventList.getNeighbours(event, neighbours);
      Serial.println(F("Neighbours:"));
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

    if (deviceID == 0){
      int reorder = random(0, 4);
//      int reorder = 1;
      serialWriteStr(String(reorder));
      switch (reorder) {
        case 0:   
          reorderList(PERSONALRULE, reorder);
          break;
        case 1:
          reorderList(FINANCE, reorder);
          break;
        default:
          Serial.println("No reorder");
          break;
      }
//      serialReadStr();
    }
    else if (deviceID == 1){
      int minigame = serialReadStr().toInt();
      switch (minigame) {
        case 0:
          reorderList(PERSONALRULE, minigame);
          break;
        case 1:
          reorderList(FINANCE, minigame);
        default:
          Serial.println("No reorder");
          break;
      }
      serialWriteStr("finished");
    }
    // With a random probability, choose a new order for the list (only do this if master device)
    //    Wait for all events in the list to be scanned through in the new order
    //    Events that are scanned through wrong now change and are no longer part of the event list
    // Receive whether master device has decided to reorder the list (only do this if slave device)
    //    Wait for all events in the list to be scanned through in the new order
    //    Events that are scanned through wrong now change and are no longer part of the event list
  }
  
  loopCount++;
}
