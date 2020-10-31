#include "PushButton.h"

PushButton::PushButton(){}

PushButton::PushButton(int _pinNumber, VoidFunc _func){
  pinNumber = _pinNumber;
  func = _func;
}

void PushButton::checkState(){
  byte buttonValue = digitalRead(pinNumber);
  if (buttonValue == LOW) wasPressed = true;
  else {
          if(wasPressed) {
                  func();
                  wasPressed = false;
          }
  }
}
