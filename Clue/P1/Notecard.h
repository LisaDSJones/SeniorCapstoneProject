#pragma once
#include "ButtonTypes.h"
#include "Card.h"
#include "Accusation.h"

class Notecard
{
public:
	Notecard(cardName character, std::vector<cardName> oppenents); // eventually cards
	~Notecard();
	void Draw();
	bool checkClicked(double mouseX, double mouseY);
	std::vector<Card> checkHand(Accusation accusation);
	void unClick();
	void giveCard(Card card);
	void disableAccusation();
	void enableAccusation();

	Accusation getAccusation();
private:
	cardName mCharacter;
	std::vector<CharacterNoteToggleButton> mHeader;
	std::vector<Button*> mNotes;
	std::vector<Card> mHand;
	double mLeft;
	bool mOpponentsPlaying[6];
	size_t mClickedButtonIndex;
	std::vector<Button> mColumnTitles;
	RadioButtons mSuspects;
	RadioButtons mWeapons;
	RadioButtons mRooms;
	Accusation mPrevousAccusation;


};

