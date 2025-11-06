#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <ctime>
#include <cmath>

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <random>

#include "Hunter.h"
#include "MyHunter.h"
#include "Monster.h"
#include "Bullet.h"

using namespace glm;
using namespace std;

float canvasSize[] = { 900.0f, 650.0f };
int rasterSize[] = { 1800, 1300 };

float radius = 200.0f;

// tracking the game time - millisecond 
unsigned int curTime = 0;
unsigned int preTime = 0;
float deltaTime = 0;
float countDownTime = 60.0f;

vector<Hunter*> hunters;
int hunterNum = 30;			// default value is 30, will be loaded from setting.txt
int monsterHealthPoint = 3;	// default value is 3, will be loaded from setting.txt

vector<Monster*> monsters;
float monsterSpawnTimer;
float minSpawnTime = 0.1f;
float maxSpawnTime = 0.2f;

float getRandomValue(float min_value, float max_value)
{
	return min_value + (std::rand()) / (RAND_MAX / (max_value - min_value));;
}

bool compareHunters(const Hunter *a, const Hunter *b) {
	if (a->score > b->score)
		return true;
	else if (a->score == b->score && a->damage > b->damage)
		return true;
	else
		return false;
}

void init(void)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	ifstream file("setting.txt");
	if (file.is_open()) {
		std::string line;
		if (getline(file, line)) {
			file >> hunterNum;
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		if (getline(file, line)) {
			file >> monsterHealthPoint;
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			Monster::g_difficultyLevel = monsterHealthPoint;
		}
		if (getline(file, line)) {
			file >> countDownTime;
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		file.close(); // Close the file
	}
	else {
		std::cout << "Unable to open setting file" << std::endl;
	}

	for (int i = 0; i < hunterNum; i++) {
		float angle = 2.0f * glm::pi<float>() * i / hunterNum;
		Hunter* player = new MyHunter(vec2(radius * cos(angle), radius * sin(angle)), i);
		hunters.push_back(player);
	}
	monsterSpawnTimer = getRandomValue(minSpawnTime, maxSpawnTime);

	minSpawnTime *= 30.0f / hunterNum;
	maxSpawnTime *= 30.0f / hunterNum;
}

void drawText(const std::string& text, float x, float y) {
	glRasterPos2f(x, y);

	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c); 
	}
}

void drawSpiderWeb() {
	const int numRadialLines = 12; // Number of radial lines
	const float centerX = 0.0f, centerY = 0.0f; // Center of the web
	const float radius = 50.0f; // Radius of the web
	const int numCircles = 6; // Number of concentric circles

	glColor3f(1.0f, 1.0f, 1.0f);
	// Draw radial lines
	glBegin(GL_LINES);
	for (int i = 0; i < numRadialLines; ++i) {
		float angle = 2.0f * glm::pi<float>() * i / numRadialLines;
		glVertex2f(centerX, centerY);
		glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
	}
	glEnd();

	// Draw concentric circles (approximated by polygons)
	for (int j = 1; j <= numCircles; ++j) {
		float r = radius * j / numCircles;
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < numRadialLines; ++i) {
			float angle = 2.0f * glm::pi<float>() * i / numRadialLines;
			glVertex2f(centerX + r * cos(angle), centerY + r * sin(angle));
		}
		glEnd();
	}
}

void display(void)
{
	glClearColor(0.45f, 0.45f, 0.35f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw ground
	glColor3f(0.9f, 0.9f, 0.7f);
	glBegin(GL_POLYGON);
	glVertex2f(-300, -300);
	glVertex2f(300, -300);
	glVertex2f(300, 300);
	glVertex2f(-300, 300);
	glEnd();

	// drawSpiderWeb
	drawSpiderWeb();

	glPushMatrix();
	
	// draw hunters
	for (int i = 0; i < hunters.size(); i++) {
		hunters[i]->draw();
		drawText(hunters[i]->playerName, hunters[i]->position.x - 10, hunters[i]->position.y + 20);
	}

	// draw monsters
	for (int i = 0; i < monsters.size(); i++) {
		monsters[i]->draw();
	}

	// draw bullets
	for (int i = 0; i < Bullet::bullets.size(); i++) {
		Bullet::bullets[i]->draw();
	}

	glPopMatrix();

	// draw count down timer
	glColor3f(1.0f, 1.0f, 1.0f);
	string countDownStr = to_string(static_cast<int>(ceil(countDownTime)));
	drawText(countDownStr, 350, 250);

	// draw player status
	sort(hunters.begin(), hunters.end(), compareHunters);

	drawText("KILL", 430, 225);
	drawText("Dealing Damage", 480, 225);
	for (int i = 0; i < hunters.size(); i++) {
		if (hunters[i]->isActived)
			glColor3f(1.0f, 1.0f, 1.0f);
		else
			glColor3f(0.75f, 0.75f, 0.75f);

		string playerStr = hunters[i]->playerName + ":";
		drawText(playerStr, 320, 210 - 10 * i);
		string scoreStr = to_string(hunters[i]->score);
		drawText(scoreStr, 430, 210 - 10 * i);
		string dpsStr = to_string(hunters[i]->damage);
		drawText(dpsStr, 480, 210 - 10 * i);
	}

	
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	rasterSize[0] = w;
	rasterSize[1] = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-350, 550, -canvasSize[1] / 2.0f, canvasSize[1] / 2.0f);
	glViewport(0, 0, rasterSize[0], rasterSize[1]);

	glutPostRedisplay();
}

void update() {
	curTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (float)(curTime - preTime) / 1000.0f;

	// countDown
	if (countDownTime <= 0.0f)
		return;
	countDownTime -= deltaTime;

	// generate new monsters
	monsterSpawnTimer -= deltaTime;
	while (monsterSpawnTimer <= 0.0f) {
		monsterSpawnTimer += getRandomValue(minSpawnTime, maxSpawnTime);
		monsters.push_back(new Monster());
	}

	// erase died monsters
	monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [](Monster* monster) {
		return monster->isActived == false;
		}), monsters.end());

	// erase died bullets
	Bullet::bullets.erase(std::remove_if(Bullet::bullets.begin(), Bullet::bullets.end(), [](Bullet* bullet) {
		return bullet->isActived == false;
		}), Bullet::bullets.end());

	// update
	for (int i = 0; i < hunters.size(); i++)
		hunters[i]->update(deltaTime, monsters, hunters);
	for (int i = 0; i < monsters.size(); i++)
		monsters[i]->update(deltaTime, hunters);
	for (int i = 0; i < Bullet::bullets.size(); i++)
		Bullet::bullets[i]->update(deltaTime);

	// collision detection
	for (int i = 0; i < hunters.size(); i++)
		hunters[i]->collisionDetection(monsters);
	for (int i = 0; i < monsters.size(); i++)
		monsters[i]->collisionDetection(Bullet::bullets);

	preTime = curTime;
	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(rasterSize[0], rasterSize[1]);
	glutCreateWindow("Hunter vs Monster");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(update);

	preTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
	return 0;
}