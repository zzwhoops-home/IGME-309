#define _USE_MATH_DEFINES

#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <cmath>

// the window's width and height
int width, height;
float canvasWidth, canvasHeight;

// user settings
const int minVertices = 3;
const int maxVertices = 100;
int circleVertices = 10;

void init(void)
{
    // initialize the size of the window
    width = 400;
    height = 400;

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

void drawCircle(float x, float y, float radius, float color[]) {
    glColor3f(color[0], color[1], color[2]);

    // draw circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < circleVertices; i++) {
        float theta = ((float)i / circleVertices) * (M_PI * 2.0f);

        glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();
}

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0f, 0.0f, 0.0f); // blue

    float color[] = {1.0f, 0.0f, 0.0f};
    drawCircle(5.0f, 5.0f, 2.0f, color);

    //glBegin(GL_TRIANGLES);
    //glVertex2f(-2.0f, 2.0f);  // top-left
    //glVertex2f(2.0f, 2.0f);  // top-right
    //glVertex2f(0.0f, -2.0f);  // middle-bottom
    //glEnd();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    // esc leaves game
    case 27:
        exit(0);
        break;
    case '+':
        if (circleVertices + 1 <= maxVertices) {
            circleVertices++;
        }
        break;
    case '-':
        if (circleVertices - 1 >= minVertices) {
            circleVertices--;
        }
        break;
    }
}

int main(int argc, char** argv)
{
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
