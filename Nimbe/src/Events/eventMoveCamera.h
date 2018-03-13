/*
eventMoveCamera.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event moves the camera, so that the point of view changes. It can change the position of the camera and the direction it looks at.
*/

#pragma once
#include "baseEvent.h"

class eventMoveCamera :
	public baseEvent
{
public:
	//constructor, takes a timing, duration, the ending position of the camera, the ending target of the camera (where it looks), and whether to use the camera distance (z axis). The rest of the parameters are pointers from ofApp
	eventMoveCamera(float timing, float duration, ofVec3f positionEnd, ofVec3f targetEnd, bool useCameraDistance, ofEasyCam *camera, bool *cameraMode, double *cameraDistance);

	//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
	//timeElaspedFrame is the time that was elapsed during the last frame
	//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
	bool activateEvent(double timeElapsedFrame);

	//destructor
	~eventMoveCamera();

private:
	//variables
	ofVec3f _positionEnd;
	ofVec3f _targetEnd;
	bool _useCameraDistance;
	ofVec3f _positionStart;
	ofVec3f _targetStart;

	//pointers from ofApp
	ofEasyCam *_camera;
	bool *_cameraMode;
	double *_cameraDistance;

	
};

