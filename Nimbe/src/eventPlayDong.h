/*
eventPlayDong.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

//This event triggers a large bell sound
*/

#pragma once
#include "D:\OpenFrameworks\of_v0.9.8_vs_release\apps\myApps\Nimbe\src\baseEvent.h"

class eventPlayDong :
	public baseEvent
{
public:
	//constructor, only takes a timing
	eventPlayDong(float timing);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventPlayDong();

};

