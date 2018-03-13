/*
eventChangeSquare.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

this event changes the size the squares that forms the arc. It also changes the samples that are used by the tracks at the same time..
*/

#pragma once
#include "baseEvent.h"
#include "track.h"
#include "definitions.h"

class eventChangeSquare :
	public baseEvent
{
public:
	//constructor, uses timing, duration, the space of the new square (1 being full arc, less being squares), and the next sample to play. The rest are pointers from ofApp
	eventChangeSquare(float timing, float duration, float newSquare, int sampleAfter, float *spaceSquares, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample);


	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventChangeSquare();

private:
	//variables
	float _oldSquare;
	float _newSquare;
	int _sampleAfter;

	//pointers from ofApp
	float *_spaceSquares;
	int *_trackAmount;
	track(*_tracks)[MAXTRACKS];
	float *_volumeGlobal;
	int *_sample;

};

