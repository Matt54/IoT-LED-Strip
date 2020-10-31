#pragma once
#ifndef _PUSHBUTTON_H_
#define _PUSHBUTTON_H_

#include <arduino.h>

struct PushButton
{
    int pinNumber;
    bool wasPressed = false;
    typedef void (*VoidFunc) (); // Generic void funtion
    VoidFunc func;

    PushButton();
    PushButton(int _pinNumber, VoidFunc _func);
    void checkState();
};

#endif /* PUSHBUTTON_H */
