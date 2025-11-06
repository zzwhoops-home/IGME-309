#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <vector>

#include "Hunter.h"

using namespace std;
using namespace glm;

class Hunter;
class Monster;

class Bullet
{
public:
	static vector<Bullet*> bullets;
public:
	vec2 position;
	float rotation;
	Hunter* attacker;
	vec3 color;

private:
	float speed;
	float timer;
	float duration = 5.0f;

public:
	bool isActived = true;
public:
	Bullet(vec2 _position, float _rotation, Hunter* _attacker, float _speed, vec3 _color);
	void update(float _deltaTime);
	void draw();
};

