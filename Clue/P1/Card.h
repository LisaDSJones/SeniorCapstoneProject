#pragma once
#include "Classes.h"

class Card
{
public:
	Card(cardName name, cardType type, std::string text);
	~Card();
	cardName getName();
	cardType getType();
	std::string getText();

private:
	cardName mName;
	cardType mType;
	std::string mText;
};

