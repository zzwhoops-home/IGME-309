#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <iostream>
#include "functions.h"

// the window's width and height
int width, height;
float canvasWidth, canvasHeight;

// user settings
const int minVertices = 3;
const int maxVertices = 100;
// global variable
int circleVertices = 10;

void init(void)
{
    // initialize the size of the window
    width = 600;
    height = 600;

    canvasWidth = 10.0f;
    canvasHeight = 10.0f;
}

// called when window is first created or when window is resized
void reshape(int w, int h)
{
    // update the screen dimensions
    width = w;
    height = h;

    // do an orthographic parallel projection, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvasWidth, 0.0, canvasHeight);

    /* tell OpenGL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glutPostRedisplay();
}

void display()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    // BEGIN PANDA DRAWING
    // ears (in the back)
    drawFilledCircle(0.0, 0.0, 0.0, 6.25, 8.25, 0.5); // left ear
    drawFilledCircle(0.0, 0.0, 0.0, 3.75, 8.25, 0.5); // right ear
    drawWireframeCircle(0.6, 0.0, 0.85, 6.25, 8.25, 0.55, 5.0); // left ear outline
    drawWireframeCircle(0.6, 0.0, 0.85, 3.75, 8.25, 0.55, 5.0); // left ear outline

    // Panda body
    drawFilledCircle(0.75, 0.0, 1.0, 5.0, 4.0, 1.25); // purple body
    drawFilledCircle(1.0, 1.0, 1.0, 5.0, 6.5, 2.0); // white head

    // nose
    drawFilledCircle(0.0, 0.0, 0.0, 5.0, 6.0, 0.4); // main nose
    drawFilledCircle(1.0, 1.0, 1.0, 4.8, 6.0, 0.1); // highlight

    // eyes
    drawFilledCircle(0.0, 0.0, 0.0, 4.0, 6.75, 0.5); // left eye
    drawFilledCircle(0.0, 0.0, 0.0, 6.0, 6.75, 0.5); // right eye
    drawFilledCircle(1.0, 1.0, 1.0, 4.15, 6.65, 0.3); // left pupil
    drawFilledCircle(1.0, 1.0, 1.0, 5.85, 6.65, 0.3); // right pupil
    drawWireframeCircle(0.0, 0.0, 0.0, 4.15, 6.65, 0.3, 1.5); // left pupil
    drawWireframeCircle(0.0, 0.0, 0.0, 5.85, 6.65, 0.3, 1.5); // right pupil
    drawFilledCircle(0.8, 0.0, 0.0, 4.15, 6.65, 0.25); // inner left pupil
    drawFilledCircle(0.8, 0.0, 0.0, 5.85, 6.65, 0.25); // inner right pupil

    // hands
    drawFilledCircle(0.8, 0.0, 0.0, 5.9, 4.5, 0.5); // left hand
    drawFilledCircle(0.8, 0.0, 0.0, 4.1, 4.5, 0.5); // right hand

    // feet
    drawFilledCircle(0.8, 0.0, 0.0, 6.2, 3.25, 0.7); // left foot
    drawFilledCircle(0.8, 0.0, 0.0, 3.8, 3.25, 0.7); // right foot

    // END PANDA DRAWING

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    // log mouse position
    std::cout << "X: " << x / 60.0f << " Y: " << (canvasHeight) - (y / 60.0f) << std::endl;

    switch (key) {
    // esc leaves game
    case 27:
        exit(0);
        break;
    // technically "+", but without pressing shift
    case '=':
        if (circleVertices + 1 <= maxVertices) {
            circleVertices++;
        }

        // request redraw
        glutPostRedisplay();
        break;
    case '-':
        if (circleVertices - 1 >= minVertices) {
            circleVertices--;
        }

        // request redraw
        glutPostRedisplay();
        break;
    }
    std::cout << circleVertices << std::endl;
}

int main(int argc, char** argv)
{
    // call init function
    init();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // set the initial window size */
    glutInitWindowSize((int)width, (int)height);

    glutCreateWindow("Assignment 1!");

    // register function to handle window resizes
    glutReshapeFunc(reshape);

    // register function to handle keyboard events
    glutKeyboardFunc(keyboard);

    // register function that draws in the window
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
