#include "Branch.h"
#include "Tree.h"
#include "utils.h"
#include <iostream>

Branch::Branch(vec3 parent_end_pt, vec3 direction, float height, float width, float theta, float phi)
{
	this->start_pt = parent_end_pt;
	this->direction = direction;
	this->height = height;
	this->width = width;

	this->theta = theta;
	this->phi = phi;

	this->rotOffset = 0.0f;

	this->color[0] = 0.35f;
	this->color[1] = 0.35f;
	this->color[2] = 0.7f;
	this->color[3] = 0.85f;

	// set end point
	vec3 end_pt = start_pt + (direction * height);
	this->end_pt = end_pt;
}

Branch::~Branch()
{
	for (Branch* child : this->child_branches)
	{
		delete child;
	}
}

vec3 Branch::calc_child_dir(const vec3& parent_dir, float theta, float phi)
{
	// find local basis
	vec3 world_right = vec3(1.0f, 0.0f, 0.0f);
	
	// parent direction is local y-axis
	vec3 local_up = normalize(parent_dir);

	// local z-axis
	vec3 local_forward = cross(local_up, world_right);

	// if we get a very short vector
	if (length(local_forward) < 0.0001f)
	{
		local_forward = cross(local_up, vec3(0.0f, 0.0f, 1.0f));
	}
	local_forward = normalize(local_forward);

	// local x-axis
	vec3 local_right = cross(local_forward, local_up);

	// rotate based on local basis
	vec3 outward_vec = local_right * cos(theta) + local_forward * sin(theta);
	// rotate outward, adjust outward vec taking it to be the opposite side
	vec3 child_dir = normalize((local_up * cos(phi)) + (outward_vec * sin(phi)));

	return child_dir;
}

void Branch::generate_children(const Tree* base_tree, int cur_depth, int max_depth)
{
	// leaf node
	if (cur_depth >= max_depth)
	{
		return;
	}

	vec3 child_start_pt = end_pt;

	// std::cout << cur_depth << std::endl;

	// spawn children, recursively call generate_children()
	int min_children = base_tree->min_children;
	int max_children = base_tree->max_children;

	int total_children = (cur_depth == 0) ? randInt(min_children * 5, max_children * 5) : randInt(min_children, max_children);
	for (int i = 0; i < total_children; i++)
	{
		float child_height = base_tree->start_height * pow(base_tree->height_falloff, cur_depth + 1);
		float child_width = base_tree->start_width * pow(base_tree->width_falloff, cur_depth + 1);

		// the tree base should have random rotation all around the tree
		float childTheta = (cur_depth == 0) ? randFloat(0.0f, M_PI * 2) : randFloat(base_tree->min_rotation, base_tree->max_rotation);
		float childPhi = randFloat(0, base_tree->max_vert_angle);
		vec3 child_direction = calc_child_dir(direction, theta, phi);

		// create new branch, add to array
		Branch* branch = new Branch(child_start_pt, child_direction, child_height, child_width, childTheta, childPhi);
		child_branches.push_back(branch);

		// generate those children
		if (cur_depth == 0)
		{
			int branch_max_depth = randInt(base_tree->min_depth, base_tree->max_depth);
			branch->generate_children(base_tree, cur_depth + 1, branch_max_depth);
		}
		else
		{
			branch->generate_children(base_tree, cur_depth + 1, max_depth);
		}
	}
}

void Branch::update_branch(float rotOffset, int cur_depth)
{
	this->end_pt = this->start_pt + (this->direction * this->height);

	vec3 child_start_pt = this->end_pt;

	// std::cout << cur_depth << std::endl;

	for (Branch* child_branch : child_branches)
	{
		child_branch->start_pt = this->end_pt;

		// the tree base should have random rotation all around the tree
		vec3 child_direction = calc_child_dir(direction, child_branch->theta, child_branch->phi + rotOffset);

		child_branch->direction = child_direction;

		child_branch->update_branch(rotOffset, cur_depth + 1);

		// update color
		child_branch->color[0] = rotOffset; // r
		child_branch->color[3] = 1.0f - rotOffset; // a
	}

	// leaf node
	if (child_branches.size() == 0)
	{
		return;
	}
}

void Branch::draw()
{
	//std::cout << "Drawing (" << start_pt.x << ", " << start_pt.y << ", " << start_pt.z	<< ")" << std::endl;
	vec3 offset_to_center = (direction * height) * 0.5f;
	
	vec3 world_up = vec3(0.0f, 1.0f, 0.0f);
	vec3 rotation_axis = cross(world_up, direction);
	float rotation_angle = degrees(acos(dot(world_up, direction)));

	glPushMatrix();

	// translate to start pt
	glTranslatef(start_pt.x, start_pt.y, start_pt.z);
	glTranslatef(offset_to_center.x, offset_to_center.y, offset_to_center.z);

	// rotate around branch axis
	glRotatef(rotation_angle, rotation_axis.x, rotation_axis.y, rotation_axis.z);

	// scale
	glScalef(width, height, width);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(1 - color[0], color[1], color[2], color[3]);
	glutSolidCube(1.0f);

	glColor4f(color[0], color[1], color[2], color[3]);
	glutWireCube(1.02f);

	glPopAttrib();

	glPopMatrix();
	
	for (Branch* child_branch : child_branches)
	{
		child_branch->draw();
	}
}
