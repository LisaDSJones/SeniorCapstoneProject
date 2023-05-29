#include "graphics.h"
#include "ButtonTypes.h" 
#include "Piece.h"
#include "Player.h"
#include "Game.h"
#include "glut.h"
#include "Card.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

Game::Game()
{
	mScreenState = OPTIONS;
	initializeOptionsScreen();
	mTurnNumber = -1;
	mClickedButtionIndex = -1;
}


Game::~Game()
{
//	for (std::vector< Button* >::iterator it = mButtons.begin(); it != mButtons.end(); ++it)
//		delete (*it);
}

int Game::getPlayersTurn() {
	if (mPlayers.size() == 0)
		return -1;
	if (mTurnNumber < mPlayers.size())
		return mTurnNumber;
	return mTurnNumber%mPlayers.size(); }

void Game::draw()
{
	switch (mScreenState)
	{
	case OPTIONS:
		drawOptionsScreen();
		break;
	case STARTTURN:
		drawStartTurnScreen();
		break;
	case TAKETURN:
		drawTakeTurnScreen();
		break;
	case INTRODUCEPLAYERHAND:
		drawIntroducePlayerHand();
		break;
	case SHOWPLAYERHAND:
		drawShowPlayerHand();
		break;
	case SHOWACCUSOREVIDENCE:
		drawShowAccusorEvidence();
		break;
	case PLAYERWINS:
		drawPlayerWins();
		break;
	}
	for (size_t i = 0; i < mButtons.size(); i++)
		mButtons[i]->Draw();
}
// basically the only place i could find to put the turn taking -- computer turn -- logic
void Game::setScreenState(place toState)
{
	place fromState = mScreenState;
	if (fromState == OPTIONS) {
			collectOptionScreenInfo();
			prepareDeck();
		}
	bool playerComputer = mPlayers[getPlayersTurn()]->isComputer();
	int playerTurn = getPlayersTurn();

	clearLastState();
	switch (toState)
	{
	case STARTTURN: {
		incrementTurn();
		mScreenState = STARTTURN;
		mPlayerAccused = false;
		// Start Turn and Take Turn states are to be skipped and computer Accuses
		playerComputer = mPlayers[getPlayersTurn()]->isComputer();
		playerTurn = getPlayersTurn();
		if (playerComputer)
		{
			mCurrentAccusation = mPlayers[playerTurn]->getAccusation();
			if (mPlayers[playerTurn]->isFinalAccusation()) {
				setScreenState(PLAYERWINS);
				return;
			}
			findEvidenceAndReport();
			if (mDisprover > mPlayers.size()) { // no one could disprove
				// tell computers results
				for (size_t i = 0; i < mPlayers.size(); i++)
					mPlayers[i]->newEvidence("", mCurrentAccusation, mPlayers[playerTurn]->getName(), mPlayers[playerTurn]->getName());
				// end turn = go to next turn
				setScreenState(STARTTURN);
				return;
			}
			// someone could disprove	
			setScreenState(INTRODUCEPLAYERHAND);
			return;
		}
		// otherwise, Human is notified of the beginning of their turn
		initializeStartTurnScreen();
		mScreenState = STARTTURN;
		break;
	}
	case TAKETURN: 
		if (playerComputer) { // always human Turn 
			setScreenState(STARTTURN);
			return;
		}
		initializeTakeTurnScreen();
		mScreenState = TAKETURN;
		break;
	case INTRODUCEPLAYERHAND: {
		// if it is a computer's turn we've already searched for evidence
		if (!playerComputer)
			findEvidenceAndReport();
		// if No one can disprove a player's hand (computerPlayer, won't get this far)
		if (mDisprover >= mPlayers.size()) { // No one can Disprove
			setScreenState(SHOWACCUSOREVIDENCE); 
			return;
		}
		// if a computer can disprove a hand 
		if (mPlayers[mDisprover]->isComputer())
		{
			// skip the card picking and show the accusor the evidence
			mEvidenceToShow = mEvidence[0].getText();
			// if the accusor is a computer, now is the time to recieve the evidence
			for (size_t i = 0; i < mPlayers.size(); i++)
				if(i == playerTurn)
					mPlayers[i]->newEvidence(mEvidence[0].getText(), mCurrentAccusation, mPlayers[playerTurn]->getName(), mPlayers[mDisprover]->getName());
				else // now is the time for bystander computers to take note
					mPlayers[i]->newEvidence("noCard", mCurrentAccusation, mPlayers[playerTurn]->getName(), mPlayers[mDisprover]->getName());

			setScreenState(SHOWACCUSOREVIDENCE);
			return;
		}
		// otherwise a human is the disprover. let them know they can disprove
		initializeIntroducePlayerHand();
		mScreenState = INTRODUCEPLAYERHAND;
		break;
	}
	case SHOWPLAYERHAND: 
		if (mPlayers[mDisprover]->isComputer()) // If this isn't initialized I'll use the revolver
		{ // and it sure better not be a computer
			std::cout << "ERROR: someone's logicker broked (setScreenState(SHOWPLAYERHAND))" << std::endl;
		}
		initializeShowPlayerHand();
		mScreenState = SHOWPLAYERHAND;
		break;
	case SHOWACCUSOREVIDENCE:
		for (size_t i = 0; i < mPlayers.size(); i++)
			if (i == playerTurn)
				mPlayers[i]->newEvidence(mEvidenceToShow, mCurrentAccusation, mPlayers[playerTurn]->getName(), mPlayers[(mDisprover<mPlayers.size()) ? mDisprover : playerTurn]->getName());
			else // now is the time for bystander computers to take note
				mPlayers[i]->newEvidence("noCard", mCurrentAccusation, mPlayers[playerTurn]->getName(), mPlayers[(mDisprover<mPlayers.size()) ? mDisprover : playerTurn]->getName());
		if (playerComputer) {
			setScreenState(STARTTURN);
			return;
		}
		initializeShowAccusorEvidence();
		mScreenState = SHOWACCUSOREVIDENCE;
		break;
	case PLAYERWINS:
		initializePlayerWins();
		mScreenState = PLAYERWINS;
		break;
	case OPTIONS:
		initializeOptionsScreen();
		while (mPlayers.size() > 0)
			mPlayers.pop_back();
		mScreenState = OPTIONS;
		break;
	}
}

// not idempotent
void Game::findEvidenceAndReport()
{
	int playerTurn = getPlayersTurn(); // start the scan at playerTurn +1 and go mPlayers.size() times
	int whosTurnisIt = playerTurn;
	for (size_t i = 1; i < mPlayers.size(); i++)
	{
		playerTurn = (playerTurn + 1) % mPlayers.size();
		mEvidence = mPlayers[playerTurn]->checkHand(mCurrentAccusation);
		if (mEvidence.size() >= 1) {
			mRoundNews.addAccusation(mPlayers[whosTurnisIt]->getName(), mCurrentAccusation, mPlayers[playerTurn]->getName());
			mDisprover = playerTurn;
			return;
		}
	}
	mRoundNews.addAccusation(mPlayers[getPlayersTurn()]->getName(), mCurrentAccusation, NOCARD);
	mDisprover = mPlayers.size();
}

void Game::clearLastState() {
	while (mButtons.size() > 0)
		mButtons.pop_back();
	while (mPawns.size() > 0)
		mPawns.pop_back();
}

void Game::collectOptionScreenInfo() { // AND prepare deck
	size_t i;
	playerType player = NONE;
	std::vector<cardName> players;
	std::vector<bool> isComputers;
	std::vector<bool> difficultys;
	for (i = 0; i < 6; i++) {
		//collect the player name from the button
		if ((*mButtons[i]).getID() == "ScarletPlayerType") 
			players.push_back(MISSSCARLET);
		else if ((*mButtons[i]).getID() == "PeacockPlayerType") 
			players.push_back(MRSPEACOCK);
		else if ((*mButtons[i]).getID() == "WhitePlayerType") 
			players.push_back(MRSWHITE);
		else if ((*mButtons[i]).getID() == "PlumPlayerType") 
			players.push_back(PROFPLUM);
		else if ((*mButtons[i]).getID() == "MustardPlayerType") 
			players.push_back(COLONELMUSTARD);
		else if ((*mButtons[i]).getID() == "GreenPlayerType") 
			players.push_back(MRGREEN);

		player = (*dynamic_cast<PlayerTypeOptionButton*>(mButtons.at(i))).getOptionSelected();
		if (player == HUMAN) {
			isComputers.push_back(false);
			difficultys.push_back(false);
		}
		else if (player == COMPUTER) {
			isComputers.push_back(true);
			if ((*dynamic_cast<DifficultyOptionButton*>(mButtons.at(i + 6))).getOptionSelected() == EASY)
				difficultys.push_back(false);
			else
				difficultys.push_back(true);
		}
		else if (player == NONE)
			players.pop_back();

		player = NONE; // otherwise we will almost always add 6 players with duplicates
	}
	for (i = 0; i < players.size(); i++) {
		if (isComputers[i])
			mPlayers.push_back(new ComputerPlayer(players[i], players, difficultys[i]));
		else
			mPlayers.push_back(new HumanPlayer(players[i], players));
		mLostPlayers.push_back(false);
	}

}

void Game::prepareDeck()
{
	std::vector<Card> deck;
	// suspects
	deck.push_back(Card(MISSSCARLET, SUSPECT,"Miss Scarlet"));
	deck.push_back(Card(MRSPEACOCK, SUSPECT, "Mrs. Peacock"));
	deck.push_back(Card(MRSWHITE, SUSPECT, "Mrs. White"));
	deck.push_back(Card(PROFPLUM, SUSPECT, "Prof. Plum"));
	deck.push_back(Card(COLONELMUSTARD, SUSPECT, "Col. Mustard"));
	deck.push_back(Card(MRGREEN, SUSPECT, "Mr. Green"));
	// Weapons
	deck.push_back(Card(ROPE, WEAPON, "Rope"));
	deck.push_back(Card(LEADPIPE, WEAPON, "Lead Pipe"));
	deck.push_back(Card(KNIFE, WEAPON, "Knife"));
	deck.push_back(Card(WRENCH, WEAPON, "Wrench"));
	deck.push_back(Card(CANDLESTICK, WEAPON, "Candlestick"));
	deck.push_back(Card(REVOLVER, WEAPON, "Revolver"));
	// Rooms
	deck.push_back(Card(KITCHEN, ROOM, "Kitchen"));
	deck.push_back(Card(DININGROOM, ROOM, "Dining Room"));
	deck.push_back(Card(LOUNGE, ROOM, "Lounge"));
	deck.push_back(Card(BALLROOM, ROOM, "Ballroom"));
	deck.push_back(Card(HALL, ROOM, "Hall"));
	deck.push_back(Card(CONSERVATORY, ROOM, "Conservatory"));
	deck.push_back(Card(BILLIARDROOM, ROOM, "Billiard Room"));
	deck.push_back(Card(LIBRARY, ROOM, "Library"));
	deck.push_back(Card(STUDY, ROOM, "Study"));

	// Shuffle Deck
	std::srand(unsigned(std::time(0)));
	std::random_shuffle(deck.begin(), deck.end());

	// Distributing Cards
	bool hasSuspect = false, hasWeapon = false, hasRoom = false;
	int playerTurn = 0;
	int numPlayers = mPlayers.size();

	// debugging var
	int sumCards = 0;

	// empty hands
	std::vector<int>numCards;
	numCards.push_back(0);
	numCards.push_back(0);
	numCards.push_back(0);
	numCards.push_back(0);
	numCards.push_back(0);
	numCards.push_back(0);

	for (size_t i = 0; i < deck.size(); i++) {
		// hide the first suspect in the deck
		if ((!hasSuspect) && deck[i].getType() == SUSPECT) {
			mSolution.setSuspect(deck[i].getName());
			hasSuspect = true;
		}
		// hide the first weapon in the deck
		else if ((!hasWeapon) && deck[i].getType() == WEAPON) {
			mSolution.setWeapon(deck[i].getName());
			hasWeapon = true;
		}
		// hide the first room in the deck
		else if ((!hasRoom) && deck[i].getType() == ROOM) {
			mSolution.setRoom(deck[i].getName());
			hasRoom = true;
		}
		else
		{
			mPlayers[playerTurn]->addCard(deck[i]);
			numCards[playerTurn]++;
			playerTurn = (playerTurn + 1) % mPlayers.size();
		}
	}

	for (size_t i = 0; i < numCards.size(); i++)
		sumCards += numCards[i];
	return;
}

void Game::incrementTurn()
{
	mTurnNumber++;
	int playerTurn;
	for (size_t i = 0; i < mPlayers.size(); i++)
	{
		playerTurn = getPlayersTurn();
		if (!mLostPlayers[playerTurn])
			break;
		else {
			mTurnNumber++;
		}
	}
}

bool Game::checkClick(double mouseX, double mouseY)
{
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		if (mButtons[i]->Clicked(mouseX, mouseY))
		{
			mClickedButtionIndex = i;
			mButtons[i]->OnClick();
			if (mButtons[i]->getID() == "AccuseButton") {
				if (!mPlayerAccused) {
					mCurrentAccusation = mPlayers[getPlayersTurn()]->getAccusation();
					mPlayerAccused = true;
					setScreenState(INTRODUCEPLAYERHAND);
				}
			}
			else if (mButtons[i]->getID() == "FinalAccuseButton") {
				if (!mPlayerAccused) {
					mCurrentAccusation = mPlayers[getPlayersTurn()]->getAccusation();
					mPlayerAccused = true;
					setScreenState(PLAYERWINS);
				}
			}
			else if (mButtons[i]->getID() == "OkButton") {
				mEvidenceToShow = mButtons[i]->getText();
				setScreenState(dynamic_cast<NavigationButton*>(mButtons.at(i))->getOptionSelected());
			}
			return true;
		}
	}
	if (mScreenState == TAKETURN && getPlayersTurn() >= 0 && mPlayers[getPlayersTurn()]->checkClick(mouseX, mouseY))
		return true;
	return false;
}

void Game::unClick()
{
	if (getPlayersTurn() >= 0)
		mPlayers[getPlayersTurn()]->unClick();

	if (mClickedButtionIndex >= 0 && mButtons.size() > mClickedButtionIndex)
		mButtons[mClickedButtionIndex]->setPushed(false);
	mClickedButtionIndex = -1;
}



//////////////////////////////////////////
//// Private Methods
//////////////////////////////////////////



void Game::initializeOptionsScreen()
{
	// Pawns
	mPawns.push_back(Piece(-.5, 17.5, RED, false)); // Miss Scarlet
	mPawns.push_back(Piece(-.5, 10.5, BLUE, false)); // Mrs. Peacock
	mPawns.push_back(Piece(-.5, 3.5, WHITE, false)); // Mrs. White
	mPawns.push_back(Piece(11.5, 17.5, PURPLE, false)); // Prof. Plum
	mPawns.push_back(Piece(11.5, 10.5, YELLOW, false)); // ColonelMustard
	mPawns.push_back(Piece(11.5, 3.5, GREEN, false)); // Mr. Green

	// Human/Computer/None buttons
	mButtons.push_back(new PlayerTypeOptionButton("Human", 4, H - 0 * 7 - 4, 3, 1.5, RED, "ScarletPlayerType"));
	mButtons.push_back(new PlayerTypeOptionButton("Human", 4, H - 1 * 7 - 4, 3, 1.5, BLUE, "PeacockPlayerType"));
	mButtons.push_back(new PlayerTypeOptionButton("Human", 4, H - 2 * 7 - 4, 3, 1.5, WHITE, "WhitePlayerType"));
	mButtons.push_back(new PlayerTypeOptionButton("Human", 16, H - 0 * 7 - 4, 3, 1.5, PURPLE, "PlumPlayerType"));
	mButtons.push_back(new PlayerTypeOptionButton("Human", 16, H - 1 * 7 - 4, 3, 1.5, YELLOW, "MustardPlayerType"));
	mButtons.push_back(new PlayerTypeOptionButton("Human", 16, H - 2 * 7 - 4, 3, 1.5, GREEN, "GreenPlayerType"));

	// if computer, Easy or Hard
	mButtons.push_back(new DifficultyOptionButton("Easy", 7.5, H - 0 * 7 - 4, 3, 1.5, RED, "ScarletCPUDifficulty"));
	mButtons.push_back(new DifficultyOptionButton("Easy", 7.5, H - 1 * 7 - 4, 3, 1.5, BLUE, "PeacockCPUDifficulty"));
	mButtons.push_back(new DifficultyOptionButton("Easy", 7.5, H - 2 * 7 - 4, 3, 1.5, WHITE, "WhiteCPUDifficulty"));
	mButtons.push_back(new DifficultyOptionButton("Easy", 19.5, H - 0 * 7 - 4, 3, 1.5, PURPLE, "PlumCPUDifficulty"));
	mButtons.push_back(new DifficultyOptionButton("Easy", 19.5, H - 1 * 7 - 4, 3, 1.5, YELLOW, "MustardCPUDifficulty"));
	mButtons.push_back(new DifficultyOptionButton("Easy", 19.5, H - 2 * 7 - 4, 3, 1.5, GREEN, "GreenCPUDifficulty"));

	for (size_t i = mButtons.size() - 1; i > mButtons.size() - 7; i--) {
		(*mButtons[i]).setVisible(false);
	}

	mButtons.push_back(new NavigationButton("Start", W / 2 - 3, 3, 4, 2, .5, .5, .5, "OkButton", STARTTURN));
}

void Game::drawOptionsScreen()
{
	// Border
	glColor3d(0, 0, 0);
	DrawRectangle(-1, -1, W + 1, H + 1);

	// Background
	glColor3d(.1, .1, .1);
	DrawRectangle(0, 0, W, H);

	// Hide or Show specific Buttons
	for (size_t i = 0; i < 6; i++) {
		PlayerTypeOptionButton *child = dynamic_cast<PlayerTypeOptionButton*>(mButtons.at(i));
		if (child) {
			if (child->getOptionSelected() == COMPUTER)
				mButtons[i + 6]->setVisible(true);
			else
				mButtons[i + 6]->setVisible(false);
		}
	}

	// Draw Pawns
	for (size_t i = 0; i < mPawns.size(); i++) {
		mPawns[i].Draw(6);
	}

	// Subtitle Pawns
	DrawTextCentered(2.5, 16.8, "Miss Scarlet", RED);
	DrawTextCentered(2.5, 9.8, "Mrs. Peacock", BLUE);
	DrawTextCentered(2.5, 2.8, "Mrs. White", WHITE);
	DrawTextCentered(14.5, 16.8, "Professor Plum", PURPLE);
	DrawTextCentered(14.5, 9.8, "Colonel Mustard", YELLOW);
	DrawTextCentered(14.5, 2.8, "Mr. Green", GREEN);
}

void Game::initializeStartTurnScreen()
{
	int playerTurn = getPlayersTurn();

	double cW = W / 2; // center of width
	double cH = H / 2;    // center of height
	double pSize = 7;    // piece size

	Player* player = mPlayers[playerTurn];
	mPawns.push_back(player->getPawn());

	double r = player->getR(), g = player->getG(), b = player->getB();
	mButtons.push_back(new NavigationButton("Begin Turn", cW-2, cH-2, 4, 2, r, g, b, "OkButton", TAKETURN));
																  
}

void Game::drawStartTurnScreen()
{
	int playerTurn = getPlayersTurn();
	if (mPlayers.size() == 0)
		return;

	// draw Border and Background
	glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
	DrawRectangle(-1, -1, W + 1, H + 1);
	glColor3d(.1, .1, .1);
	DrawRectangle(0, 0, W, H);
	// Subtitle Pawns
	switch (mPlayers[playerTurn]->getName())
	{
	case MISSSCARLET: {
		DrawTextCentered(W/2, H/2-1, "Miss Scarlet's Turn", RED);
		}break;
	case MRSPEACOCK: {
		DrawTextCentered(W / 2, H / 2 - 1, "Mrs. Peacock's Turn", BLUE);
	} break;
	case MRSWHITE: {
		DrawTextCentered(W / 2, H / 2 - 1, "Mrs. White's Turn", WHITE);
	} break;
	case PROFPLUM: {
		DrawTextCentered(W / 2, H / 2 - 1, "Prof. Plum's Turn", PURPLE);
	} break;
	case COLONELMUSTARD: {
		DrawTextCentered(W / 2, H / 2 - 1, "Col. Mustard's Turn", YELLOW);
	} break;
	case MRGREEN: {
		DrawTextCentered(W / 2, H / 2 - 1, "Mr. Green's Turn", GREEN);
	} break;
	}

	// Draw Pawns
	for (size_t i = 0; i < mPawns.size(); i++) {
		mPawns[i].Draw(7);
	}
}

void Game::drawTakeTurnScreen()
{
	int playerTurn = getPlayersTurn();
	// draw Border and Background
	glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
	DrawRectangle(-1, -1, W + 1, H + 1);
	glColor3d(.1, .1, .1);
	DrawRectangle(0, 0, W, H);
	
	// Draw Notecard
	mPlayers[playerTurn]->drawNotecard();

	// Draw round News
	mRoundNews.Draw();
}

void Game::initializeIntroducePlayerHand()
{
//	mButtons.push_back(new NavigationButton("Ok", W / 2 - 1.5, H / 2 - 2, 3, 2, mPlayers[getPlayersTurn()]->getR(), mPlayers[getPlayersTurn()]->getG(), mPlayers[getPlayersTurn()]->getB(), "OkButton", TAKETURN));
	mPawns.push_back(mPlayers[mDisprover]->getPawn());
	mButtons.push_back(new NavigationButton("Ok", W / 2 - 1.5, H / 2 - 2, 3, 2, mPlayers[mDisprover]->getR(), mPlayers[mDisprover]->getG(), mPlayers[mDisprover]->getB(), "OkButton", SHOWPLAYERHAND));
}

void Game::drawIntroducePlayerHand()
{
	int playerTurn = getPlayersTurn();

	// draw Border and Background
	glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
	DrawRectangle(-1, -1, W + 1, H + 1);
	glColor3d(.1, .1, .1);
	DrawRectangle(0, 0, W, H);

	// draw pawn
	for(size_t i = 0; i <mPawns.size(); i++)
		mPawns[i].Draw(7);


	double centerX = W / 2;
	if (mDisprover < mPlayers.size()) {
		glColor3d(mPlayers[mDisprover]->getR(), mPlayers[mDisprover]->getG(), mPlayers[mDisprover]->getB());
		DrawTextCenteredStr(centerX, H / 2 - 1, cardStrings[mPlayers[mDisprover]->getName()] + ", you can disprove " + cardStrings[mPlayers[getPlayersTurn()]->getName()] + "'s Accusation");
	}

}

void Game::initializeShowPlayerHand()
{

	for (size_t i = 0; i < mEvidence.size(); i++)
	{
		mButtons.push_back(new NavigationButton(mEvidence[i].getText(),W/2 - mEvidence.size()*2 +i*4+1, H / 2 - 1.5, 3, 2, mPlayers[mDisprover]->getR(), mPlayers[mDisprover]->getG(), mPlayers[mDisprover]->getB(), "OkButton", SHOWACCUSOREVIDENCE));
	}
}

void Game::drawShowPlayerHand()
{
	int playerTurn = getPlayersTurn();

	// draw Border and Background color of player turn
	glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
	DrawRectangle(-1, -1, W + 1, H + 1);
	glColor3d(.1, .1, .1);
	DrawRectangle(0, 0, W, H);

	// write what the player accused
	glColor3d(mPlayers[mDisprover]->getR(), mPlayers[mDisprover]->getG(), mPlayers[mDisprover]->getB());
	std::string message = cardStrings[mPlayers[getPlayersTurn()]->getName()] + " Accused " + mCurrentAccusation.toString();
	DrawTextCenteredStr(W / 2+.5, H / 2+1, message);
	// write instructions
	if(mPlayers[playerTurn]->getName() <= 2)
		message = "Pick a card that disproves her accusation." ;
	else 
		message = "Pick a card that disproves his accusation.";
	DrawTextCenteredStr(W / 2+.5, H / 2, message);
}

void Game::initializeShowAccusorEvidence()
{
	mButtons.push_back(new NavigationButton("Ok", W / 2-.5, H / 2 - 1.5, 3, 2, mPlayers[getPlayersTurn()]->getR(), mPlayers[getPlayersTurn()]->getG(), mPlayers[getPlayersTurn()]->getB(), "OkButton", TAKETURN));
}

void Game::drawShowAccusorEvidence()
{
	int playerTurn = getPlayersTurn();

	// draw Border and Background
	glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
	DrawRectangle(-1, -1, W + 1, H + 1);
	glColor3d(.1, .1, .1);
	DrawRectangle(0, 0, W, H);

	glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
	
	// if this player Just lost
	if (mLostPlayers[getPlayersTurn()]) {
		std::string message = " You Lose ";
		DrawTextCenteredStr(W / 2 + .5, H / 2 + 3, message);
		message = "You said it was " + mCurrentAccusation.toString() + ", but";
		DrawTextCenteredStr(W / 2 + .5, H / 2 + 2, message);
		message = mEvidenceToShow + " is what really happened.";
		DrawTextCenteredStr(W / 2 + .5, H / 2 + 1, message);
		return;
	}
	
	if (mEvidenceToShow.size() > 3 && mEvidence.size() != 0) {
		bool isPerson = false;
		if (strcmp(new char(mEvidenceToShow[0]), "M") == 1) 
			isPerson = true;
		else if ( strcmp(new char(mEvidenceToShow[0]), "C") == 1 && strcmp(new char(mEvidenceToShow[1]), "o") == 1 && strcmp(new char(mEvidenceToShow[2]), "l") == 1)
			isPerson = true;
		else if ( strcmp(new char(mEvidenceToShow[0]), "P") == 1 && strcmp(new char(mEvidenceToShow[1]), "r") == 1 )
			isPerson = true;

		std::string message = "The " + mEvidenceToShow + " was not involved with the Murder.";
		if (isPerson) 
			message = mEvidenceToShow + " was not involved with the Murder.";
		DrawTextCenteredStr(W / 2 + .5, H / 2 + 1, message);

		glColor3d(mPlayers[mDisprover]->getR(), mPlayers[mDisprover]->getG(), mPlayers[mDisprover]->getB());
		DrawTextCenteredStr(W / 2 + .5, H / 2 + 2, cardStrings[mPlayers[mDisprover]->getName()] + " says:");
	}
	else {
		glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
		DrawTextCenteredStr(W/2, H / 2 - 1, "No one could disprove your Accusation!!");
	}
}

void Game::initializePlayerWins()
{
	if (mCurrentAccusation == mSolution)
	{
		mButtons.push_back(new NavigationButton("Ok", W / 2 - 1.5, H / 2 - 1.5, 3, 2, mPlayers[getPlayersTurn()]->getR(), mPlayers[getPlayersTurn()]->getG(), mPlayers[getPlayersTurn()]->getB(), "OkButton", OPTIONS));
	}
	else {
		// YOU LOSE!
		setScreenState(SHOWACCUSOREVIDENCE);
		mEvidenceToShow = mSolution.toString();
		mLostPlayers[getPlayersTurn()] = true;
		draw();
	}
}

void Game::drawPlayerWins()
{
	int playerTurn = getPlayersTurn();
	if (!mLostPlayers[playerTurn]) {
		// draw Border and Background
		glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
		DrawRectangle(-1, -1, W + 1, H + 1);
		glColor3d(.1, .1, .1);
		DrawRectangle(0, 0, W, H);

		glColor3d(mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB());
		std::string message = cardStrings[mPlayers[playerTurn]->getName()] + " Wins!! ";
		DrawTextCenteredStr(W / 2, H / 2 + 1, message);
	}
	else {
		setScreenState(SHOWACCUSOREVIDENCE);
		draw();
	}
}

void Game::initializeTakeTurnScreen()
{
	int playerTurn = getPlayersTurn();
	mButtons.push_back(new NavigationButton("End Turn", 1, 6, 3, 2, mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB(), "OkButton", STARTTURN));
	mButtons.push_back(new Button("Accuse", 5, 6, 3, 2, mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB(), "AccuseButton"));
	mButtons.push_back(new Button("Final Accusation", 2.5, 3, 4, 2, mPlayers[playerTurn]->getR(), mPlayers[playerTurn]->getG(), mPlayers[playerTurn]->getB(), "FinalAccuseButton"));
	if (mPlayerAccused) {
		mButtons[mButtons.size()-2]->setDisabled(true);
		mButtons[mButtons.size()-1]->setDisabled(true);
	}
}
