#include "utils.h"
#include "Tree.h"
#include "Branch.h"
#include <iostream>

// static cube vertices/indices
// create cube
float vertices[] = {
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f
};

unsigned int indices[] = {
    // Front Face (using Indices 0, 1, 2, 3)
    0, 1, 2,
    2, 1, 3,

    // Back Face (using Indices 4, 5, 6, 7)
    4, 6, 5,
    5, 6, 7,

    // Top Face (using Indices 2, 3, 6, 7)
    2, 3, 6,
    6, 3, 7,

    // Bottom Face (using Indices 0, 1, 4, 5)
    0, 4, 1,
    1, 4, 5,

    // Right Face (using Indices 1, 3, 5, 7)
    1, 5, 3,
    3, 5, 7,

    // Left Face (using Indices 0, 2, 4, 6)
    0, 2, 4,
    4, 2, 6
};

/*
Initial settings (constructor)
*/
Tree::Tree(int min_depth_in, int max_depth_in,
    int min_children_in, int max_children_in,
    float start_height_in, float start_width_in,
    float height_falloff_in, float width_falloff_in,
    float min_rotation_in, float max_rotation_in,
    float max_vert_angle_in,
    const float color_in[3])
{
    // random seed
    srand(time(NULL));

    this->min_depth = min_depth_in;
    this->max_depth = max_depth_in;
    this->min_children = min_children_in;
    this->max_children = max_children_in;

    this->start_height = start_height_in;
    this->start_width = start_width_in;
    this->height_falloff = height_falloff_in;
    this->width_falloff = width_falloff_in;

    this->min_rotation = min_rotation_in;
    this->max_rotation = max_rotation_in;
    this->max_vert_angle = max_vert_angle_in;

    this->color[0] = color_in[0];
    this->color[1] = color_in[1];
    this->color[2] = color_in[2];

    // trunk branch null until generate_tree called
    this->trunk_branch = nullptr;
}

Tree::~Tree()
{
    // clean up tree
    if (trunk_branch != nullptr)
    {
        delete this->trunk_branch;
        trunk_branch = nullptr;
    }
}

void Tree::generate_tree()
{
    vec3 origin = vec3(0.0f, 0.0f, 0.0f);
    vec3 start_dir = vec3(0.0f, 1.0f, 0.0f);

    // all branches will be this color
    glColor3fv(color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    // create trunk
    trunk_branch = new Branch(origin, start_dir, start_height, start_width);

    // start branch child recursion
    trunk_branch->generate_children(this, 0, max_depth);
}

void Tree::update_tree()
{
    //for (Branch* branch : )
}

void Tree::draw()
{
    glMatrixMode(GL_MODELVIEW);

    trunk_branch->draw();
}
