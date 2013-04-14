/* --------------------------------------------------------------------------
 * SimpleOpenNI DepthImage Test
 * --------------------------------------------------------------------------
 * Processing Wrapper for the OpenNI/Kinect library
 * http://code.google.com/p/simple-openni
 * --------------------------------------------------------------------------
 * prog:  Max Rheiner / Interaction Design / zhdk / http://iad.zhdk.ch/
 * date:  02/16/2011 (m/d/y)
 * ----------------------------------------------------------------------------
 */
import processing.serial.*;

Serial myPort;

int S_HEIGHT = 384;
PImage receiptImage;
PGraphics tempDepth;

int BACKGROUND_DEPTH = 30; 
int MAX_DEPTH = 10;
float MIN_DEPTH_FILTER = 100;

int PRINT_SECTION = 50;
int currPrintIndex = 0;
boolean printerReady = true;
boolean doPrint = false;
boolean first = true;

void setup()
{
  size(384, 1000);
  //println(Serial.list());
  myPort = new Serial(this, "/dev/tty.usbmodemfd121", 9600);
  receiptImage = loadImage("shit.bmp");
  println("receipt size: " + receiptImage.width + ":" + receiptImage.height);
}

void draw()
{
  if (doPrint){
    handlePrinting();
  }
  set(0, 0, receiptImage);
}

void handlePrinting(){
  if (!printerReady){
    return;
  }
  int imageBits = S_HEIGHT*PRINT_SECTION;
  byte[] bytes = new byte[imageBits/8];
  
  //for each pixel, convert it to a bitmap
  byte currByte = 0;
  int i = 0;
  int stopPrintIndex = currPrintIndex + imageBits;
  println(currPrintIndex + " -> " + stopPrintIndex + " : " + (stopPrintIndex - currPrintIndex));
  for(; currPrintIndex < stopPrintIndex; currPrintIndex++){
    currByte <<= 1;
    if (currPrintIndex < receiptImage.pixels.length){
      currByte |= (red(receiptImage.pixels[currPrintIndex]) > 120 ? 0 : 1);
      receiptImage.pixels[currPrintIndex] = ((currByte & 1) == 1 ? 0 : 255);
    }
    if (currPrintIndex%8 == 7){
      //we have filled a character, so lets store it and reset
      bytes[i++] = currByte;
      currByte = 0;
    } else if (currPrintIndex == stopPrintIndex - 1){
      //this is the last iteration of the loop, so we should save the current character
      bytes[i++] = currByte;
    }
  }
  
  myPort.write(bytes);
  if (currPrintIndex >= receiptImage.pixels.length){
    currPrintIndex = 0;
    exit();
    println("done printing full image");
  } else {
    println("done printing image section");
  }
  println(currPrintIndex + " -> " + stopPrintIndex + " : " + (stopPrintIndex - currPrintIndex));
  
  printerReady = false;
}

void keyPressed(){
  if (key == ' '){
    doPrint = true;
  }
}

void serialEvent(Serial myPort) {
  if (myPort.read() == 1){
    printerReady = true;
    println("printer ready");
  }
}
