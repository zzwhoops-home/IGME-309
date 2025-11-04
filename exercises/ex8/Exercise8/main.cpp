#define _USE_MATH_DEFINES

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <ctime>
#include <cmath>

using namespace std;

float canvasSize[] = { 20.0f, 20.0f };
int rasterSize[] = { 600, 600 };

// tracking the game time - millisecond 
unsigned int curTime = 0;
unsigned int preTime = 0;
float deltaTime = 0;

float rotationAngle = 45.0f;
float rotateSpeed = 0.0f;

float anchorPoint[2] = {canvasSize[0] / 2, 16.5};
float pendulumDistance = 12.0f;

void drawCircle(float cx, float cy, float r, int num_segments) {
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < num_segments; i++) {
		float theta = 2.0f * M_PI * ((float)i / (float)num_segments);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}

void display(void)
{
	glClearColor(0.9f, 0.9f, 0.7f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*****************************/
	// write your code below

	// draw anchor point
	glColor3f(0, 0, 0);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
		glVertex2fv(anchorPoint);
	glEnd();
	glPointSize(1.0f);

	// move to origin
	glPushMatrix();

	// translate pivot to origin, rotate
	glTranslatef(anchorPoint[0], anchorPoint[1], 0);
	glRotatef(rotationAngle, 0, 0, 1);
	
	// move along y axis
	glColor3f(1, 1, 1);
	drawCircle(0.0f, -pendulumDistance, 0.75f, 100);

	glColor3f(0, 0, 0);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, -pendulumDistance);
	glEnd();
	glLineWidth(1.0f);

	glPopMatrix();

	// write your code above
	/*****************************/

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	rasterSize[0] = w;
	rasterSize[1] = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, canvasSize[0], 0, canvasSize[1]);
	glViewport(0, 0, rasterSize[0], rasterSize[1]);

	glutPostRedisplay();
}


void update() {
	curTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (float)(curTime - preTime) / 1000.0f;

	// angular acceleration
	float radians = rotationAngle * (M_PI / 180.0f);
	float angularAcceleration = ((-9.81f / pendulumDistance) * sin(radians)) * (180.0f / M_PI);
	
	rotateSpeed += angularAcceleration * deltaTime;

	//if (rotationAngle > 45.0f) {
	//	rotateSpeed = -abs(rotateSpeed);
	//}
	//else if (rotationAngle < -45.0f) {
	//	rotateSpeed = abs(rotateSpeed);
	//}
	rotationAngle += deltaTime * rotateSpeed;

	preTime = curTime;
	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(rasterSize[0], rasterSize[1]);
	glutCreateWindow("Simple Pendulum");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(update);

	preTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
	return 0;
}