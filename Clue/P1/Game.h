#pragma once
#include "Classes.h"
#include "Accusation.h"
#include "ThisRound.h"

class Game
{
public:
	Game();
	~Game();
	int getPlayersTurn(); // returns index of Player whose turn it is
	void draw();
	void setScreenState(place state);
	void collectOptionScreenInfo();
	void prepareDeck();
	void incrementTurn();
	bool checkClick(double mouseX, double mouseY);
	void unClick();

private:
	void findEvidenceAndReport();
	void clearLastState();
	void initializeOptionsScreen();
	void drawOptionsScreen();
	void initializeStartTurnScreen();
	void drawStartTurnScreen();
	void initializeTakeTurnScreen();
	void drawTakeTurnScreen();
	void initializeIntroducePlayerHand();
	void drawIntroducePlayerHand();
	void initializeShowPlayerHand();
	void drawShowPlayerHand();
	void initializeShowAccusorEvidence();
	void drawShowAccusorEvidence();
	void initializePlayerWins();
	void drawPlayerWins();

	Accusation mSolution;
	Accusation mCurrentAccusation;
	std::vector<Piece> mPawns;
	std::vector<Player*> mPlayers;
	std::vector<bool> mLostPlayers;
	std::vector<Button*> mButtons;
	std::vector<Card> mEvidence;
	bool mGameStarted, mPlayerAccused;
	size_t mTurnNumber, mDisprover, mClickedButtionIndex;
	place mScreenState;
	ThisRound mRoundNews;
	std::string mEvidenceToShow;
};

