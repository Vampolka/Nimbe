/*
baseEvent.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Parent of all Events that will be called during the performance to change the visuals or audio in the timeline
Note that while many different events can be called at the same time if they are of different types,
the duration of different instances of the same type of event must not overlap.

All events will use a few different parameters as well as a few or many pointers to variables from ofApp in their constructor.
That is so that activateEvent can be used using only one parameter, the same for every type of event (timeElapsedFrame), and to enable the use of polymorphism.
ActivateEvent will change the value of those pointers when necessary, but they will not be deleted in the destructor because they variables are owned by ofApp.
*/

#include "baseEvent.h"

//constructor
baseEvent::baseEvent()
{
	_timing = 0;
	_sender.setup("localhost", OSCPORT);
}

//Returns the timing of the event (the time at which the event will trigger)
float baseEvent::getTiming()
{
	return _timing;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
bool baseEvent::activateEvent(double timeElapsedFrame)
{
	return false;
}

//destructor
baseEvent::~baseEvent()
{
}
