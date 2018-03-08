#include "eventFluidTransition.h"


//constructor. Takes a timing, duration, the next drawmode, the next sample, and pointers from ofApp
eventFluidTransition::eventFluidTransition(float timing, float duration, int drawModeAfter, int sampleAfter, int* drawMode, bool *fadeDrawMode, int *previousDrawMode, double *transitionDrawModeDegree, int *mode, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample)
{
	_stage = 0;
	_timeElapsed = 0;
	_sender.setup("localhost", 5678);

	_drawModeAfter = drawModeAfter;
	_sampleAfter = sampleAfter;
	_timing = timing;
	_duration = duration;

	_drawMode = drawMode;
	_fadeDrawMode = fadeDrawMode;
	_previousDrawMode = previousDrawMode;
	_transitionDrawModeDegree = transitionDrawModeDegree;
	_mode = mode;
	_trackAmount = trackAmount;
	_tracks = tracks;
	_volumeGlobal = volumeGlobal;
	_sample = sample;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventFluidTransition::activateEvent(double timeElapsedFrame)
{
	//first execution of the event
	if (_stage == 0)
	{
		_stage = 1;
		_timeElapsed = 0;

		*_fadeDrawMode = true;
		*_previousDrawMode = *_drawMode;
		*_drawMode = _drawModeAfter;
		*_transitionDrawModeDegree = 0;

		*_sample = _sampleAfter;

		//send OSC to supercollider to change the samples
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

		double temp = _timeElapsed / _duration;
		*_transitionDrawModeDegree = temp;

		//check if duration is over
		if (_timeElapsed > _duration)
		{
			_stage = 2;
			*_fadeDrawMode = false;
		}
	}

	//if event has finished, return false
	if (_stage == 2)
		return false;
	else
		return true;
}

//destructor
eventFluidTransition::~eventFluidTransition()
{
}
