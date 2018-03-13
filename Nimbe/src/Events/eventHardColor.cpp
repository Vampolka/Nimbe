/*
eventHardColor.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the color of the arcs, background and movements, fading from the previous colors over a chosen duration.
*/

#include "eventHardColor.h"

//constructor, takes a timing, duration, the new color of the background, the new color of the arcs, the new color of the animated arcs, and the new chord. THe other variables are pointers from ofApp
eventHardColor::eventHardColor(float timing, float duration, ofColor newBackColor, ofColor newFrontColor, ofColor newAnimationColor, float chord, ofColor *backColor, ofColor *frontColor, ofColor *animationColor, ofColor *centerColor, ofColor *oldFrontColor, ofColor *oldBackColor, ofColor *oldAnimationColor, ofColor *oldCenterColor, int *mode, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample)
{
	_stage = 0;
	_timeElapsed = 0;
	_sender.setup("localhost", 5678);

	_timing = timing;
	_duration = duration;
	_newBackColor = newBackColor;
	_newFrontColor = newFrontColor;
	_newAnimationColor = newAnimationColor;
	_chord = chord;

	_backColor = backColor;
	_frontColor = frontColor;
	_animationColor = animationColor;
	_centerColor = centerColor;
	_oldBackColor = oldBackColor;
	_oldFrontColor = oldFrontColor;
	_oldAnimationColor = oldAnimationColor;
	_oldCenterColor = oldCenterColor;
	_mode = mode;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_volumeGlobal = volumeGlobal;
	_sample = sample;

}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventHardColor::activateEvent(double timeElapsedFrame)
{
	//first execution of the event
	if (_stage == 0)
	{
		_stage = 1;
		_timeElapsed = 0;
		*_oldAnimationColor = *_animationColor;
		*_oldBackColor = *_backColor;
		*_oldFrontColor = *_frontColor;
		*_oldCenterColor = *_centerColor;
		*_mode = _chord;

		//send OSC to supercollider to change the chord
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
	//core of the event
	else if (_stage == 1)
	{
		_timeElapsed += timeElapsedFrame;
		double temp = (cos(ofDegToRad((_timeElapsed / _duration) * 180 + 180)) + 1) / 2;

		//change all colors to their new value during the transition

		(*_backColor).r = (*_oldBackColor).r*(1 - temp) + temp*(_newBackColor).r;
		(*_backColor).g = (*_oldBackColor).g*(1 - temp) + temp*(_newBackColor).g;
		(*_backColor).b = (*_oldBackColor).b*(1 - temp) + temp*(_newBackColor).b;
		ofBackground(*_backColor);

		(*_animationColor).r = (*_oldAnimationColor).r*(1 - temp) + temp*(_newAnimationColor).r;
		(*_animationColor).g = (*_oldAnimationColor).g*(1 - temp) + temp*(_newAnimationColor).g;
		(*_animationColor).b = (*_oldAnimationColor).b*(1 - temp) + temp*(_newAnimationColor).b;

		(*_frontColor).r = (*_oldFrontColor).r*(1 - temp) + temp*(_newFrontColor).r;
		(*_frontColor).g = (*_oldFrontColor).g*(1 - temp) + temp*(_newFrontColor).g;
		(*_frontColor).b = (*_oldFrontColor).b*(1 - temp) + temp*(_newFrontColor).b;

		(*_centerColor).r = (*_oldCenterColor).r*(1 - temp) + temp*(_newFrontColor).r;
		(*_centerColor).g = (*_oldCenterColor).g*(1 - temp) + temp*(_newFrontColor).g;
		(*_centerColor).b = (*_oldCenterColor).b*(1 - temp) + temp*(_newFrontColor).b;

		//check if duration is over
		if (_timeElapsed > _duration)
		{
			_stage = 2;
		}
	}

	//return false if event has ended
	if (_stage == 2)
		return false;
	else
		return true;
}

eventHardColor::~eventHardColor()
{
}
