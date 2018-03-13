/*
eventOpacityFade.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the opecity of the arcs overtime. It is used at the beginning to fade in the arcs.
*/

#pragma once
#include "baseEvent.h"

class eventOpacityFade :
	public baseEvent
{
public:
	//constructor. Takes a timing, duration, the new opacity, and a pointer to globalOpacity from ofApp
	eventOpacityFade(float timing, float duration, float opacityEnd, double *globalOpacity);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventOpacityFade();

private:
	//variables
	float _opacityStart;
	float _opacityEnd;

	//pointers to ofApp
	double *_globalOpacity;
};

