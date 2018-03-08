/*
arc.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This class represents a single arc on the screen (the graphical part of what a user controls), the visual counterpart of the track.
*/

#pragma once
#include "ofMain.h"
#include "movementArc.h"
#include <vector>
#include <iostream>


class arc
{
public:
	//Constructor
	arc();

	//Draws the arc. The parameters should be pretty self-explanatory. See ofApp.h parameters
	void drawArc(ofColor color, ofColor colorMovement, int drawMode, float opacity, float spaceSquares, float vertikalFactor, float horizontFactor);

	//Draws the arc for one of the center ones
	void drawArcCentre(ofColor color, ofColor colorMovement, int drawMode, float blurFactor, float backOpacity, float opacity, float vertikalFactor, float horizontFactor);

	//Move the arc. 
	void moveArc(double newRadius, double newWidth, double newRotation, double duration, bool color);

	//Adjusts the position based on the "wiggle" factor, which makes the arcs oscillate slightly while staying on the same radius.
	void updateWiggle(float timeSince, float speed, float range);

	//destructor
	~arc();

	//getters and setters
	//for ranking
	void setRanking(double newRank) { _ranking = newRank; }
	double getRanking() { return _ranking; }
	//for centerX
	void setCenterX(double newX) { _centerX = newX; }
	double getCenterX() { return _centerX; }
	//for centerY
	void setCenterY(double newY) { _centerX = newY; }
	double getCenterY() { return _centerY; }
	//for radius
	void setRadius(double newRadius) { _radius = newRadius; }
	double getRadius() { return _radius; }
	//for width
	void setWidth(double newWidth) { _width = newWidth; }
	double getWidth() { return _width; }
	//for rotation
	void setRotation(double newRotation) { _rotation = newRotation; }
	double getRotation() { return _rotation; }
	//for height
	void setHeight(double newHeight) { _height = newHeight; }
	double getHeight() { return _height; }
	//for trueAngle
	void setTrueRotation(double newTrueAngle) { _trueRotation = newTrueAngle; }
	double getTrueRotation() { return _trueRotation; }
	//for closed
	void setClosed(bool newClosed) { _closed = newClosed; }
	bool getClosed() { return _closed; }

private:
	//Ranking within the arcs. The higher the radius, the higher the rank.
	int _ranking;

	//coordinates of the center of the arc
	double _centerX;
	double _centerY;

	//Basic specs of the arc
	double _radius;
	double _width;
	double _rotation; //angle in degrees, taking the wiggle into account
	double _trueRotation; //Angle without taking the wiggle into account
	bool _closed; //Closed means the arc doesn't show and its associated tracks volume is off
	vector<movementArc> movements; //ongoing movements the arc is doing
	double _height;

	double _wiggle; //number of degrees off angle due to the wiggle
	double _randWiggle; //starting point of the wiggle, picked randomly

};

