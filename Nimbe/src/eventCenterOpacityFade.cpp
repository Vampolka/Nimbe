/*
eventCenterOpacityFade.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the opacity of the centerpiece of the visuals, over a certain duration.
*/

#include "eventCenterOpacityFade.h"

	//constructor. Takes a timing, a duration, the opacity at end of the event, and pointers from ofApp
eventCenterOpacityFade::eventCenterOpacityFade(float timing, float duration, float opacityStart, float opacityEnd, ofColor *baseColor, ofColor *backgroundColor, ofColor *centerColor)
{
	_stage = 0;
	_timeElapsed = 0;

	_timing = timing;
	_duration = duration;
	_opacityStart = opacityStart;
	_opacityEnd = opacityEnd;

	_baseColor = baseColor;
	_backgroundColor = backgroundColor;
	_centerColor = centerColor;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventCenterOpacityFade::activateEvent(double timeElapsedFrame)
{
	//first execution of activateEvent
	if (_stage == 0)
	{
		_stage = 1;
		(*_centerColor).r = round((*_baseColor).r*_opacityStart + (*_backgroundColor).r*(1 - _opacityStart));
		(*_centerColor).g = round((*_baseColor).g*_opacityStart + (*_backgroundColor).g*(1 - _opacityStart));
		(*_centerColor).b = round((*_baseColor).b*_opacityStart + (*_backgroundColor).b*(1 - _opacityStart));
		_timeElapsed = 0;
	}
	//core of the event
	else if (_stage == 1)
	{
		_timeElapsed += timeElapsedFrame;
		double temp = (cos(ofDegToRad((_timeElapsed / _duration) * 180 + 180)) + 1) / 2;
		(*_centerColor).r = round(((*_baseColor).r*_opacityStart + (*_backgroundColor).r*(1 - _opacityStart))*(1 - temp) + ((*_baseColor).r*_opacityEnd + (*_backgroundColor).r*(1 - _opacityEnd))*(temp));
		(*_centerColor).g = round(((*_baseColor).g*_opacityStart + (*_backgroundColor).g*(1 - _opacityStart))*(1 - temp) + ((*_baseColor).g*_opacityEnd + (*_backgroundColor).g*(1 - _opacityEnd))*(temp));
		(*_centerColor).b = round(((*_baseColor).b*_opacityStart + (*_backgroundColor).b*(1 - _opacityStart))*(1 - temp) + ((*_baseColor).b*_opacityEnd + (*_backgroundColor).b*(1 - _opacityEnd))*(temp));

		//if duration is completed
		if (_timeElapsed > _duration)
		{
			_stage = 2;
			(*_centerColor).r = round(((*_baseColor).r*_opacityEnd + (*_backgroundColor).r*(1 - _opacityEnd)));
			(*_centerColor).g = round(((*_baseColor).g*_opacityEnd + (*_backgroundColor).g*(1 - _opacityEnd)));
			(*_centerColor).b = round(((*_baseColor).b*_opacityEnd + (*_backgroundColor).b*(1 - _opacityEnd)));
		}
	}

	//check if event has finished
	if (_stage == 2)
		return false;
	else
		return true;
}

eventCenterOpacityFade::~eventCenterOpacityFade()
{
}
