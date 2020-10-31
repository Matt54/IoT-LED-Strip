// led library
#include <FastLED.h>
#include <math.h>

// I2C stuff
#include <Wire.h>
#define SLAVE_ADDRESS 0x08

// led library definitions
#define LED_TYPE WS2812
#define NUM_LEDS 433
#define DATA_PIN 6

#define powerPin 52
bool wasPressed = false;

#include "MattsLedStrip.h"

MattsLedStrip *ptr;



void setup() {
        ptr = new MattsLedStrip(433);
        Serial.begin(9600);
        FastLED.addLeds<NEOPIXEL, DATA_PIN>(ptr->leds, 433);
        set_max_power_in_volts_and_milliamps(5, 8000);
        FastLED.clear();

        Wire.begin(SLAVE_ADDRESS);
        Wire.onReceive(receiveData);
        Wire.onRequest(sendData);

//ptr->TurnOff();
        ptr->TurnOn();

// Define pin #12 as input and activate the internal pull-up resistor
        pinMode(powerPin, INPUT_PULLUP);
}

void receiveData(int bytecount)
{
        byte register_num = 0;
        byte data_to_echo = 0;
        for (byte i = 0; i < bytecount; i++) {
                if(i == 0) register_num = Wire.read();
                else data_to_echo = Wire.read();
        }
        AdjustSettings(register_num,data_to_echo);
}

void AdjustSettings(int r,int val)
{
        switch (r)
        {
        case 0:
                ptr->timerInterval = val;
                Serial.println("In Register 0 :");
                Serial.println(val);
                Serial.println("");
                break;
        case 1:
                ptr->mod = val;
                ptr->CalculateMaxBrightness();
                Serial.println("In Register 1 :");
                Serial.println(val);
                Serial.println("");
                break;
        case 2:
                ptr->program_number = val;
                Serial.println("In Register 2 :");
                Serial.println(val);
                Serial.println("");
                ptr->ProgramSetup();
                break;
        case 3:
                ptr->SetBrightness(val);
                Serial.println("In Register 3 :");
                Serial.println(val);
                Serial.println("");
                break;
        case 4:
                ptr->SetAudioReactive(val);
                Serial.println("In Register 4 :");
                Serial.println(val);
                Serial.println("");
                break;
        }
}

void sendData()
{
//Wire.write(data_to_echo);
}

/*
   int getSoundLevel()
   {
   return analogRead(0);
   }
 */

void checkSoundLevel(){
        int val = analogRead(0);
        Serial.print("Sound=");
        Serial.println(val,DEC);//print the sound value to serial
}

/*Checks toggle power button*/
void checkPowerPin(){
        byte buttonValue = digitalRead(powerPin);
        if (buttonValue == LOW) wasPressed = true;
        else {
                if(wasPressed) {
                        ptr->TogglePower();
                        wasPressed = false;
                }
        }
}

void loop() {
        if(ptr->isOn) {
                if(ptr->isAudioReactive) ptr->AudioLevelCheck();
                ptr->CheckTime();
        }
        checkPowerPin();
//Serial.println(ptr->thresholdCount);
//Serial.println(ptr->isAudioReactive);
//checkSoundLevel();
}
