/*
eventFluidTransition.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This even changes the drawmode to another, changing the type of square, rather than the size of the square. It is EventChangeSquare's counterpart. It also changes the sample of the tracks.
*/

#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"
#include "track.h"
#include "definitions.h"

class eventFluidTransition :
	public baseEvent
{
public:
	//constructor. Takes a timing, duration, the next drawmode, the next sample, and pointers from ofApp
	eventFluidTransition(float timing, float duration, int drawModeAfter, int sampleAfter, int* drawMode, bool *fadeDrawMode, int *previousDrawMode, double *transitionDrawModeDegree, int *mode, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventFluidTransition();

private:
	//variables
	int _drawModeAfter;
	int _sampleAfter;

	//pointers from ofApp
	int *_drawMode;
	bool *_fadeDrawMode;
	int *_previousDrawMode;
	double *_transitionDrawModeDegree;
	int *_mode;
	int *_trackAmount;
	track(*_tracks)[MAXTRACKS];
	float *_volumeGlobal;
	int *_sample;
};

