bool buttonA(){
  M5.update();
  while (!M5.BtnA.wasReleased()){
    if (M5.BtnB.wasReleased()) return false;
    else if (M5.BtnC.wasReleased()) return false;
    else M5.update();
  }
  return true;
}

bool buttonB(){
  M5.update();
  while (!M5.BtnB.wasReleased()){
    if (M5.BtnA.wasReleased()) return false;
    else if (M5.BtnC.wasReleased()) return false;
    else M5.update();
  }
  return true;
}

bool buttonC(){
  M5.update();
  while (!M5.BtnC.wasReleased()){
    if (M5.BtnA.wasReleased()) return false;
    else if (M5.BtnB.wasReleased()) return false;
    else M5.update();
  }
  return true;
} 

void waitOnButton(char button){
  bool finished = false;
  if (button == 'a' || button == 'A'){
    while (!finished) finished = buttonA();
  }
  else if (button == 'b' || button == 'B'){
    while (!finished) finished = buttonB();
  }
  else if (button == 'c' || button == 'C'){
    while (!finished) finished = buttonC();
  }
}
