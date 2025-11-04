//Usage:
//Hold down the number keys , 1-7, to select one or multiple circles.
//While circle(s) are selected, use the left mouse button to translate and use the right mouse button to rotate.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <math.h>
using namespace std;

#define NUM_SHAPES 16
#define CIRCLE_RADIUM 2.0

int win_width = 600, win_height = 600;
float canvas_width = 20.0f; float canvas_height = 20.0f;


bool keyStates[256];
int buttonState;

int totalShapes = 0;
float shapes[4 * NUM_SHAPES];
float colors[3 * NUM_SHAPES];
float translations[2 * NUM_SHAPES];
float rotations[NUM_SHAPES];

float curMouse[2];
float preMouse[2];

void addRectToShapes(float x, float y, float w, float h)
{
    shapes[totalShapes * 4] = x;
    shapes[totalShapes * 4 + 1] = y;
    shapes[totalShapes * 4 + 2] = w;
    shapes[totalShapes * 4 + 3] = h;
    totalShapes++;
}

void init(void)
{
    for (int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }

    // torso is center (first transform)
    addRectToShapes(0.0, 0.0, 4.0, 2.0);

    // chest connected to torso (2nd transform)
    addRectToShapes(0.0, 0.0, 3.0, 2.0);

    // neck to chest
    addRectToShapes(0.0, 0.0, 0.5, 1.0);

    // head to neck
    addRectToShapes(0.0, 0.0, 2.0, 2.0);

    // l upper arm to chest
    // l lower arm to l upper arm
    // l hand to l lower arm

    // r upper arm to chest
    // r lower arm to r upper arm
    // r hand to r lower arm

    // l upper leg to torso
    // l lower leg to l upper leg
    // l foot to l lower leg

    // u upper leg to torso
    // u lower leg to u upper leg
    // u foot to u lower leg

    for (int i = 0; i < NUM_SHAPES; i++) {
        colors[i * 3 + 0] = 0.0f; // red
        colors[i * 3 + 1] = 0.0f; // green
        colors[i * 3 + 2] = 0.0f; // blue

        translations[i * 2 + 0] = 0.0f; // x
        translations[i * 2 + 1] = 0.0f; // y

        rotations[i] = 0.0f;
    }

    buttonState = -1;
}

void drawCircle(float radius, const float* c)
{
    glColor3fv(c);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 100; i++)
        glVertex2f(radius * cosf(3.14 * 2 / 100 * i), radius * sinf(3.14 * 2 / 100 * i));
    glEnd();
}

// from top-left corner
void drawRect(float x, float y, float width, float height)
{
    //glColor3fv(colors);
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // torso is center (first transform)
    drawRect(0.0f, 0.0f, 4.0f, 2.0f);

    // chest connected to torso (2nd transform)
    drawRect(0.0f, 0.0f, 3.0f, 2.0f);
    
    // neck to chest
    drawRect(0.0f, 0.0f, 0.5f, 1.0f);
    
    // head to neck
    drawRect(0.0f, 0.0f, 2.0f, 2.0f);

    // l upper arm to chest
    // l lower arm to l upper arm
    // l hand to l lower arm

    // r upper arm to chest
    // r lower arm to r upper arm
    // r hand to r lower arm

    // l upper leg to torso
    // l lower leg to l upper leg
    // l foot to l lower leg

    // u upper leg to torso
    // u lower leg to u upper leg
    // u foot to u lower leg


    // the following codes could be written in a for loop.
    // Here I expand them so that you can better trace the changes of cirlce's coordinate system.

    //int cid = -1; // the index of current circle
    //// circle 0
    //cid = 0;
    //glTranslatef(translations[cid * 2 + 0], translations[cid * 2 + 1], 0.0f);
    //glRotatef(rotations[cid], 0.0f, 0.0f, 1.0f);
    //drawCircle(CIRCLE_RADIUM * (MAX_NUM_CIRCLE - cid) / MAX_NUM_CIRCLE, colors + cid * 3);

    //// circle 1
    //cid = 1;
    //glTranslatef(translations[cid * 2 + 0], translations[cid * 2 + 1], 0.0f);
    //glRotatef(rotations[cid], 0.0f, 0.0f, 1.0f);
    //drawCircle(CIRCLE_RADIUM * (MAX_NUM_CIRCLE - cid) / MAX_NUM_CIRCLE, colors + cid * 3);

    //// circle 2
    //cid = 2;
    //glTranslatef(translations[cid * 2 + 0], translations[cid * 2 + 1], 0.0f);
    //glRotatef(rotations[cid], 0.0f, 0.0f, 1.0f);
    //drawCircle(CIRCLE_RADIUM * (MAX_NUM_CIRCLE - cid) / MAX_NUM_CIRCLE, colors + cid * 3);

    //// circle 3
    //cid = 3;
    //glTranslatef(translations[cid * 2 + 0], translations[cid * 2 + 1], 0.0f);
    //glRotatef(rotations[cid], 0.0f, 0.0f, 1.0f);
    //glPushMatrix(); // push the circle 1's CS to the modelview stack
    //drawCircle(CIRCLE_RADIUM * (MAX_NUM_CIRCLE - cid) / MAX_NUM_CIRCLE, colors + cid * 3);

    //// circle 4
    //cid = 4;
    //glTranslatef(translations[cid * 2 + 0], translations[cid * 2 + 1], 0.0f);
    //glRotatef(rotations[cid], 0.0f, 0.0f, 1.0f);
    //drawCircle(CIRCLE_RADIUM * (MAX_NUM_CIRCLE - cid) / MAX_NUM_CIRCLE, colors + cid * 3);
    //glPopMatrix(); // back to the CS of Circle 1

    //// circle 5
    //cid = 5;
    //glTranslatef(translations[cid * 2 + 0], translations[cid * 2 + 1], 0.0f);
    //glRotatef(rotations[cid], 0.0f, 0.0f, 1.0f);
    //drawCircle(CIRCLE_RADIUM * (MAX_NUM_CIRCLE - cid) / MAX_NUM_CIRCLE, colors + cid * 3);


    glutSwapBuffers();
}

void reshape(int w, int h)
{
    win_width = w;
    win_height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-canvas_width / 2.0f, canvas_width / 2.0f, -canvas_height / 2.0f, canvas_height / 2.0f);
    glViewport(0, 0, (GLsizei)win_width, (GLsizei)win_height);

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // 'esc' key
        exit(0);

    unsigned char asciiOffset = 49; // see an ascii table
    for (unsigned char i = '1'; i < '7'; i++) {
        if (key == i) {
            keyStates[i] = true;
            colors[(i - asciiOffset) * 3 + 0] = 1.0f;
            colors[(i - asciiOffset) * 3 + 1] = 0.0f;
            colors[(i - asciiOffset) * 3 + 2] = 0.0f;
        }
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    unsigned char asciiOffset = 49; // see an ascii table
    for (unsigned char i = '1'; i < '7'; i++) {
        if (key == i) {
            keyStates[i] = false;
            colors[(i - asciiOffset) * 3 + 0] = 0.0f;
            colors[(i - asciiOffset) * 3 + 1] = 0.0f;
            colors[(i - asciiOffset) * 3 + 2] = 0.0f;
        }
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        buttonState = button;
        curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
        curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
        preMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
        preMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
    }
    else if (state == GLUT_UP)
        button = -1;
}

void motion(int x, int y)
{
    unsigned char asciiOffset = 49; // see an ascii table

    curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
    curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;

    if (buttonState == GLUT_LEFT_BUTTON) {
        for (unsigned char i = '1'; i < '7'; i++) {
            if (keyStates[i]) {
                translations[(i - asciiOffset) * 2 + 0] += curMouse[0] - preMouse[0];
                translations[(i - asciiOffset) * 2 + 1] += curMouse[1] - preMouse[1];
            }
        }
        glutPostRedisplay();
    }

    else if (buttonState == GLUT_RIGHT_BUTTON) {
        for (unsigned char i = '1'; i < '7'; i++) {
            if (keyStates[i]) {
                rotations[i - asciiOffset] += curMouse[0] - preMouse[0];
            }
        }
        glutPostRedisplay();
    }

    preMouse[0] = curMouse[0];
    preMouse[1] = curMouse[1];

}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("2D Transformation Tree");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;

}
