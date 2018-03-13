/*
eventVolumeFade.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the global volume, fading it up or down over the course of the duration.
*/

#pragma once
#include "baseEvent.h"
#include "track.h"
#include "definitions.h"

class eventVolumeFade :
	public baseEvent
{
public:

	//constructor, takes the timing, duration, the volume at the end of the event and variables from ofApp
	eventVolumeFade(float timing, float duration, float volumeEnd, float *globalVolume, int *trackAmount, track(*tracks)[MAXTRACKS], int *sample, int *mode);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventVolumeFade();

private:
	//variables
	float _volumeEnd;

	//pointers from ofApp
	float *_globalVolume;
	int *_trackAmount;
	track(*_tracks)[MAXTRACKS];
	int *_sample;
	int *_mode;
};

