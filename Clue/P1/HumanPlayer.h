#pragma once
#include "Player.h"

class HumanPlayer : public Player
{
public:
	HumanPlayer(cardName name, std::vector<cardName> opponents);
	~HumanPlayer();
	void drawNotecard();
	bool checkClick(double mouseX, double mouseY);
	void unClick();
	void addCard(Card card);
	Accusation getAccusation();
	std::vector<Card> checkHand(Accusation accusation);
	void newEvidence(std::string evidence, Accusation accusation, cardName accuser, cardName disprover);

private:
	Notecard mNotecard;
	std::vector<Card> mCards;
};

