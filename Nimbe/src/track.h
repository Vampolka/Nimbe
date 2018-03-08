/*
track.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

The is the track, the audio counterpart of the arc. Every user controls one. It is also responsible for writing the received messages at the left of the screen.
*/

#pragma once
#include "ofMain.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "definitions.h"

class track
{
	public:
		//Constructor
		track();

		//Change the current note being played.
		//Every note is defined by a mode (the chord it is a part of), and its ranking within that chord (higher ranking = higher pitch)
		void changeRatio(int newRanking, float mode);

		//Draws the message at the left of the screen when a SMS is received.
		void drawMessage(ofColor backgroundColor, ofColor baseColor, ofColor mouvementColor, float opacity);

		//Reset the time elapsed since the last text message was received.
		void resetTime();

		//Destructor
		~track();

		//getters and setters
		void setPhoneNumber(string phoneNumber) { _phoneNumber = phoneNumber; }
		void setlastMessage(string lastMessage) { _lastMessage = lastMessage; }
		void setID(int ID) { _ID = ID; }

		void setRatioRanking(int ratioRanking) { _ratioRanking = ratioRanking; }
		int getRatioRanking() { return _ratioRanking; }

		float getRatio() { return _ratio; }

		void setLevel(float newLevel) { _level = newLevel; }
		float getLevel() { return _level; }

		void setPan(float newPan) { _pan = newPan; }
		float getPan() { return _pan; }

	private:
		//Set the modes, called in the construction.
		//The modes are essentially harcoded, because they are chords that were carefully chosen manually.
		void setModes();

		//ID of the track, used to know where to draw its messages.
		int _ID;

		//Sound-related data
		int _ratioRanking; //Ranking of the note played within its mode
		float _ratio; //ratio = transposition level of the sample when a sound is played. The samples are at 440, so 1 ratio is 440hz, 2 is 880, and so on.
		float _pan; //Pan between 0 and 2
		float _level; //Level (volume) between 0 and 1

		//infos texteur
		string _phoneNumber;
		string _lastMessage;

		//time variables
		double _timeTotal;
		double _timeElapsed;

		//font used to write the last messages received
		ofTrueTypeFont textFont;

		//6 different modes, 4 variants per mode, 13 notes per mode.
		float _modes[6][4][13];
};

