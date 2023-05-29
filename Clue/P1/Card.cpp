#include "Card.h"



Card::Card(cardName name, cardType type, std::string text)
	: mName(name), mType(type), mText(text)
{ }

Card::~Card()
{
}

cardName Card::getName()
{ return mName; }

cardType Card::getType()
{ return mType; }

std::string Card::getText()
{ return mText; }

