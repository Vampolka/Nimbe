/*
eventBass.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event changes the rotation  of the centerpiece of the visuals, over a certain duration.
Since the center represents the bass, and moves accordingly to the bass, the event is called eventBass.
*/

#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"

class eventBass :
	public baseEvent
{
public:
	//constructor
	//Takes a timing, a duration, and the wanted center "speed" at the end of the event. The other parameters are pointers from ofApp.
	eventBass(float timing, float duration, float endSpeed, double *centerSpeed, double *centreFactor, double *centreOpacity);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventBass();

private:
	//Variables
	float _startSpeed;
	float _endSpeed;

	//pointers to ofApp
	double *_centerSpeed;
	double *_centreFactor;
	double *_centreOpacity;


};

