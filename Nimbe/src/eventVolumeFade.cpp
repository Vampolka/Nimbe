/*
eventVolumeFade.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the global volume, fading it up or down over the course of the duration.
*/

#include "eventVolumeFade.h"

//constructor, takes the timing, duration, the volume at the end of the event and variables from ofApp
eventVolumeFade::eventVolumeFade(float timing, float duration, float volumeEnd, float *globalVolume, int *trackAmount, track(*tracks)[MAXTRACKS], int *sample, int *mode)
{
	_stage = 0;
	_timeElapsed = 0;
	_sender.setup("localhost", 5678);

	_timing = timing;
	_duration = duration;
	_volumeEnd = volumeEnd;

	_globalVolume = globalVolume;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_sample = sample;
	_mode = mode;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventVolumeFade::activateEvent(double timeElapsedFrame)
{
	//executes only once
	*_globalVolume = _volumeEnd;

	//send OSC to supercollider to change the volume
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

	//executes only once, so always returns false
	return false;
}

//destructor
eventVolumeFade::~eventVolumeFade()
{
}
