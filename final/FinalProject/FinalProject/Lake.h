#pragma once
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// include the gl mathematic library 
#include <glm/glm.hpp>

#include <deque>
#include <vector>

#include "utils.h"

using namespace glm;

struct Vertex {
	float x; float y; float z;
	float xN; float yN; float zN;
	float r; float g; float b; float a;
};

class Lake
{
private:
	int history_length = 64;
	float yAmp = 5.0f;
	int num_bars;
	float lake_width;
	float lake_depth;

	int max_verts;

	std::deque<std::vector<float>> history;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	GLuint vbo_id, vao_id, ibo_id = 0;

	vec3 offset;
public:
	Lake(vec3 offset, int num_bars, float lake_width, float lake_depth);
	~Lake();
	void init();
	void update_mesh(std::vector<float> spectrum);
	void draw();
};

