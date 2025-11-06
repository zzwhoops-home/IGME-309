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

int selectedShape = 0;
int totalShapes = 0;
float rotationAmount = 5.0f;

float shapes[4 * NUM_SHAPES];
float colors[3 * NUM_SHAPES];
float translations[2 * NUM_SHAPES];
float rotations[NUM_SHAPES];

float curMouse[2];
float preMouse[2];

void addRectToShapes(float localX, float localY, float w, float h, char type)
{
    shapes[totalShapes * 4 + 0] = localX;
    shapes[totalShapes * 4 + 1] = localY;
    shapes[totalShapes * 4 + 2] = w;
    shapes[totalShapes * 4 + 3] = h;

    float translateX = 0.0f; float translateY = 0.0f;

    switch (type) {
    // anchor point on bottom
    case 'b':
        translateX = -w / 2;
        break;
    case 'l':
        translateY = -h / 2;
        break;
    case 'r':
        translateY = -h / 2;
        translateX = -w;
        break;
    case 't':
        translateX = -w / 2;
        translateY = -h;
        break;
    }
    translations[totalShapes * 2 + 0] = translateX;
    translations[totalShapes * 2 + 1] = translateY;

    totalShapes++;
}

void resetColor(int index) {
    colors[index * 3 + 0] = 0.0f;
    colors[index * 3 + 1] = 0.0f;
    colors[index * 3 + 2] = 0.0f;
    glutPostRedisplay();
}

void selectColor(int index) {
    colors[index * 3 + 0] = 1.0f;
    colors[index * 3 + 1] = 0.0f;
    colors[index * 3 + 2] = 0.0f;
    glutPostRedisplay();
}

void init(void)
{
    for (int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }

    for (int i = 0; i < NUM_SHAPES; i++) {
        colors[i * 3 + 0] = 0.0f; // red
        colors[i * 3 + 1] = 0.0f; // green
        colors[i * 3 + 2] = 0.0f; // blue

        translations[i * 2 + 0] = 0.0f; // x
        translations[i * 2 + 1] = 0.0f; // y

        rotations[i] = 0.0f;
    }

    int i = 0;
    // torso is center (first transform)
    addRectToShapes(0.0, 0.0, 3.0, 2.0, 'b');

    // chest connected to torso (2nd transform)
    addRectToShapes(0.0, 2.0, 4.0, 2.0, 'b');

    // neck to chest
    addRectToShapes(0.0, 2.0, 0.5, 1.0, 'b');

    // head to neck
    addRectToShapes(0.0, 1.0, 2.0, 2.0, 'b');

    // l upper arm to chest
    addRectToShapes(3.0, 0.0, 2.0, 0.7, 'l');
    // l lower arm to l upper arm
    addRectToShapes(2.0, 0.0, 2.0, 0.7, 'l');
    // l hand to l lower arm
    addRectToShapes(2.0, 0.0, 1.5, 0.7, 'l');

    // r upper arm to chest
    // r lower arm to r upper arm
    // r hand to r lower arm

    // l upper leg to torso
    // l lower leg to l upper leg
    // l foot to l lower leg

    // u upper leg to torso
    // u lower leg to u upper leg
    // u foot to u lower leg

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
void drawRect(float* shapePtr, float* curColors)
{
    float x = *shapePtr;
    float y = *(shapePtr + 1);
    float width = *(shapePtr + 2);
    float height = *(shapePtr + 3);

    glColor3fv(curColors);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void drawTransformed(int i) {
    float x = shapes[i * 4 + 0];
    float y = shapes[i * 4 + 1];
    float width = shapes[i * 4 + 2];
    float height = shapes[i * 4 + 3];

    float anchorOffsetX = translations[i * 2 + 0];
    float anchorOffsetY = translations[i * 2 + 1];

    glTranslatef(x, y, 0.0f);

    glRotatef(rotations[i], 0.0f, 0.0f, 1.0f);
    
    glTranslatef(anchorOffsetX, anchorOffsetY, 0.0f);
    drawRect(shapes + (i * 4), colors + (i * 3));

}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Translate the entire figure down so it's visible on the 20x20 canvas
    glTranslatef(0.0f, -5.0f, 0.0f); // Example: Move entire figure down by 5 units

    int i = 0;

    // --- 1. Torso (Root) ---
    glPushMatrix();
    drawTransformed(i); // i=0: Torso

    // --- 2. Chest (Child of Torso) ---
    glPushMatrix();
    drawTransformed(++i); // i=1: Chest

    // --- 3. Neck and Head Branch (Children of Chest) ---
    glPushMatrix(); // Save after Chest
    drawTransformed(++i); // i=2: Neck
    drawTransformed(++i); // i=3: Head
    glPopMatrix(); // Restore to Chest's final position before Neck/Head

    // --- 4. Left Arm Branch (Children of Chest) ---
    glPushMatrix(); // Save after Chest
    drawTransformed(++i); // i=4: L Upper Arm (connected to chest)
    drawTransformed(++i); // i=5: L Lower Arm (connected to L Upper Arm)
    drawTransformed(++i); // i=6: L Hand (connected to L Lower Arm)
    glPopMatrix(); // Restore to Chest's final position before L Arm

    // --- 5. Right Arm Branch (Siblings of Left Arm) ---
    // NOTE: You haven't initialized these parts in init(), but this is where they would go.
    /*
    glPushMatrix();
        // Add Right Arm parts here (i=7, 8, 9)
    glPopMatrix();
    */

    glPopMatrix(); // Restore to Torso's final position

    // --- 6. Left Leg Branch (Children of Torso) ---
    // NOTE: You haven't initialized these parts in init(), but this is where they would go.
    /*
    glPushMatrix();
        // Add Left Leg parts here
    glPopMatrix();

    // --- 7. Right Leg Branch (Children of Torso) ---
    glPushMatrix();
        // Add Right Leg parts here
    glPopMatrix();
    */

    glPopMatrix(); // Restore to initial global translation (end of figure)

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

    cout << selectedShape << endl;
    if (key == 'a') {
        rotations[selectedShape] += rotationAmount;
    }
    else if (key == 'd') {
        rotations[selectedShape] -= rotationAmount;
    }

    glutPostRedisplay();
}

void specialKeyHandler(int key, int x, int y)
{
    if (key == GLUT_KEY_UP) {
        if (selectedShape + 1 < totalShapes) {
            resetColor(selectedShape);

            selectedShape++;

            selectColor(selectedShape);
        }
    }
    else if (key == GLUT_KEY_DOWN) {
        if (selectedShape - 1 >= 0) {
            resetColor(selectedShape);

            selectedShape--;

            selectColor(selectedShape);
        }
    }
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("2D Transformation Tree");

    // initial color of torso selected
    selectColor(0);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyHandler);
    glutMainLoop();
    return 0;

}
