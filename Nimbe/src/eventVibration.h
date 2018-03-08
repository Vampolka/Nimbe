/*
eventVibration.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the vibration or "shaking" of the arcs, overtime.
*/

#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"

class eventVibration :
	public baseEvent
{
public:
	//constructor, takes the timing, duration, the next vertical shaking factor and the next horizontal shaking factor (horizontal = parallel to the other arcs, vertical = perpendicular), as well as the respective pointers from ofApp.
	eventVibration(float timing, float duration, float endVertikal, float endHorizont, float *vertikalFactor, float *horizontFactor);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventVibration();

private:
	//variables
	float _startVertikal;
	float _endVertikal;
	float _startHorizont;
	float _endHorizont;

	//pointers from ofApp
	float *_vertikalFactor;
	float *_horizontFactor;


};

