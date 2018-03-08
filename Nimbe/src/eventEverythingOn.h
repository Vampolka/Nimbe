/*
eventEverythingOn.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This even turns everything on, to be used after a breakdown with eventEverythingOff
*/

#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"
#include "track.h"
#include "definitions.h"

class eventEverythingOn :
	public baseEvent
{
public:
	//constructor, takes a timing, the new color of the back, front, animation, the new draw mode, oacity, and chord, as well as pointers from ofApp
	eventEverythingOn(float timing, ofColor backColor, ofColor frontColor, ofColor animationColor, int newDrawMode, float opacity, int chord, int *drawMode, ofColor *backgroundColor, ofColor *baseColor, ofColor *centerColor, float *globalOpacity, int *mode, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventEverythingOn();

private:
	//variables
	ofColor _backColor;
	ofColor _frontColor;
	ofColor _animationColor;
	int _newDrawMode;
	float _opacity;
	int _chord;

	//pointers from ofApp
	int *_drawMode;
	ofColor *_backgroundColor;
	ofColor *_baseColor;
	ofColor *_centerColor;
	float *_globalOpacity;
	int *_mode;
	int *_trackAmount;
	track(*_tracks)[MAXTRACKS];
	float *_volumeGlobal;
	int *_sample;



};

