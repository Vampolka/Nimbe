/*
eventCenterOpacityFade.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the opacity of the centerpiece of the visuals, over a certain duration.
*/

#pragma once
#include "baseEvent.h"


class eventCenterOpacityFade :
	public baseEvent
{
public:
	//constructor. Takes a timing, a duration, the opacity at the start and end of the event, and pointers from ofApp
	eventCenterOpacityFade(float timing, float duration, float opacityStart, float opacityEnd, ofColor *baseColor, ofColor *backgroundColor, ofColor *centerColor);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);
	
	//destructor
	~eventCenterOpacityFade();

private:
	//variables
	float _opacityStart;
	float _opacityEnd;

	//pointers to ofApp
	ofColor *_baseColor;
	ofColor *_backgroundColor;
	ofColor *_centerColor;

	
};

