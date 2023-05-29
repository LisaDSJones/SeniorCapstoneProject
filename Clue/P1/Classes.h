#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <iostream>
#include <ctime>
#include <fstream>
#include <algorithm>



// Include this file to get access to all classes, enums, Macros, and 
//                    the global Coordinates associated with the game

// Classes
class Button;
class PlayerTypeOptionButton;
class DifficultyOptionButton;
class NavigationButton;
class HasCardToggleButton;
class Player;
class HumanPlayer;
class ComputerPlayer;
class Piece;
class Game;
class Notecard;
class Card;
class Accusation;
class ThisRound;

// Global Coords
const int W = 23;
const int H = 25;


// Macros
#define RED		1,0,0
#define BLUE	0,0,1
#define WHITE	.99,.99,.99
#define PURPLE	.5,0,.5
#define YELLOW	237.0/255.0, 209.0/255.0, 0
#define GREEN   0,1,0
#define BLACK	0,0,0
#define OFFWHITE .85,.85,.85

// really helpful -- especially in conjunction with cardName
const double COLORS[6][3] = {
	{ RED },
	{ BLUE },
	{ WHITE },
	{ PURPLE },
	{ YELLOW },
	{ GREEN }
};

const std::string cardStrings[22] = {
	"Miss Scarlet", "Mrs. Peacock", "Mrs. White", "Prof. Plum", "Col. Mustard", "Mr. Green",
	"Rope", "Lead Pipe", "Knife", "Wrench", "Candlestick", "Revolver",
	"Kitchen", "Dining Room", "Lounge", "Ballroom", "Hall", "Conservatory", "Billiard Room", "Library", "Study", ""
};

const std::string nameAbbreviations[7] = {
	"S", "Pe", "W", "Pl", "M", "G", ""
};


// Enums

enum playerType
{ HUMAN, COMPUTER, NONE };

enum difficultyType
{ EASY, HARD, NODIFFICULTY };

enum place
{ OPTIONS, STARTTURN, TAKETURN, INTRODUCEPLAYERHAND, SHOWPLAYERHAND, SHOWACCUSOREVIDENCE, PLAYERWINS, COMPUTERTURN };

enum cardName
{
	MISSSCARLET, MRSPEACOCK, MRSWHITE, PROFPLUM, COLONELMUSTARD, MRGREEN,
	ROPE, LEADPIPE, KNIFE, WRENCH, CANDLESTICK, REVOLVER,
	KITCHEN, DININGROOM, LOUNGE, BALLROOM, HALL, CONSERVATORY, BILLIARDROOM, LIBRARY, STUDY,
	NOCARD
};

enum cardType {
	SUSPECT, WEAPON, ROOM
};

enum hasCardToggleOption
{
	NO, YES, MAYBE, NOONE, UNKNOWN
};


