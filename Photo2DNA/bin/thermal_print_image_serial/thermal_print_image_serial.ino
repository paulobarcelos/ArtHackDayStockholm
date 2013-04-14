#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"
#include "Stream.h"

/**
 * Declare which pins to communicate to the printer over
 */
int printer_RX_Pin = 5; // green wire
int printer_TX_Pin = 6; // yellow wire
const int maxWidth = 384;
const int arrayWidth = 384/8;
uint8_t line[arrayWidth];
const int charsPerLine = 32;
const int maxLines = 3;
const int maxChars = charsPerLine*maxLines;
char str[maxChars];
int newlineTimeout = 5000;
boolean waitForNewline = false;
int newlineTime;
boolean printUpsidedown = true;
const int arrayHeight = 5;
const int arraySize = maxWidth*arrayHeight;
uint8_t holder[128*128/8];
boolean useFlip = true;
int currIndex = 0;

/**
 * Initialize the thermal printer
 */
Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

void setup(){
  printer.begin();
  Serial.begin(9600);
  //make the line array all white
  for(int i = 0; i < arrayWidth; i++){
    line[i] = 0x00;
  }
  if (printUpsidedown){
    printer.upsideDownOn();
  }
  printer.println("Hello");
  printer.feed(2);
}

void loop(){
  if(Serial.available() > 0){
    printer.printBitmap(384, 50, &Serial);
    //printer.feed(2);
    delay(1000);
    printer.flush();
    printer.reset();
    delay(1000);
    Serial.write(1);
    /*
    holder[currIndex] = Serial.read();
    //Serial.write(holder[currIndex]);
    currIndex++;
    if (currIndex == 128*128/8){
      printer.printBitmap(128, 128, holder, false);
      currIndex = 0;
      for(int i = 0; i < 128*128/8; i++){
        Serial.write(holder[i]);
      }
      Serial.flush();
    }*/
//    printer.printBitmap(100,100,&Serial);
    /*
    if (useFlip){
      flip[currIndex++] = Serial.read();
    } else {
      flop[currIndex++] = Serial.read();
    }
    if (currIndex >= arraySize){
      currIndex = 0;
      printer.printBitmap(maxWidth, arrayHeight, (useFlip ? flip : flop), false);
      useFlip = !useFlip;
    }
    */
  }
}
