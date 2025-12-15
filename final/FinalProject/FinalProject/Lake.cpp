#include "Lake.h"
#include <iostream>

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
	glDeleteBuffers(1, &vbo_id);
	glDeleteBuffers(1, &vao_id);
	glDeleteBuffers(1, &ibo_id);
}

void Lake::init()
{
	// create vao buffer
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	// create vbo buffer
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	// allocate memory
	glBufferData(GL_ARRAY_BUFFER, max_verts * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

	// attrib pointers
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableClientState(GL_NORMAL_ARRAY);

	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableClientState(GL_COLOR_ARRAY);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// create ibo buffer
	glGenBuffers(1, &ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);

	// allocate memory
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_verts * 1.5 * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lake::update_mesh(std::vector<float> spectrum)
{
	// clear vertices & indices to draw new mesh
	vertices.clear();
	indices.clear();

	history.push_back(spectrum);

	if (history.size() > history_length)
	{
		history.pop_front();
	}

	float width = lake_width / num_bars;
	float depth = lake_depth / history.size();

	unsigned int vertex_offset = 0;

	if (history.size() >= 2)
	{
		for (size_t z = 0; z < history.size() - 1; z++)
		{
			for (size_t x = 0; x < num_bars - 1; x++)
			{
				if (x >= history[z].size())
				{
					break;
				}
				// may have to swap depending on which one is back
				float x0 = width * x;
				float z0 = depth * z;

				float x1 = x0 + width;
				float z1 = z0 + depth;

				// xz-order
				float y00 = history[z][x] * yAmp;
				float y10 = history[z][x + 1] * yAmp;
				float y01 = history[z + 1][x] * yAmp;
				float y11 = history[z + 1][x + 1] * yAmp;

				// standard normal
				vec3 normal = vec3(0.0f, 1.0f, 0.0f);
				float colors[4] = {
					0.5f + y00 / yAmp,
					0.75f + y00 / yAmp,
					0.8f + y00 / yAmp,
					0.8f
				};

				Vertex v1 = { x0, y00, z0, normal.x, normal.y, normal.z, colors[0], colors[1], colors[2], colors[3] };
				Vertex v2 = { x1, y10, z0, normal.x, normal.y, normal.z, colors[0], colors[1], colors[2], colors[3] };
				Vertex v3 = { x0, y01, z1, normal.x, normal.y, normal.z, colors[0], colors[1], colors[2], colors[3] };
				Vertex v4 = { x1, y11, z1, normal.x, normal.y, normal.z, colors[0], colors[1], colors[2], colors[3] };

				vertices.push_back(v1);
				vertices.push_back(v2);
				vertices.push_back(v3);
				vertices.push_back(v4);

				// 013, 023 indices make up 2 triangles that form quad
				indices.push_back(vertex_offset + 0);
				indices.push_back(vertex_offset + 1);
				indices.push_back(vertex_offset + 3);

				indices.push_back(vertex_offset + 0);
				indices.push_back(vertex_offset + 3);
				indices.push_back(vertex_offset + 2);

				vertex_offset += 4;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Lake::draw()
{
	glPushMatrix();
	vec3 translation = vec3(-lake_width / 2.0f, 0.0f, -lake_depth / 2.0f);
	translation += offset;
	glTranslatef(translation.x, translation.y, translation.z);

	glBindVertexArray(vao_id);

	// draw indices
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	// unbind vao and vbo
	glBindVertexArray(0);

	glPopMatrix();
}