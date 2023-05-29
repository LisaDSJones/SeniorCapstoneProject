#include "HumanPlayer.h"


HumanPlayer::HumanPlayer(cardName name, std::vector<cardName> opponents)
	:Player(name, false), mNotecard(name, opponents)
{
}

HumanPlayer::~HumanPlayer()
{
}

void HumanPlayer::drawNotecard()
{ 
	mNotecard.Draw();
}

bool HumanPlayer::checkClick(double mouseX, double mouseY)
{
	return mNotecard.checkClicked(mouseX, mouseY);
}

void HumanPlayer::unClick()
{
	mNotecard.unClick();
}

Accusation HumanPlayer::getAccusation()
{
	return mNotecard.getAccusation();
}


void HumanPlayer::addCard(Card card)
{
	mCards.push_back(card);
	mNotecard.giveCard(card);
}


std::vector<Card> HumanPlayer::checkHand(Accusation accusation)
{
	return mNotecard.checkHand(accusation);
}

void HumanPlayer::newEvidence(std::string evidence, Accusation accusation, cardName accuser, cardName disprover) {}
