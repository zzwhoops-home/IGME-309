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
}
