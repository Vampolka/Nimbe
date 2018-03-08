/*
movementArc.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This class is used by arc to move itself. Since an arc can have more than one movement at the same time, it is a separate class.
*/

#include <stdlib.h>     // srand, rand
#include <time.h>       // time

#pragma once


class movementArc
{
public:
	//Constructor, takes the old and the new coordinates, the duration of the movement and whether the color should change to the movement color while moving.
	movementArc(double oldRadius, double oldRotation, double oldWidth, double newRadius, double newRotation, double newWidth, double duration, bool color);

	//Moves the arc. Returns whether the movement is compelted or not.
	bool move();

	//Destructor
	~movementArc();

	//Getters
	double getCurrentRadius() { return _currentRadius; }
	double getCurrentWidth() { return _currentWidth; }
	double getCurrentRotation() { return _currentRotation; }
	double getProportionCompleted() { return _proportionCompleted; }
	bool getBoolColor() { return _boolColor; };

private:
	//old position (start of the movement)
	double _oldRadius;
	double _oldRotation;
	double _oldWidth;

	//new position (end of the movement)
	double _newRadius;
	double _newRotation;
	double _newWidth;

	//amount of movement (new - old)
	double _mouvRadius;
	double _mouvRotation;
	double _mouvWidth;

	//current position
	double _currentRadius;
	double _currentRotation;
	double _currentWidth;

	//Amount of movement completed
	double _movementCompletedRadius;
	double _movementCompletedWidth;
	double _movementCompletedRotation;
	double _proportionCompleted;

	//Time variables
	double _duration;
	double _totalTime;
	double _timeElapsedTotal;
	bool _isFinished;
	
	//Whether the movement changes the color
	bool _boolColor;
};

