/*
eventChangeChord.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the chord that is played by the different tracks. Every note will transpose to its equivalent in the new chord (keeping the same note ranking (see track)), over the chosen duration.
*/

#pragma once
#include "baseEvent.h"
#include "track.h"
#include "definitions.h"

class eventChangeChord :
	public baseEvent
{
public:
	//constructor. Takes a timing, a duration, the new mode, and pointers to variables from ofApp
	eventChangeChord(float timing, float duration, int newMode, float *globalVolume, int *trackAmount, track(*tracks)[MAXTRACKS], int *sample, int *mode);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventChangeChord();

private:
	//variables
	int _newMode; //the new mode

	//pointers to ofApp
	float *_globalVolume;
	int *_trackAmount;
	track(*_tracks)[MAXTRACKS];
	int *_sample;
	int *_mode;
};

