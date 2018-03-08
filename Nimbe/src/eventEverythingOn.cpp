/*
eventEverythingOn.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This even turns everything on, to be used after a breakdown with eventEverythingOff
*/

#include "eventEverythingOn.h"

//constructor, takes a timing, the new color of the back, front, animation, the new draw mode, oacity, and chord, as well as pointers from ofApp
eventEverythingOn::eventEverythingOn(float timing, ofColor backColor, ofColor frontColor, ofColor animationColor, int newDrawMode, float opacity, int chord, int *drawMode, ofColor *backgroundColor, ofColor *baseColor, ofColor *centerColor, float *globalOpacity, int *mode, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample)
{
	_stage = 0;
	_timeElapsed = 0;

	_timing = timing;
	_backColor = backColor;
	_frontColor = frontColor;
	_animationColor = animationColor;
	_newDrawMode = newDrawMode;
	_opacity = opacity;
	_chord = chord;


	_drawMode = drawMode;
	_backgroundColor = backgroundColor;
	_baseColor = baseColor;
	_centerColor = centerColor;
	_globalOpacity = globalOpacity;
	_mode = mode;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_volumeGlobal = volumeGlobal;
	_sample = sample;
	

}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventEverythingOn::activateEvent(double timeElapsedFrame)
{
	//change the colors and mode to the new ones
	*_backgroundColor = _backColor;
	ofBackground(*_backgroundColor);
	*_baseColor = *_centerColor = _frontColor;
	*_globalOpacity = _opacity;
	*_drawMode = _newDrawMode;
	_animationColor = _animationColor;
	*_mode = _chord;
	*_volumeGlobal = 0;

	//send OSC to open all tracks
	for (int i = 0; i < *_trackAmount; i++)
	{
		(*_tracks)[i].changeRatio((*_tracks)[i].getRatioRanking(), *_mode);

		ofxOscMessage message;
		message.setAddress("/Change");
		message.addFloatArg(i);
		message.addFloatArg((*_tracks)[i].getRatio());
		message.addFloatArg((*_tracks)[i].getLevel()*(*_volumeGlobal));
		message.addFloatArg((*_tracks)[i].getPan());
		message.addFloatArg(0.1);
		message.addIntArg(*_sample);
		_sender.sendMessage(message);
	}
	//executes only once
	return false;
}

//destructor
eventEverythingOn::~eventEverythingOn()
{
}
