/*
eventPlayDong.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

//This event triggers a large bell sound
*/

#include "eventPlayDong.h"

//constructor, only takes a timing
eventPlayDong::eventPlayDong(float timing)
{
	_stage = 0;
	_timeElapsed = 0;
	_sender.setup("localhost", 5678);
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventPlayDong::activateEvent(double timeElapsedFrame)
{
		ofxOscMessage message;
		message.setAddress("/Dong");
		_sender.sendMessage(message);
		return false;
}

//destructor
eventPlayDong::~eventPlayDong()
{
}
