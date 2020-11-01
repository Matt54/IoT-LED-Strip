#pragma once
#ifndef _LEDCONTROLLER_H_
#define _LEDCONTROLLER_H_

#include <FastLED.h>
#include <arduino.h>

class LEDController{
private:
public:

  /*
  MAIN TIME KEEPING
  LED steps are determined by:
  timeNow - timePast > timerInterval
  */
 unsigned long timeNow;
 unsigned long timePast;
 uint8_t timerInterval = 50;

 int counter = 0; // needs to be an int because it can go up to NUM_LEDS

 byte maxPatterns = 5;

 CRGB leds[433];

 CHSV chsv; // Color (Hue, Saturation, Value)
 // Hue - color value from 0 - 255
 // Saturation - Colorfulness
 // Value - brightness

 uint8_t maxB = 0;
 uint8_t rememberB = 0;

 bool fadeLocked = false;
 int ledFading = 0;
 int ledFading2 = 0;
 int ledFading3 = 0;
 int ledFading4 = 0;
 int fadeValue = 0;

 uint8_t program_number = 2;
 uint8_t mod = 25;
 bool isBackwards = false;
 bool autoColor = true;

 unsigned long bumpNow;
 unsigned long bumpPast;
 int bumpInterval = 100;

 bool isAudioReactive = false;
 bool bumpColor = false;
 byte fadeFactor = 20;
 byte bumpNumber = 16;

 bool isFading = true;
 uint8_t thresholdCount = 0;
 bool transition = false;

 bool isOn = true;

 int numLEDS;

 LEDController(int num);
 void CalculateMaxBrightness();
 void CheckTime();
 void StepStrip();
 void ProgramSetup();
 void RandomRed();
 void ModStep(bool hasTail);
 void WriteLedTrail(int ledNum, byte dist);
 void PixelRun();
 void BounceCount();
 void AllOn();
 void Wave();
 void RandomColor();
 void ChangeHue();
 void ClearStrip();
 void AudioLevelCheck();
 int  GetSoundLevel();
 void SetBrightness(int val);
 void SetAudioReactive(int val);
 void AudioFadeOut();
 void ResetMaxB();
 void RandomPattern();
 void RandomProperties();
 void TurnOff();
 void TurnOn();
 void TogglePower();
 void SetPower(int val);
 void IncrementProgramNumber();
};

#endif /* LEDCONTROLLER_H */
