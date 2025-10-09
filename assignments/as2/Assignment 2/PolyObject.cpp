#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include "PolyObject.h"

PolyObject::PolyObject()
{
	this->vertices = vector<float>();
	this->color = new float[3];
}

PolyObject::~PolyObject()
{
	delete[] this->color;
}

void PolyObject::addVertex(float x, float y)
{
	this->vertices.push_back(x);
	this->vertices.push_back(y);
}

void PolyObject::setColor(float cr, float cg, float cb)
{
	this->color[0] = cr;
	this->color[1] = cg;
	this->color[2] = cb;
}

unsigned int PolyObject::getVertNum()
{
	return this->vertices.size() / 2;
}

void PolyObject::draw()
{
	int verts = getVertNum();

	// set color
	glColor3fv(this->color);

	// draw a single point
	if (verts == 1) {
		glBegin(GL_POINTS);
	}
	// draw a line
	else if (verts == 2) {
		glBegin(GL_LINES);
	}
	// draw a polygon
	else if (verts > 2) {
		glBegin(GL_POLYGON);
	}

	// iterate through vertices
	for (int i = 0; i < getVertNum(); i++) {
		glVertex2fv(vertices.data() + (i * 2));
	}

	glEnd();
}

void PolyObject::unfinishedDraw(float* mousePos)
{
	int verts = getVertNum();

	// set color
	glColor3fv(this->color);

	// draw line from first pt to next 
	if (verts == 1)
	{
		glBegin(GL_LINE_STRIP);

		glVertex2fv(vertices.data());
	}
	// draw polygon extending to mouse position
	else if (verts > 1)
	{
		glBegin(GL_POLYGON);
		
		for (int i = 0; i < verts; i++) {
			glVertex2fv(vertices.data() + (i * 2));
		}
	}

	// extend to mouse pos
	glVertex2fv(mousePos);
	glEnd();
}
