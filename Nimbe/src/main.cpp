/*
Main.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Main of the application, runs ofApp.
*/

#include "ofMain.h"
#include "ofApp.h"
#include "ofxOsc.h"

int main( ){
	//ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);	
	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());
}
