#include "Lake.h"

Lake::Lake(vec3 offset, int num_bars, float lake_width, float lake_depth)
{
	this->offset = offset;
	this->num_bars = num_bars;
	this->lake_width = lake_width;
	this->lake_depth = lake_depth;

	// we're adding quads
	this->max_verts = num_bars * history_length * 4;
}

Lake::~Lake()
{
}

void Lake::init()
{
	// create vbo buffer
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	// allocate memory
	glBufferData(GL_ARRAY_BUFFER, max_verts * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lake::update_mesh()
{
	vertices.clear();
}
