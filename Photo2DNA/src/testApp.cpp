#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetWindowShape(900, 480);
	
	ps3eye.initGrabber(620, 480);
	
	gui.setup("PS3Eye");
	gui.setPosition(660,20);
	
	ofxToggle * autoGainAndShutter = new ofxToggle("Auto Gain & Shutter", false);
	autoGainAndShutter->addListener(this, &testApp::onAutoGainAndShutterChange);
	gui.add(autoGainAndShutter);
	
	ofxFloatSlider * gain = new ofxFloatSlider("Gain", 0.5, 0.0, 1.0);
	gain->addListener(this, &testApp::onGainChange);
	gui.add(gain);
	
	ofxFloatSlider * shutter = new ofxFloatSlider("Shutter", 0.5, 0.0, 1.0);
	shutter->addListener(this, &testApp::onShutterChange);
	gui.add(shutter);
	
	ofxFloatSlider * gamma = new ofxFloatSlider("Gamma", 0.5, 0.0, 1.0);
	gamma->addListener(this, &testApp::onGammaChange);
	gui.add(gamma);
	
	ofxFloatSlider * brightness = new ofxFloatSlider("Brightness", 0.5, 0.0, 1.0);
	brightness->addListener(this, &testApp::onBrightnessChange);
	gui.add(brightness);
	
	ofxFloatSlider * contrast = new ofxFloatSlider("Contrast", 0.5, 0.0, 1.0);
	contrast->addListener(this, &testApp::onContrastChange);
	gui.add(contrast);
	
	ofxFloatSlider * hue = new ofxFloatSlider("Hue", 0.5, 0.0, 1.0);
	hue->addListener(this, &testApp::onHueChange);
	gui.add(hue);
	
	ofxIntSlider * flicker = new ofxIntSlider("Flicker Type", 0, 0, 2);
	flicker->addListener(this, &testApp::onFlickerChange);
	gui.add(flicker);
	
	ofxIntSlider * wb = new ofxIntSlider("White Balance Mode", 4, 1, 4);
	wb->addListener(this, &testApp::onFlickerChange);
	gui.add(wb);
	
	ofxToggle * led = new ofxToggle("LED", true);
	led->addListener(this, &testApp::onLedChange);
	gui.add(led);
	
	gui.loadFromFile("settings.xml");
	
	// Load initial values
	onAutoGainAndShutterChange(gui.getToggle("Auto Gain & Shutter"));
	onGainChange(gui.getFloatSlider("Gain"));
	onShutterChange(gui.getFloatSlider("Shutter"));
	onGammaChange(gui.getFloatSlider("Gamma"));
	onBrightnessChange(gui.getFloatSlider("Brightness"));
	onContrastChange(gui.getFloatSlider("Contrast"));
	onHueChange(gui.getFloatSlider("Hue"));
	onLedChange(gui.getToggle("LED"));
	onFlickerChange(gui.getIntSlider("Flicker"));
	onWhiteBalanceChange(gui.getIntSlider("White Balance"));
	
	image.allocate(480, 480);
	imageSmall.allocate(48, 48);
	
	display.allocate(image.getWidth(), image.getHeight(), OF_IMAGE_GRAYSCALE);
	bmp.allocate(imageSmall.getWidth(), imageSmall.getHeight(), OF_IMAGE_GRAYSCALE);
	
	ofImage imageA;
	ofImage imageG;
	ofImage imageC;
	ofImage imageT;
	imageA.loadImage("A.png");
	imageG.loadImage("G.png");
	imageC.loadImage("C.png");
	imageT.loadImage("T.png");
	bases.push_back(imageA);
	bases.push_back(imageG);
	bases.push_back(imageT);
	bases.push_back(imageC);

	debug = false;
	
	stage = 0;
	stageDuration.push_back(0);
	stageDuration.push_back(3);
	stageDuration.push_back(5);
	stageDuration.push_back(20);
	stageDuration.push_back(5);
	
	ofSetWindowPosition(0, 0);
	ofSetFullscreen(true);
	/*
	 x=0.f;
	 y=500.f;
	 r=20.f;
	 tweener_1.setup(1.f,				// @duration: tween duration in seconds  - @default: 1.f
	 0.5f,				// @delay: delay to start the tween (will also apply to repat BACK_AND_FORTH and LOOP) - @default: 0.f
	 &(Sine::easeOut),	// @easingFunc: tween equasion (All PennerEasing equasions are available)  - @default: &(Linear::easeNone)
	 BACK_AND_FORTH,		// @repeat: NONE, LOOP or BACK_AND_FORTH - @default: NONE
	 &(Sine::easeIn));	// @backEasingFunc: if specified, will be used on the way back (only for repeat BACK_AND_FORTH) - @default: NULL
	 
	 tweener_1.addTween(&x,				// @var: pointer to the variable we want to tween
	 500);*/
}

//--------------------------------------------------------------
void testApp::update(){
	ps3eye.update();
	if(ps3eye.isFrameNew()){
		image.begin();
			ofPushMatrix();
				ofScale(image.getHeight()/ps3eye.getHeight(), image.getHeight()/ps3eye.getHeight());
				ps3eye.draw(-(ps3eye.getWidth() * image.getHeight()/ps3eye.getHeight() - image.getWidth())/2, 0);
			ofPopMatrix();
		image.end();
		
		imageSmall.begin();
			ofPushMatrix();
				ofScale(imageSmall.getHeight()/ps3eye.getHeight(), imageSmall.getHeight()/ps3eye.getHeight());
				ps3eye.draw(-(ps3eye.getWidth() * imageSmall.getHeight()/ps3eye.getHeight() - imageSmall.getWidth())/2, 0);
			ofPopMatrix();
		imageSmall.end();
		
		unsigned char * greyPixels =  display.getPixels();
		ofPixels pixels;
		pixels.allocate(display.getWidth(), display.getHeight(), 3);
		image.readToPixels(pixels);
		ofPixels channel = pixels.getChannel(0);
		
		for (int i = 0; i < channel.size(); i++) {
			greyPixels[i] = channel[i];
		}
		display.update();

	}
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofClear(0, 0, 0);
	
	ofPoint position(0,0);
	
	switch (stage) {
		case 0:
		case 1:
			position = ofPoint(ofGetWidth()/2 - ofGetHeight()/2, 0);
			break;
			
		default:
			break;
	}
	
	display.draw(position, ofGetHeight(), ofGetHeight());
	
	if(debug){
		gui.draw();
	}
	
	
	
}

void testApp::takePicture(){
	unsigned char * greyPixels =  bmp.getPixels();
	ofPixels pixels;
	pixels.allocate(bmp.getWidth(), bmp.getHeight(), 3);
	imageSmall.readToPixels(pixels);
	ofPixels channel = pixels.getChannel(0);
	
	for (int i = 0; i < channel.size(); i++) {
		greyPixels[i] = channel[i];
	}
	bmp.update();
	string timestamp = ofToString(ofGetUnixTime());
	
	string filename = timestamp + ".bmp";
	bmp.saveImage(filename);
	
	string script = ofFilePath::getEnclosingDirectory(ofFilePath::getCurrentExePath(), false) + "../Resources/Bits2DNA.pl";
	string command = "perl " + script + " \"" + ofToDataPath(filename) + "\" \"" + ofToDataPath(filename)+".dna" + "\"";
	system(command.c_str());
	
	ofBuffer buffer = ofBufferFromFile(filename + ".dna" );
	vector<int> data;
	while (!buffer.isLastLine()) {
		string line = ofToUpper(buffer.getNextLine());
		for (int i = 0; i < line.size(); i++) {
			char letter = line[i];
			switch (letter) {
				case 'A':
					data.push_back(0);
					break;
				case 'G':
					data.push_back(1);
					break;
				case 'T':
					data.push_back(2);
					break;
				case 'C':
					data.push_back(3);
					break;
			}
		}
	}
	ofFbo printImage;
	printImage.allocate(384, 50 + 384 + (int) (data.size() / (384 / 4)) * 4);
	printImage.begin();
	ofClear(255,255,255);
	ofSetColor(0, 0, 0, 255);
	ofDrawBitmapString("ART HACK DAY STOCKHOLM 2013", 0, 14);
	ofDrawBitmapString(timestamp + ".BMP.DNA", 0, 30);
	ofSetColor(255, 255, 255, 255);
	image.draw(20, 60, 344,344);
	
	int row = 0;
	int col = 0;
	int maxCols = 384/4;
	ofPushMatrix();
	ofTranslate(0, 50 + 384);
	
	for (int i = 0; i < data.size(); i++) {
		ofSetColor(255,255,255);
		bases[data[i]].draw(col*4, row*4, 4,4);
		col++;
		if(col >= maxCols){
			col = 0;
			row++;
		}
	}
	ofPopMatrix();
	
	printImage.end();
	
	
	ofImage printBmp;
	printBmp.allocate(printImage.getWidth(), printImage.getHeight(), OF_IMAGE_GRAYSCALE);
	unsigned char * printPixels =  printBmp.getPixels();
	ofPixels printFBOpixels;
	printFBOpixels.allocate(printImage.getWidth(), printImage.getHeight(), 3);
	printImage.readToPixels(printFBOpixels);
	ofPixels printChannel = printFBOpixels.getChannel(0);
	
	for (int i = 0; i < printChannel.size(); i++) {
		printPixels[i] = printChannel[i];
	}
	printBmp.update();
	
	printBmp.saveImage(filename+".print.bmp");
}
void testApp::keyReleased(int key){
	if (key == 'd') debug = !debug;
	if (key == 'f') ofToggleFullscreen();
	
	if(key == ' '){
		takePicture();
	}
}
//--------------------------------------------------------------
void testApp::onAutoGainAndShutterChange(bool & value){
	ps3eye.setAutoGainAndShutter(value);
}

//--------------------------------------------------------------
void testApp::onGainChange(float & value){
	// Only set if auto gain & shutter is off
	if(!gui.getToggle("Auto Gain & Shutter")){
		ps3eye.setGain(value);
	}
}

//--------------------------------------------------------------
void testApp::onShutterChange(float & value){
	// Only set if auto gain & shutter is off
	if(!gui.getToggle("Auto Gain & Shutter")){
		ps3eye.setShutter(value);
	}
}

//--------------------------------------------------------------
void testApp::onGammaChange(float & value){
	ps3eye.setGamma(value);
}

//--------------------------------------------------------------
void testApp::onBrightnessChange(float & value){
	ps3eye.setBrightness(value);
}

//--------------------------------------------------------------
void testApp::onContrastChange(float & value){
	ps3eye.setContrast(value);
}

//--------------------------------------------------------------
void testApp::onHueChange(float & value){
	ps3eye.setHue(value);
}

//--------------------------------------------------------------
void testApp::onLedChange(bool & value){
	ps3eye.setLed(value);
}

//--------------------------------------------------------------
void testApp::onFlickerChange(int & value){
	ps3eye.setFlicker(value);
}

//--------------------------------------------------------------
void testApp::onWhiteBalanceChange(int & value){
	ps3eye.setWhiteBalance(value);
}