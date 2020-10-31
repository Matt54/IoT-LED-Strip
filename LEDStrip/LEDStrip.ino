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
#define programPin 50

#include "LEDController.h"
#include "PushButton.h"

LEDController * controller;
PushButton * powerButton, * programButton;

int dataToWrite = 0;

void setup() {
        controller = new LEDController(433);
        powerButton = new PushButton(powerPin, togglePower);
        programButton = new PushButton(programPin, incrementProgramNumber);

        Serial.begin(9600);
        FastLED.addLeds<NEOPIXEL, DATA_PIN>(controller->leds, 433);
        set_max_power_in_volts_and_milliamps(5, 8000);
        FastLED.clear();

        Wire.begin(SLAVE_ADDRESS);
        Wire.onReceive(receiveData);
        Wire.onRequest(sendData);

        //DEFAULT POWER SETTING:
        //controller->TurnOff();
        controller->TurnOn();

        // Define pin #52 and 50 as input
        // activate the internal pull-up resistor
        pinMode(powerPin, INPUT_PULLUP);
        pinMode(programPin, INPUT_PULLUP);
}

/*
This is used to receive data from RPi
*/
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
                controller->timerInterval = val;
                Serial.println("In Register 0 :");
                Serial.println(val);
                Serial.println("");
                break;
        case 1:
                controller->mod = val;
                controller->CalculateMaxBrightness();
                Serial.println("In Register 1 :");
                Serial.println(val);
                Serial.println("");
                break;
        case 2:
                controller->program_number = val;
                Serial.println("In Register 2 :");
                Serial.println(val);
                Serial.println("");
                controller->ProgramSetup();
                break;
        case 3:
                controller->SetBrightness(val);
                Serial.println("In Register 3 :");
                Serial.println(val);
                Serial.println("");
                break;
        case 4:
                controller->SetAudioReactive(val);
                Serial.println("In Register 4 :");
                Serial.println(val);
                Serial.println("");
                break;

        case 5:
                //sendData();
                Serial.println("In Register 5 :");
                Serial.println(val);
                Serial.println("");
                dataToWrite = val;
                //Wire.write(12);
                break;
      }
}

/*
This is used to send data from Arduino to RPi
*/
void sendData()
{
  switch (dataToWrite)
  {
        case 0:
            Wire.write(controller->timerInterval);
            break;
        case 1:
            Wire.write(controller->mod);
            break;
        case 2:
            Wire.write(controller->program_number);
            break;
        case 3:
            Wire.write(controller->maxB);
            break;
        case 4:
            if(controller->isAudioReactive){
              Wire.write(1);
            }
            else{
              Wire.write(0);
            }
            break;
  }
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

void loop() {
        if(controller->isOn) {
                if(controller->isAudioReactive) controller->AudioLevelCheck();
                controller->CheckTime();
        }
        powerButton->checkState();
        programButton->checkState();
        //checkPowerPin();
        //Serial.println(controller->thresholdCount);
        //Serial.println(controller->isAudioReactive);
        checkSoundLevel();
}

void togglePower(){
  controller->TogglePower();
}

void incrementProgramNumber(){
  controller->IncrementProgramNumber();
}
