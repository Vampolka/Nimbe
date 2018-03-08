/*
track.cpp
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

The is the track, the audio counterpart of the arc. Every user controls one. It is also responsible for writing the received messages at the left of the screen.
*/

#include "track.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <direct.h>


//Constructor
track::track()
{
	_timeTotal = clock();
	_timeElapsed = 0;

	//Base phone number.
	_phoneNumber = std::to_string(DEFAULTPHONENUMBER);
	_lastMessage = " ";

	char *path = NULL;
	path = getcwd(NULL, 0); // or _getcwd
	string path2 = path;
	//Font
	textFont.load(path2 + "\\data\\cone.ttf", 14);

	int i = 1;
	int mode = 1;

	_ID = i;

	_pan = 0;
	_level = 0;

	this->setModes();
}

//Set the modes, called in the construction.
//The modes are essentially harcoded, because they are chords that were carefully chosen manually.
void track::setModes() 
{
	//set all variants of the first mode to the first one, then adjust the differences
	for (int i = 0; i<4; i++)
	{
		_modes[0][i][0] = 0.25;
		_modes[0][i][1] = 0.374576769;
		_modes[0][i][2] = 0.5;
		_modes[0][i][3] = 0.629960525;
		_modes[0][i][4] = 0.749153538;
		_modes[0][i][5] = 0.943874313;
		_modes[0][i][6] = 1.122462048;
		_modes[0][i][7] = 1.25992105;
		_modes[0][i][8] = 1.498307077;
		_modes[0][i][9] = 2;
		_modes[0][i][10] = 2.5198421;
		_modes[0][i][11] = 2.996614154;
		_modes[0][i][12] = 4;
	}

	//adjust the differences
	_modes[0][1][5] = 0.890898718;
	_modes[0][1][7] = 1.498307077;
	_modes[0][1][8] = 1.781797436;

	_modes[0][2][8] = 1.334839854;
	_modes[0][2][11] = 2.669679708;

	_modes[0][3][8] = 1.681792831;
	_modes[0][3][11] = 3.363585661;

	//set all variants of the second mode to the first one, then adjust the differences
	for (int i = 0; i<4; i++)
	{
		_modes[1][i][0] = 0.198425131;
		_modes[1][i][1] = 0.396850263;
		_modes[1][i][2] = 0.5;
		_modes[1][i][3] = 0.594603558;
		_modes[1][i][4] = 0.749153538;
		_modes[1][i][5] = 0.890898718;
		_modes[1][i][6] = 1;
		_modes[1][i][7] = 1.189207115;
		_modes[1][i][8] = 1.498307077;
		_modes[1][i][9] = 2;
		_modes[1][i][10] = 2.37841423;
		_modes[1][i][11] = 2.996614154;
		_modes[1][i][12] = 4;
	}

	//adjust the differences
	_modes[1][1][8] = 1.587401052;
	_modes[1][1][11] = 3.174802104;

	_modes[1][2][8] = 1.334839854;
	_modes[1][2][11] = 2.669679708;

	_modes[1][3][8] = 1.781797436;
	_modes[1][3][11] = 3.563594873;

	//set all variants of the third mode to the first one, then adjust the differences
	for (int i = 0; i<4; i++)
	{
		_modes[2][i][0] = 0.25;
		_modes[2][i][1] = 0.374576769;
		_modes[2][i][2] = 0.5;
		_modes[2][i][3] = 0.594603558;
		_modes[2][i][4] = 0.749153538;
		_modes[2][i][5] = 1;
		_modes[2][i][6] = 1.189207115;
		_modes[2][i][7] = 1.498307077;
		_modes[2][i][8] = 1.587401052;
		_modes[2][i][9] = 2;
		_modes[2][i][10] = 2.37841423;
		_modes[2][i][11] = 2.996614154;
		_modes[2][i][12] = 3.174802104;
	}

	//adjust the differences
	_modes[2][1][8] = 1.781797436;
	_modes[2][1][12] = 3.563594873;

	_modes[2][2][7] = 1.334839854;
	_modes[2][2][11] = 2.669679708;

	_modes[2][3][6] = 1.334839854;
	_modes[2][3][10] = 2.669679708;

	//set all variants of the fourth mode to the first one, then adjust the differences
	for (int i = 0; i<4; i++)
	{
		_modes[3][i][0] = 0.187288385;
		_modes[3][i][1] = 0.374576769;
		_modes[3][i][2] = 0.561231024;
		_modes[3][i][3] = 0.667419927;
		_modes[3][i][4] = 0.749153538;
		_modes[3][i][5] = 0.890898718;
		_modes[3][i][6] = 1.122462048;
		_modes[3][i][7] = 1.334839854;
		_modes[3][i][8] = 1.681792831;
		_modes[3][i][9] = 1.781797436;
		_modes[3][i][10] = 2.244924097;
		_modes[3][i][11] = 2.996614154;
		_modes[3][i][12] = 3.563594873;
	}

	//adjust the differences
	_modes[3][1][5] = 1;
	_modes[3][1][9] = 2;

	_modes[3][2][6] = 1.189207115;
	_modes[3][2][10] = 2.37841423;

	_modes[3][3][7] = 1.25992105;
	_modes[3][3][11] = 2.5198421;

	//set all variants of the fifth mode to the first one, then adjust the differences
	for (int i = 0; i<4; i++)
	{
		_modes[4][i][0] = 0.148650889;;
		_modes[4][i][1] = 0.297301779;
		_modes[4][i][2] = 0.445449359;
		_modes[4][i][3] = 0.594603558;
		_modes[4][i][4] = 0.749153538;
		_modes[4][i][5] = 0.890898718;
		_modes[4][i][6] = 1.122462048;
		_modes[4][i][7] = 1.334839854;
		_modes[4][i][8] = 1.681792831;
		_modes[4][i][9] = 2;
		_modes[4][i][10] = 2.37841423;
		_modes[4][i][11] = 2.996614154;
		_modes[4][i][12] = 3.563594873;
	}

	//adjust the differences
	_modes[4][1][7] = 1.334839854;
	_modes[4][1][11] = 2.669679708;

	_modes[4][2][3] = 0.561231024;
	_modes[4][2][10] = 2.244924097;

	_modes[4][3][8] = 1.781797436;
	_modes[4][3][12] = 3.363585661;

	//set all variants of the sixth mode to the first one, then adjust the differences
	for (int i = 0; i<4; i++)
	{
		_modes[5][i][0] = 0.140307756;;
		_modes[5][i][1] = 0.280615512;
		_modes[5][i][2] = 0.420448208;
		_modes[5][i][3] = 0.561231024;
		_modes[5][i][4] = 0.707106781;
		_modes[5][i][5] = 0.840896415;
		_modes[5][i][6] = 1.122462048;
		_modes[5][i][7] = 1.414213562;
		_modes[5][i][8] = 1.681792831;
		_modes[5][i][9] = 2;
		_modes[5][i][10] = 2.37841423;
		_modes[5][i][11] = 2.828427125;
		_modes[5][i][12] = 4;
	}

	//adjust the differences
	_modes[5][1][5] = 0.890898718;
	_modes[5][1][8] = 1.781797436;

	_modes[5][2][3] = 0.5;
	_modes[5][2][6] = 1;

	_modes[5][3][7] = 1.25992105;
	_modes[5][3][11] = 2.5198421;
}

//Change the current note being played.
//Every note is defined by a mode (the chord it is a part of), and its ranking within that chord (higher ranking = higher pitch)
void track::changeRatio(int newRanking, float mode)
{
	_ratio = _modes[(int)round(mode)-1][((int)round(mode * 10)%10)][newRanking];
	_ratioRanking = newRanking;
}

//Reset the time elapsed since the last text message was received.
void track::resetTime()
{
	_timeTotal = clock();
	_timeElapsed = 0;
}

//Draws the message at the left of the screen when a SMS is received.
void track::drawMessage(ofColor backgroundColor, ofColor baseColor, ofColor mouvementColor, float opacity)
{
	//Calculate the time
	double timeElaspedTotal, timeElapsedFrame;
	timeElaspedTotal = clock() - _timeTotal;
	timeElaspedTotal /= CLOCKS_PER_SEC;

	timeElapsedFrame = timeElaspedTotal - _timeElapsed;
	_timeElapsed = timeElaspedTotal;

	//calculate the color of the writing
	double baseFactor = 0;

	if (_timeElapsed < 6)
		baseFactor = 1-(_timeElapsed/6);
	else
		baseFactor = 0;

	double amountOfBaseColor = 1;
	double amountOfMovementColor = 1;

	if (_timeElapsed < 8)
		amountOfMovementColor = 1 - (_timeElapsed / 8);
	else
		amountOfMovementColor = 0;

	ofColor textColor;
	textColor.r = (mouvementColor.r*baseFactor + baseColor.r*(1- baseFactor))*amountOfBaseColor+ backgroundColor.r*(1 - amountOfBaseColor);
	textColor.g = (mouvementColor.g*baseFactor + baseColor.g*(1 - baseFactor))*amountOfBaseColor + backgroundColor.g*(1 - amountOfBaseColor);
	textColor.b = (mouvementColor.b*baseFactor + baseColor.b*(1 - baseFactor))*amountOfBaseColor + backgroundColor.b*(1 - amountOfBaseColor);

	//Calculate opacity
	int opacityNow = round(opacity * 255* amountOfMovementColor);
	ofSetColor(textColor, opacityNow);

	//Write the message
	string spacedPhoneNumber = _phoneNumber;

	//Insert "-" and spaces in the phone number where it belongs
	spacedPhoneNumber.insert(3,"-");
	spacedPhoneNumber.insert(7, "-");

	for (int i = 0; i < _phoneNumber.size()+2; i++)
	{
		spacedPhoneNumber.insert((_phoneNumber.size()+2)-i, " ");
	}

	string spacedLastMessage = _lastMessage;

	for (int i = 0; i < _lastMessage.size(); i++)
	{
		spacedLastMessage.insert(_lastMessage.size()-i, " ");
	}

	//Draw the message on the screen
	if(_ID<67)
		textFont.drawString(spacedPhoneNumber + "  : " + spacedLastMessage, 10, _ID * 16+16);
	else
		textFont.drawString(spacedPhoneNumber + "  : " + spacedLastMessage, 140, (_ID-66) * 16 + 8);

}

//Destructor
track::~track()
{
}
