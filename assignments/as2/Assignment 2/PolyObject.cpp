#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include "PolyObject.h"

PolyObject::PolyObject()
{
	this->vertices = vector<float>();
	this->color = new float(3);
}

PolyObject::~PolyObject()
{
	delete[] color;
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
	glBegin(GL_POLYGON);

	// draw a line 
	for (int i = 0; i < getVertNum(); i++) {
		glVertex2fv(vertices.data() + i * 2);
	}

	glEnd();
}

void PolyObject::unfinishedDraw(float* mousePos)
{
	int verts = getVertNum();

	// draw line from first pt to next 
	if (verts == 1)
	{
		glBegin(GL_LINE_STRIP);

		glVertex2fv(vertices.data());
		glVertex2fv(mousePos);

		glEnd();
	}
	// draw polygon extending to mouse position
	else if (verts > 1)
	{

	}
}
