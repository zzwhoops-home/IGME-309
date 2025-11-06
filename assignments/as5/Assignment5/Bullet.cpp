#include "Bullet.h"

Bullet::Bullet(vec2 _position, float _rotation, Hunter* _attacker, float _speed, vec3 _color)
{
	this->position = _position;
	this->rotation = _rotation;
	this->attacker = _attacker;
	this->color = _color;
	this->speed = _speed;

	timer = duration;
}

void Bullet::update(float _deltaTime)
{
	if (isActived) {
		timer -= _deltaTime;
		if (timer < 0.0f)
			isActived = false;

		position.x += speed * _deltaTime * cos(glm::radians(rotation));
		position.y += speed * _deltaTime * sin(glm::radians(rotation));
	}
}

void Bullet::draw()
{
	if (isActived) {
		glPushMatrix();
		glPointSize(5.0f);
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_POINTS);
		glVertex2f(position.x, position.y);
		glEnd();
		glPopMatrix();
	}
}

vector<Bullet*> Bullet::bullets = {};
