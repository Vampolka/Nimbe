/*
eventChangeWiggle.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the "wiggle" (back and forth natural movement) that the arcs do permanently.
*/

#include "eventChangeWiggle.h"

//constructor, takes a timing, duration, the new wiggle speed and the new wiggle range, and variables from ofApp
eventChangeWiggle::eventChangeWiggle(float timing, float duration, float newWiggleSpeed, float newWiggleRange, float *wiggleSpeed, float *wiggleRange)
{
	_stage = 0;
	_timeElapsed = 0;

	_timing = timing;
	_duration = duration;
	_newWiggleSpeed = newWiggleSpeed;
	_newWiggleRange = newWiggleRange;

	_wiggleSpeed = wiggleSpeed;
	_wiggleRange = wiggleRange;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventChangeWiggle::activateEvent(double timeElapsedFrame)
{
	//first execution
	if (_stage == 0)
	{
		_stage = 1;
		_timeElapsed = 0;
		_oldWiggleSpeed = *_wiggleSpeed;
		_oldWiggleRange = *_wiggleRange;
	}
	//core of the event
	else if (_stage == 1)
	{
		_timeElapsed += timeElapsedFrame;
		double temp = (cos(ofDegToRad((_timeElapsed / _duration) * 180 + 180)) + 1) / 2;
		//adjust the wigglespeed and range to their temporary values duriont the transition
		*_wiggleSpeed = temp*_newWiggleSpeed + (1 - temp)*_oldWiggleSpeed;
		*_wiggleRange = temp*_newWiggleRange + (1 - temp)*_oldWiggleRange;

		//check if event has ended
		if (_timeElapsed > _duration)
		{
			_stage = 2;
			*_wiggleSpeed = _newWiggleSpeed;
			*_wiggleRange = _newWiggleRange;
		}
	}

	//return false if the event has ended
	if (_stage == 2)
		return false;
	else
		return true;
}

//destructor
eventChangeWiggle::~eventChangeWiggle()
{
}
