/*
eventEverythingOff.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event shuts everything down, the sound and the graphics, with only the background left. It's used for a breakdown or at the end.
*/

#include "eventEverythingOff.h"

//constructor. Takes a timing, the color to fade to, and parameters from ofApp
eventEverythingOff::eventEverythingOff(float timing, ofColor fadeTo, ofColor *baseColor, ofColor *centerColor, ofColor *backgroundColor, double *globalOpacity, bool *start, bool *end, int *trackAmount, track(*tracks)[MAXTRACKS], int *mode, int *sample)
{
	_stage = 0;
	_timeElapsed = 0;

	_timing = timing;
	_fadeTo = fadeTo;

	_baseColor = baseColor;
	_centerColor = centerColor;
	_backgroundColor = backgroundColor;
	_globalOpacity = globalOpacity;
	_start = start;
	_end = end;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_mode = mode;
	_sample = sample;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventEverythingOff::activateEvent(double timeElapsedFrame)
{
	*_start = false;
	*_end = true;

	//change the color and opacity of everything to make everythign go to the fadeTo color
	*_baseColor = *_centerColor = *_backgroundColor = _fadeTo;
	ofBackground(*_backgroundColor);
	*_globalOpacity = 0;

	//send OSC messages to close all tracks
	for (int i = 0; i < *_trackAmount; i++)
	{
		(*_tracks)[i].changeRatio((*_tracks)[i].getRatioRanking(), *_mode);
		ofxOscMessage message;
		message.setAddress("/Change");
		message.addFloatArg(i);
		message.addFloatArg((*_tracks)[i].getRatio());
		message.addFloatArg(0);
		message.addFloatArg((*_tracks)[i].getPan());
		message.addFloatArg(0.1);
		message.addIntArg(*_sample);
		_sender.sendMessage(message);
	}
	//executes only once
	return false;
}

//destructor
eventEverythingOff::~eventEverythingOff()
{
}
