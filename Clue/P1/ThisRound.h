#pragma once
#include "Classes.h"


class ThisRound
{
public:
	ThisRound();
	~ThisRound();

	void addAccusation(cardName accuser, Accusation accusation, cardName disprover);
	void Draw();
	cardName getLastAccuser();
	cardName getLastDisprover();
private:
	std::vector<cardName> mAccusers;	
	std::vector<Accusation> mAccusations;
	std::vector<cardName> mDisprovers;
};

