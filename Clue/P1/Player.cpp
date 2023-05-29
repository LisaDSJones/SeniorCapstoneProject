#include "Classes.h"
#include "Player.h"
#include "Card.h"


Player::Player(cardName name, bool isComputer)
	:mName(name), mIsComputer(isComputer), mPawn(W/2-3.5, H/2, COLORS[name][0], COLORS[name][1], COLORS[name][2], false)  {
	mR = COLORS[name][0];
	mG = COLORS[name][1];
	mB = COLORS[name][2];
}

Player::~Player()
{
}

void Player::Draw() {
	mPawn.Draw();
}

// Getters

cardName Player::getName()
{ return mName; }

Piece & Player::getPawn()
{ return mPawn; }

double Player::getR()
{ return mR; }

double Player::getG()
{ return mG; }

double Player::getB()
{ return mB; }

bool Player::isComputer()
{ return mIsComputer; }


// Virtual Functions -- none of them do anything of any value -- except be there

bool Player::isFinalAccusation()
{
	return false;
}

void Player::drawNotecard() { }

bool Player::checkClick(double mouseX, double mouseY) { return false; }

void Player::unClick() { }

void Player::addCard(Card card) { }

std::vector<Card> Player::checkHand(Accusation accusation) {
	std::vector<Card> empty;
	return empty;
}

Accusation Player::getAccusation() { 
	return Accusation(NOCARD,NOCARD, NOCARD );  
}
void Player::newEvidence(std::string evidence, Accusation accusation, cardName accuser, cardName disprover) { }


