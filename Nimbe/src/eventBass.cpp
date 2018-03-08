/*
eventBass.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the rotation  of the centerpiece of the visuals, over a certain duration.
Since the center represents the bass, and moves accordingly to the bass, the event is called eventBass.
*/

#include "eventBass.h"
#include <math.h>


//constructor
//Takes a timing, a duration, and the wanted center "speed" at the end of the event. The other parameters are pointers from ofApp.
eventBass::eventBass(float timing, float duration, float endSpeed, double *vitesseCentre, double *centreFactor, double *centreOpacity)
{
	_timing = timing;
	_duration = duration;
	_endSpeed = endSpeed;
	_stage = 0;
	_timeElapsed = 0;

	//Pointers
	_centerSpeed = vitesseCentre;
	_centreFactor = centreFactor;
	_centreOpacity = centreOpacity;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventBass::activateEvent(double timeElapsedFrame)
{
	//first execution of the event
	if (_stage == 0)
	{
		_startSpeed = *_centerSpeed;
		_stage = 1;
	}
	//core of the event
	else if (_stage == 1)
	{
		_timeElapsed += timeElapsedFrame;
		
		//calculate the center speeds/factors/opacity during the middle of the vent
		double temp = (cos(ofDegToRad((_timeElapsed / _duration) * 180 + 180)) + 1) / 2;
		*_centerSpeed = temp*_endSpeed + (1 - temp)*_startSpeed;
		*_centreFactor = pow(*_centerSpeed / 8, 0.5);
		*_centreOpacity = pow(*_centerSpeed / 8, 0.5);

		//if the event has finished
		if (_timeElapsed > _duration)
		{
			_stage = 2;
			*_centerSpeed = _endSpeed;
			*_centreFactor = pow(*_centerSpeed / 8, 0.5);
			*_centreOpacity = pow(*_centerSpeed / 8, 0.5);
		}
	}
	
	//check if the event has finished
	if (_stage == 2)
		return false;
	else
		return true;

}


eventBass::~eventBass()
{
}
