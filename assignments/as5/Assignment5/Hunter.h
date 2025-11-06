#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <vector>

#include "Monster.h";

using namespace std;
using namespace glm;

class Monster;
class Bullet;

class Hunter
{
public:
	vec2 position;
	float rotation = 0;

	int ID;
	bool isActived = true;
	string playerName = "";

	int damage = 0;
	int score = 0;

public:
	virtual void update(float _deltaTime, const vector<Monster*> _monsters, const vector<Hunter*> _players) = 0;
	virtual void collisionDetection(vector<Monster*> _monsters) = 0;
	virtual void draw() = 0;
	virtual void getDamagePoint(int _damage) = 0;
	virtual void getScore(int _score) = 0;
};

