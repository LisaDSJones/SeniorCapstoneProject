#include "ButtonTypes.h"

//////////////////////////
// Player Type Buttons
//////////////////////////
PlayerTypeOptionButton::PlayerTypeOptionButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id)
	:Button(text, left, top, width, height, r, g, b, id), mOptionSelected(HUMAN) {
}

void PlayerTypeOptionButton::OnClick()
{
	this->setPushed(true);
	switch (mOptionSelected)	{
	case HUMAN:
		mOptionSelected = COMPUTER;
		this->setText("Computer");
		break;
	case COMPUTER:
		mOptionSelected = NONE;
		this->setText("None");
		break;
	case NONE:
		mOptionSelected = HUMAN;
		this->setText("Human");
		break;
	default:
		break;
	}
}

playerType PlayerTypeOptionButton::getOptionSelected()
{ 	return mOptionSelected; }

void PlayerTypeOptionButton::setOptionSelected(playerType option)
{ mOptionSelected = option; }

//////////////////////////
// Difficulty Buttons
//////////////////////////
DifficultyOptionButton::DifficultyOptionButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id)
	:Button(text, left, top, width, height, r, g, b, id), mDifficulty(EASY) {
}

void DifficultyOptionButton::OnClick()
{
	this->setPushed(true);
	switch (mDifficulty)
	{
	case EASY:
		mDifficulty = HARD;
		this->setText("Hard");
		break;
	case HARD:
		mDifficulty = EASY;
		this->setText("Easy");
		break;
	default:
		break;
	}
}

difficultyType DifficultyOptionButton::getOptionSelected()
{ return mDifficulty; }

void DifficultyOptionButton::setOptionSelected(difficultyType difficulty)
{ mDifficulty = difficulty; }

//////////////////////////
// Navigation Buttons
//////////////////////////
NavigationButton::NavigationButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id, place whereToGo)
	:Button(text, left, top, width, height, r, g, b, id), mGoToLocation(whereToGo) {
}

place NavigationButton::getOptionSelected()
{ return mGoToLocation;}

//////////////////////////
// Has Card Toggle
//////////////////////////

HasCardToggleButton::HasCardToggleButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id)
	: Button(text, left, top, width, height, r, g, b, id) {
	mToggledOption = UNKNOWN;
	mOptions[4] = "";
	mOptions[0] = "-";
	mOptions[1] = "X";
	mOptions[2] = "?";
	mOptions[3] = "!";
}

void HasCardToggleButton::OnClick()
{
	this->setPushed(true);
	mToggledOption = hasCardToggleOption((int(mToggledOption) + 1) % 5);
	this->setText(mOptions[int(mToggledOption)]);
}

hasCardToggleOption HasCardToggleButton::getOptionSelected()
{ return mToggledOption; }

void HasCardToggleButton::setOptionSelected(hasCardToggleOption toggleOption)
{ mToggledOption = hasCardToggleOption( toggleOption-1);
 HasCardToggleButton::OnClick();
}


//////////////////////////
// Character Note Toggle
//////////////////////////


CharacterNoteToggleButton::CharacterNoteToggleButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id, cardName toggledOption)
	: Button(text, left, top, width, height, r, g, b, id) {
	mToggledOption = toggledOption; // secretly this is just the last option and has nothing to do with rope
	mOptions[0] = "S";
	mOptions[1] = "Pe";
	mOptions[2] = "W";
	mOptions[3] = "Pl";
	mOptions[4] = "M";
	mOptions[5] = "G";
	mOptions[6] = "";
	setText(mOptions[mToggledOption]);

}

void CharacterNoteToggleButton::OnClick()
{
	this->setPushed(true);
	mToggledOption = cardName((int(mToggledOption) + 1) % 7);
	this->setText(mOptions[int(mToggledOption)]);
	if (mToggledOption == ROPE) {
		this->setR(.95);
		this->setG(.95);
		this->setB(.95);
		return;
	}
	this->setR(COLORS[(int)mToggledOption][0]);
	this->setG(COLORS[(int)mToggledOption][1]);
	this->setB(COLORS[(int)mToggledOption][2]);
}

cardName CharacterNoteToggleButton::getOptionSelected()
{ return mToggledOption; }

void CharacterNoteToggleButton::setOptionSelected(cardName toggleOption)
{ mToggledOption = toggleOption; }


//////////////////////////
// Radio Buttons
//////////////////////////

RadioButtons::RadioButtons(std::vector<Button> buttons, std::vector<cardName> options, double defaultColorR, double defaultColorG, double defaultColorB, double selectedColorR, double selectedColorG, double selectedColorB, int defaultIndex)
	: mButtons(buttons), mOptions(options), mDefaultR(defaultColorR), mDefaultG(defaultColorG), mDefaultB(defaultColorB), mSelectedR(selectedColorR), mSelectedG(selectedColorG), mSelectedB(selectedColorB), mSelected(defaultIndex)
{
}

RadioButtons::RadioButtons()
	: mButtons(), mOptions(), mDefaultR(0), mDefaultG(0), mDefaultB(0), mSelectedR(1), mSelectedG(1), mSelectedB(1), mSelected(0)
{
}

bool RadioButtons::checkClick(double mouseX, double mouseY)
{
	for (size_t i = 0; i < mButtons.size(); i++) {
		if (mButtons[i].Clicked(mouseX, mouseY)) {
			deselect();

			mButtons[i].setR(mSelectedR);
			mButtons[i].setG(mSelectedG);
			mButtons[i].setB(mSelectedB);
			mButtons[i].setText("-->");
			mSelected = i;
			return true;
		}
	}
	return false;
}

void RadioButtons::unClick()
{
	for (size_t i = 0; i < mButtons.size(); i++)
		mButtons[i].setPushed(false);
}

void RadioButtons::Draw()
{
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		mButtons[i].Draw();
	}
}

cardName RadioButtons::getOptionSelected()
{
	if(mSelected >=0)
		return mOptions[mSelected];
	return NOCARD;
}

void RadioButtons::disable(int index)
{ 
	deselect();
	if (index < mButtons.size());
		mButtons[index].setDisabled(true);
}

void RadioButtons::disableAll()
{
	deselect();
	for (size_t i = 0; i < mButtons.size(); i++)
		mButtons[i].setDisabled(true);
}

void RadioButtons::enable(int index)
{ mButtons[index].setDisabled(false); }

void RadioButtons::enableAll()
{
	for (size_t i = 0; i < mButtons.size(); i++)
		mButtons[i].setDisabled(false);
}

void RadioButtons::deselect()
{
	if (mSelected >= 0) {
		mButtons[mSelected].setR(mDefaultR);
		mButtons[mSelected].setG(mDefaultG);
		mButtons[mSelected].setB(mDefaultB);
		mButtons[mSelected].setText("");
	}
	mSelected = -1;
}
