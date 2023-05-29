#pragma once
#include "Classes.h"
#include "Button.h"

////////////////////////////////////////////////////////////
class PlayerTypeOptionButton : public Button {
public:
	PlayerTypeOptionButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id);
	void OnClick();
	playerType getOptionSelected();
	void setOptionSelected(playerType option);
private:
	playerType mOptionSelected;
};


////////////////////////////////////////////////////////////
class DifficultyOptionButton : public Button {
public:
	DifficultyOptionButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id);
	void OnClick();
	difficultyType getOptionSelected();
	void setOptionSelected(difficultyType difficulty);
private:
	difficultyType mDifficulty;
};

////////////////////////////////////////////////////////////

class NavigationButton : public Button {
public:
	NavigationButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id, place whereToGo);
	place getOptionSelected();
private:
	place mGoToLocation;
};

////////////////////////////////////////////////////////////
class HasCardToggleButton : public Button {
public:
	HasCardToggleButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id);
	void OnClick();
	hasCardToggleOption getOptionSelected();
	void setOptionSelected(hasCardToggleOption toggleOption);
private:
	hasCardToggleOption mToggledOption;
	std::string mOptions[5];
};

////////////////////////////////////////////////////////////
class CharacterNoteToggleButton : public Button {
public:
	CharacterNoteToggleButton(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id, cardName toggledOption);
	void OnClick();
	cardName getOptionSelected();
	void setOptionSelected(cardName toggleOption);
private:
	cardName mToggledOption;
	std::string mOptions[7];
	double mColor[6][3];
};

////////////////////////////////////////////////////////////
//////////////Doesn't actually inherit from Button
////////////////////////////////////////////////////////////

class RadioButtons {
public:
	RadioButtons(std::vector<Button> buttons, std::vector<cardName> options, double defaultColorR, double defaultColorG, double defaultColorB, double selectedColorR, double selectedColorG, double selectedColorB, int defaultIndex = 0);
	RadioButtons();
	bool checkClick(double mouseX, double mouseY);
	void unClick();
	void Draw();
	cardName getOptionSelected();
	void disable(int index);
	void disableAll();
	void enable(int index);
	void enableAll();
	
private:
	void deselect();
	std::vector<Button> mButtons;
	std::vector<cardName> mOptions;
	double mDefaultR, mDefaultG, mDefaultB, mSelectedR, mSelectedG, mSelectedB;
	int mSelected;
};

