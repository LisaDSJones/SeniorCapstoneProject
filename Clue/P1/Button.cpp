#include "Button.h"
#include "glut.h"
#include "graphics.h"

// Constructors
Button::Button(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id)
 : mText(text), mLeft(left), mTop(top), mWidth(width), mHeight(height), mR(r), mG(g), mB(b), mPushed(false), mVisible(true), mDisabled(false), mID(id) { }

Button::~Button()
{
}

// Fancy Methods 
bool Button::Clicked(double mouseX, double mouseY)
{
	if (!mVisible ||!mEditable || mDisabled)
		return false;
	if (mouseX >= mLeft && mouseX <= mLeft+mWidth && mouseY <=mTop && mouseY >=mTop-mHeight)
		return true;
	return false;
}

void Button::Draw()
{
	if (!mVisible)
		return;
	// change bk ground color if it's been clicked or disabled
	if(mPushed || mDisabled)
		glColor3d(mR/2, mG/2, mB/2);
	else
		glColor3d(mR, mG, mB);
	DrawRectangle(mLeft, mTop - mHeight, mLeft + mWidth, mTop);
	// change text color according to darkness of bk ground color
	if ((mR + mG + mB)<1)
		glColor3d(1, 1, 1);
	else 
		glColor3d(0, 0, 0);
	DrawTextCenteredStr(mLeft+mWidth/2, mTop-mHeight/2-.1, mText);
}

// virtual 
void Button::OnClick() 
{	mPushed = true;}

// Getters
std::string Button::getText()
{ return mText; }
double Button::getLeft()
{ return mLeft; }
double Button::getTop()
{ return mTop; }
double Button::getWidth()
{ return mWidth; }
double Button::getHeight()
{ return mHeight; }
double Button::getR()
{ return mR; }
double Button::getG()
{ return mG; }
double Button::getB()
{ return mB; }
bool Button::isPushed()
{ return mPushed; }
bool Button::isVisible()
{ return mPushed; }
std::string Button::getID()
{ return mID; }
bool Button::isEditable()
{ return mEditable; }
void Button::setEditable(bool editable)
{ mEditable = editable; }



// Setters
void Button::setText(std::string text)
{ mText = text; }
void Button::setLeft(double left)
{ mLeft = left; }
void Button::setTop(double top) 
{ mTop = top; }
void Button::setWidth(double width) 
{ mWidth = width;}
void Button::setHeight(double height)
{ mHeight = height ; }
void Button::setR(double r)
{ mR = r; }
void Button::setG(double g)
{ mG = g; }
void Button::setB(double b)
{ mB = b; }
void Button::setPushed(bool pushed)
{ mPushed = pushed; }
void Button::setVisible(bool visible)
{mVisible = visible; }
void Button::setDisabled(bool disable)
{ mDisabled = disable; }
