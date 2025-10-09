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

float curColor[3];
vector<PolyObject*> polyObjects;
PolyObject* curObj;

void init(void)
{
    // initialize the size of the window
    rasterWidth = 600;
    rasterHeight = 600;

    canvasWidth = 10.0f;
    canvasHeight = 10.0f;

    // create initial polyobject & init array
    curObj = new PolyObject();
    polyObjects = vector<PolyObject*>();

    // default color
    curColor[0] = 0.0f;
    curColor[1] = 0.0f;
    curColor[2] = 1.0f;
    curObj->setColor(curColor[0], curColor[1], curColor[2]);
}

void drawCursor()
{
    glColor3f(curColor[0], curColor[1], curColor[2]);

    glBegin(GL_POINTS);
    glVertex2fv(mousePos);
    glEnd();
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
    for (PolyObject* obj : polyObjects) {
        obj->draw();
    }

    // draw current object over top
    curObj->unfinishedDraw(mousePos);

    // draw cursor on top
    drawCursor();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        curObj->addVertex(mousePos[0], mousePos[1]);
    }
}

void motion(int x, int y) {
    // mouse events are handled by OS, eventually. When using mouse in the raster window, it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
    mousePos[0] = (float)x / rasterWidth * canvasWidth;
    mousePos[1] = (float)(rasterHeight - y) / rasterHeight * canvasHeight;

    glutPostRedisplay();
}

void clear() {
    // clean up every single vector element
    for (PolyObject* obj : polyObjects) {
        delete obj;
    }

    // good practice or whatever
    polyObjects.clear();
}

void cleanup() {
    clear();
    
    // clean current object if it hasn't been added/deleted already
    if (curObj != nullptr) {
        delete curObj;
        curObj = nullptr;
    }
    
    // exit program
    exit(0);
}

void endCurShape() {
    // add to vector of all objects
    polyObjects.push_back(curObj);

    // create new object
    curObj = new PolyObject();

    // set to current color 
    curObj->setColor(curColor[0], curColor[1], curColor[2]);

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    // esc leaves game
    case 27:
        // cleanup heap memory & exit
        cleanup();
        break;
    case 13:
        if (curObj->getVertNum() != 0) {
            // close shape
            endCurShape();
        }
        break;
    }
}

void menu(int value)
{
    if (value >= 2 && value <= 8) {
        switch (value) {
        case 2: // red
            curColor[0] = 1.0f;
            curColor[1] = 0.0f;
            curColor[2] = 0.0f;
            break;
        case 3: // orange
            curColor[0] = 1.0f;
            curColor[1] = 0.6f;
            curColor[2] = 0.0f;
            break;
        case 4: // yellow
            curColor[0] = 1.0f;
            curColor[1] = 1.0f;
            curColor[2] = 0.0f;
            break;
        case 5: // green
            curColor[0] = 0.0f;
            curColor[1] = 1.0f;
            curColor[2] = 0.0f;
            break;
        case 6: // blue
            curColor[0] = 0.0f;
            curColor[1] = 0.0f;
            curColor[2] = 1.0f;
            break;
        case 7: // purple
            curColor[0] = 0.5f;
            curColor[1] = 0.0f;
            curColor[2] = 0.5f;
            break;
        case 8: // violet
            curColor[0] = 0.5f;
            curColor[1] = 0.0f;
            curColor[2] = 1.0f;
            break;
        default:
            break;
        }

        // set color based on current object setting
        curObj->setColor(curColor[0], curColor[1], curColor[2]);
        glutPostRedisplay();
    }
    else {
        switch (value) {
        case 0: // clear
            clear();
            glutPostRedisplay();
            break;
        case 1: //exit
            cleanup();
            break;
        default:
            break;
        }
    }
}

void createMenu() {
    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Orange", 3);
    glutAddMenuEntry("Yellow", 4);
    glutAddMenuEntry("Green", 5);
    glutAddMenuEntry("Blue", 6);
    glutAddMenuEntry("Purple", 7);
    glutAddMenuEntry("Violet", 8);

    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddSubMenu("Colors", colorMenu);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
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

    // default sizes
    glPointSize(10.0f);
    glLineWidth(3.0f);

    // create right-click menu
    createMenu();

    glutMainLoop();
    return 0;
}
