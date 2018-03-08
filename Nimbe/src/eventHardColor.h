#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"
#include "track.h"

//This Event changes the colors of the arcs, background and moving arcs over a duration.
class eventHardColor :
	public baseEvent
{
public:
	//constructor, takes a timing, duration, the new color of the background, the new color of the arcs, the new color of the animated arcs, and the new chord. THe other variables are pointers from ofApp
	eventHardColor(float timing, float duration, ofColor newBackColor, ofColor newFrontColor, ofColor newAnimationColor, float chord, ofColor *backColor, ofColor *frontColor, ofColor *animationColor, ofColor *centerColor, ofColor *oldFrontColor, ofColor *oldBackColor, ofColor *oldAnimationColor, ofColor *oldCenterColor, int *mode, int *trackAmount, track(*tracks)[MAXTRACKS], float *volumeGlobal, int *sample);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventHardColor();

private:
	//variables
	ofColor _newBackColor;
	ofColor _newFrontColor;
	ofColor _newAnimationColor;
	float _chord;

	//pointers from ofApp
	ofColor *_backColor;
	ofColor *_frontColor;
	ofColor *_animationColor;
	ofColor *_centerColor;
	ofColor *_oldBackColor;
	ofColor *_oldFrontColor;
	ofColor *_oldAnimationColor;
	ofColor *_oldCenterColor;
	int *_mode;
	int *_trackAmount;
	track(*_tracks)[MAXTRACKS];
	float *_volumeGlobal;
	int *_sample;

};

