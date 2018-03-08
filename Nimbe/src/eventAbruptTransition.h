/*
eventAbruptTransition.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the drawmode from one to another abuptly. Instead of fading, it alternates between one mode and the other, proportionally to the time elapsed.
*/

#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"
#include "track.h"

class eventAbruptTransition :
	public baseEvent
{
public:
	//Constructor
	//Takes a timing, duration, the draw mode it wants to transition to, the mode of the chord it wants to transition into, and many pointers from ofApp
	eventAbruptTransition(float timing, float duration, int drawModeAfter, int chordModeAfter, int* drawMode, bool *transitionDrawMode, int (*transitionDrawModeC)[2], double *transitionDrawModeDegree, int *mode, int *trackAmount, track (*tracks)[MAXTRACKS], float *volumeGlobal, int *sample);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventAbruptTransition();

private:
	//The draw modes of before and after the event
	int _drawModeBefore;
	int _drawModeAfter;
	int _chordModeAfter;

	//Pointers from ofApp.
	int *_drawMode;
	bool *_transitionDrawMode;
	int (*_transitionDrawModeC)[2];
	double *_transitionDrawModeDegree;
	int *_mode;
	int *_trackAmount;
	track (*_tracks)[MAXTRACKS];
	float *_volumeGlobal;
	int *_sample;



};

