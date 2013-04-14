#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMacamPs3Eye.h"
#include "ofxPatches.h"
#include "ofSerial.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void takePicture();
	
	void onAutoGainAndShutterChange(bool & value);
	void onGainChange(float & value);
	void onShutterChange(float & value);
	void onGammaChange(float & value);
	void onBrightnessChange(float & value);
	void onContrastChange(float & value);
	void onHueChange(float & value);
	void onLedChange(bool & value);
	void onFlickerChange(int & value);
	void onWhiteBalanceChange(int & value);
	
	void keyReleased(int key);

    // Arduino
    void printBitmap(int w, int h, const uint8_t *bitmap);
    void setupArduino();
    void updateArduino();
    ofSerial arduino;
	bool arduinoReady;

    // GUI
	ofxPanel gui;
	ofxMacamPs3Eye ps3eye;
	ofFbo image;
	ofFbo imageSmall;
	ofImage display;
	ofImage bmp;
	
	vector<ofImage> bases;
	
	int stage; // 0 - waiting 1 - countdown - 2 - transtion to print - 3 printing - 4 transition to waiting
	vector<float> stageDuration;
	float playHead = 0;
	
	bool debug;
	
	ofFbo printImage;
};
