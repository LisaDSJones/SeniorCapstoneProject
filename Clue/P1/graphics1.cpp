// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include "Classes.h"
#include "glut.h"
#include "Button.h"
#include "Game.h"
#include "graphics.h"

// Global Variables (Only what you need!)
double screen_x = 1300;
double screen_y = 700;
double world_margin_x = 0.5;
double world_margin_y = 0.5;
Game gClue;

/////////////////////////////////////////////////////////////
/////////////////////////////////// Primitive Draw Functions
/////////////////////////////////////////////////////////////

void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawTextCentered(double x, double y, char *string, double r, double g, double b)
{
	if(r < 2 && g <2 && b <2)
		glColor3d(r, g ,b);
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	len = (int)strlen(string);
	glRasterPos2d(x-len*.08, y);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}
void DrawText(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}
void DrawTextStr(double x, double y, std::string string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = string.size();
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}
void DrawTextCenteredStr(double x, double y, std::string string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	len = string.size();
	glRasterPos2d(x - len*.08, y);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}


void Interpolate(double t, double t1, double t2, double &v, double v1, double v2)
{
	if (t<t1)
		t = t1;
	if (t>t2)
		t = t2;
	double ratio = (t - t1) / (t2 - t1);
	v = v1 + ratio*(v2 - v1);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////// GLUT callback Functions
/////////////////////////////////////////////////////////////

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.

void SetTopView()
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-world_margin_x, W + world_margin_x,
		-world_margin_y, H + world_margin_y);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (false)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		gluLookAt(W / 2 + .2, -H / 4, (W + H / 2),  // eye
			W / 2, H / 2, 0,  // at
			0, 0, 1); // up
	}
	else if (true) // Top View
	{ SetTopView();	}


	// Test lines that draw all three shapes and some text.
	// Delete these when you get your code working.
	gClue.draw();
	glutSwapBuffers();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program

			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int mouse_x, int mouse_y)
{
	double x = ((W+world_margin_x*2)*mouse_x / (screen_x))-world_margin_x;
	double y = ((H+world_margin_y*2)*mouse_y / (screen_y))-world_margin_y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gClue.checkClick(x, H - y);
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gClue.unClick();
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Clue");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();
	// initialize Game class

	// while !mGameStarted
		// opening window initialization(Game)
			// Settings per character
				// bool Playing: if true, Human and EasyAi are revealed or un-shaded
				// bool Human: if true, EasyAi is shaded or hidden
				// bool EasyAi
			// start Button
				// Erases Option Screen, sets mGameStarted to true, and returns true			

	// while game hasn't ended
		// Game.getNextPlayer.StartTurnScreen() // begins with a screen encompassing rect with a button in the middle. 
		// while !Player.isMyTurn()
			// button press -> Player.mMyTurn = true
		// TakeTurnScreen()
			// 
		// player Must Roll or use secret passage 
			

	return 0;
}
