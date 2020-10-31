#include "LEDController.h"
#include <math.h>

LEDController::LEDController(int num){
        leds[num];
        CalculateMaxBrightness();
        timeNow = millis();
        timePast = millis();
        numLEDS = num;
        chsv.hue = 0;
        chsv.saturation = 255;
}

/*
   This is specific to my Led strip which has A LOT of Leds (433)
   - I use this to limit the brightness when there are lots of them on,
    otherwise there are glitches that occur in the strip
   - I determined the function through testing and curve fitting
 */
void LEDController::CalculateMaxBrightness(){

        //double y = 3.3972 * pow(mod,2) - 8.9219 * mod + 38.476;
        double y = 22.083 * mod + 88;
        if(y > 255) y = 255;
        maxB = round(y);
        rememberB = maxB;
        chsv.value = maxB;
}

/*
SetBrightness: Convenient function for setting the overall brightness
THIS FUNCTION IS TYPICALLY OVERRIDEN DUE TO THE STRIP CALCULATING BRIGHTNESS
*/
void LEDController::SetBrightness(int val){
        maxB = val;
        rememberB = maxB;
        chsv.value = maxB;
}

/*
CheckTime gate keeps the stepping functions so that they only occur
at the rate of time timeInterval variable
*/
void LEDController::CheckTime(){
        timeNow = millis();
        if( (timeNow - timePast) > timerInterval) {
                timePast = timeNow;
                StepStrip();
        }
}

/*
StepStrip routes the program based on the program_number
*/
void LEDController::StepStrip(){
        switch(program_number) {
        case 1:
                ModStep(false);
                //RandomRed();
                break;
        case 2:
                ModStep(true);
                break;
        case 3:
                PixelRun();
                break;
        case 4:
                AllOn();
                break;
        case 5:
                Wave();
                break;
        }
}

/*
RandomPattern: when called, a random pattern is selected
*/
void LEDController::RandomPattern(){
        program_number = random8(1,maxPatterns + 1);
        RandomProperties();
        ProgramSetup();
}

/*
RandomProperties: when called, the program currently selected will have its properties randomized
*/
void LEDController::RandomProperties(){
        if(program_number == 1) mod = random8(1,100);
        if(program_number == 2) mod = random8(1,9);
}

void LEDController::IncrementProgramNumber(){
  program_number = program_number + 1;
  if(program_number > maxPatterns){
    program_number = 1;
  }
  ClearStrip();
  ProgramSetup();
}

/*
Called during program change to adjust settings.
*/
void LEDController::ProgramSetup(){
        switch(program_number) {
        case 1:
                bumpColor = false;
                fadeFactor = 5;
                CalculateMaxBrightness();
                break;
        case 2:
                bumpColor = false;
                CalculateMaxBrightness();
                break;
        case 3:
                bumpColor = false;
                SetBrightness(255);
                PixelRun();
                break;
        case 4:
                SetBrightness(120); //Override brightness
                bumpColor = true;
                break;
        case 5:
                SetBrightness(120);
                bumpColor = true;
                break;
        }
}


/*
WIP
*/
void LEDController::RandomRed(){

        if(chsv.value >= fadeValue)
        {
                //make it white
                chsv.saturation = 0;
                hsv2rgb_rainbow(chsv, leds[ledFading]);
                hsv2rgb_rainbow(chsv, leds[ledFading2]);
                hsv2rgb_rainbow(chsv, leds[ledFading3]);
                hsv2rgb_rainbow(chsv, leds[ledFading4]);
                FastLED.show();
                chsv.saturation = 255;

                fadeValue = random8 (0,155);
                int ledToKill = random16 (0,433);
                chsv.value = 0;
                hsv2rgb_rainbow(chsv, leds[ledToKill]);
                ledToKill = random16 (0,433);
                hsv2rgb_rainbow(chsv, leds[ledToKill]);
                ledToKill = random16 (0,433);
                hsv2rgb_rainbow(chsv, leds[ledToKill]);
                ledToKill = random16 (0,433);
                hsv2rgb_rainbow(chsv, leds[ledToKill]);
                chsv.value = 0;


                ledFading = random16 (0,433);
                ledFading2 = random16 (0,433);
                ledFading3 = random16 (0,433);
                ledFading4 = random16 (0,433);
        }
        else
        {
                chsv.hue = random8(0,255);
                hsv2rgb_rainbow(chsv, leds[ledFading]);
                hsv2rgb_rainbow(chsv, leds[ledFading2]);
                hsv2rgb_rainbow(chsv, leds[ledFading3]);
                hsv2rgb_rainbow(chsv, leds[ledFading4]);
                chsv.value = chsv.value + 3;
                FastLED.show();
        }
}


/*
ModStep is a stepping function that spaces LedS according to a modulus
variable. The spaced LedS move from beginning to end and then restart.
INPUT: hasTail (bool) - creates a tail after the LedS (meteor effect)
*/
void LEDController::ModStep(bool hasTail){
        if (autoColor) ChangeHue();
        FastLED.clear();
        for(int i = 0; i < numLEDS - counter; i++)
        {
                if(i % mod == 0)
                {
                        // Always write the main Led
                        chsv.value = maxB;
                        hsv2rgb_rainbow(chsv, leds[i + counter]);
                        /*
                           If hasTail, we need to loop through the rest of the LedS between
                           our main Led and the previous Led and light those to dimmer values
                           that we calculate in WriteLedTrail
                         */
                        if(hasTail) for(int j = 1; j < mod; j = j + 1) WriteLedTrail(i + counter - j, j);
                }
        }
        FastLED.show();
        counter++;
        if(counter > mod - 1) counter = 0;
}

void LEDController::WriteLedTrail(int ledNum, byte dist){
        chsv.value = (float) maxB / (dist + 1);
        if (ledNum >= 0) hsv2rgb_rainbow(chsv, leds[ledNum]);
}

/*
   PixelRun is a stepping function that has one Led pixel move through the
   light strip from beginning to end, and then back.
 */
void LEDController::PixelRun(){
        if (autoColor) ChangeHue();
        chsv.value = maxB;
        hsv2rgb_rainbow(chsv, leds[counter]);
        FastLED.show();
        chsv.value = 0;
        hsv2rgb_rainbow(chsv, leds[counter]);
        BounceCount();
}

void LEDController::BounceCount(){
        if(!isBackwards)
        {
                counter++;
                if(counter > numLEDS)
                {
                        counter--;
                        isBackwards = true;
                }
        }
        else
        {
                counter--;
                if(counter < 0)
                {
                        counter++;
                        isBackwards = false;
                }
        }
}

void LEDController::AllOn(){
        chsv.value = maxB;
        for(int i = 0; i < numLEDS; i++) hsv2rgb_rainbow(chsv, leds[i]);
        FastLED.show();
}

void LEDController::Wave(){
        counter++;
        if(counter >= numLEDS) counter = 0;
        chsv.value = maxB;
        for(int i = 0; i < numLEDS; i++)
        {
                chsv.hue = map(i+counter, 0, numLEDS, 0, 255);
                hsv2rgb_rainbow(chsv, leds[i]);
        }
        FastLED.show();
}

void LEDController::RandomColor(){
        if(bumpColor) chsv.hue = random8(0,255);
        thresholdCount = 0;
}

void LEDController::ChangeHue(){
        if(chsv.hue < 255) chsv.hue = chsv.hue + 1;
        else chsv.hue = 0;
}

void LEDController::ClearStrip(){
        FastLED.clear();
        FastLED.show();
}

/*AudioLevelCheck - Manages the brightness of the strip based on sound levels*/
void LEDController::AudioLevelCheck(){
    bumpNow = millis();
    //if( (bumpNow - bumpPast) > bumpInterval)
    //{
            if(GetSoundLevel() > 200) {// && !transition){
                    bumpPast= millis();
                    ResetMaxB();
                    isFading = true;
                    /*
                    thresholdCount++;

                    if(thresholdCount > bumpNumber) {
                            ClearStrip();
                            RandomColor();
                            RandomPattern();
                            transition = false;
                          }
                          */
            }
            else if (!isFading) {}
            else AudioFadeOut();
    //}
}

void LEDController::AudioFadeOut(){
        if(maxB >= fadeFactor)
        {
                maxB = maxB - maxB / fadeFactor;
        }
        else {
                if(transition == true) {
                        /*
                           ClearStrip();
                           RandomColor();
                           RandomPattern();
                           transition = false;
                         */
                }
                isFading = false;
        }
}

void LEDController::ResetMaxB(){
        maxB = rememberB;
}

int LEDController::GetSoundLevel(){
        return analogRead(0);
}

void LEDController::SetAudioReactive(int val){
        if(val > 0) {
                isAudioReactive = true;
                isFading = true;
        }
        else isAudioReactive = false;
}


void LEDController::TurnOff(){
        isOn = false;
        ClearStrip();
}

void LEDController::TurnOn(){
        isOn = true;
}

void LEDController::TogglePower(){
        if(isOn) TurnOff();
        else TurnOn();
}

void LEDController::SetPower(int val){
        if(val > 0) TurnOn();
        else {
          TurnOff();
          ClearStrip();
        }
}
