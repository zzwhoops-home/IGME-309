// Exercise 3: Reshape Triangle
// Zachary Deng
// 9/11/2025
#include <GL/freeglut.h> //include glut for Windows
#include <iostream>

// the window's width and height
int width, height;
bool useRedColor = true;

const float SCALING_RATIO = 0.01f;

void init(void)
{
    // initialize the size of the window
    width = 400;
    height = 400;
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
    // calculate half of the window's dimensions
    float halfWidth = (float)width * SCALING_RATIO;
    float halfHeight = (float)height * SCALING_RATIO;

    // Set up an orthographic projection where (0,0) is the center of the window
    // and one unit corresponds to one pixel.
    gluOrtho2D(-halfWidth, halfWidth, -halfHeight, halfHeight);

    /* tell OpenGL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glutPostRedisplay();
}

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // wipe the entire color buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Default coordinate bounds
    // x range: –1.0 (left) to +1.0 (right)
    // y range: –1.0 (bottom) to +1.0 (top)
    // z range: –1.0 (near) to +1.0 (far)

    if (useRedColor)
    {
        glColor3f(1.0f, 0.0f, 0.0f); // blue
    }
    else
    {
        glColor3f(0.0f, 0.0f, 1.0f); // blue
    }

    glBegin(GL_TRIANGLES);
    glVertex2f(-2.0f, 2.0f);  // top-left
    glVertex2f(2.0f, 2.0f);  // top-right
    glVertex2f(0.0f, -2.0f);  // middle-bottom
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // set the initial window size */
    glutInitWindowSize((int)width, (int)height);

    glutCreateWindow("My First Triangle!");

    // register function to handle window resizes
    glutReshapeFunc(reshape);

    // register function that draws in the window
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
