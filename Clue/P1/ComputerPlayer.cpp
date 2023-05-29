#include "ComputerPlayer.h"


ComputerPlayer::ComputerPlayer(cardName name, std::vector<cardName> opponents, bool isHard)
	:Player(name, true), mIsHard(isHard)
{
	mIsFinalAccustion = false;
	mTHESuspect = NOCARD;
	mTHEWeapon = NOCARD;
	mTHERoom = NOCARD;

	mPlayers = opponents;
	for (size_t i = 0; i < mPlayers.size(); i++)
		mMetMaxCards.push_back(false);

	// create quick lookup vectors
	for (size_t i = 0; i < 6; i++)
		mPossibleSuspects.push_back(cardName(i));
	for (size_t i = 0 + 6; i < 6 + 6; i++)
		mPossibleWeapons.push_back(cardName(i));
	for (size_t i = 0 + 12; i < 9 + 12; i++)
		mPossibleRooms.push_back(cardName(i));

	// create suspects, weapons, and room logic tables
	for (size_t i = 0; i < opponents.size(); i++)
	{
		mSuspects.push_back(std::vector<hasCardToggleOption>());
		mWeapons.push_back(std::vector<hasCardToggleOption>());
		mRooms.push_back(std::vector<hasCardToggleOption>());
		for (size_t j = 0; j < 6; j++)
		{
			mSuspects[i].push_back(UNKNOWN);
			mWeapons[i].push_back(UNKNOWN);
		}
		for (size_t j = 0; j < 9; j++)
			mRooms[i].push_back(UNKNOWN);
	}

}

ComputerPlayer::~ComputerPlayer()
{
}

void ComputerPlayer::addCard(Card card)
{
	mCards.push_back(card);
	learnHasCard(getName(), true, card.getName());
}

Accusation ComputerPlayer::getAccusation()
{
	// its the beginning of the computer turn NOW
	scanNotecard();
	srand(time(0));

	cardName suspect = mPossibleSuspects[rand() % mPossibleSuspects.size()];
	cardName weapon = mPossibleWeapons[rand() % mPossibleWeapons.size()];
	cardName room = mPossibleRooms[rand() % mPossibleRooms.size()];
	/// the things i do...
	if (mTHESuspect != NOCARD) {
		if (mTHEWeapon != NOCARD) {
			// know     suspect,     weapon,     room
			if (mTHERoom != NOCARD) {
				suspect = mTHESuspect; weapon = mTHEWeapon; room = mTHERoom;
				mIsFinalAccustion = true;
			}
			// know     suspect,     weapon, not room
			else {
				while(ownCard(room))
					room = mPossibleRooms[rand() % mPossibleRooms.size()];
			}
		}
		else {
			// know     suspect, not weapon,     room
			if (mTHERoom != NOCARD) {
				while (ownCard(weapon))
					weapon = mPossibleWeapons[rand() % mPossibleWeapons.size()];
			}
			// know     suspect, not weapon, not room
			else {
				while (ownCard(weapon) && ownCard(room)) {
					room = mPossibleRooms[rand() % mPossibleRooms.size()];
					weapon = mPossibleWeapons[rand() % mPossibleWeapons.size()];
				}
			}
		}
	}
	else {
		if (mTHEWeapon != NOCARD) {
			// know not suspect,     weapon,     room
			if (mTHERoom != NOCARD) {
				while (ownCard(suspect))
					suspect = mPossibleSuspects[rand() % mPossibleSuspects.size()];
			}
			// know not suspect,     weapon, not room
			else {
				while (ownCard(suspect) && ownCard(room)) {
					room = mPossibleRooms[rand() % mPossibleRooms.size()];
					suspect = mPossibleSuspects[rand() % mPossibleSuspects.size()];
				}

			}
		}
		else {
			// know not suspect, not weapon,     room
			if (mTHERoom != NOCARD) {
				while (ownCard(suspect) && ownCard(weapon)) {
					weapon = mPossibleWeapons[rand() % mPossibleWeapons.size()];
					suspect = mPossibleSuspects[rand() % mPossibleSuspects.size()];
				}

			}
			// know not suspect, not weapon, not room
			else {
				while (ownCard(suspect) && ownCard(weapon) && ownCard(room)) {
					weapon = mPossibleWeapons[rand() % mPossibleWeapons.size()];
					suspect = mPossibleSuspects[rand() % mPossibleSuspects.size()];
					room = mPossibleRooms[rand() % mPossibleRooms.size()];
				}
			}
		}

	}
	Accusation accusation(suspect, weapon, room);
	return accusation;
}

std::vector<Card> ComputerPlayer::checkHand(Accusation accusation)
{
	std::vector<Card> possibleEvedence;
	for (size_t i = 0; i < mCards.size(); i++)
		if (accusation == mCards[i].getName())
			possibleEvedence.push_back(mCards[i]);
	std::vector<Card> evedence;
	int randomNumber = rand() % (evedence.size()+1);

	if (randomNumber >= 3  && possibleEvedence.size() > 2)
		evedence.push_back(possibleEvedence[2]);
	else if (randomNumber >=2 && possibleEvedence.size() > 1)
		evedence.push_back(possibleEvedence[1]);
	else if (possibleEvedence.size() >= 1)
		evedence.push_back(possibleEvedence[0]);
	return evedence;
}

void ComputerPlayer::newEvidence(std::string evidence, Accusation accusation, cardName accuser, cardName disprover)
{
	// computer dosn't learn anything about others' accusations unless they're hard;
	if (accuser != getName() && !mIsHard)
		return;
	cardName card = lookUpCardName(evidence); // possibly NOCARD
	if(card != NOCARD)
		learnHasCard(disprover, true, card);
	// any one who does not answer, does not have any of those cards
	size_t begin = (lookUpPlayerOrder(accuser)+1)%mPlayers.size(), end = lookUpPlayerOrder(disprover);
	for (size_t i = begin; i != end; i= (i+1)%mPlayers.size())
	{
		if (mPlayers[i] == getName())
			continue;
		learnHasCard(mPlayers[i], false, accusation.getSuspect());
		learnHasCard(mPlayers[i], false, accusation.getWeapon());
		learnHasCard(mPlayers[i], false, accusation.getRoom());
	}
	// TODO: disprover has at least one of these cards;
}

bool ComputerPlayer::isFinalAccusation()
{
	return mIsFinalAccustion;
}

cardName ComputerPlayer::lookUpCardName(std::string cardText)
{
	for (size_t i = 0; i < 22; i++)
	{
		if (cardStrings[i]== cardText )
			return cardName(i);
	}
	return NOCARD;
}

cardType ComputerPlayer::lookUpCardType(cardName card)
{
	if (card <6)
		return SUSPECT;
	if (card < 12)
		return WEAPON;
	else
		return ROOM;
	// technically could be nocard
}

size_t ComputerPlayer::lookUpPlayerOrder(cardName player)
{
	for (size_t i = 0; i < mPlayers.size(); i++)
		if (mPlayers[i] == player)
			return i;
	return mPlayers.size();
}

int ComputerPlayer::lookUpMaxCards(cardName player)
{
	// 21 cards - 3 MurderCards
	// 18 cards dealt
	if (mPlayers.size() == 6)
		// 3 3 3 3 3 3
		return 3;
	if (mPlayers.size() == 3)
		// 6 6 6 -- (0_0)
		return 6;
	if (mPlayers.size() == 2)
		// 9 9
		return 9;

	// now we have to figure out distance from Starting Player
	int start, distFromStartingPlayer;
	cardName i = MISSSCARLET;
	do {
		start = lookUpPlayerOrder(i);
		i = cardName(i + 1);
	} while (start >= mPlayers.size());
	distFromStartingPlayer = lookUpPlayerOrder(player) - start;
	if (distFromStartingPlayer < 0) distFromStartingPlayer += mPlayers.size();

	if (mPlayers.size() == 5) {
		// 4 4 4 3 3 -- starting with MISSSCARLET
		if (distFromStartingPlayer < 3)
			return 4;
		else
			return 3;
	}
	if (mPlayers.size() == 4)
		// 5 5 4 4
		if (distFromStartingPlayer < 2)
			return 5;
		else
			return 4;

	return 18; // i don't know why this would ever be the case
}

void ComputerPlayer::noMoreCardsInCol(size_t i)
{
	for (size_t j = 0; j < mSuspects[i].size(); j++)
		if (mSuspects[i][j] == UNKNOWN)
			mSuspects[i][j] = NO;
	for (size_t j = 0; j < mWeapons[i].size(); j++)
		if (mWeapons[i][j] == UNKNOWN)
			mWeapons[i][j] = NO;
	for (size_t j = 0; j < mRooms[i].size(); j++)
		if (mRooms[i][j] == UNKNOWN)
			mRooms[i][j] = NO;
}

void ComputerPlayer::scanNotecard()
{
	//search all notecards for : DONE rows of NO  : DONE all but one card is owned : TODO: if Hard -- player owns at least one of...
	// DONE:check to see max number of cards has been met by any player
	std::vector<int> numCards;
	std::vector<bool> cardOwned;
	int allCards = 6;
	for (size_t i = 0; i < mPlayers.size(); i++)
		numCards.push_back(0);
	for (size_t i = 0; i < 6; i++)
		cardOwned.push_back(false);

	// scan Suspects
	for (size_t i = 0; i < mSuspects[0].size(); i++) // we're going cards X players but mSuspects is players X cards
	{
		int numNOsInRow = 0;
		for (size_t j = 0; j < mSuspects.size(); j++) {
			if (mSuspects[j][i] == NO) {
				numNOsInRow++;
				if (numNOsInRow == mPlayers.size())
					mTHESuspect = cardName(i);
			}
			else if (mSuspects[j][i] == YES) {
				numCards[j]++;
				allCards--;
				cardOwned[i] = true;
				if (!mMetMaxCards[j] && numCards[j] == lookUpMaxCards(mPlayers[j])) {
					mMetMaxCards[j] = true;
					noMoreCardsInCol(j);
					// lots of new info. restart search
					scanNotecard();
					return;
				}
			}
		}
	}
	if (allCards == 1 && mTHESuspect == NOCARD)
		for (size_t i = 0; i < cardOwned.size(); i++)
			if (!cardOwned[i])
				mTHESuspect = cardName(i);

	// scan Weapons
	allCards = 6;
	for (size_t i = 0; i < 6; i++)
		cardOwned[i] = false;

	for (size_t i = 0; i < mWeapons[0].size(); i++) // we're going cards X players but mWeapons is players X cards
	{
		int numNOsInRow = 0;
		for (size_t j = 0; j < mWeapons.size(); j++) {
			if (mWeapons[j][i] == NO) {
				numNOsInRow++;
				if (numNOsInRow == mPlayers.size())
					mTHEWeapon = cardName(i+6);
			}
			else if (mWeapons[j][i] == YES) {
				numCards[j]++;
				allCards--;
				cardOwned[i] = true;
				if (!mMetMaxCards[j] && numCards[j] == lookUpMaxCards(mPlayers[j])) {
					mMetMaxCards[j] = true;
					noMoreCardsInCol(j);
					// lots of new info. restart search
					scanNotecard();
					return;
				}
			}
		}
	}

	if (allCards == 1 && mTHEWeapon == NOCARD)
		for (size_t i = 0; i < cardOwned.size(); i++)
			if (!cardOwned[i])
				mTHEWeapon = cardName(i+6);

	// scan Rooms
	allCards = 6;
	for (size_t i = 0; i < 6; i++)
		cardOwned[i] = false;
	for (size_t i = 0; i < 3; i++)
		cardOwned.push_back(false);
	for (size_t i = 0; i < mRooms[0].size(); i++) // we're going cards X players but mRooms is players X cards
	{
		int numNOsInRow = 0;
		for (size_t j = 0; j < mRooms.size(); j++) {
			if (mRooms[j][i] == NO) {
				numNOsInRow++;
				if (numNOsInRow == mPlayers.size())
					mTHERoom = cardName(i+12);
			}
			else if (mRooms[j][i] == YES) {
				numCards[j]++;
				allCards--;
				cardOwned[i] = true;
				if (!mMetMaxCards[j] && numCards[j] == lookUpMaxCards(mPlayers[j])) {
					mMetMaxCards[j] = true;
					noMoreCardsInCol(j);
					// lots of new info. restart search
					scanNotecard();
					return;
				}
			}
		}
	}
	if (allCards == 1 && mTHESuspect == NOCARD)
		for (size_t i = 0; i < cardOwned.size(); i++)
			if (!cardOwned[i])
				mTHERoom = cardName(i+12);
}

void ComputerPlayer::removeFromPossibleCards(cardName card)
{
	cardType cType = lookUpCardType(card);
	switch (cType)
	{
	case SUSPECT:
		for (size_t i = 0; i < mPossibleSuspects.size(); i++)
			if (mPossibleSuspects[i] == card)
				mPossibleSuspects.erase(mPossibleSuspects.begin() + i, mPossibleSuspects.begin() + i + 1);
		break;
	case WEAPON:
		for (size_t i = 0; i < mPossibleWeapons.size(); i++)
			if (mPossibleWeapons[i] == card)
				mPossibleWeapons.erase(mPossibleWeapons.begin() + i, mPossibleWeapons.begin() + i + 1);
		break;
	case ROOM:
		for (size_t i = 0; i < mPossibleRooms.size(); i++)
			if (mPossibleRooms[i] == card)
				mPossibleRooms.erase(mPossibleRooms.begin() + i, mPossibleRooms.begin() + i + 1);
		break;
	}
}

void ComputerPlayer::learnHasCard(cardName player, bool hasCard, cardName card)
{
	size_t pIndex = lookUpPlayerOrder(player);
	cardType cType = lookUpCardType(card);
	int cardOffset;
	std::vector<std::vector<hasCardToggleOption>>* notecard;
	switch (cType)
	{
	case SUSPECT:
		notecard = &mSuspects;
		cardOffset = 0;
		break;
	case WEAPON:
		notecard = &mWeapons;
		cardOffset = 6;
		break;
	case ROOM:
		notecard = &mRooms;
		cardOffset = 12;
		break;
	}

	if (!hasCard)
		(*notecard)[pIndex][card-cardOffset] = NO;
	else {
		for (size_t i = 0; i < mPlayers.size(); i++) {
			(*notecard)[i][card - cardOffset] = NO;
			if (i == pIndex) {
				(*notecard)[i][card - cardOffset] = YES;
			}
		}
		if (player != getName())
			removeFromPossibleCards(card);
	}
}

bool ComputerPlayer::ownCard(cardName card)
{
	for (size_t i = 0; i < mCards.size(); i++)
		if (mCards[i].getName() == card)
			return true;
	return false;
}

