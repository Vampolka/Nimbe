/*
eventChangeChord.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the chord that is played by the different tracks. Every note will transpose to its equivalent in the new chord (keeping the same note ranking (see track)), over the chosen duration.
*/

#include "eventChangeChord.h"

//constructor. Takes a timing, a duration, the new mode, and pointers to variables from ofApp
eventChangeChord::eventChangeChord(float timing, float duration, int newMode, float *globalVolume, int *trackAmount, track(*tracks)[MAXTRACKS], int *sample, int *mode)
{
	_stage = 0;
	_timeElapsed = 0;
	_sender.setup("localhost", 5678);

	_timing = timing;
	_duration = duration;
	_newMode = newMode;

	_globalVolume = globalVolume;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_sample = sample;
	_mode = mode;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventChangeChord::activateEvent(double timeElapsedFrame)
{
	*_mode = _newMode;

	//send OSC to superCollider
	for (int i = 0; i < *_trackAmount; i++)
	{
		(*_tracks)[i].changeRatio((*_tracks)[i].getRatioRanking(), *_mode);
		ofxOscMessage message;
		message.setAddress("/Change");
		message.addFloatArg(i);
		message.addFloatArg((*_tracks)[i].getRatio());
		message.addFloatArg((*_tracks)[i].getLevel()*(*_globalVolume));
		message.addFloatArg((*_tracks)[i].getPan());
		message.addFloatArg(_duration);
		message.addIntArg(*_sample);
		_sender.sendMessage(message);
	}
	//this event executes itself only once
	return false;
}

//destructor
eventChangeChord::~eventChangeChord()
{
}
