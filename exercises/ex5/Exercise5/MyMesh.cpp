#include "MyMesh.h"
#include <GL/freeglut.h>

#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
using namespace std;

MyMesh::MyMesh()
{
	vertNum = triNum = 0;
	vertices = NULL;
	indices = NULL;
	vertColors = NULL;
}

MyMesh::~MyMesh()
{
	if (vertices != NULL)
		delete[] vertices;
	if (indices != NULL)
		delete[] indices;

	if (vertColors != NULL)
		delete[] vertColors;
}

void MyMesh::load(char *fileName)
{
	vertNum = 0;
	triNum = 0;

	vertices = new float[100 * 2];
	indices = new unsigned int[100 * 3];
	vertColors = new float[100 * 3];

	ifstream file(fileName);

	if (!file.is_open())
	{
		/****************************************/
		// Write your code below
		cout << "Error: " << fileName << " failed to open. Check that the file exists." << endl;
		// Write your code above
		/****************************************/
	}

	/****************************************/
	// Write your code below

	// store strings read
	string line;
	string delimiter = " ";

	while (getline(file, line)) {
		char type = line[0];

		// find next two values, space separated, assuming index 2 begins string
		string values = line.substr(2);

		if (type == 'v') {
			// get next space to split
			int nextSpace = values.find(delimiter);

			// get substrings of coords
			float x = stof(values.substr(0, nextSpace));
			float y = stof(values.substr(nextSpace + delimiter.length()));

			vertices[vertNum] = x;
			vertices[vertNum + 1] = y;

			// update index
			vertNum += 2;
		}
		else if (type == 'f') {
			// better solution is with while loop
			size_t curPos = 0;
			string token;
			int index = 0;

			while ((curPos = values.find(delimiter)) != string::npos) {
				// get up to next space
				token = values.substr(0, curPos);

				indices[triNum * 3 + index] = stoi(token) - 1;
				index++;

				// erase part of string that we already dealt with
				values.erase(0, curPos + delimiter.length());
			}

			// add last token (remaining character/string)
			indices[triNum * 3 + index] = stoi(values) - 1;

			// we've added a tri
			triNum++;
		}
	}
	// Write your code above
	/****************************************/

	// generate random vertex colors
	for (unsigned int i = 0; i < vertNum; i++)
	{
		vertColors[i * 3 + 0] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 1] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 2] = float(rand()) / (RAND_MAX + 1);
	}
}

void MyMesh::load(const unsigned int p_vertNum, const unsigned int p_triNum, const float *p_vertices, const unsigned int *p_indices)
{
	vertNum = p_vertNum;
	triNum = p_triNum;

	vertices = new float[vertNum * 2];
	indices = new unsigned int[triNum * 3];
	vertColors = new float[vertNum * 3];

	for (unsigned int i = 0; i < vertNum * 2; i++)
	{
		vertices[i] = p_vertices[i];
	}

	for (unsigned int i = 0; i < triNum * 3; i++)
	{
		indices[i] = p_indices[i];
	}

	// generate random vertex colors
	for (unsigned int i = 0; i < vertNum; i++)
	{
		vertColors[i * 3 + 0] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 1] = float(rand()) / (RAND_MAX + 1);
		vertColors[i * 3 + 2] = float(rand()) / (RAND_MAX + 1);
	}
}

void MyMesh::update()
{
	// we will use this for transformations later....
}

void MyMesh::draw()
{
	for (int i = 0; i < triNum; ++i)
	{
		glBegin(GL_TRIANGLES);
		glColor3fv(vertColors + indices[i * 3 + 0] * 3);
		glVertex2fv(vertices + indices[i * 3 + 0] * 2);

		glColor3fv(vertColors + indices[i * 3 + 1] * 3);
		glVertex2fv(vertices + indices[i * 3 + 1] * 2);

		glColor3fv(vertColors + indices[i * 3 + 2] * 3);
		glVertex2fv(vertices + indices[i * 3 + 2] * 2);
		glEnd();
	}
}