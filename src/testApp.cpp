#include "testApp.h"

extern "C" {
#include "macGlutfix.h"
}

#include <GLUT/glut.h>

void testApp::setup(){
	
    ofSetWindowTitle("SendScreen Window");
	tex.allocate(INIT_W, INIT_H, GL_RGBA);
	ofSetFrameRate(60);
	cout << "Sending to " << OUT_HOST << "\n";
	
	// open an outbound OSC 
	sender.setup( OUT_HOST, OUT_PORT );

    // stuff that would be in constructor
    capW = INIT_W;
    capH = INIT_H;

    // dbg stuff
	bufferCounter = 0;
	drawCounter = 0;
    updateCounter = 0;

    // Let's listen to some audio so we get called more often -- hackery!
    psnFlag = 0;
	ofSoundStreamSetup(0,2,this, 44100, 256, 4);	
}

int now=0,then=0; int ms=0;

void testApp::update(){
    now = ofGetElapsedTimeMillis();
    ms = now-then;
    then = now;

    updateCounter++;
}

void testApp::draw(){

    static int i=0;
    	
    i=0; // don't animate
	uint32 * data = pixelsBelowWindow(ofGetWindowPositionX()+(i++%300),ofGetWindowPositionY(),capW,capH);
    // convert to GL_RGBA format
    if(data!=NULL){
        for (int i = 0; i < capW*capH; i++){
            // GL_RGBA = (uint32) AABBGGRR
            // NSImage = (uint32) BBGGRRAA
            data[i] = (data[i]>>8) | 0xff000000; 	// scoot down 8 bits - full alpha
        }
        tex.loadData((unsigned char *) data, capW, capH, GL_RGBA);
    }

    // send this image via OSC
    if(data!=NULL) sendImage(data, capW, capH);

    // cover the window
	tex.draw(0,0, ofGetWidth(), ofGetHeight());
    drawCounter++;

	// ofSetColor(0x0);
	// char reportString[255];
	// sprintf(reportString, "MS: %05i,%05i,%05i,%05i", ms, bufferCounter,drawCounter,updateCounter);
	// ofDrawBitmapString(reportString,40,40);
	// ofSetColor(0xffffffff);

}

void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
    // not actually receiving audio, just forcing some I/O
	bufferCounter++;

    // hackery! - works! But really?
    if(!psnFlag){
        GetFrontProcess(&psn);
        psnFlag=1;
    }
    CGEventRef e = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)83, true);
    CGEventPostToPSN (&psn,e);
    CFRelease(e);
}

void testApp::sendImage(uint* data, int w, int h){
    ofxOscMessage m;
    m.setAddress( "/screen" );
    m.addIntArg( w );
    m.addIntArg( h );
    m.addBlobArg(data, w*h );
    sender.sendRawMessage( m );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    // OF_KEY_LEFT, OF_KEY_UP, OF_KEY_RIGHT, OF_KEY_DOWN
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

