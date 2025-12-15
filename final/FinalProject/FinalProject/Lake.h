#pragma once
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// include the gl mathematic library 
#include <glm/glm.hpp>

#include <vector>

using namespace glm;

class Lake
{
private:
	int history_length = 50;
	int num_bars;
	float lake_width;
	float lake_depth;

	int max_verts;

	std::vector<vec3> vertices;

	unsigned int vbo_id, vao_id, ibo_id;

	vec3 offset;
public:
	Lake(vec3 offset, int num_bars, float lake_width, float lake_depth);
	~Lake();
	void init();
	void update_mesh();
};

