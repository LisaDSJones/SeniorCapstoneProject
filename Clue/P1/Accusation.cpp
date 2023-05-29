#include "Accusation.h"



Accusation::Accusation()
	: mSuspect(NOCARD), mWeapon(NOCARD), mRoom(NOCARD)
{ }

Accusation::Accusation(cardName suspect, cardName weapon, cardName room)
	: mSuspect(suspect), mWeapon(weapon), mRoom(room)
{ }

Accusation::~Accusation()
{
}

std::string Accusation::toString()
{
	std::string total = cardStrings[mSuspect] + " with the " + cardStrings[mWeapon] + " in the " + cardStrings[mRoom];
	return total;
}

void Accusation::setSuspect(cardName suspect)
{
	mSuspect = suspect;
}

void Accusation::setWeapon(cardName weapon)
{
	mWeapon = weapon;
}

void Accusation::setRoom(cardName room)
{
	mRoom = room;
}

cardName Accusation::getSuspect()
{
	return mSuspect;
}

cardName Accusation::getWeapon()
{
	return mWeapon;
}

cardName Accusation::getRoom()
{
	return mRoom;
}

bool Accusation::operator==(Accusation right)
{
	if (mSuspect == right.mSuspect && mWeapon == right.mWeapon && mRoom == right.mRoom)
		return true;
	return false;
}

bool Accusation::operator==(std::vector<Card> right)
{
	for (size_t i = 0; i < right.size(); i++)
	{
		if (*this == right[i].getName())
			return true;
	}
	return false;
}

bool Accusation::operator==(cardName right)
{
	if (mSuspect == right || mRoom == right || mWeapon == right)
		return true;
	return false;
}
