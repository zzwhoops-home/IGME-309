#pragma once

#include <vector>
#include <cstdlib> // srand, rand, time
#include <ctime>
#include <deque>
#include "utils.h"

class Branch;

class Tree
{
private:
	Branch* trunk_branch;

	// used for animation
	std::deque<float> pitchHistory;

	// maximum rotation
	float maxPitchValue = 2200.0f;
	float curPitchValue = 0.0f;
	float pitchTargetValue = 0.0f; // for lerping
	float lerpDampening = 0.5f;

	float targetTimer = 0.0f;
	float targetUpdateInterval = 0.25f; // seconds
	
	//// buffer object ids
	//unsigned int vao_id; // vertex arrays
	//unsigned int vbo_id; // vertex buffer id
	//unsigned int ibo_id; // index buffer id

	//void init_tree_buffers();

public:
	// random max depth selection
	int min_depth;
	int max_depth;
	
	// random number of children per branch
	int min_children;
	int max_children;

	// starting trunk height/width
	float start_height;
	float start_width;

	// decreasing branch height/width until children (multiplicative)
	float height_falloff;
	float width_falloff;

	// min/max rotation at each child segment
	float min_rotation;
	float max_rotation;
	float max_vert_angle; // if we think of sphere, constrain | phi | to ~>60 so tree always points up

	// color of branches
	float color[3];
	Tree(int min_depth_in, int max_depth_in,
		int min_children_in, int max_children_in,
		float start_height_in, float start_width_in,
		float height_falloff_in, float width_falloff_in,
		float min_rotation_in, float max_rotation_in,
		float max_vert_angle_in,
		const float color_in[3]);
	~Tree();
	void generate_tree();
	void update_tree(float deltaTime, float pitch);
	void draw();
};

