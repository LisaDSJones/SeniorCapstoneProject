#pragma once
#include "Classes.h"

class Button
{
public:
	Button(std::string text, double left, double top, double width, double height, double r, double g, double b, std::string id );
	~Button();
	bool Clicked(double mouseX, double mouseY);
	void Draw();
	virtual void OnClick();


	// Getters and Setters
	std::string getText();
	void setText(std::string text);
	double getLeft();
	void setLeft(double left);
	double getTop();
	void setTop(double top);
	double getWidth();
	void setWidth(double width);
	double getHeight();
	void setHeight(double height);
	double getR();
	void setR(double r);
	double getG();
	void setG(double g);
	double getB();
	void setB(double b);
	bool isPushed();
	void setPushed(bool pushed);
	bool isVisible();
	void setVisible(bool visible);
	std::string getID();
	bool isEditable();
	void setEditable(bool editable);
	void setDisabled(bool disable);
private:
	std::string mText;
	double mLeft, mTop, mWidth, mHeight;
	double mR, mG, mB;
	bool mPushed, mVisible;
	std::string mID;
	bool mEditable, mDisabled;
};

