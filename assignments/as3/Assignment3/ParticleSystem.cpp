#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ParticleSystem.h"
#include <iostream>

ParticleSystem::ParticleSystem(int _numParticles)
{
	numParticles = _numParticles; // Set the number of particles

	// Allocate memory for positions, velocities, colors, and lifetimes.
	positions = new float[numParticles * 3];
	velocities = new float[numParticles * 3];
	colors = new float[numParticles * 4];
	lifeTimes = new float[numParticles];

	for (int i = 0; i < numParticles; i++)
	{
		/***************************/
		// Write your code below
		// Please add initializations for other arrays as you see appropriate.

		reset(i);

		/***************************/
	}
}

void ParticleSystem::reset(int i)
{
	// initialize lifetime
	lifeTimes[i] = maxLifeTime - maxLifeTime * i / numParticles;

	positions[i * 3] = positions[i * 3 + 1] = positions[i * 3 + 2] = 0.0f; // x, y, and z are initially 0

	velocities[i * 3] = getRandomValue(minSpeedX, maxSpeedX);	  // initial x velocity
	velocities[i * 3 + 1] = getRandomValue(minSpeedY, maxSpeedY); // initial y velocity
	velocities[i * 3 + 2] = getRandomValue(minSpeedZ, maxSpeedZ); // initial z velocity

	colors[i * 4] = getRandomValue(minRGB, maxRGB);		// initial color r
	colors[i * 4 + 1] = getRandomValue(minRGB, maxRGB); // initial color g
	colors[i * 4 + 2] = getRandomValue(minRGB, maxRGB); // initial color b
}

void ParticleSystem::update(float deltaTime)
{
	for (int i = 0; i < numParticles; i++)
	{
		/***************************/
		// Write your code below
		// Update lifetime, velocity, position, and color.
		// Reset particle states (positions, velocities, colors, and lifetimes) when the lifetime reaches the maxLifeTime

		lifeTimes[i] += deltaTime;

		// check end of life, reset to original pos if so
		if (lifeTimes[i] >= maxLifeTime)
		{
			reset(i);
		}

		// update position: rate (velocity x, y or z) times time (deltaTime)
		positions[i * 3] += deltaTime * velocities[i * 3];
		positions[i * 3 + 1] += deltaTime * velocities[i * 3 + 1];
		positions[i * 3 + 2] += deltaTime * velocities[i * 3 + 2];

		// increase y velocity from gravity, keep others the same
		velocities[i * 3 + 1] += deltaTime * acceleration[1];

		// update color to reflect lifetime
		colors[i * 4 + 3] = 1 - (lifeTimes[i] / maxLifeTime); // a value for rgba

		// Write your code above
		/***************************/
	}
}

void ParticleSystem::draw()
{
	/***************************/
	// Write your code below
	// Use GL_POINTS for rendering
	glPointSize(3.0f);

	glBegin(GL_POINTS);

	for (int i = 0; i < numParticles; i++)
	{
		glColor4fv(colors + (i * 4));
		glVertex3fv(positions + (i * 3));
	}

	glEnd();
	// Write your code above
	/***************************/
}

float ParticleSystem::getRandomValue(float min_value, float max_value)
{
	return min_value + (std::rand()) / (RAND_MAX / (max_value - min_value));
	;
}
