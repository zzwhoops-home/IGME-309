#define _USE_MATH_DEFINES
#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <cmath>

// define M_PI if not supported
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern int circleVertices;

/// <summary>
/// Draws a filled circle of the given color
/// </summary>
/// <param name="red">R (0.0-1.0)</param>
/// <param name="green">G (0.0-1.0)</param>
/// <param name="blue">B (0.0-1.0)</param>
/// <param name="center_x">X-coordinate of circle center</param>
/// <param name="center_y">Y-coordinate of circle center</param>
/// <param name="radius">Radius of circle</param>
void drawFilledCircle(float red, float green, float blue, float center_x, float center_y, float radius);

/// <summary>
/// Draws a hollow circle of the given color
/// </summary>
/// <param name="red">R (0.0-1.0)</param>
/// <param name="green">G (0.0-1.0)</param>
/// <param name="blue">B (0.0-1.0)</param>
/// <param name="center_x">X-coordinate of circle center</param>
/// <param name="center_y">Y-coordinate of circle center</param>
/// <param name="radius">Radius of circle</param>
/// <param name="lineWidth">Width of wireframe line</param>
void drawWireframeCircle(float red, float green, float blue, float center_x, float center_y, float radius, float lineWidth);

void drawArc(float red, float green, float blue, float center_x, float center_y, float radius, float lineWidth, float arcStart, float arcEnd);