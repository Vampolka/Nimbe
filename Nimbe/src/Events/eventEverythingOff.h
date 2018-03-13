/*
eventEverythingOff.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event shuts everything down, the sound and the graphics, with only the background left. It's used for a breakdown or at the end.
*/

#pragma once
#include "baseEvent.h"
#include "track.h"
#include "definitions.h"

class eventEverythingOff :
	public baseEvent
{
public:
	//constructor. Takes a timing, the color to fade to, and parameters from ofApp
	eventEverythingOff(float timing, ofColor fadeTo, ofColor *baseColor, ofColor *centerColor, ofColor *backgroundColor, double *globalOpacity, bool *start, bool *end, int *trackAmount, track(*tracks)[MAXTRACKS], int *mode, int *sample);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventEverythingOff();

private:
	//variables
	ofColor _fadeTo;

	//pointers from ofApp
	ofColor *_baseColor;
	ofColor *_centerColor;
	ofColor *_backgroundColor;
	bool *_start;
	bool *_end;
	int *_trackAmount;
	track(*_tracks)[MAXTRACKS];
	int *_mode;
	int *_sample;
	double *_globalOpacity;


};

