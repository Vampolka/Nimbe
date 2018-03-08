/*
eventChangeWiggle.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the "wiggle" (back and forth natural movement) that the arcs do permanently.
*/

#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"

class eventChangeWiggle :
	public baseEvent
{
public:
	//constructor, takes a timing, duration, the new wiggle speed and the new wiggle range, and variables from ofApp
	eventChangeWiggle(float timing, float duration, float newWiggleSpeed, float newWiggleRange, float *wiggleSpeed, float *wiggleRange);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventChangeWiggle();

private:
	//variables
	float _oldWiggleSpeed;
	float _oldWiggleRange;
	float _newWiggleSpeed;
	float _newWiggleRange;
	float _curveOfChange;

	//pointers to variables from ofApp
	float *_wiggleSpeed;
	float *_wiggleRange;
};

