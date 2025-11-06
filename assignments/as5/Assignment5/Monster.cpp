#include "Monster.h"

void Monster::update(float _deltaTime, vector<Hunter*> hunters)
{
	if (isActived) {
		// chasing the closest player
		float minDist = 1000.0f;
		if (target != nullptr && target->isActived == true) {
			minDist = distance(position, target->position);
		}

		for (int i = 0; i < hunters.size(); i++) {
			if (hunters[i]->isActived == false)
				continue;
			float dist = glm::distance(position, hunters[i]->position);
			if (dist < minDist - 10.0f) {
				minDist = dist;
				target = hunters[i];
			}
		}

		if (target != nullptr && target->isActived == true) {
			targetPosition = target->position;
		}
		
		this->position += normalize(targetPosition - this->position) * speed * _deltaTime;

		// rotate animation
		this->rotation += rotateSpeed * _deltaTime;
		if (this->rotation > 15.0f)
			rotateSpeed = -abs(rotateSpeed);
		if (this->rotation < -15.0f)
			rotateSpeed = abs(rotateSpeed);
	}
}

void Monster::collisionDetection(vector<Bullet*> bullets)
{
	if (isActived) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]->isActived == false)
				continue;
			if (distance(bullets[i]->position, position) < radius) {
				bullets[i]->isActived = false;
				life--;
				bullets[i]->attacker->getDamagePoint(1);
				if (life <= 0) {
					bullets[i]->attacker->getScore(1);
					isActived = false;
				}
			}
		}
	}
}

void Monster::draw()
{
	if (isActived) {
		glPushMatrix();
		glTranslatef(position.x, position.y, 0.0f);
		drawLifeBar();

		glRotatef(rotation, 0, 0, 1);
		
		// body
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 30; i++) {
			float angle = 2 * pi<float>() * i / 30;
			glVertex2f(radius * cos(angle), radius * sin(angle));
		}
		glEnd();

		// left eye
		glPushMatrix();
		glTranslatef(-0.5f * radius, 0.5f * radius, 0);
		drawEye();
		glPopMatrix();

		// right eye
		glPushMatrix();
		glTranslatef(0.5f * radius, 0.5f * radius, 0);
		drawEye();
		glPopMatrix();

		// mouth
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0);
		drawMouth();
		glPopMatrix();

		// legs
		drawLeg();

		glPopMatrix();
	}
}

void Monster::drawEye()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 10; i++) {
		float angle = 2 * pi<float>() * i / 10;
		glVertex2f(0.2f * radius * cos(angle), 0.2f * radius * sin(angle));
	}
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 10; i++) {
		float angle = 2 * pi<float>() * i / 10;
		glVertex2f(0.1f * radius * cos(angle), 0.1f * radius * sin(angle));
	}
	glEnd();
}

void Monster::attackPlayer(int _playerID)
{
	this->isActived = false;
}

void Monster::drawMouth()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(-0.5f * radius, -0.5f * radius);
	glVertex2f(0.5f * radius, -0.5f * radius);
	glVertex2f(0.5f * radius, -0.1f * radius);
	glVertex2f(-0.5f * radius, -0.1f * radius);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	int touthNum = 4;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < touthNum; i++) {
		glVertex2f((-0.5f + 1.0f * i / touthNum) * radius, -0.1f * radius);
		glVertex2f((-0.5f + (i + 0.5f) / touthNum) * radius, -0.5f * radius);
		glVertex2f((-0.5f + (i + 1.0f) / touthNum) * radius, -0.1f * radius);
	}
	glEnd();
}

void Monster::drawLeg()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(radius * 0.5f);

	for (int i = 0; i < 3; i++) {
		vec2 p1 = radius * vec2(cos(glm::radians(150.0f + 30 * i)), sin(glm::radians(150.0f + 30 * i)));
		vec2 p2 = p1 + 0.5f * radius * vec2(-1.0f, 1.0f);
		vec2 p3 = p2 + 0.75f * radius * vec2(-1.0f, -1.0f);
		glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
		glEnd();
	}

	for (int i = 0; i < 3; i++) {
		vec2 p1 = radius * vec2(cos(glm::radians(-30.0f + 30 * i)), sin(glm::radians(-30.0f + 30 * i)));
		vec2 p2 = p1 + 0.5f * radius * vec2(1.0f, 1.0f);
		vec2 p3 = p2 + 0.75f * radius * vec2(1.0f, -1.0f);
		glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
		glEnd();
	}
}

void Monster::drawLifeBar()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(-1.0f * radius, 1.2f * radius);
	glVertex2f((-1.0f + 2.0f * life / maxLife) * radius, 1.2f * radius);
	glVertex2f((-1.0f + 2.0f * life / maxLife) * radius, 1.5f * radius);
	glVertex2f(-1.0f * radius, 1.5f * radius);
	glEnd();
}

int Monster::g_difficultyLevel = 3;
