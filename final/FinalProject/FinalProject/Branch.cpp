#include "Branch.h"
#include "Tree.h"
#include "utils.h"

Branch::Branch(vec3 parent_end_pt, vec3 direction, float height, float width)
{
	this->start_pt = parent_end_pt;
	this->height = height;
	this->width = width;

	// set end point
	vec3 end_pt = start_pt + (direction * height);
	this->end_pt = end_pt;
}

Branch::~Branch()
{
	for (Branch* child : this->childBranches)
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
		vec3 world_forward = vec3(0.0f, 0.0f, 1.0f); // world right
		local_up = cross(local_forward, world_forward);
	}
	local_forward = normalize(local_forward);

	// local x-axis
	vec3 local_right = cross(local_forward, local_up);

	// rotate based on local basis
	vec3 outward_vec = local_right * cos(theta) + local_forward * sin(theta);
	// rotate outward, adjust outward vec taking it to be the opposite side
	vec3 child_dir = normalize((local_up * cos(theta)) + (outward_vec * sin(theta)));

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

	// spawn children, recursively call generate_children()
	int total_children = randInt(base_tree->min_children, base_tree->max_children);
	for (int i = 0; i < total_children; i++)
	{
		float child_height = base_tree->start_height * pow(base_tree->height_falloff, cur_depth + 1);
		float child_width = base_tree->start_width * pow(base_tree->width_falloff, cur_depth + 1);

		// the tree base should have random rotation all around the tree
		float theta = (cur_depth == 0) ? randFloat(0.0f, M_PI * 2) : randFloat(base_tree->min_rotation, base_tree->max_rotation);
		float phi = randFloat(0, base_tree->max_vert_angle);
		vec3 child_direction = calc_child_dir(direction, theta, phi);

		// create new branch, add to array
		Branch* branch = new Branch(child_start_pt, child_direction, child_height, child_width);
		childBranches.push_back(branch);

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

void Branch::draw()
{
	for (Branch* branch : childBranches)
	{

	}
}
