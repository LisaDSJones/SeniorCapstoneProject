#pragma once
#include "Classes.h"



class Piece
{
public:
	Piece(double x, double z, double r, double g, double b, bool is3d);
	void Draw(double size = 1);
	void setX(double x);
	void setZ(double y);

	double getX();
	double getZ();

private:

	double mX, mY, mZ;
	double mR, mG, mB;
	std::string mName;
	bool mIs3D;
};

