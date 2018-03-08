/*
definitions.h
Author: Émile Ouellet-Delorme
Last Edited: 07/03/18

Define global values that are used throughout the different classes.
*/

//track and level settings
#define MAXTRACKS 100 //Max number of users that can participate
#define BASELEVEL 0.6 //Volume of each user
#define BASSLEVEL 1.0 //Volume of the bass
#define BASEMOVEMENTSPEED 4.0 //Base movement speed of the arcs

//start values
#define STARTGLOBALVOLUME 1.0 //starting global volume, from 0 to 1
#define STARTSAMPLE 1 //Starting samples used by supercollider
#define STARTMODE 4.0 //starting mode, the modes are different chords
#define STARTDRAWMODE 1 //Starting draw mode, 1 means full arcs
#define STARTSPACESQUARES 1 //size of the squares forming the arcs, 1 forms full arcs, below that forms squares.
#define WIGGLESTARTSPEED 0.3 //speed between every oscillation for the wiggle
#define WIGGLESTARTRANGE 0 //range of the oscillations for the wiggle

//duration of transitions
#define DURATIONHARDCOLORCHANGE 0.5 //Duration for a "hard" change of color, in seconds
#define DURATIONSOFTCOLORCHANGE 12.0 //Duration for a "soft" change of color, in seconds

//ports
#define OSCPORT 5678 //Port used to communicate in OSC (has to be set to the same thing on the receiving end, for superCollider.
#define UDPPORT 5677 //port used to communicate in UDP (to receive the SMS, has to be set to the same thing on the sending end in c#)

//Default testing values
#define DEFAULTPHONENUMBER 514555555 //Default phone number, only used when testing (using the mouse to create tracks)
