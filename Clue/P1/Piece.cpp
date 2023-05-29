#include "Classes.h"
#include "piece.h"
#include "graphics.h"
#include "glut.h"

//using namespace std;
void FindTriangleNormal(double x[], double y[], double z[], double n[])
{
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = x[1] - x[0];
	v1[1] = y[1] - y[0];
	v1[2] = z[1] - z[0];
	v2[0] = x[2] - x[0];
	v2[1] = y[2] - y[0];
	v2[2] = z[2] - z[0];

	// Take the cross product of v1 and v2, to find the vector perpendicular to both.
	n[0] = v1[1] * v2[2] - v1[2] * v2[1];
	n[1] = -(v1[0] * v2[2] - v1[2] * v2[0]);
	n[2] = v1[0] * v2[1] - v1[1] * v2[0];

	double size = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	n[0] /= -size;
	n[1] /= -size;
	n[2] /= -size;
}

void Draw2DPawn(double x, double y, double size = 1 ) {
	y += size;
	// Base
	glBegin(GL_POLYGON);
	glVertex2d(x + 0.29 * size, y - 1.00 * size);
	glVertex2d(x + 0.39 * size, y - 0.85 * size);
	glVertex2d(x + 0.61 * size, y - 0.85 * size);
	glVertex2d(x + 0.71 * size, y - 1.00 * size);
	glEnd();

	// Body
	glBegin(GL_POLYGON);
	glVertex2d(x + 0.39 * size, y - 0.85 * size);
	glVertex2d(x + 0.47 * size, y - 0.23 * size);
	glVertex2d(x + 0.53 * size, y - 0.23 * size);
	glVertex2d(x + 0.61 * size, y - 0.85 * size);
	glEnd();

	// Head
	glBegin(GL_POLYGON);
	glVertex2d(x + 0.45 * size, y - 0.23 * size);
	glVertex2d(x + 0.42 * size, y - 0.20 * size);
	glVertex2d(x + 0.40 * size, y - 0.18 * size);
	glVertex2d(x + 0.40 * size, y - 0.11 * size);
	glVertex2d(x + 0.42 * size, y - 0.065 * size);
	glVertex2d(x + 0.47 * size, y - 0.017 * size);
	glVertex2d(x + 0.50 * size, y - 0.01 * size);
	glVertex2d(x + 0.53 * size, y - 0.017 * size);
	glVertex2d(x + 0.58 * size, y - 0.065 * size);
	glVertex2d(x + 0.60 * size, y - 0.11 * size);
	glVertex2d(x + 0.60 * size, y - 0.18 * size);
	glVertex2d(x + 0.58 * size, y - 0.20 * size);
	glVertex2d(x + 0.55 * size, y - 0.23 * size);
	glEnd();
}

void Draw3DPawn(double size = 1) {
	char buffer[200];
	std::ifstream in("PAWN.POL");
	double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices.
	int done = false;
	int verts = 0; // vertices in the current polygon
	int polygons = 0; // total polygons in this file.
	int LargestNumber = 1045;
	int count;
	do
	{
		in.getline(buffer, 200); // get one line (point) from the file.
		count = sscanf_s(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
		z[verts] = (z[verts] / LargestNumber)*size;
		x[verts] = (x[verts] / LargestNumber)*size;
		y[verts] = (y[verts] / LargestNumber)*size;
		done = in.eof();
		if (!done)
		{
			if (count == 3) // if this line had an x,y,z point.
				verts++;
			else // the line was empty. Finish current polygon and start a new one.
				if (verts >= 3)
				{
					glBegin(GL_POLYGON);
					double n[3];
					FindTriangleNormal(x, y, z, n);
					glNormal3dv(n);
					for (int i = 0; i < verts; i++)
					{
						glVertex3d(x[i], y[i], z[i]);
					}
					glEnd(); // end previous polygon
					polygons++;
					verts = 0;
				}
		}
	} while (!done);
}

Piece::Piece(double x, double z, double r, double g, double b, bool is3D)
	:mX(x), mY(0), mZ(z), mR(r), mG(g), mB(b), mName("PAWN.POL"), mIs3D(false)
{

}

void stringcpy(char * dest, const char *source)
{
	unsigned i;
	for (i = 0; source[i] != NULL; i++)
	{
		dest[i] = source[i];
	}
	dest[i] = 0;
}

void Piece::Draw(double size)
{
	char *cstr = new char[mName.length() + 1];
	stringcpy(cstr, mName.c_str());

	if (mIs3D) {
		GLfloat mat_amb_diff1[] = { mR, mG, mB, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
		glPushMatrix();
		glTranslatef(mX, mY, mZ);
		Draw3DPawn();
		glPopMatrix();
	}
	else {
		glColor3d(mR, mG, mB);
		Draw2DPawn(mX, mZ, size);
	}
}


void Piece::setX(double x) { mX = x; }

void Piece::setZ(double z) { mZ = z; }


double Piece::getX() { return mX; }
double Piece::getZ() { return mZ; }


