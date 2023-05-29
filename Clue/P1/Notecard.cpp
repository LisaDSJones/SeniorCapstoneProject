#include "Classes.h"
#include "Notecard.h"
#include "ButtonTypes.h"
#include "glut.h"
#include "graphics.h"

Notecard::Notecard(cardName character, std::vector<cardName> oppenents)
{
	int numOpponents = oppenents.size()-1;
	mLeft = W-numOpponents*2-1;

	// assume noone is playing
	for (int j = 0; j < 6; j++)
		mOpponentsPlaying[j] = false;

	// get opponent list (in order) and binary
	for (int j = 0; j < numOpponents+1; j++) 
		mOpponentsPlaying[int(oppenents[j])] = true;	
	mOpponentsPlaying[character] = false;

	// create actual notecard
	for (size_t j = mLeft; j < mLeft + numOpponents+1; j++) {
	
		// Caracter Headings
		if (j == mLeft) {
			// you
			mHeader.push_back(CharacterNoteToggleButton("Me", j + 0.025, H - 1.0125, .95, .975, COLORS[character][0], COLORS[character][1], COLORS[character][2], "CharacterToggleButtonHeader", character));
			// opponents
			for (int i = 1; i < 7; i++)
			{
				cardName nextCharacter = cardName((character + i) % 6);
				if (mOpponentsPlaying[nextCharacter]) {
					mHeader.push_back(CharacterNoteToggleButton("", j + 0.025+ mHeader.size(), H - 1.0125, .95, .975, COLORS[nextCharacter][0], COLORS[nextCharacter][1], COLORS[nextCharacter][2], "CharacterToggleButtonHeader", nextCharacter));
				}
			}
		}
		if(j <mHeader.size())
			mHeader.at(j).setEditable(false);

		// Note cells
		for (double i = 1; i < 22; i +=1 ) {
			if (i == 7) i += .15;
			if (i == 13.15) i += .15;
			mNotes.push_back(new HasCardToggleButton("", j + .03, H - i - 1.02, .94, .96, OFFWHITE, "HasCardToggleButton"));
			if (j == mLeft)
				(*mNotes[mNotes.size() - 1]).setEditable(false);
		}
	}
	// Extra Note Cells
	for (int j = mLeft + numOpponents+1; j < W; j++) {
		for (double i = 1; i < 23; i+= 1) {
			if (i == 7) i += .15;
			if (i == 13.15) i += .15;
			mNotes.push_back(new CharacterNoteToggleButton("", j + .03, H - i - 1.02, .94, .96, .95,.95,.95, "CharacterToggleButton", ROPE));
		}
	}

	// Column Titles
	mColumnTitles.push_back(Button("Miss Scarlet", mLeft - 2.2, H - 2, 2, .96, BLACK, "MissScarletColumnTitle"));
	mColumnTitles.push_back(Button("Mrs. Peacock", mLeft - 2.2, H - 3, 2, .96, BLACK, "MrsPeacockColumnTitle"));
	mColumnTitles.push_back(Button("Mrs. White", mLeft - 2.2, H - 4, 2, .96, BLACK, "MrsWhiteColumnTitle"));
	mColumnTitles.push_back(Button("Prof. Plum", mLeft - 2.2, H - 5, 2, .96, BLACK, "ProfPlumColumnTitle"));
	mColumnTitles.push_back(Button("Col. Mustard", mLeft - 2.2, H - 6, 2, .96, BLACK, "ColMustardColumnTitle"));
	mColumnTitles.push_back(Button("Mr. Green", mLeft - 2.2, H - 7, 2, .96, BLACK, "MrGreenColumnTitle"));

	mColumnTitles.push_back(Button("Rope", mLeft - 2.2, H - 8.1, 2, .96, BLACK, "RopeColumnTitle"));
	mColumnTitles.push_back(Button("Lead Pipe", mLeft - 2.2, H - 9.1, 2, .96, BLACK, "LeadPipeColumnTitle"));
	mColumnTitles.push_back(Button("Knife", mLeft - 2.2, H - 10.1, 2, .96, BLACK, "KnifeColumnTitle"));
	mColumnTitles.push_back(Button("Wrench", mLeft - 2.2, H - 11.1, 2, .96, BLACK, "WrenchColumnTitle"));
	mColumnTitles.push_back(Button("Candlestick", mLeft - 2.2, H - 12.1, 2, .96, BLACK, "CandlestickColumnTitle"));
	mColumnTitles.push_back(Button("Revolver", mLeft - 2.2, H - 13.1, 2, .96, BLACK, "RevolverColumnTitle"));

	mColumnTitles.push_back(Button("Kitchen", mLeft - 2.2, H - 14.2, 2, .96, BLACK, "KitchenColumnTitle"));
	mColumnTitles.push_back(Button("Dining Room", mLeft - 2.2, H - 15.2, 2, .96, BLACK, "DiningRoomColumnTitle"));
	mColumnTitles.push_back(Button("Lounge", mLeft - 2.2, H - 16.2, 2, .96, BLACK, "LoungeColumnTitle"));
	mColumnTitles.push_back(Button("Ballroom", mLeft - 2.2, H - 17.2, 2, .96, BLACK, "BallroomColumnTitle"));
	mColumnTitles.push_back(Button("Hall", mLeft - 2.2, H - 18.2, 2, .96, BLACK, "HallColumnTitle"));
	mColumnTitles.push_back(Button("Conservatory", mLeft - 2.2, H - 19.2, 2, .96, BLACK, "ConservatoryColumnTitle"));
	mColumnTitles.push_back(Button("Billiard Room", mLeft - 2.2, H - 20.2, 2, .96, BLACK, "BilliardRoomColumnTitle"));
	mColumnTitles.push_back(Button("Library", mLeft - 2.2, H - 21.2, 2, .96, BLACK, "LibraryColumnTitle"));
	mColumnTitles.push_back(Button("Study", mLeft - 2.2, H - 22.2, 2, .96, BLACK, "StudyColumnTitle"));

	// Radio Buttons
	std::vector<Button> suspects;
	std::vector<Button> weapons;
	std::vector<Button> rooms;

	suspects.push_back(Button("", mLeft - 3.2, H - 2, 1, .96, OFFWHITE, "MissScarletRadioButton"));
	suspects.push_back(Button("", mLeft - 3.2, H - 3, 1, .96, OFFWHITE, "MrsPeacockRadioButton"));
	suspects.push_back(Button("", mLeft - 3.2, H - 4, 1, .96, OFFWHITE, "MrsWhiteRadioButton"));
	suspects.push_back(Button("", mLeft - 3.2, H - 5, 1, .96, OFFWHITE, "ProfPlumRadioButton"));
	suspects.push_back(Button("", mLeft - 3.2, H - 6, 1, .96, OFFWHITE, "ColMustardRadioButton"));
	suspects.push_back(Button("", mLeft - 3.2, H - 7, 1, .96, OFFWHITE, "MrGreenRadioButton"));		

	weapons.push_back(Button("", mLeft - 3.2, H - 8.1, 1, .96, OFFWHITE, "RopeRadioButton"));
	weapons.push_back(Button("", mLeft - 3.2, H - 9.1, 1, .96, OFFWHITE, "LeadPipeRadioButton"));
	weapons.push_back(Button("", mLeft - 3.2, H - 10.1, 1, .96, OFFWHITE, "KnifeRadioButton"));
	weapons.push_back(Button("", mLeft - 3.2, H - 11.1, 1, .96, OFFWHITE, "WrenchRadioButton"));
	weapons.push_back(Button("", mLeft - 3.2, H - 12.1, 1, .96, OFFWHITE, "CandlestickRadioButton"));
	weapons.push_back(Button("", mLeft - 3.2, H - 13.1, 1, .96, OFFWHITE, "RevolverRadioButton"));

	rooms.push_back(Button("", mLeft - 3.2, H - 14.2, 1, .96, OFFWHITE, "KitchenRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 15.2, 1, .96, OFFWHITE, "DiningRoomRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 16.2, 1, .96, OFFWHITE, "LoungeRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 17.2, 1, .96, OFFWHITE, "BallroomRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 18.2, 1, .96, OFFWHITE, "HallRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 19.2, 1, .96, OFFWHITE, "ConservatoryRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 20.2, 1, .96, OFFWHITE, "BilliardRoomRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 21.2, 1, .96, OFFWHITE, "LibraryRadioButton"));
	rooms.push_back(Button("", mLeft - 3.2, H - 22.2, 1, .96, OFFWHITE, "StudyRadioButton"));

	std::vector<cardName> options = { MISSSCARLET, MRSPEACOCK, MRSWHITE, PROFPLUM, COLONELMUSTARD, MRGREEN };
	mSuspects = RadioButtons(suspects, options, OFFWHITE, mHeader[0].getR(), mHeader[0].getG(), mHeader[0].getB());

	options = { ROPE, LEADPIPE, KNIFE, WRENCH, CANDLESTICK, REVOLVER };
	mWeapons = RadioButtons(weapons, options, OFFWHITE, mHeader[0].getR(), mHeader[0].getG(), mHeader[0].getB());

	options = { KITCHEN, DININGROOM, LOUNGE, BALLROOM, HALL, CONSERVATORY, BILLIARDROOM, LIBRARY, STUDY };
	mRooms = RadioButtons(rooms, options, OFFWHITE, mHeader[0].getR(), mHeader[0].getG(), mHeader[0].getB());

}

Notecard::~Notecard()
{
//	for (std::vector< Button* >::iterator it = mNotes.begin(); it != mNotes.end(); ++it)
//		delete (*it); 
}

void Notecard::Draw()
{
	// Notepad Background
	glColor3d(BLACK);
	DrawRectangle(mLeft-2.2, H, W, 0);

	// title
	glColor3d(WHITE);
	DrawTextCentered((mLeft+W)/2, H-.5, "Notecard");

	// Header
	for (size_t i = 0; i < mHeader.size(); i++)
		mHeader[i].Draw();

	for (size_t i = 0; i < mNotes.size(); i++)
		mNotes[i]->Draw();

	// Row Titles
	for (size_t i = 0; i < mColumnTitles.size(); i++)
		mColumnTitles[i].Draw();
	mSuspects.Draw();
	mWeapons.Draw();
	mRooms.Draw();
}

bool Notecard::checkClicked(double mouseX, double mouseY)
{
	if (mLeft < mouseX && mouseX < W) 
		for (size_t i = 0; i < mNotes.size(); i++)
			if (mNotes[i]->Clicked(mouseX, mouseY)) {
				mClickedButtonIndex = i;
				if (mNotes[i]->getID() == "CharacterToggleButton") {
					CharacterNoteToggleButton *child;
					do {
						mNotes[i]->OnClick();
						child = dynamic_cast<CharacterNoteToggleButton*>(mNotes.at(i));
						if (child->getOptionSelected() == ROPE)
							break;
					} while (!mOpponentsPlaying[child->getOptionSelected()]);
				}
				else
					mNotes[i]->OnClick();
				return true;
			}
	if (mSuspects.checkClick(mouseX, mouseY) || mWeapons.checkClick(mouseX, mouseY) || mRooms.checkClick(mouseX, mouseY))
		return true;
	return false;
}

std::vector<Card> Notecard::checkHand(Accusation accusation)
{
	disableAccusation();
	enableAccusation();
	std::vector<Card> evedence;
	for (size_t i = 0; i < mHand.size(); i++)
	{
		if (accusation == mHand[i].getName())
			evedence.push_back(mHand[i]);
	}
	return evedence;
}

void Notecard::unClick()
{
	if(mClickedButtonIndex >= 0 && mClickedButtonIndex < mNotes.size())
		mNotes[mClickedButtonIndex]->setPushed(false);
	mClickedButtonIndex = -1;
	mSuspects.unClick();
	mRooms.unClick();
	mWeapons.unClick();
}

void Notecard::giveCard(Card card)
{
	for (size_t i = 0; i < mColumnTitles.size(); i++)
	{
		if (mColumnTitles[i].getText() == card.getText()) {
			dynamic_cast<HasCardToggleButton*>(mNotes.at(i))->setOptionSelected(YES);
			break;
		}
	}
	mHand.push_back(card);
}

void Notecard::disableAccusation()
{
	mSuspects.disableAll();
	mRooms.disableAll();
	mWeapons.disableAll();
}

void Notecard::enableAccusation()
{
	mSuspects.enableAll();
	mWeapons.enableAll();
	mRooms.enableAll();
	if(mPrevousAccusation.getRoom() != NOCARD)
		mRooms.disable(mPrevousAccusation.getRoom() - 12);
}

Accusation Notecard::getAccusation()
{
	mPrevousAccusation.setRoom(mRooms.getOptionSelected());
	mPrevousAccusation.setSuspect(mSuspects.getOptionSelected());
	mPrevousAccusation.setWeapon(mWeapons.getOptionSelected());
	return mPrevousAccusation;
}

