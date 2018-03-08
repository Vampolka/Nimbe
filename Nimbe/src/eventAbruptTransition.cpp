/*
eventAbruptTransition.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the drawmode from one to another abuptly. Instead of fading, it alternates between one mode and the other, proportionally to the time elapsed.
*/

#include "eventAbruptTransition.h"

//Constructor
//Takes a timing, duration, the draw mode it wants to transition to, the mode of the chord it wants to transition into, and many pointers from ofApp
eventAbruptTransition::eventAbruptTransition(float timing, float duration, int drawModeAfter, int chordModeAfter, int* drawMode, bool *transitionDrawMode, int (*transitionDrawModeC)[2], double *transitionDrawModeDegree, int *mode, int *trackAmount, track (*tracks)[MAXTRACKS], float *volumeGlobal, int *sample)
{
	_drawModeAfter = drawModeAfter;
	_chordModeAfter = chordModeAfter;
	_timing = timing;
	_duration = duration;
	_stage = 0;
	_timeElapsed = 0;
	_sender.setup("localhost", 5678);

	_drawMode = drawMode;
	_transitionDrawMode = transitionDrawMode;
	_transitionDrawModeC = transitionDrawModeC;
	_transitionDrawModeDegree = transitionDrawModeDegree;
	_mode = mode;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_volumeGlobal = volumeGlobal;
	_sample = sample;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventAbruptTransition::activateEvent(double timeElapsedFrame)
{
	//Only on the first activation
	if (_stage == 0)
	{
		_drawModeBefore = *_drawMode;
		_stage = 1;
		_timeElapsed = 0;

		*_transitionDrawMode = true;
		(*_transitionDrawModeC)[0] = _drawModeBefore;
		(*_transitionDrawModeC)[1] = _drawModeAfter;
		*_transitionDrawModeDegree = 0;

		*_mode = _chordModeAfter;

		//Tell supercollider to change the mode
		for (int i = 0; i < *_trackAmount; i++)
		{
			(*_tracks)[i].changeRatio((*_tracks)[i].getRatioRanking(), *_mode);
			ofxOscMessage message;
			message.setAddress("/Change");
			message.addFloatArg(i);
			message.addFloatArg((*_tracks)[i].getRatio());
			message.addFloatArg((*_tracks)[i].getLevel()*(*_volumeGlobal));
			message.addFloatArg((*_tracks)[i].getPan());
			message.addFloatArg(_duration);
			message.addIntArg(*_sample);
			_sender.sendMessage(message);
		}

	}
	//Core of the event
	else if (_stage == 1)
	{
		double durationEvent = _duration;
		_timeElapsed += timeElapsedFrame;
		double temp = (cos(ofDegToRad((_timeElapsed / durationEvent) * 180 + 180)) + 1) / 2;

		if (temp < 0.5)
			temp = pow((temp * 2), 4) / 2;
		else if (temp > 0.5)
			temp = 1 - (pow((1 - temp) * 2, 2) / 2);

		*_transitionDrawModeDegree = temp;

		if (_timeElapsed > durationEvent)
		{
			_stage = 2;
			*_drawMode = _drawModeAfter;
			*_transitionDrawMode = false;
		}
	}

	//Check if the event has finished or not
	if (_stage == 2)
		return false;
	else
		return true;
}

//destructor
eventAbruptTransition::~eventAbruptTransition()
{
}
