/*
ofApp.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

This class is the core of the application. It creates and triggers the events, draws the graphics and updates the situation on every frame.
*/

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ofAppNoWindow.h"
#include "ofxNetwork.h"
#include <vector>

//include definitions
#include "definitions.h"

//include other classes
#include "track.h"
#include "arc.h"
#include "movementArc.h"

//include all event types
#include "events/eventBass.h"
#include "events/eventAbruptTransition.h"
#include "events/eventCenterOpacityFade.h"
#include "events/eventChangeChord.h"
#include "events/eventChangeSquare.h"
#include "events/eventChangeWiggle.h"
#include "events/eventEverythingOff.h"
#include "events/eventEverythingOn.h"
#include "events/eventFluidTransition.h"
#include "events/eventHardColor.h"
#include "events/eventMoveCamera.h"
#include "events/eventOpacityFade.h"
#include "events/eventPlayDong.h"
#include "events/eventVibration.h"
#include "events/eventVolumeFade.h"

class ofApp : public ofBaseApp
{
	public:
		
		//Setup, executed once at the launch of the program
		void setup();

		//destructor
		~ofApp();

	public:		
		//Runs every frame, draw the visuals
		void draw();

		//When you press "a", change a random _tracks localisation (for testing)
		//when you press "s", change volume of a random track (on/off) (for testing)
		// when you press "d", change pitch of a random track (for testing)
		//When Spacebar is pressed, it begins.
		void keyPressed(int key);

		//Also for testing
		//When you left click, add a new track with default properties
		void mousePressed(int x, int y, int button);

	private:
		//runs every frame, updates variables, recieves SMS and calls events
		void update();

		//Add a new track
		void newTrack();

		//Change the pitch of theis track. Plus is whether the track is going up ro down.
		void changePitch(int trackID, bool plus);

		//Change the pan of the chosen track to the angle of rotationDegree (from 0 to 360, 0 being north-west, going counter-clockwise. (so north is 315))
		void changePan(int trackID, int rotationDegree);

		//Change the volume of the chosen track. Since the volume can only be "on" or "off", the variable on is a bool.
		void changeVolume(int trackID, bool on);

		//Create all of the events for the performance. These are hardcoded because they are composed, they represent the core of the music.
		void createEvents();

		//Sort all events in chronologic order
		void sortEvents();

		//Variables

		float _volumeGlobal; //Current global volume (between 0 and 1)

		//_tracks
		int _trackAmount;
		track _tracks[MAXTRACKS]; //All tracks
		arc _arcs[MAXTRACKS]; //All arcs
		arc _arcsCenter[8]; //Arcs for the center
		vector<int> displayList; //Order in which the tracks are to be displayed, each int representing the index of the track and arc	

		//osc
		ofxOscSender _sender; //used to send OSC

		//mode and sample
		int _mode; //The mode is the current chord being played, there are 6 modes, from 1 to 6
		int _sample; //Represents the sound samples used by the users.The higher it is, the harsher the sounds. Starts at 1, max value is 49.

		//Graphics
		//From 1 to 5, changes the way the arcs are drawn
		//1: Full arcs/rectangles
		//2: Outline of arcs
		//3: Draw the arcs with X shaped lines that form it
		//4: Outline of rectangles
		//5: Draw the arcs with small crosses
		int _drawMode;
		int _previousDrawMode; //used when transitioning between drawModes

		//Colors
		ofColor _baseColor; //color of the arcs
		ofColor _animationColor; //color of an arc when animated
		ofColor _backgroundColor;
		ofColor _centerColor; //color of the center arcs

		ofColor _oldBaseColor; //previous iterations of the same colors, used for transitions
		ofColor _oldAnimationColor;
		ofColor _oldBackgroundColor;
		ofColor _oldCenterColor;
		
		//Camera
		ofEasyCam _camera;
		double _positionCam; //Z distance from the center
		double _positionEnd; //Position towards which the camera is moving, used when adding tracks
		double _speedCam; //Speed at which the camera is moving
		bool _cameraMode; //Once the value is false, the camera will not move anymore when adding new tracks
		

		//Time
		double _totalTime; //Time of the clock
		double _timeElapsed; //Time since the start of the program
		double _timeElapsedFrame; //Time elapsed this frame
		double _timeElapsedSpace = 0; //Time since the start of the performance (when space is pressed)

		//center
		double _centerFactor; //Spread - Between 0 and 1
		double _centerOpacity; //Flower - Between 0 and 1

		//AI
		double _centerSpeed; // Between 0 et X (1 = typical speed)
		double _globalOpacity; // Between 0 and 1
		bool _start; //Whether the performance has started
		bool _end; //Whether the performance has ended

		bool _transitionDrawMode; //Whether a hard transition is going on between 2 draw modes
		bool _FadeDrawMode; //Whether a fade between 2 draw modes is happening
		double _transitionDrawModeDegree; //stage of the transition

		//The wiggle is the back and forth movement that each arc does when nothing happens
		float _wiggleSpeed;
		float _wiggleRange;

		//fonts
		ofTrueTypeFont titleFont;
		ofTrueTypeFont startFont;
		ofTrueTypeFont textFont; //font for the box
		ofTrueTypeFont presFont; //font for the title of the box
	
		//for receiving SMS
		ofxUDPManager manager;
		string _listPhones[200];

		//title
		bool drawTitle;
		int titleOpacity;

		//Shaking
		float _vertikalFactor; //Vertical (perpendicular to the arc) factor of the shaking
		float _horizontFactor; //Horizontal (parallel to the arc) factor of the shaking 

		//Squares
		float _spaceSquares; //current size of the squares making the arcs

		//Events
		vector<baseEvent*> _allEvents; //All events (uses polymorphism)
		vector<baseEvent*> _runningEvents; //Currently running Events
		int _nextEvent; //Index of the next even that will trigger
};
