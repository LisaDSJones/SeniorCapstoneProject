#pragma once
#include "Classes.h"
#include "Card.h"

class Accusation
{
public:
	Accusation();
	Accusation(cardName suspect, cardName weapon, cardName room);
	~Accusation();

	std::string toString();

	void setSuspect(cardName suspect);
	void setWeapon(cardName weapon);
	void setRoom(cardName room);

	cardName getSuspect();
	cardName getWeapon();
	cardName getRoom();

	bool operator==(Accusation right);
	bool operator==(std::vector<Card> right);
	bool operator==(cardName right);
private:
	cardName mSuspect;
	cardName mWeapon;
	cardName mRoom;
};

