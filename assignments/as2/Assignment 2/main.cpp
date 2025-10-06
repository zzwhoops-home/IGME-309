#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <iostream>
#include <vector>
#include "PolyObject.h"

// the window's width and height
int rasterWidth, rasterHeight;
float canvasWidth, canvasHeight;

// user mouse position
float mousePos[2];

vector<PolyObject> polyObjects;
PolyObject curObj;

void init(void)
{
    // initialize the size of the window
    rasterWidth = 600;
    rasterHeight = 600;

    canvasWidth = 10.0f;
    canvasHeight = 10.0f;

    // create initial polyobject & init array
    curObj = PolyObject();
    polyObjects = vector<PolyObject>();
}

void drawCursor()
{
    glColor3f(0.1f, 0.5f, 1.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2fv(mousePos);
    glEnd();
    glPointSize(1.0f);
}

// called when window is first created or when window is resized
void reshape(int w, int h)
{
    // update the screen dimensions
    rasterWidth = w;
    rasterHeight = h;

    // do an orthographic parallel projection, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvasWidth, 0.0, canvasHeight);

    /* tell OpenGL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei)rasterWidth, (GLsizei)rasterHeight);

    glutPostRedisplay();
}

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);
    
    // draw current poly objects
    for (PolyObject& obj : polyObjects) {
        obj.draw();
    }

    // draw cursor on top
    drawCursor();

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

void motion(int x, int y) {
    // mouse events are handled by OS, eventually. When using mouse in the raster window, it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
    mousePos[0] = (float)x / rasterWidth * canvasWidth;
    mousePos[1] = (float)(rasterHeight - y) / rasterHeight * canvasHeight;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    // esc leaves game
    case 27:
        exit(0);
        break;
    case 13:
        cout << "Enter pressed, shape should be completed " << endl;
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
    glutInitWindowSize((int)rasterWidth, (int)rasterHeight);

    glutCreateWindow("Assignment 2!");

    // register function to handle window resizes
    glutReshapeFunc(reshape);

    // register function to handle keyboard & mouse events
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    // register function that draws in the window
    glutDisplayFunc(display);

    // mouse movement + invisible cursor
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc(motion);

    glutMainLoop();
    return 0;
}
