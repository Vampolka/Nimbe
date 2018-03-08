/*
arc.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This class represents a single arc on the screen (the graphical part of what a user controls), the visual counterpart of the track.
*/

#include "arc.h"

//Constructor
arc::arc()
{
	//init variables
	_centerX = 0;
	_centerY = 0;
	_radius = 0;
	_width = 0;
	_rotation = 0;
	_closed = false;
	_height = 0;
	_trueRotation = 0;

	//force the vector to have at least 10 slots (had a few spikes issues without it)
	movements.reserve(10);

	//set a random value to the starting wiggle position
	_randWiggle = (rand() % 1000)/100;
}

//Draws the arc. The parameters should be pretty self-explanatory. See ofApp.h parameters
void arc::drawArcCentre(ofColor couleur, ofColor couleurMouv, int drawMode, float blurFactor, float backOpacity, float opacity, float vertikalFactoru, float horizontFactoru)
{
	//==================================Move the arc==================================
	double minProportionDone = 1;

	for (int i = 0; i < movements.size(); i++)
	{
		bool isfinished;
		isfinished = movements[i].move();
		_radius += movements[i].getCurrentRadius();
		_width += movements[i].getCurrentWidth();
		_rotation += movements[i].getCurrentRotation();
		if (minProportionDone > movements[i].getProportionCompleted() && movements[i].getBoolColor() == true)
			minProportionDone = movements[i].getProportionCompleted();
		if (isfinished)
			movements.erase(movements.begin() + i);
	}


	//==================================Draw the arc==================================

	ofColor currentColor;
	currentColor.r = round(couleur.r*minProportionDone + couleurMouv.r*(1 - minProportionDone));
	currentColor.g = round(couleur.g*minProportionDone + couleurMouv.g*(1 - minProportionDone));
	currentColor.b = round(couleur.b*minProportionDone + couleurMouv.b*(1 - minProportionDone));

	int alpha = 0;
	alpha = round(_radius / 2) + 20;
	if (alpha > 255)
		alpha = 255;

	ofSetColor(currentColor, alpha);

	//randomness
	float vertikalModuli;
	float horizontModuli;

	if (drawMode == 0)
	{
		//rotate to the original position (0 = east)
		ofRotate(_rotation);

		//Draw the rectangles that form the arc
		double z = 90;

		//Front rectangle
		ofSetColor(currentColor,255*opacity);
		for (int i = 0; i < z; i++)
		{
			vertikalModuli = round((rand() % 20 * vertikalFactoru) - 20 * vertikalFactoru);
			horizontModuli = round((rand() % 20 * horizontFactoru) - 20 * horizontFactoru);
			ofRotate(90 / z, 0, 0, 1);
			ofDrawRectangle(0+horizontModuli, _radius, _height, _radius / 30, _width+vertikalModuli);
		}

		ofRotate(-90);

		//back rectangle, see-through
		ofSetColor(currentColor, 120*opacity*backOpacity);
		for (int i = 0; i < z; i++)
		{
			vertikalModuli = round((rand() % 20 * vertikalFactoru) - 20 * vertikalFactoru);
			horizontModuli = round((rand() % 20 * horizontFactoru) - 20 * horizontFactoru);
			ofRotate(90 / z, 0, 0, 1);
			ofDrawRectangle(0 + horizontModuli, _radius, _height - 1, _radius / 30 + _width*blurFactor / 2, _width + _width*blurFactor / 2 + vertikalModuli);
		}

		ofRotate(-90);

		//last rectangle, very see-through
		ofSetColor(currentColor, 40*opacity*backOpacity);
		for (int i = 0; i < z; i++)
		{
			vertikalModuli = round((rand() % 20 * vertikalFactoru) - 20 * vertikalFactoru);
			horizontModuli = round((rand() % 20 * horizontFactoru) - 20 * horizontFactoru);
			ofRotate(90 / z, 0, 0, 1);
			ofDrawRectangle(0 + horizontModuli, _radius, _height - 2, _radius / 30 + _width*blurFactor, _width + _width*blurFactor + vertikalModuli);
		}


		//return to the initial position
		ofRotate(-_rotation - 90);
	}

}

//Draws the arc. The parameters should be pretty self-explanatory. See ofApp.h parameters
void arc::drawArc(ofColor couleur, ofColor couleurMouv, int drawMode, float opacity, float facteurCarres, float vertikalFactoru, float horizontFactoru)
{

	//wiggle application
	_rotation += _wiggle;

	//==================================Move the arc==================================

	double minProportionDone = 1;

	for (int i = 0; i < movements.size(); i++)
	{
		bool isfinished;
		isfinished = movements[i].move();
		_radius += movements[i].getCurrentRadius();
		_width += movements[i].getCurrentWidth();
		_rotation += movements[i].getCurrentRotation();
		if(minProportionDone > movements[i].getProportionCompleted() && movements[i].getBoolColor() == true)
			minProportionDone = movements[i].getProportionCompleted();
		if (isfinished)
			movements.erase(movements.begin() + i);
	}


	//==================================Draw the arc==================================

	ofColor currentColor;
	currentColor.r = round(couleur.r*minProportionDone + couleurMouv.r*(1 - minProportionDone));
	currentColor.g = round(couleur.g*minProportionDone + couleurMouv.g*(1 - minProportionDone));
	currentColor.b = round(couleur.b*minProportionDone + couleurMouv.b*(1 - minProportionDone));

	int alpha = 0;
	alpha = round(_radius / 2) + 20;
	if (alpha > 255)
		alpha = 255;
	alpha = round(opacity*alpha);

	ofSetColor(currentColor, alpha);

	//randomness
	float vertikalModuli;
	float horizontModuli;
	vertikalFactoru = vertikalFactoru*(_width/8);

	//appliquer la couleur par proportion faite
	if (_width > 0.2)
	{
		
		//full squares or full lines
		if (drawMode == 1)
		{
			//Rotate to the original position (0 = east)
			ofRotate(_rotation - 0.5);

			//Draw the rectangles that form the arc
			double z = 30;
			ofRotate(-1);
			
			for (int i = 0; i < z; i++)
			{
				vertikalModuli = round((rand() % 20 * vertikalFactoru) - 20 * vertikalFactoru);
				horizontModuli = round((rand() % 20 * horizontFactoru) - 20 * horizontFactoru);
				ofRotate(90 / z, 0, 0, 1);
				ofDrawRectangle(0 + horizontModuli, _radius, _height, (_radius / 18.75)*facteurCarres, _width + vertikalModuli);
			}

			//Return to the initial rotation
			ofRotate(1);
			ofRotate(-_rotation - 90 + 0.5);
		}
		
		//outline of the arc
		if (drawMode == 2)
		{
			//draw the rectangles that form the arc
			double z = 30;

			ofPolyline polyA;
			ofPolyline polyB;
			ofPolyline sideA;
			ofPolyline sideB;
			
			for (int i = -1; i < z; i++)
			{
				vertikalModuli = round((rand() % 20 * vertikalFactoru) - 20 * vertikalFactoru);
				horizontModuli = round((rand() % 20 * horizontFactoru) - 20 * horizontFactoru);
				double x1 = -(_radius + _width / 2+ vertikalModuli/2)*sin(ofDegToRad(_rotation + horizontModuli/2+ (90 / z)*(i + 1)));
				double y1 = (_radius + _width / 2+ vertikalModuli/2)*cos(ofDegToRad(_rotation + horizontModuli/2+ (90 / z)*(i + 1)));
				double x2 = -(_radius - _width / 2+ vertikalModuli/2)*sin(ofDegToRad(_rotation + horizontModuli/2+ (90 / z)*(i + 1)));
				double y2 = (_radius - _width / 2+ vertikalModuli/2)*cos(ofDegToRad(_rotation + horizontModuli/2+ (90 / z)*(i + 1)));

				if (i == -1)
				{
					sideA.addVertex(ofPoint(x1, y1, _height));
					sideA.addVertex(ofPoint(x2, y2, _height));
				}
				else if (i == z - 1)
				{
					sideB.addVertex(ofPoint(x1, y1, _height));
					sideB.addVertex(ofPoint(x2, y2, _height));
				}

				polyA.addVertex(ofPoint(x1, y1, _height));
				polyB.addVertex(ofPoint(x2, y2, _height));

			}
			polyA.draw();
			polyB.draw();
			sideA.draw();
			sideB.draw();
		}

		//X that form the squares
		if (drawMode == 3)
		{
			
			//draw the Xs that form the arcs
			double z = 45;

			ofPolyline polyA;
			ofPolyline polyB;
			ofPolyline sideA;
			ofPolyline sideB;
			ofPolyline xsesA;
			ofPolyline xsesB;

			for (int i = -1; i < z; i++)
			{
				vertikalModuli = round((rand() % 20 * vertikalFactoru) - 20 * vertikalFactoru);
				horizontModuli = round((rand() % 20 * horizontFactoru) - 20 * horizontFactoru);
				double x1 = -(_radius + _width / 2 + vertikalModuli/2)*sin(ofDegToRad(_rotation + horizontModuli/2 + (90 / z)*(i + 1)));
				double y1 = (_radius + _width / 2 + vertikalModuli/2)*cos(ofDegToRad(_rotation + horizontModuli/2 + (90 / z)*(i + 1)));
				double x2 = -(_radius - _width / 2 + vertikalModuli/2)*sin(ofDegToRad(_rotation + horizontModuli/2 + (90 / z)*(i + 1)));
				double y2 = (_radius - _width / 2 + vertikalModuli/2)*cos(ofDegToRad(_rotation + horizontModuli/2 + (90 / z)*(i + 1)));

				if (i == -1)
				{
					sideA.addVertex(ofPoint(x1, y1, _height));
					sideA.addVertex(ofPoint(x2, y2, _height));
				}
				else if (i == z - 1)
				{
					sideB.addVertex(ofPoint(x1, y1, _height));
					sideB.addVertex(ofPoint(x2, y2, _height));
				}
					

				if ((i+2) % 2 == 0)
				{
					xsesA.addVertex(ofPoint(x1, y1, _height));
					xsesB.addVertex(ofPoint(x2, y2, _height));
				}
				else if ((i+2) % 2 == 1)
				{
					xsesA.addVertex(ofPoint(x2, y2, _height));
					xsesB.addVertex(ofPoint(x1, y1, _height));

				}

				polyA.addVertex(ofPoint(x1, y1, _height));
				polyB.addVertex(ofPoint(x2, y2, _height));

			}
			polyA.draw();
			polyB.draw();
			sideA.draw();
			sideB.draw();
			xsesA.draw();
			xsesB.draw();
		}


		//Outline of the squares
		if (drawMode == 4)
		{
			//pivoter à la position d'origine (0 = est)
			ofRotate(_rotation - 0.5);

			//dessiner les X rectangles qui forment l'arc
			ofPushStyle();
			ofNoFill();
			double z = 30;
			ofRotate(-1);
			for (int i = 0; i < z; i++)
			{
				vertikalModuli = round((rand() % 20 * vertikalFactoru)- 20* vertikalFactoru);
				horizontModuli = round((rand() % 20 * horizontFactoru) - 20*horizontFactoru);
				ofRotate(90 / z, 0, 0, 1);
				ofDrawRectangle(0+ horizontModuli, _radius, _height, (_radius / 18.75)*facteurCarres, _width+ vertikalModuli);
			}

			//revenir à la rotation initiale
			ofPopStyle();
			ofRotate(1);
			ofRotate(-_rotation - 90 + 0.5);
		}

		//Crosses
		if (drawMode == 5)
		{
			//pivoter à la position d'origine (0 = est)
			ofRotate(_rotation);

			//dessiner les X rectangles qui forment l'arc
			int k = rand() % 20;
			if (k > 1)
				k = 1;

			double z = round(_radius / 4) + 4 + k;
			for (int i = 0; i < z; i++)
			{


				ofRotate(90 / z, 0, 0, 1);

				int x = -_radius*sin(ofDegToRad(_rotation + (90 / z)*(i + 1)));
				int y = _radius*cos(ofDegToRad(_rotation + (90 / z)*(i + 1)));

				ofRotate(-(_rotation + (90 / z)*(i + 1)));
				ofDrawLine(x - _width / 2, y, 0, x + (_width / 2), y, _height);
				ofDrawLine(x, y - _width / 2, 0, x, y + _width / 2, _height);
				ofRotate(_rotation + (90 / z)*(i + 1));
			}
			//revenir à la rotation initiale
			ofRotate(-_rotation - 90);
		}		
	}
	_rotation -= _wiggle;
	
}

//Move the arc
void arc::moveArc(double newRayon, double newEpaisseur, double newRotation, double duration, bool couleur)
{
	movementArc newMovement =  movementArc(_radius, _rotation, _width, newRayon, newRotation, newEpaisseur, duration, couleur);
	movements.push_back(newMovement);
}

//Adjusts the position based on the "wiggle" factor, which makes the arcs oscillate slightly while staying on the same radius.
//speed: wiggles/second
//range: max pixels
void arc::updateWiggle(float timeSince, float speed, float range)
{
	
	double temp = (cos(ofDegToRad(((timeSince+ _randWiggle)*speed) * 180 + 180)));
	_wiggle = range*temp;
}

arc::~arc()
{
}
