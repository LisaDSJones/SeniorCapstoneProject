#pragma once
#include "Player.h"
class ComputerPlayer : public Player
{
public:
	ComputerPlayer(cardName name, std::vector<cardName> opponents, bool isHard);
	~ComputerPlayer();
	void addCard(Card card);
	Accusation getAccusation();
	std::vector<Card> checkHand(Accusation accusation);
	void newEvidence(std::string evidence, Accusation accusation, cardName accuser, cardName disprover);
	bool isFinalAccusation();

private:
	cardName lookUpCardName(std::string cardText);
	cardType lookUpCardType(cardName card);
	size_t lookUpPlayerOrder(cardName player);
	int lookUpMaxCards(cardName player);
	void noMoreCardsInCol(size_t i);
	void scanNotecard();
	void removeFromPossibleCards(cardName card);
	void learnHasCard(cardName player, bool hasCard, cardName card);
	bool ownCard(cardName card);

	std::vector<cardName> mPlayers;
	bool mIsHard, mIsFinalAccustion;
	std::vector<Card> mCards;
	std::vector<bool>mMetMaxCards;

	std::vector<std::vector<hasCardToggleOption>> mSuspects;
	std::vector<std::vector<hasCardToggleOption>> mWeapons;
	std::vector<std::vector<hasCardToggleOption>> mRooms;

	std::vector<cardName> mPossibleSuspects;
	std::vector<cardName> mPossibleWeapons;
	std::vector<cardName> mPossibleRooms;

	cardName mTHESuspect;
	cardName mTHEWeapon;
	cardName mTHERoom;
};

