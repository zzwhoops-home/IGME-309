#include "utils.h"
#include "Tree.h"
#include "Branch.h"

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

    // buffers
    vbo_id = ibo_id = 0;
    init_tree_buffers();
}

Tree::~Tree()
{
    // clean up tree
    if (trunk_branch != nullptr)
    {
        delete this->trunk_branch;
        trunk_branch = nullptr;
    }

    // clean buffers when done
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ibo_id);
    glDeleteVertexArrays(1, &vao_id);
}

void Tree::init_tree_buffers()
{
    // prepare VBO
    // generate a new VBO and get the associated ID
    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);
    glGenBuffers(1, &ibo_id);

    glBindVertexArray(vao_id);

    // bind VBO in order to use
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    // upload data to VBO - data went to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 8 * 3, indices, GL_STATIC_DRAW);

    // vao
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Tree::generate_tree()
{
    vec3 origin = vec3(0.0f, 0.0f, 0.0f);
    vec3 start_dir = vec3(0.0f, 1.0f, 0.0f);

    // all branches will be this color
    glColor3fv(color);

    // create trunk
    trunk_branch = new Branch(origin, start_dir, start_height, start_width);

    // start branch child recursion
    trunk_branch->generate_children(this, 0, 0);
}

void Tree::draw()
{
    trunk_branch->draw();
}
