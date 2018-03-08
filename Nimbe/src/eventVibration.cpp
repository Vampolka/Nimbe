/*
eventVibration.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the vibration or "shaking" of the arcs, overtime.
*/

#include "eventVibration.h"

//constructor, takes the timing, duration, the next vertical shaking factor and the next horizontal shaking factor (horizontal = parallel to the other arcs, vertical = perpendicular), as well as the respective pointers from ofApp.
eventVibration::eventVibration(float timing, float duration, float endVertikal, float endHorizont, float *vertikalFactor, float *horizontFactor)
{
	_stage = 0;
	_timeElapsed = 0;

	_timing = timing;
	_duration = duration;
	_endVertikal = endVertikal;
	_endHorizont = endHorizont;

	_vertikalFactor = vertikalFactor;
	_horizontFactor = horizontFactor;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventVibration::activateEvent(double timeElapsedFrame)
{
	//first execution
	if (_stage == 0)
	{
		_stage = 1;
		_timeElapsed = 0;

		_startVertikal = *_vertikalFactor;
		_startHorizont = *_horizontFactor;
	}
	//core of the event
	else if (_stage == 1)
	{
		_timeElapsed += timeElapsedFrame;
		double temp = (cos(ofDegToRad((_timeElapsed / _duration) * 180 + 180)) + 1) / 2;
		//change the factors accordingly to the time elapsed
		*_vertikalFactor = temp*_endVertikal + (1 - temp)*_startVertikal;
		*_horizontFactor = temp*_endHorizont + (1 - temp)*_startHorizont;

		//check if duration is over
		if (_timeElapsed > _duration)
		{
			_stage = 2;
			*_vertikalFactor = _endVertikal;
			*_horizontFactor = _endHorizont;
		}
	}

	//if event is over, return false
	if (_stage == 2)
		return false;
	else
		return true;
}

//destructor
eventVibration::~eventVibration()
{
}
