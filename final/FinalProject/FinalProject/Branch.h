#pragma once
#define _USE_MATH_DEFINES
#include <GL/glew.h>

#include "utils.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// include the gl mathematic library 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>


using namespace glm;

class Tree;

class Branch
{
private:
	std::vector<Branch*> childBranches;
public:
	vec3 start_pt;
	vec3 end_pt;
	vec3 direction;
	float height;
	float width;

	Branch(vec3 parent_end_pt, vec3 direction, float height, float width);
	~Branch();
	
	vec3 calc_child_dir(const vec3& parent_dir, float theta, float phi);
	void generate_children(const Tree* base_tree, int cur_depth, int max_depth);
	void draw();
};

