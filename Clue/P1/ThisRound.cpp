#include "ThisRound.h"
#include "Accusation.h"
#include "graphics.h"
#include "glut.h"

ThisRound::ThisRound()
{
}


ThisRound::~ThisRound()
{
}

void ThisRound::addAccusation(cardName accuser, Accusation accusation, cardName disprover)
{
	if (mAccusations.size() > 5) {
		mAccusations.erase(mAccusations.begin());
		mAccusers.erase(mAccusers.begin());
		mDisprovers.erase(mDisprovers.begin());
	}
	mAccusers.push_back(accuser);
	mAccusations.push_back(accusation);
	mDisprovers.push_back(disprover);
}

void ThisRound::Draw()
{
	// Draw Background
	glColor3d(BLACK);
	DrawRectangle(0, H, 8.5, H-mAccusations.size()*3 -.5 );

	// Draw Accusers
	for (size_t i = 0; i < mAccusers.size(); i ++)
	{
		glColor3d(COLORS[mAccusers[i]][0], COLORS[mAccusers[i]][1], COLORS[mAccusers[i]][2]);
		DrawRectangle(0.2,H- i*3 - .2, 1.2, H - i*3 - 1.2);
		glColor3d(WHITE);
		DrawTextStr(1.3, H - i * 3 - .85, "Accused");
		glColor3d(BLACK);
		DrawTextCenteredStr((.2 + 1.2) / 2, H - i * 3 - .85, nameAbbreviations[mAccusers[i]]);
	}
	for (size_t i = 0; i < mAccusations.size(); i ++)
	{
		glColor3d(WHITE);
		DrawTextCenteredStr((.5+ 8.5) / 2, H-i*3 - 1.85, mAccusations[i].toString());
	}
	for (size_t i = 0; i < mDisprovers.size(); i ++)
	{
		if (mDisprovers[i] == NOCARD) {
			glColor3d(WHITE);
			DrawTextStr(2.8, H - i * 3 - 2.85, "Not Disproved!!");
		}
		else {
			glColor3d(COLORS[mDisprovers[i]][0], COLORS[mDisprovers[i]][1], COLORS[mDisprovers[i]][2]);
			DrawRectangle(1.7, H - i * 3 - 2.2, 2.7, H - i * 3 - 3.2);
			glColor3d(WHITE);
			DrawTextStr(2.8, H - i * 3 - 2.85, "Disproved");
			glColor3d(BLACK);
			DrawTextCenteredStr((1.7 + 2.7) / 2, H - i * 3 - 2.85, nameAbbreviations[mDisprovers[i]]);
		}
	}
}

cardName ThisRound::getLastAccuser()
{
	return mAccusers[mAccusers.size() - 1];
}

cardName ThisRound::getLastDisprover()
{
	return mDisprovers[mDisprovers.size() - 1];
}
