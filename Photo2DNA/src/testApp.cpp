#include "testApp.h"
#include "adalogo.h"

#define BAUDRATE  19200
#define BYTE_TIME (11L * 1000000L / BAUDRATE)

//--------------------------------------------------------------
void testApp::setup(){
    setupArduino();

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
			position = ofPoint(0,0);
			break;
			
		default:
			break;
	}
	
	ofPushMatrix();
	ofTranslate(position + ofPoint(ofGetHeight(),0));
	ofScale(-1, 1);
	display.draw(0,0 , ofGetHeight(), ofGetHeight());
	ofPopMatrix();
	
	if(debug){
		gui.draw();
	}
	
	if(printImage.isAllocated())printImage.draw(ofGetHeight(),- 1000, ofGetWidth()-ofGetHeight(), printImage.getHeight());
	
	ofDrawBitmapString("DNA ENCODED", 100, ofGetHeight()/2);
	ofDrawBitmapString("LARGER THEN LIFE", ofGetWidth() /2 + ofGetHeight()/2 + 100, ofGetHeight()/2);
	
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
	
	
	
	
	/*ofPixels pixelsBig;
	pixelsBig.allocate(image.getWidth(), image.getHeight(), 3);
	image.readToPixels(pixelsBig);
	ofPixels channelBig = pixelsBig.getChannel(0);
	uint8_t * printData = new uint8_t [(int) image.getWidth()* (int)image.getHeight()];
	uint8_t currByte;
	for (int i = 0; i < channelBig.size(); i++) {
		currByte <<= 1;
		currByte |= ((int)channelBig[i] > 100) ? 0 : 1;
		if(i%8 == 7){
			printData[i] = currByte;
		}
	}*/
	//printBitmap(image.getWidth(), image.getHeight(), printData);
	
	
	
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
	printImage.allocate(384, 50 + 384 + (int) (data.size() / (384 / 4)) * 4);
	//printImage.allocate(100, 20 );
	printImage.begin();
	ofClear(255,255,255);
	ofSetColor(0, 0, 0, 255);
	ofDrawBitmapString("ART HACK DAY STOCKHOLM 2013", 0, 14);
	ofDrawBitmapString(timestamp + ".BMP.DNA", 0, 30);
	ofSetColor(255, 255, 255, 255);
	image.draw(20, 60, 344,344);
	//image.draw(0, 0, 344,344);
	
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
	
	
	//uint8_t * printData = new uint8_t [(int) printImage.getWidth()* (int)printImage.getHeight()/8];
	//uint8_t currByte;
	
	for (int i = 0; i < printChannel.size(); i++) {
		printPixels[i] = ((int)printChannel[i] > 100) ? 255 : 0;
		
		//currByte <<= 1;
		//currByte |= ((int)channel[i] > 100) ? 0 : 1;
		//if(i%8 == 7){
			//printData[i] = currByte;
		//}
	}

	
	printBmp.update();
	int width = printImage.getWidth();
	int height = printImage.getHeight();
	//zzprintBitmap(width, height, printData);
	//delete printData;
	
	printBmp.saveImage(filename+".print.bmp");
}
void testApp::keyReleased(int key){
	if (key == 'd') debug = !debug;
	if (key == 'f') ofToggleFullscreen();
	if(key =='t'){
		ofBuffer buffer;
		buffer = ofBufferFromFile("image_bytes");
		printBitmap(100, 100, (uint8_t *)buffer.getBinaryBuffer());
	}
	if(key == 'p') printBitmap(300, 20, adalogo_data);
	
	if(key == ' '){
		takePicture();
	}
}

// Adafruit
void testApp::printBitmap(int w, int h, const uint8_t *bitmap) {
    int dotPrintTime = 30000; // See comments near top of file for
    int dotFeedTime  =  2100; // an explanation of these values.

    int rowBytes, rowBytesClipped, rowStart, chunkHeight, x, y, i;

    rowBytes        = (w + 7) / 8; // Round up to next byte boundary
    rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width

    for(i=rowStart=0; rowStart < h; rowStart += 255) {
        // Issue up to 255 rows at a time:
        chunkHeight = h - rowStart;
        if(chunkHeight > 255) chunkHeight = 255;

        arduino.writeByte(18);
        arduino.writeByte(42);
        arduino.writeByte(chunkHeight);
        arduino.writeByte(rowBytesClipped);

        usleep(2 * BYTE_TIME);

        for(y=0; y < chunkHeight; y++) {
            for(x=0; x < rowBytesClipped; x++, i++) {
                arduino.writeByte(*(bitmap+i));
            }
            i += rowBytes - rowBytesClipped;
        }
        usleep(chunkHeight * dotPrintTime);
    }
}

void testApp::setupArduino() {
	arduino.listDevices();
	arduinoReady = arduino.setup("/dev/tty.usbmodemfd1241", 19200);
	if(arduinoReady) {
        printf("arduino ready");
		arduino.flush();
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