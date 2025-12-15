#pragma once
#include <GL/glew.h>

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

#include <cstdlib>
#include <ctime>
#include <vector>

using namespace glm;

vec3 getVecDirection(vec3 end, vec3 start);

int randInt(int min, int max);
float randFloat(float min, float max);

vec3 getRandomVec(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

float lerp(float a, float b, float t);