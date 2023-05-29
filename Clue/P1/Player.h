#pragma once
#include "Classes.h"
#include "Piece.h"
#include "Notecard.h"
#include "Accusation.h"

class Player
{
public:
	Player(cardName name, bool isComputer);
	~Player();
	void Draw();

	cardName getName();
	Piece& getPawn();
	double getR();
	double getG();
	double getB();
	bool isComputer();

	virtual bool isFinalAccusation();
	virtual void drawNotecard();
	virtual bool checkClick(double mouseX, double mouseY);
	virtual void unClick();
	virtual void addCard(Card card);
	virtual std::vector<Card> checkHand(Accusation accusation);
	virtual Accusation getAccusation();
    virtual void newEvidence(std::string evidence, Accusation accusation, cardName accuser, cardName disprover);

private:
	cardName mName;
	bool mIsComputer;
	Piece mPawn;
	double mR, mG, mB;
};

