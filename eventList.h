#define _EventList_h

// A class for keeping track of events in a player's timeline
// Would be better implemented as a linked list, but the size of arrays used for this program means this is an ok solution
class EventList
{
  public:
    EventList(int listSize);
    void initEvents();
    bool addEvent(int event);
    bool removeEvent(int event);
    int getSize();
    void getNeighbours(int event, int neighbours[]);
  private:
    int events[];
    int currentPos;
    int maxSize;
};

EventList::EventList(int listSize){
  initEvents();
  currentPos = 0;
  maxSize = listSize;
}

// Initialises all events to -1
void EventList::initEvents(){
  for (int i = 0; i < maxSize; i++){
    events[i] = -1;
  }
}

// Adds an event into the right place in the array to keep order
bool EventList::addEvent(int event){
  if (currentPos < maxSize-1){
    int pos = 0;
    while (event < events[pos]){
      pos++;
    }
  
    for (int i = currentPos; i > pos; i--){
      events[i] = events[i-1];
    }
    events[pos] = event;
    return true;
  }
  return false;
}

// Removes an event from the array
bool EventList::removeEvent(int event){
  for (int i = 0; i < currentPos; i++){
    if (events[i] == event){
      for (int j = i+1; j < currentPos; j++){
        events[j-1] = events[j];
      }
      currentPos--;
      events[currentPos] = -1;
      return true;
    }
  }
  return false;
}

int EventList::getSize(){
  return currentPos;
}

// Gets the neighbours either side of the event
void EventList::getNeighbours(int event, int neighbours[]){
  if (currentPos == 1){
    neighbours[0] = events[0];
    neighbours[1] = events[1];
  }
  else{
    for (int i = 0; i < currentPos; i++){
      if (event < events[i] && i == 0){
        neighbours[0] = events[i];
        neighbours[1] = events[i];
      }
      else if (i == currentPos - 1){
        neighbours[0] = events[i];
        neighbours[1] = events[i];
      }
      else if (event < events[i]){
        neighbours[0] = events[i-1];
        neighbours[1] = events[i];
      }
    }
  }
}