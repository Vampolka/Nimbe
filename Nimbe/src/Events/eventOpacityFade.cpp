/*
eventOpacityFade.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the opecity of the arcs overtime. It is used at the beginning to fade in the arcs.
*/

#include "eventOpacityFade.h"

//constructor. Takes a timing, duration, the new opacity, and a pointer to globalOpacity from ofApp
eventOpacityFade::eventOpacityFade(float timing, float duration, float opacityEnd, double *globalOpacity)
{
	_stage = 0;
	_timeElapsed = 0;

	_timing = timing;
	_duration = duration;
	_opacityEnd = opacityEnd;

	_globalOpacity = globalOpacity;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventOpacityFade::activateEvent(double timeElapsedFrame)
{
	//executed only once
	if (_stage == 0)
	{
		_opacityStart = *_globalOpacity;
		_stage = 1;
		*_globalOpacity = _opacityStart;
		_timeElapsed = 0;
	}
	//core of the event
	else if (_stage == 1)
	{
		_timeElapsed += timeElapsedFrame;
		double temp = (cos(ofDegToRad((_timeElapsed / _duration) * 180 + 180)) + 1) / 2;
		*_globalOpacity = pow(temp, 0.5)*_opacityEnd + (1 - pow(temp, 0.5))*_opacityStart;

		//check if duration is over
		if (_timeElapsed > _duration)
		{
			_stage = 2;
			*_globalOpacity = _opacityEnd;
		}
	}
	//if event has ended, return false
	if (_stage == 2)
		return false;
	else
		return true;
}

//destructor
eventOpacityFade::~eventOpacityFade()
{
}
