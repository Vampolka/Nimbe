/*
eventMoveCamera.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This event moves the camera, so that the point of view changes. It can change the position of the camera and the direction it looks at.
*/

#include "eventMoveCamera.h"

//constructor, takes a timing, duration, the ending position of the camera, the ending target of the camera (where it looks), and whether to use the camera distance (z axis). The rest of the parameters are pointers from ofApp
eventMoveCamera::eventMoveCamera(float timing, float duration, ofVec3f positionEnd, ofVec3f targetEnd, bool useCameraDistance, ofEasyCam *camera, bool *cameraMode, double *cameraDistance)
{
	_stage = 0;
	_timeElapsed = 0;

	_timing = timing;
	_duration = duration;

	_positionEnd = positionEnd;
	_targetEnd = targetEnd;
	_useCameraDistance = useCameraDistance;

	_camera = camera;
	_cameraMode = cameraMode;
	_cameraDistance = cameraDistance;
}

//Activates the event. The event has to be activated on every frame when it is ongoing, and only when it is ongoing.
//timeElaspedFrame is the time that was elapsed during the last frame
//returns whether the event has finished or not. "True" means it is still ongoing, "false" means it has finished.
bool eventMoveCamera::activateEvent(double timeElapsedFrame)
{
	//first execution
	if (_stage == 0)
	{
		_stage = 1;
		_timeElapsed = 0;

		*_cameraMode = false;
		_positionStart = (*_camera).getPosition();
		_targetStart = (*_camera).getTarget().getGlobalPosition();

		if (_useCameraDistance == true)
			_positionEnd.z = _positionEnd.z+(*_cameraDistance);

	}
	//core of the event
	else if (_stage == 1)
	{
		_timeElapsed += timeElapsedFrame;
		double temp = (cos(ofDegToRad((_timeElapsed / _duration) * 180 + 180)) + 1) / 2;

		(*_camera).setPosition(ofVec3f(_positionEnd.x*temp + _positionStart.x*(1 - temp), _positionEnd.y*temp + _positionStart.y*(1 - temp), _positionEnd.z*temp + _positionStart.z*(1 - temp)));			
		(*_camera).setTarget(_targetEnd*temp + _targetStart*(1 - temp));

		//check if event is over
		if (_timeElapsed > _duration)
		{
			_stage = 2;

			(*_camera).setTarget(_targetEnd);
			(*_camera).setPosition(_positionEnd);
		}
	}

	//if event is over, return false
	if (_stage == 2)
		return false;
	else
		return true;
}

//destructor
eventMoveCamera::~eventMoveCamera()
{
}
