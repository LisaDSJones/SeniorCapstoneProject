#pragma once
#include "Classes.h"

void DrawCircle(double x1, double y1, double radius);
void DrawRectangle(double x1, double y1, double x2, double y2);
void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
void DrawText(double x, double y, char *string);
void DrawTextCentered(double x, double y, char *string, double r = 2, double g = 2, double b = 2 );
void DrawTextStr(double x, double y, std::string string);
void DrawTextCenteredStr(double x, double y, std::string string);

void Interpolate(double t, double t1, double t2, double &v, double v1, double v2);



