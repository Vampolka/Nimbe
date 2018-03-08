/*
baseEvent.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Parent of all Events that will be called during the performance to change the visuals or audio in the timeline
Note that while many different events can be called at the same time if they are of different types,
the duration of different instances of the same type of event must not overlap.

All events will use a few different parameters as well as a few or many pointers to variables from ofApp in their constructor. 
That is so that activateEvent can be used using only one parameter, the same for every type of event (timeElapsedFrame), and to enable the use of polymorphism.
ActivateEvent will change the value of those pointers when necessary, but they will not be deleted in the destructor because they variables are owned by ofApp.
*/

#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "definitions.h"

class baseEvent
{
public:
	//Constructor
	baseEvent();

	//Destructor
	~baseEvent();

	//Returns the timing of the event (the time at which the event will trigger)
	float getTiming();

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	virtual bool activateEvent(double timeElapsedFrame);

protected:

	double _timing; //timing at which the events will trigger
	double _duration; //duration of the vent
	int _stage; //the stage is used to know the "section" of the event that should be triggered now. Some events use 1, some many.
	float _timeElapsed; //Time elapsed since the first time the event got activated
	ofxOscSender _sender; //Used to send OSC to supercollider
};

