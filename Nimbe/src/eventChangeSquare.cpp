/*
eventChangeSquare.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

this event changes the size the squares that forms the arc. It also changes the samples that are used by the tracks at the same time..
*/

#include "eventChangeSquare.h"

//constructor, uses timing, duration, the space of the new square (1 being full arc, less being squares), and the next sample to play. The rest are pointers from ofApp
eventChangeSquare::eventChangeSquare(float timing, float duration, float newSquare, int sampleAfter, float *spaceSquares, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample)
{
	_stage = 0;
	_timeElapsed = 0;
	_sender.setup("localhost", 5678);

	_timing = timing;
	_duration = duration;
	_newSquare = newSquare;
	_sampleAfter = sampleAfter;

	_spaceSquares = spaceSquares;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_volumeGlobal = volumeGlobal;
	_sample = sample;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventChangeSquare::activateEvent(double timeElapsedFrame)
{
	//first call of the activation, executes only once
	if (_stage == 0)
	{
		_stage = 1;
		_timeElapsed = 0;
		_oldSquare = *_spaceSquares;

		*_sample = _sampleAfter;

		//send OSC to supercollider to change the sample
		for (int i = 0; i < *_trackAmount; i++)
		{
			ofxOscMessage message;
			message.setAddress("/ChangeSample");
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
		*_spaceSquares = temp*_newSquare + (1 - temp)*_oldSquare;
		//check if the event has finished
		if (_timeElapsed > _duration)
		{
			_stage = 2;
			*_spaceSquares = _newSquare;
		}
	}

	//return false if the event has finished
	if (_stage == 2)
		return false;
	else
		return true;
}

//destructor
eventChangeSquare::~eventChangeSquare()
{
}
