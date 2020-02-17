#define _EventList_h

class EventList
{
  public:
    EventList(int listSize);
    void initEvents();
    bool addEvent(int event);
    bool removeEvent(int event);
    int getSize();
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

void EventList::initEvents(){
  for (int i = 0; i < maxSize; i++){
    events[i] = -1;
  }
}

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
