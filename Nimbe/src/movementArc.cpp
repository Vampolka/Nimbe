/*
movementArc.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This class is used by arc to move itself. Since an arc can have more than one movement at the same time, it is a separate class.
*/

#include "movementArc.h"
#include <iostream>


//Constructor, takes the old and the new coordinates, the duration of the movement and whether the color should change to the movement color while moving.
movementArc::movementArc(double oldRayon, double oldRotation, double oldEpaisseur, double newRayon, double newRotation, double newEpaisseur, double duration, bool couleur)
{
	_boolColor = couleur;

	_oldRadius = oldRayon;
	_oldRotation = oldRotation;
	_oldWidth = oldEpaisseur;
	
	_newRadius = newRayon;
	_newRotation = newRotation;
	_newWidth = newEpaisseur;

	_mouvRadius = _newRadius - _oldRadius;
	_mouvRotation = _newRotation - _oldRotation;
	_mouvWidth = _newWidth-_oldWidth;

	_duration = duration;

	_totalTime = clock();
	_timeElapsedTotal = 0;

	_isFinished = false;

	_movementCompletedWidth = _movementCompletedRadius = _movementCompletedRotation = 0;
}

//Moves the arc
bool movementArc::move()
{
	double timeElapsedTotal, tempsElapsedFrame;
	timeElapsedTotal = clock()-_totalTime;
	timeElapsedTotal /= CLOCKS_PER_SEC;

	tempsElapsedFrame = timeElapsedTotal - _timeElapsedTotal;
	_timeElapsedTotal = timeElapsedTotal;

	//calculate the average movement speed
	double a = (timeElapsedTotal - tempsElapsedFrame);

	if (timeElapsedTotal < _duration)
	{
		double proportionLeft;

		//First half of the movement
		if (timeElapsedTotal <= _duration/2)
		{
			double averageSpeed;
			averageSpeed = (timeElapsedTotal / (_duration / 2)+(timeElapsedTotal-tempsElapsedFrame) / (_duration / 2));
			proportionLeft = (tempsElapsedFrame*averageSpeed) / _duration;
		}
		//Middle of the movement
		else if ((timeElapsedTotal >= _duration / 2) && (timeElapsedTotal <= (_duration / 2 + tempsElapsedFrame)))
		{
			double timePost = timeElapsedTotal - _duration / 2;
			double timePre = tempsElapsedFrame - timePost;
			double averageSpeedPre;
			averageSpeedPre = ((_duration / 2 - timePre) / (_duration / 2) * 2 + 2) / 2;
			double averageSpeedPost;
			averageSpeedPost = ((_duration / 2 - timePost) / (_duration / 2) * 2 + 2) / 2;
			double averageSpeed;
			averageSpeed = (averageSpeedPre*timePre) / tempsElapsedFrame + (averageSpeedPost*timePost) / tempsElapsedFrame;
			proportionLeft = (tempsElapsedFrame*averageSpeed) / _duration;
			
		}
		//Second half of the movement
		else if (timeElapsedTotal >= (_duration / 2 + tempsElapsedFrame))
		{
			double averageSpeed;
			averageSpeed = ((2 - (timeElapsedTotal - (_duration / 2)) / (_duration / 4) + (2 - ((timeElapsedTotal - tempsElapsedFrame) - (_duration / 2)) / (_duration / 4))))/2;
			proportionLeft = (tempsElapsedFrame*averageSpeed) / _duration;
		}

		//Apply movement
		_currentRadius = proportionLeft*_mouvRadius;
		_movementCompletedRadius += proportionLeft;
		_currentWidth = proportionLeft*_mouvWidth;
		_movementCompletedWidth += proportionLeft;
		_currentRotation = proportionLeft*_mouvRotation;
		_movementCompletedRotation += proportionLeft;

		if (_movementCompletedWidth > _movementCompletedRadius && _movementCompletedWidth > _movementCompletedRotation)
			_proportionCompleted = _movementCompletedWidth;
		else if (_movementCompletedRadius > _movementCompletedRotation)
			_proportionCompleted = _movementCompletedRadius;
		else
			_proportionCompleted = _movementCompletedRotation;
	}
	else
	{
		_isFinished = true;
	}
		

	return _isFinished;	
}

//Destructor
movementArc::~movementArc()
{
}
