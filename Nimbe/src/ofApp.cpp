/*
ofApp.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This class is the core of the application. It creates and triggers the events, draws the graphics and updates the situation on every frame.
*/

#include "ofApp.h"
#include <direct.h>

//Setup, executed once at the launch of the program
void ofApp::setup()
{
	//Init OSC sender
	_sender.setup("localhost", OSCPORT);

	//init UDP
	manager.Create();
	manager.Bind(UDPPORT);
	manager.SetNonBlocking(true);

	//init starting values
	_volumeGlobal = STARTGLOBALVOLUME;
	_trackAmount = 0;
	_mode = STARTMODE;
	_sample = STARTSAMPLE;
	_drawMode = STARTDRAWMODE;
	_previousDrawMode = STARTDRAWMODE;
	_spaceSquares = STARTSPACESQUARES;

	//Init center values
	_centerSpeed = 0;
	_centerFactor = 1;
	_centerOpacity = 1;

	//init Fonts
	char *pathC = NULL;
	pathC = getcwd(NULL, 0);
	string path = pathC;

	presFont.load(path + "\\data\\cone.ttf", 300);
	startFont.load(path + "\\data\\cone.ttf", 150);
	titleFont.load(path + "\\data\\cone.ttf",24);
	textFont.load(path + "\\data\\cone.ttf", 14);

	//init title
	titleOpacity = 255;
	drawTitle = true;

	//init shaking
	_vertikalFactor = 0;
	_horizontFactor = 0;

	//init transitions
	_transitionDrawMode = false;
	_FadeDrawMode = false;

	//init start/end
	_start = false;
	_end = false;

	//Starting colors
	_globalOpacity = 0;

	//white
	_baseColor.r = 255;
	_baseColor.g = 255;
	_baseColor.b = 255;

	//red
	_animationColor.r = 178;
	_animationColor.g = 34;
	_animationColor.b = 34;

	//black
	_backgroundColor.r = 0;
	_backgroundColor.g = 0;
	_backgroundColor.b = 0;

	//black
	_centerColor.r = 0;
	_centerColor.g = 0;
	_centerColor.b = 0;

	ofBackground(_backgroundColor); //set background

	//Blend amd draw modes
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableAlphaBlending();
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofHideCursor();

	//init the arcs at the center
	for (int i = 0; i < 8; i++)
	{
		_arcsCenter[i].setCenterX(0);
		_arcsCenter[i].setCenterY(0);
		_arcsCenter[i].setRadius(round(pow(i, 0.6)*25));
		_arcsCenter[i].setWidth(round(pow(i, 0.6)/0.08));
		_arcsCenter[i].setRotation((i%3*225));
		_arcsCenter[i].setHeight(i);
	}

	//init time
	_totalTime = clock();
	_timeElapsed = 0;

	//init camera
	_camera.disableMouseInput();
	ofEnableDepthTest();
	ofSetVerticalSync(true);
	_camera.setDistance(1000);

	_positionCam = 300;
	_speedCam = 0;
	_cameraMode = true;

	//init wiggle
	_wiggleSpeed = WIGGLESTARTSPEED;
	_wiggleRange = WIGGLESTARTRANGE;

	//init events
	_nextEvent = 0;
	this->createEvents();
	this->sortEvents();
}

//Create all of the events for the performance. These are hardcoded because they are composed, they represent the core of the music.
void ofApp::createEvents()
{
	//event 0 = turn on globalopacity sur 8 secondes
	_allEvents.push_back(new eventOpacityFade(0, 10, 1, &_globalOpacity));

	//Bass events
	_allEvents.push_back(new eventCenterOpacityFade(48, 12, 0, 1, &_baseColor, &_backgroundColor, &_centerColor));

	_allEvents.push_back(new eventBass(48, 40, 1, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(140, 40, 0, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(182, 30, 1.2, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(295, 30, 0, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(326, 25, 1.5, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(415, 25, 0, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(445, 20, 2, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(515, 20, 0, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(540, 20, 2.5, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(595, 20, 0.5, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(620, 15, 3, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(660, 15, 1.5, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(680, 15, 3.5, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(715, 15, 2, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(735, 10, 4, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(760, 10, 3, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(775, 10, 5, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(795, 10, 3, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(810, 8, 6, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(822, 8, 5, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(835, 7, 7, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(843, 7, 6, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(855, 6, 8, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(864, 6, 7, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(872, 5, 8, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(880, 5, 7, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(887, 4, 8, &_centerSpeed, &_centerFactor, &_centerOpacity));
	_allEvents.push_back(new eventBass(892, 4, 7, &_centerSpeed, &_centerFactor, &_centerOpacity));

	_allEvents.push_back(new eventBass(897, 3, 8, &_centerSpeed, &_centerFactor, &_centerOpacity));

	//Vivration events
	_allEvents.push_back(new eventVibration(495, 12, 0.35, 0, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(511, 10, 0, 0, &_vertikalFactor, &_horizontFactor));

	_allEvents.push_back(new eventVibration(556, 12, 0.5, 0, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(574, 16, 0, 0, &_vertikalFactor, &_horizontFactor));

	_allEvents.push_back(new eventVibration(625, 12, 0.5, 0.2, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(650, 12, 0, 0, &_vertikalFactor, &_horizontFactor));

	_allEvents.push_back(new eventVibration(688, 12, 0.5, 0.5, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(702, 10, 0.2, 0.8, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(714, 10, 0, 0, &_vertikalFactor, &_horizontFactor));

	_allEvents.push_back(new eventVibration(770, 8, 0.6, 0, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(780, 8, 1, 0.2, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(790, 8, 0.5, 0.3, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(800, 8, 0.2, 0.5, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(810, 8, 0.6, 0.6, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(820, 8, 1, 0.5, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(830, 8, 0.2, 1, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(840, 8, 0.6, 0.6, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(850, 8, 1, 0, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(860, 8, 1.5, 0.2, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(870, 8, 1.2, 0.6, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(880, 8, 1.8, 1.2, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(890, 5, 2, 2, &_vertikalFactor, &_horizontFactor));
	_allEvents.push_back(new eventVibration(896, 12, 4, 4, &_vertikalFactor, &_horizontFactor));

	//Wiggle de 0 à 12mins
	_allEvents.push_back(new eventChangeWiggle(120, 840, 0.2, 25, &_wiggleSpeed, &_wiggleRange));

	//Color and Camera events
	//Color order: Back -> Front -> Animation

	//White/Black/Blue
	_allEvents.push_back(new eventHardColor(180, DURATIONHARDCOLORCHANGE, ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(0, 128, 255), 5, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(250, DURATIONSOFTCOLORCHANGE, ofColor(255, 255, 255), ofColor(12, 25, 47), ofColor(0, 128, 255), 5.2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	//Black/Red/White
	_allEvents.push_back(new eventHardColor(325.8, DURATIONHARDCOLORCHANGE, ofColor(0, 0, 0), ofColor(213, 205, 146), ofColor(255, 255, 255), 4, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(360, DURATIONSOFTCOLORCHANGE, ofColor(0, 0, 0), ofColor(208, 88, 70), ofColor(255, 255, 255), 4.1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(410, DURATIONSOFTCOLORCHANGE, ofColor(0, 0, 0), ofColor(178, 34, 34), ofColor(255, 255, 255), 4, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	//White/Orange/Blance
	_allEvents.push_back(new eventHardColor(443.9, DURATIONHARDCOLORCHANGE, ofColor(255, 255, 255), ofColor(78, 27, 17), ofColor(0, 0, 0), 2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(470, DURATIONSOFTCOLORCHANGE, ofColor(255, 255, 255), ofColor(175, 63, 17), ofColor(0, 0, 0), 2.1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(510, DURATIONSOFTCOLORCHANGE, ofColor(255, 255, 255), ofColor(255, 69, 0), ofColor(0, 0, 0), 2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	//Black/YellowGreen/White
	_allEvents.push_back(new eventHardColor(539.6, DURATIONHARDCOLORCHANGE, ofColor(0, 0, 0), ofColor(154, 205, 50), ofColor(255, 255, 255), 3, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(565, DURATIONSOFTCOLORCHANGE, ofColor(0, 0, 0), ofColor(154, 115, 50), ofColor(255, 255, 255), 3.1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(595, DURATIONSOFTCOLORCHANGE, ofColor(0, 0, 0), ofColor(215, 94, 103), ofColor(255, 255, 255), 3, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	//White/Magenta/Black
	_allEvents.push_back(new eventHardColor(617, DURATIONHARDCOLORCHANGE, ofColor(255, 255, 255), ofColor(199, 21, 133), ofColor(0, 0, 0), 1.3, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(635, DURATIONSOFTCOLORCHANGE, ofColor(255, 255, 255), ofColor(120, 49, 133), ofColor(0, 0, 0), 1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(660, DURATIONSOFTCOLORCHANGE, ofColor(255, 255, 255), ofColor(120, 49, 57), ofColor(0, 0, 0), 1.2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	//Gray/Red/White
	_allEvents.push_back(new eventHardColor(679.8, DURATIONHARDCOLORCHANGE, ofColor(64, 64, 64), ofColor(255, 51, 51), ofColor(255, 255, 255), 5.2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(690, DURATIONSOFTCOLORCHANGE, ofColor(64, 64, 64), ofColor(255, 140, 51), ofColor(255, 255, 255), 5, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(705, DURATIONSOFTCOLORCHANGE, ofColor(64, 64, 64), ofColor(186, 206, 51), ofColor(255, 255, 255), 5.3, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(679.8, DURATIONHARDCOLORCHANGE, ofVec3f(0, 0, - 300), ofVec3f(0, 0, 0),true, &_camera, &_cameraMode, &_positionCam));

	//White/Blue/Black
	_allEvents.push_back(new eventHardColor(730.6, DURATIONHARDCOLORCHANGE, ofColor(255, 255, 255), ofColor(0, 128, 255), ofColor(0, 0, 0), 6.1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(740, DURATIONSOFTCOLORCHANGE, ofColor(255, 255, 255), ofColor(50, 71, 255), ofColor(0, 0, 0), 6, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(760, DURATIONSOFTCOLORCHANGE, ofColor(255, 255, 255), ofColor(50, 99, 108), ofColor(0, 0, 0), 6.2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(730.6, DURATIONHARDCOLORCHANGE, ofVec3f(0, 0,100), ofVec3f(0, 0, 0),true, &_camera, &_cameraMode, &_positionCam));
	_allEvents.push_back(new eventMoveCamera(732, 35, ofVec3f(0, 0,1000), ofVec3f(0, 0, 0), true, &_camera, &_cameraMode, &_positionCam));

	//Black/White/Gray
	_allEvents.push_back(new eventHardColor(771.8, DURATIONHARDCOLORCHANGE, ofColor(0, 0, 0), ofColor(255, 255, 255), ofColor(96, 96, 96), 4.2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(775, DURATIONSOFTCOLORCHANGE, ofColor(0, 0, 0), ofColor(201, 198, 219), ofColor(96, 96, 96), 4, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(790, DURATIONSOFTCOLORCHANGE, ofColor(0, 0, 0), ofColor(98, 142, 219), ofColor(96, 96, 96), 4.3, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(771.8, DURATIONHARDCOLORCHANGE, ofVec3f(0, 0, 200), ofVec3f(0, 0, 0), true, &_camera, &_cameraMode, &_positionCam));

	//Pale Blue/Dark Blue/Gray
	_allEvents.push_back(new eventHardColor(805.2, DURATIONHARDCOLORCHANGE, ofColor(176, 196, 222), ofColor(25, 25, 112), ofColor(96, 96, 96), 2.3, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(810, DURATIONSOFTCOLORCHANGE*0.6, ofColor(176, 196, 222), ofColor(76, 87, 112), ofColor(96, 96, 96), 2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(820, DURATIONSOFTCOLORCHANGE*0.6, ofColor(176, 196, 222), ofColor(108, 59, 79), ofColor(96, 96, 96), 2.2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(805.2, DURATIONHARDCOLORCHANGE, ofVec3f(0, 500, - 300), ofVec3f(0, 500, 0), true, &_camera, &_cameraMode, &_positionCam));

	//Brown/beige/White
	_allEvents.push_back(new eventHardColor(832.2, DURATIONHARDCOLORCHANGE, ofColor(46, 30, 30), ofColor(200, 188, 115), ofColor(255, 255, 255), 4.1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(835, DURATIONSOFTCOLORCHANGE*0.5, ofColor(46, 30, 30), ofColor(168, 156, 150), ofColor(255, 255, 255), 4, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(845, DURATIONSOFTCOLORCHANGE*0.5, ofColor(46, 30, 30), ofColor(128, 173, 201), ofColor(255, 255, 255), 4.2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(832.2, DURATIONHARDCOLORCHANGE, ofVec3f(600, 0, 400), ofVec3f(600, 0, 0), false, &_camera, &_cameraMode, &_positionCam));
	_allEvents.push_back(new eventMoveCamera(833.2, 20, ofVec3f(0, 0, 200), ofVec3f(0, 0, 0), false, &_camera, &_cameraMode, &_positionCam));

	//Green/Pink/Black
	_allEvents.push_back(new eventHardColor(854.1, DURATIONHARDCOLORCHANGE, ofColor(130, 165, 85), ofColor(145, 49, 47), ofColor(0, 0, 0), 6.3, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(855, DURATIONSOFTCOLORCHANGE*0.3, ofColor(130, 165, 85), ofColor(109, 49, 68), ofColor(0, 0, 0), 6, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(862, DURATIONSOFTCOLORCHANGE*0.3, ofColor(130, 165, 85), ofColor(91, 36, 104), ofColor(0, 0, 0), 6.1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(854.1, DURATIONHARDCOLORCHANGE, ofVec3f(0, 0, _positionCam), ofVec3f(0, 0, 0), true, &_camera, &_cameraMode, &_positionCam));
	_allEvents.push_back(new eventMoveCamera(855.1, 15, ofVec3f(0, 0, 500), ofVec3f(0, 0, 0), true, &_camera, &_cameraMode, &_positionCam));

	//Blue/Orange/White
	_allEvents.push_back(new eventHardColor(871.8, DURATIONHARDCOLORCHANGE, ofColor(22, 67, 110), ofColor(218, 142, 37), ofColor(255, 255, 255), 5, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(873, DURATIONHARDCOLORCHANGE*0.25, ofColor(22, 67, 110), ofColor(218, 189, 96), ofColor(255, 255, 255), 5.1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventHardColor(880, DURATIONHARDCOLORCHANGE*0.25, ofColor(22, 67, 110), ofColor(198, 170, 73), ofColor(255, 255, 255), 5, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(871.8, DURATIONHARDCOLORCHANGE, ofVec3f(0, -500, 500), ofVec3f(0, 0, 0), false, &_camera, &_cameraMode, &_positionCam));
	_allEvents.push_back(new eventMoveCamera(872.8, 12, ofVec3f(-200, 800, 600), ofVec3f(0, 0, 0), false, &_camera, &_cameraMode, &_positionCam));

	//White/Black/Gray
	_allEvents.push_back(new eventHardColor(886.16, DURATIONHARDCOLORCHANGE, ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(96, 96, 96), 1, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	_allEvents.push_back(new eventMoveCamera(886.16, DURATIONHARDCOLORCHANGE, ofVec3f(-500, 500, 400), ofVec3f(0, 0, 0), false, &_camera, &_cameraMode, &_positionCam));
	_allEvents.push_back(new eventMoveCamera(887.16, 10, ofVec3f(0, -800, 600), ofVec3f(0, 0, 0), false, &_camera, &_cameraMode, &_positionCam));

	//Black/White/Gray
	_allEvents.push_back(new eventHardColor(897.8, DURATIONHARDCOLORCHANGE, ofColor(0, 0, 0), ofColor(255, 255, 255), ofColor(96, 96, 96), 2, &_backgroundColor, &_baseColor, &_animationColor, &_centerColor, &_oldBaseColor, &_oldBackgroundColor, &_oldAnimationColor, &_oldCenterColor, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	_allEvents.push_back(new eventMoveCamera(897.8, DURATIONHARDCOLORCHANGE, ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), true, &_camera, &_cameraMode, &_positionCam));
	_allEvents.push_back(new eventMoveCamera(900, 8, ofVec3f(0, 0, 600), ofVec3f(0, 0, 0), true, &_camera, &_cameraMode, &_positionCam));

	//Events for shapes
	//Full Squres
	_allEvents.push_back(new eventChangeSquare(60, 40.7, 0.8, 2, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Arcs
	_allEvents.push_back(new eventChangeSquare(141.4, 36.8, 1, 3, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Squares
	_allEvents.push_back(new eventChangeSquare(214.9, 33.2, 0.4, 4, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Arcs
	_allEvents.push_back(new eventChangeSquare(281.4, 30, 1, 5, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty arcs
	_allEvents.push_back(new eventFluidTransition(341.5, 27.2, 2, 6, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Arcs
	_allEvents.push_back(new eventFluidTransition(396, 24, 1, 7, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty squares as arcs
	_allEvents.push_back(new eventFluidTransition(445, 22, 4, 8, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty squares
	_allEvents.push_back(new eventChangeSquare(489, 20, 0.5, 9, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Squares
	_allEvents.push_back(new eventFluidTransition(529, 18, 1, 10, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Arcs
	_allEvents.push_back(new eventChangeSquare(565, 16.4, 1, 11, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty Arcs
	_allEvents.push_back(new eventFluidTransition(598.6, 15, 2, 12, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//X shapes
	_allEvents.push_back(new eventFluidTransition(628, 13.4, 3, 13, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty arcs
	_allEvents.push_back(new eventFluidTransition(655, 12, 2, 14, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Squares
	_allEvents.push_back(new eventChangeSquare(679, 11, 0.85, 15, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Small full squares
	_allEvents.push_back(new eventChangeSquare(701, 10, 0.2, 16, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Small empty squares
	_allEvents.push_back(new eventFluidTransition(721, 9, 4, 17, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big empty squares
	_allEvents.push_back(new eventChangeSquare(739, 8, 0.6, 18, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big full squares
	_allEvents.push_back(new eventFluidTransition(755, 7.3, 1, 19, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full arc
	_allEvents.push_back(new eventChangeSquare(770, 6.6, 1, 20, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//X shapes
	_allEvents.push_back(new eventFluidTransition(783, 6, 3, 21, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty squares
	_allEvents.push_back(new eventFluidTransition(795, 5.4, 4, 22, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Medium Squares
	_allEvents.push_back(new eventChangeSquare(806, 4.9, 0.5, 23, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Squares
	_allEvents.push_back(new eventFluidTransition(815.5, 4.4, 1, 24, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big Squares
	_allEvents.push_back(new eventChangeSquare(824.3, 4, 0.9, 25, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Small Squares
	_allEvents.push_back(new eventChangeSquare(832, 3.6, 0.4, 26, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big Squares
	_allEvents.push_back(new eventChangeSquare(839.5, 3.3, 0.7, 27, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Small Squares
	_allEvents.push_back(new eventChangeSquare(846, 3, 0.1, 28, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big Squares
	_allEvents.push_back(new eventChangeSquare(852, 2.7, 0.6, 29, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty Squares
	_allEvents.push_back(new eventFluidTransition(857, 2.4, 4, 30, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Arc of empty squares
	_allEvents.push_back(new eventChangeSquare(862, 2.2, 1, 31, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//X shapes
	_allEvents.push_back(new eventFluidTransition(866.5, 2, 3, 32, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty arc
	_allEvents.push_back(new eventFluidTransition(870.4, 1.8, 2, 33, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full arc
	_allEvents.push_back(new eventFluidTransition(874, 1.6, 1, 34, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Medium Squares
	_allEvents.push_back(new eventChangeSquare(877.2, 1.5, 0.7, 35, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big Squares
	_allEvents.push_back(new eventChangeSquare(880, 1.3, 0.9, 36, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Small Squares
	_allEvents.push_back(new eventChangeSquare(882.7, 1.2, 0.5, 37, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big Squares
	_allEvents.push_back(new eventChangeSquare(885, 1.1, 0.8, 38, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big empty Squares
	_allEvents.push_back(new eventFluidTransition(887.3, 1, 4, 39, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//empty lines
	_allEvents.push_back(new eventChangeSquare(889.2, 0.9, 1, 40, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty Arc
	_allEvents.push_back(new eventFluidTransition(891, 0.8, 2, 41, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//X Shapes
	_allEvents.push_back(new eventFluidTransition(892.5, 0.72, 3, 42, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Empty arcs
	_allEvents.push_back(new eventFluidTransition(894, 0.65, 2, 43, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Arc
	_allEvents.push_back(new eventFluidTransition(895.3, 0.59, 1, 44, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Full Squares
	_allEvents.push_back(new eventChangeSquare(896.4, 0.53, 0.6, 45, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Very Small Squares
	_allEvents.push_back(new eventChangeSquare(897.5, 0.48, 0.1, 46, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Very Small empty squares
	_allEvents.push_back(new eventFluidTransition(898.5, 0.43, 4, 47, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big empty squares
	_allEvents.push_back(new eventChangeSquare(899.3, 0.4, 2, 48, &_spaceSquares, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));
	//Big full squares
	_allEvents.push_back(new eventFluidTransition(900, 8, 1, 49, &_drawMode, &_FadeDrawMode, &_previousDrawMode, &_transitionDrawModeDegree, &_mode, &_trackAmount, &_tracks, &_volumeGlobal, &_sample));

	//At the end, turn everything off
	_allEvents.push_back(new eventEverythingOff(908, ofColor(0, 0, 0), &_baseColor, &_centerColor, &_backgroundColor, &_globalOpacity, &_start, &_end, &_trackAmount, &_tracks, &_mode, &_sample));
}

//How to sort sort Events
bool wayToSort(baseEvent *i, baseEvent *j)
{
	return (i->getTiming() < j->getTiming());
}

//Sort all events in chronologic order
void ofApp::sortEvents()
{
	std::sort(_allEvents.begin(), _allEvents.end(), wayToSort);
}

//runs every frame, updates variables, recieves SMS and calls events
void ofApp::update()
{
	//if space has been pressed
	if (_start == true)
	{
		//wiggle
		for (int i = 0; i < _trackAmount; i++)
		{
			_arcs[i].updateWiggle(_timeElapsed, _wiggleSpeed, _wiggleRange);
		}

		//UDP
		char udpMessage[1000];
		manager.Receive(udpMessage, 1000);
		string message = udpMessage;

		//Receive SMS messages
		if (message != "")
		{
			//extract message and number
			string number = message.substr(1, 10);
			message = message.substr(11, message.size());
			string lowerCaseMessage = message;
			transform(lowerCaseMessage.begin(), lowerCaseMessage.end(), lowerCaseMessage.begin(), ::tolower);

			bool found = false;

			//check for message correspondance
			for (int i = 0; i < _trackAmount; i++)
			{
				//if the number is found
				if (_listPhones[i] == number)
				{
					found = true;
					_tracks[i].resetTime();
					_tracks[i].setlastMessage(message);

					if (lowerCaseMessage.find("on") != string::npos)
					{
						//Open track
						changeVolume(i, false);
					}
					else if (lowerCaseMessage.find("off") != string::npos)
					{
						//Close track
						changeVolume(i, true);
					}
					else if ((lowerCaseMessage.find("nord") != string::npos) || (lowerCaseMessage.find("north") != string::npos))
					{

						if ((lowerCaseMessage.find("ouest") != string::npos) || (lowerCaseMessage.find("west") != string::npos))
						{
							//North-West
							changePan(i, 0);
						}
						else if ((lowerCaseMessage.find("est") != string::npos) || (lowerCaseMessage.find("east") != string::npos))
						{
							//North-East
							changePan(i, 270);
						}
						else
						{
							//North
							changePan(i, 315);
						}

					}
					else if ((lowerCaseMessage.find("sud") != string::npos) || (lowerCaseMessage.find("south") != string::npos))
					{

						if ((lowerCaseMessage.find("ouest") != string::npos) || (lowerCaseMessage.find("west") != string::npos))
						{
							//South-West
							changePan(i, 90);
						}
						else if ((lowerCaseMessage.find("est") != string::npos) || (lowerCaseMessage.find("east") != string::npos))
						{
							//South-East
							changePan(i, 180);
						}
						else
						{
							//South
							changePan(i, 135);
						}
					}
					else if ((lowerCaseMessage.find("ouest") != string::npos) || (lowerCaseMessage.find("west") != string::npos))
					{
						//West
						changePan(i, 45);
					}
					else if ((lowerCaseMessage.find("est") != string::npos) || (lowerCaseMessage.find("east") != string::npos))
					{
						//East
						changePan(i, 225);
					}
					else if (lowerCaseMessage.find("+") != string::npos)
					{
						//Augment pitch
						changePitch(i, true);
					}
					else if (lowerCaseMessage.find("-") != string::npos)
					{
						//Lower Pitch
						changePitch(i, false);
					}
				}
			}
			//if it's a new number, create a new track
			if (found == false)
			{
				if (lowerCaseMessage.find("on") != string::npos)
				{
					//add number to the list
					_listPhones[_trackAmount] = number;

					//Create track
					newTrack();
					_tracks[_trackAmount - 1].setlastMessage(message);
					_tracks[_trackAmount - 1].setPhoneNumber(number);
				}
			}
		}

		//Manage Events
		//Check if next event should start running, if so, add it to the list of running events
		if (_nextEvent < _allEvents.size())
		{
			if (_allEvents[_nextEvent]->getTiming() < _timeElapsedSpace)
			{
				_runningEvents.push_back(_allEvents[_nextEvent]);
				_nextEvent++;
			}
		}

		//Run events, if an event has ended, remove it from the list of running events
		for (int i = 0; i < _runningEvents.size(); i++)
		{
			if (!_runningEvents[i]->activateEvent(_timeElapsedFrame))
			{
				_runningEvents.erase(_runningEvents.begin() + i);
			}
		}
	}

	//Update time variables
	double tempsEcouleTotal;
	tempsEcouleTotal = clock() - _totalTime;
	tempsEcouleTotal /= CLOCKS_PER_SEC;

	_timeElapsedFrame = tempsEcouleTotal - _timeElapsed;
	_timeElapsed = tempsEcouleTotal;

	if(_start == true)
		_timeElapsedSpace += _timeElapsedFrame;

	//Manage the rotation of the center
	for (int i = 0; i < 8; i++)
	{
		_arcsCenter[i].setRotation(_arcsCenter[i].getRotation()+(i / BASEMOVEMENTSPEED*_timeElapsedFrame * 32 - 1 / BASEMOVEMENTSPEED*_timeElapsedFrame * 9 * 16)*_centerSpeed);
	}
	
	//When new tracks are added, zoom out gradueally for a bit.
	_positionEnd = _trackAmount * 11 + 300;
	if(_cameraMode == true)
	{ 
		_speedCam = (_positionEnd - _positionCam) / BASEMOVEMENTSPEED * 2;
		_positionCam = _speedCam*_timeElapsedFrame + _positionCam;
		_camera.setDistance(_positionCam);
	}
	
}

//Runs every frame, draw the visuals
void ofApp::draw()
{
	//start the camera, start drawing in the 3d scope
	_camera.begin();

	//Check for transitions
	//if no transitions
	if (_transitionDrawMode == false && _FadeDrawMode == false)
	{
		//Draw every arc
		for (int i = 0; i < _trackAmount; i++)
		{
			{
				_arcs[displayList[i]].drawArc(_baseColor, _animationColor, _drawMode, _globalOpacity,_spaceSquares,_vertikalFactor, _horizontFactor);
			}
		}
	}
	//If an abrupt transition is going on
	else if (_transitionDrawMode == true)
	{
		//Draw every arc randomly as the previous type or the new one, with a different weight depending on the time elapsed and duration of the event
		for (int i = 0; i < _trackAmount; i++)
		{
			{
				int mode;
				int modeC = rand() % 100;
				if (modeC > (_transitionDrawModeDegree * 100))
				{
					mode = _previousDrawMode;
				}
				else if (modeC <= (_transitionDrawModeDegree * 100))
				{
					mode = _drawMode;
				}
				_arcs[displayList[i]].drawArc(_baseColor, _animationColor, mode, _globalOpacity, _spaceSquares, _vertikalFactor, _horizontFactor);
			}
		}
	}
	//if a fade transitions is going on
	else if (_FadeDrawMode == true)
	{
		//Draw the old and the new arc, fading out and in respectively
		for (int i = 0; i < _trackAmount; i++)
		{
			{
				_arcs[displayList[i]].setHeight(_arcs[displayList[i]].getHeight() - 1);
				_arcs[displayList[i]].drawArc(_baseColor, _animationColor, _previousDrawMode, (1-_transitionDrawModeDegree)*_globalOpacity, _spaceSquares, _vertikalFactor, _horizontFactor);
				_arcs[displayList[i]].setHeight(_arcs[displayList[i]].getHeight() + 1);
				_arcs[displayList[i]].drawArc(_baseColor, _animationColor, _drawMode, _transitionDrawModeDegree, _spaceSquares, _vertikalFactor, _horizontFactor);
			}
		}
	}
	
	//draw center
	for (int i = 0; i < 8; i++)
	{
		_arcsCenter[i].drawArcCentre(_centerColor, _animationColor, 0,_centerFactor, _centerOpacity, _globalOpacity, _vertikalFactor, _horizontFactor);
	}

	//end Camera (and drawing in the 3d scope)
	_camera.end();

	//if the performance hasn't ended, leave the reminder at the bottom right of the screen
	if(_end == false)
	{
		//rappel carré bas-droite
		ofPushStyle();
		ofNoFill();
		ofSetColor(100, 100, 100);

		ofDrawRectangle(1745, 955, 300, 200);
		titleFont.drawString("5 1 4 - 6 0 0 - 8 1 8 8", 1920-310, 1080-200);
		textFont.drawString("O u v r i r   l a   p i s t e :   O n ", 1920 - 310, 1080 - 160);
		textFont.drawString("F e r m e r   l a   p i s t e :   O  f f ", 1920 - 310, 1080 - 140);
		textFont.drawString("M o n t e r   l a   h a u t e u r :  +", 1920 - 310, 1080 - 100);
		textFont.drawString("B a i s s e r   l a   h a u t e u r :  -", 1920 - 310, 1080 - 80);
		textFont.drawString("L o c a l i s a t i o n :   Nord / Sud / Est / Ouest", 1920 - 310, 1080 - 40);
		ofPopStyle();
	}

	//Draw the last SMS received for every track (the opacity will fade out)
	for (int i = 0; i < _trackAmount; i++)
	{
		_tracks[i].drawMessage(_backgroundColor,_baseColor, _animationColor, _globalOpacity);
	}

	//Draw the title and the introduction, at the start.
	if (drawTitle == true)
	{
		if (_timeElapsedSpace < 5)
			titleOpacity = round((1 - (_timeElapsedSpace / 5))*255);
		else
			titleOpacity = 0;

		//Write the title of the performance
		ofPushStyle();
		ofSetColor(255, 255, 255,titleOpacity);
		presFont.drawString("N i m b e",200,400);
		ofPopStyle();

		//Determine the opacity of the starting line
		float opacityTitle = 0;
		if (_timeElapsedSpace > 0 && _timeElapsedSpace < 5)
			opacityTitle = round((((_timeElapsedSpace) / 5)) * 255);
		else if(_timeElapsedSpace<6 && _timeElapsedSpace>=5)
			opacityTitle = 255;
		else if(_timeElapsedSpace>=6 && _timeElapsedSpace < 11)
			opacityTitle = round((1 - ((_timeElapsedSpace-6) / 5)) * 255);
		else if (_timeElapsedSpace >= 11)
		{
			opacityTitle = 0;
			drawTitle = false;
		}

		//Write the starting line
		ofPushStyle();
		ofSetColor(255, 255, 255, opacityTitle);
		startFont.drawString("A vous d'y aller", 200, 800);
		ofPopStyle();
	}
}

//When you press "a", change a random _tracks localisation (for testing)
//when you press "s", change volume of a random track (on/off) (for testing)
// when you press "d", change pitch of a random track (for testing)
//When Spacebar is pressed, it begins.
void ofApp::keyPressed(int key)
{
	//Start the whole thing
	if (key == 32) // space
	{
		_start = true;
		ofxOscMessage message;
		message.setAddress("/BassPunch");
		message.addFloatArg(BASSLEVEL);
		_sender.sendMessage(message);
	}
	if(_start == true)
	{
		//simulate on/off for a random track
		if (key == 97) //a
		{
			int randomTrack = rand() % _trackAmount;
			changeVolume(randomTrack,rand()%2);
		}
		//Move pan of a random track
		if(key == 115) //s
		{
			int randomTrack = rand() % _trackAmount;
			changePan(randomTrack, (rand() % 8)*45);
		}

		//Simulate +/-
		if (key == 100) //d
		{
			int randomTrack = rand() % _trackAmount;
			changePitch(randomTrack, rand() % 2);
		}
	}
}

//When you left click, add a new track with default properties (for testing)
void ofApp::mousePressed(int x, int y, int button)
{
	if (_start == true)
	{
		if (button == 0) //left click
		{
			newTrack();
		}	
	}
}

//Add a new track
void ofApp::newTrack()
{
	if (_trackAmount < MAXTRACKS)
	{
		//add the new track to the displayList
		displayList.insert(displayList.begin(), _trackAmount);

		//init the new track
		_tracks[_trackAmount].setID(_trackAmount);
		int ratioRanking = (rand() % 9)+2;
		int rotation = (rand() % 8 * 45);
		float pan = ((rotation + 225) % 360);
		pan /= 180;
		_tracks[_trackAmount].setPan(pan);
		_tracks[_trackAmount].changeRatio(ratioRanking, _mode);
		_tracks[_trackAmount].setLevel(BASELEVEL);
		_tracks[_trackAmount].resetTime();

		//Send OSC to supercollider to create a new track
		ofxOscMessage message;
		message.setAddress("/New");
		message.addFloatArg(_trackAmount);
		message.addFloatArg(_tracks[_trackAmount].getRatio());
		message.addFloatArg(pan);
		message.addIntArg(_sample);
		message.addFloatArg(BASELEVEL*_volumeGlobal);
		_sender.sendMessage(message);

		//Init a new arc
		_arcs[_trackAmount].setCenterX(0);
		_arcs[_trackAmount].setCenterY(0);
		_arcs[_trackAmount].setHeight(0);

		if (_trackAmount == 0)
			_arcs[_trackAmount].setRanking(0);
		else
		{
			int ranking = 0;
			for (int i = 0; i < _trackAmount; i++)
			{
				if (_tracks[i].getRatioRanking() < ratioRanking)
					ranking++;
			}
			_arcs[_trackAmount].setRanking(ranking);
		}

		//Init the properties of the new arc
		_arcs[_trackAmount].setRadius(_arcs[_trackAmount].getRanking() * 10 + 124);
		_arcs[_trackAmount].setWidth(0);
		_arcs[_trackAmount].setRotation(rotation);
		_arcs[_trackAmount].setTrueRotation(rotation);

		//Move all other arcs that are higher than this one to make space for this new one
		for (int i = 0; i < _trackAmount; i++)
		{
			if (_arcs[i].getRanking() >= _arcs[_trackAmount].getRanking())
			{
				_arcs[i].moveArc(_arcs[i].getRadius() + 10, _arcs[i].getWidth(), _arcs[i].getRotation(), BASEMOVEMENTSPEED, false);
				_arcs[i].setRanking(_arcs[i].getRanking() + 1);
			}
		}

		//Move this new arc to make it appear
		_arcs[_trackAmount].moveArc(_arcs[_trackAmount].getRadius(), 8, _arcs[_trackAmount].getRotation(), BASEMOVEMENTSPEED, true);
		_arcs[_trackAmount].moveArc(_arcs[_trackAmount].getRadius() + 6, _arcs[_trackAmount].getWidth(), _arcs[_trackAmount].getRotation(), BASEMOVEMENTSPEED, false);

		//One more track
		_trackAmount++;
	}
}

//Change the pan of the chosen track to the angle of rotationDegree (from 0 to 360, 0 being north-west, going counter-clockwise. (so north is 315))
void ofApp::changePan(int trackID, int rotationDegree)
{
	//Reset the time since the track has last started moving
	_tracks[trackID].resetTime();

	//Calculate the movement of the arc
	int newRotation = round((_arcs[trackID].getTrueRotation()+360)-rotationDegree);
	newRotation %= 360;
	newRotation = 360 - newRotation;
	_arcs[trackID].setTrueRotation(_arcs[trackID].getTrueRotation()+newRotation);

	//Move the arc
	_arcs[trackID].moveArc(_arcs[trackID].getRadius(), _arcs[trackID].getWidth(), _arcs[trackID].getRotation() + newRotation, BASEMOVEMENTSPEED, true);

	//Calculate the correspondance for the new pan
	float pan = int((round(newRotation + 225 + _arcs[trackID].getRotation()) / 45) * 45) % 360;
	pan /= 180;
	_tracks[trackID].setPan(pan);

	//Send an OSC message to change the pan
	ofxOscMessage message;
	message.setAddress("/Change");
	message.addFloatArg(trackID);
	message.addFloatArg(_tracks[trackID].getRatio());
	message.addFloatArg(_tracks[trackID].getLevel()*_volumeGlobal);
	message.addFloatArg(_tracks[trackID].getPan());
	message.addFloatArg(8);
	message.addIntArg(_sample);
	_sender.sendMessage(message);
}

//Change the pitch of theis track. Plus is whether the track is going up ro down.
void ofApp::changePitch(int trackID, bool plus)
{
	//Reset the time since the track has last started moving
	_tracks[trackID].resetTime();

	//Up or down?
	// Updown: -1 if going down, 1 if going up (down = closer to center)
	int upDown = 0;
	if (plus == true && _tracks[trackID].getRatioRanking()<12)
		upDown = +1;
	else if (plus == false && _tracks[trackID].getRatioRanking()>0)
		upDown = -1;

	//Send OSC to change the pitch
	_tracks[trackID].changeRatio(_tracks[trackID].getRatioRanking() + upDown, _mode);
	ofxOscMessage message;
	message.setAddress("/Change");
	message.addFloatArg(trackID);
	message.addFloatArg(_tracks[trackID].getRatio());
	message.addFloatArg(_tracks[trackID].getLevel()*_volumeGlobal);
	message.addFloatArg(_tracks[trackID].getPan());
	message.addFloatArg(8);
	message.addIntArg(_sample);
	_sender.sendMessage(message);


	//Calculate movement of the arc
	int newRank = 0;
	for (int i = 0; i < _trackAmount; i++)
	{
		if (_tracks[i].getRatioRanking() < _tracks[trackID].getRatioRanking())
			newRank++;
	}
	int ecart = newRank - _arcs[trackID].getRanking();
	bool direction = false;
	if (ecart > 0)
		direction = true;

	//Move the arc
	_arcs[trackID].moveArc(_arcs[trackID].getRadius() + 10 * ecart, _arcs[trackID].getWidth(), _arcs[trackID].getRotation(), BASEMOVEMENTSPEED, true);

	//Move other arcs accordingly to the moving of this one (if this track goes up, others have to go down)
	for (int i = 0; i < _trackAmount; i++)
	{
		if (direction == false)
		{
			if (_arcs[i].getRanking() < _arcs[trackID].getRanking() && _arcs[i].getRanking() >= newRank)
			{
				_arcs[i].moveArc(_arcs[i].getRadius() + 10, _arcs[i].getWidth(), _arcs[i].getRotation(), BASEMOVEMENTSPEED, false);
				_arcs[i].setRanking(_arcs[i].getRanking() + 1);
			}
		}
		else if (direction == true)
		{
			if (_arcs[i].getRanking() > _arcs[trackID].getRanking() && _arcs[i].getRanking() <= newRank)
			{
				_arcs[i].moveArc(_arcs[i].getRadius() - 10, _arcs[i].getWidth(), _arcs[i].getRotation(), BASEMOVEMENTSPEED, false);
				_arcs[i].setRanking(_arcs[i].getRanking() - 1);
			}
		}

		//update the display list
		if (displayList[i] == trackID)
		{
			displayList.erase(displayList.begin() + i);
			displayList.insert(displayList.begin(), trackID);
		}

	}
	_arcs[trackID].setRanking(newRank);
}

//Change the volume of the chosen track. Since the volume can only be "on" or "off", the variable on is a bool.
void ofApp::changeVolume(int trackID, bool on)
{
		//Reset the time since the track has last started moving
		_tracks[trackID].resetTime();

		//If closing track
		if (_arcs[trackID].getClosed() == false && on == true)
		{
			//Move the arc
			_arcs[trackID].moveArc(_arcs[trackID].getRadius(), _arcs[trackID].getWidth() - 8, _arcs[trackID].getRotation(), BASEMOVEMENTSPEED, true);
			//change status of the arc
			_arcs[trackID].setClosed(true);

			//Send OSC message to fade the volume
			_tracks[trackID].setLevel(0);
			ofxOscMessage message;
			message.setAddress("/Change");
			message.addFloatArg(trackID);
			message.addFloatArg(_tracks[trackID].getRatio());
			message.addFloatArg(0);
			message.addFloatArg(_tracks[trackID].getPan());
			message.addFloatArg(8);
			message.addIntArg(_sample);
			_sender.sendMessage(message);
		}

		//if opening track
		else if (_arcs[trackID].getClosed() == true && on == false)
		{
			//Move the arc
			_arcs[trackID].moveArc(_arcs[trackID].getRadius(), _arcs[trackID].getWidth() + 8, _arcs[trackID].getRotation(), BASEMOVEMENTSPEED, true);
			//change status of the arc
			_arcs[trackID].setClosed(false);

			//track
			_tracks[trackID].setLevel(BASELEVEL);
			ofxOscMessage message;
			message.setAddress("/Change");
			message.addFloatArg(trackID);
			message.addFloatArg(_tracks[trackID].getRatio());
			message.addFloatArg(BASELEVEL);
			message.addFloatArg(_tracks[trackID].getPan());
			message.addFloatArg(8);
			message.addIntArg(_sample);
			_sender.sendMessage(message);
		}
}

//destructor
ofApp::~ofApp()
{
	//Delete all event pointers
	for (int i = 0; i < _allEvents.size(); i++)
	{
		delete _allEvents[i];
	}
}