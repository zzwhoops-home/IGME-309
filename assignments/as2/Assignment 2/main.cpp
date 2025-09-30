#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <iostream>
#include <vector>
#include "PolyObject.h"

// the window's width and height
int width, height;
float canvasWidth, canvasHeight;

vector<PolyObject> polyObjects;

void init(void)
{
    // initialize the size of the window
    width = 600;
    height = 600;

    canvasWidth = 10.0f;
    canvasHeight = 10.0f;

    // create initial polyobject & init array
    PolyObject obj = PolyObject();
    polyObjects = vector<PolyObject>();
    polyObjects.push_back(obj);
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
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        cout << "Left click pressed" << endl;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        cout << "Right click pressed" << endl;
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    // esc leaves game
    case 27:
        exit(0);
        break;
    case 13:
        cout << "Enter pressed " << endl;
        break;
    }
}

int main(int argc, char** argv)
{
    // call init function
    init();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // set the initial window size */
    glutInitWindowSize((int)width, (int)height);

    glutCreateWindow("Assignment 2!");

    // register function to handle window resizes
    glutReshapeFunc(reshape);

    // register function to handle keyboard & mouse events
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    // register function that draws in the window
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
