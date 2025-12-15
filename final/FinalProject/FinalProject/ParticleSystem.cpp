#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ParticleSystem.h"
#include "utils.h"
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
		reset(i);
	}
}

void ParticleSystem::reset(int i)
{
	// initialize lifetime
	//lifeTimes[i] = maxLifeTime - maxLifeTime * i / numParticles;
	lifeTimes[i] = 0.0f;

	// random position on the plane
	positions[i * 3] = randFloat(-xzBound, xzBound);
	positions[i * 3 + 1] = randFloat(0.0f, startY);
	positions[i * 3 + 2] = randFloat(-xzBound, xzBound);

	velocities[i * 3] = randFloat(minSpeedX, maxSpeedX);	  // initial x velocity
	velocities[i * 3 + 1] = randFloat(minSpeedY, maxSpeedY); // initial y velocity
	velocities[i * 3 + 2] = randFloat(minSpeedZ, maxSpeedZ); // initial z velocity

	float rg_color = randFloat(redGreenAmount - colorVariation, redGreenAmount + colorVariation);
	float b_color = randFloat(blueAmount - colorVariation, blueAmount + colorVariation);

	colors[i * 4] = rg_color;		// initial color r
	colors[i * 4 + 1] = rg_color; // initial color g
	colors[i * 4 + 2] = b_color; // initial color b
	colors[i * 4 + 3] = 0.95f; // initial color a
}

void ParticleSystem::update(float deltaTime, float totalTime)
{
	for (int i = 0; i < numParticles; i++)
	{
		// Update lifetime, velocity, position, and color.
		// Reset particle states (positions, velocities, colors, and lifetimes) when the lifetime reaches the maxLifeTime

		lifeTimes[i] += deltaTime;

		// check end of life, reset to original pos if so
		if (positions[i * 3 + 1] <= 0)
		{
			reset(i);
		}

		// update position: rate (velocity x, y or z) times time (deltaTime)
		positions[i * 3] += deltaTime * velocities[i * 3];
		positions[i * 3 + 1] += deltaTime * velocities[i * 3 + 1];
		positions[i * 3 + 2] += deltaTime * velocities[i * 3 + 2];

		// increase y velocity from gravity, keep others the same
		velocities[i * 3 + 1] += deltaTime * acceleration[1];

		// wind effect of sorts
		velocities[i * 3] += cos(totalTime / 2000) * (static_cast<float>(activeParticles) / 100);
		velocities[i * 3 + 2] += sin(totalTime / 2000) * (static_cast<float>(activeParticles) / 100);

		// update color to reflect lifetime
		//colors[i * 4 + 3] = static_cast<float>(activeParticles) / static_cast<float>(numParticles);
		//colors[i * 4 + 3] = clamp(1 - ((startY - positions[i * 3 + 1]) / startY), 0.5f, 1.0f); // a value for rgba

		for (int i = 0; i < activeParticles; i++)
		{
			colors[i * 4 + 3] = 1 - (lifeTimes[i] / maxLifeTime);
		}
		for (int i = activeParticles; i < numParticles; i++)
		{
			colors[i * 4 + 3] = 0.0f;
		}
	}
}

void ParticleSystem::draw()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	for (int i = 0; i < numParticles; i++)
	{
		glColor4fv(colors + (i * 4));
		glVertex3fv(positions + (i * 3));
	}

	glEnd();
	glPopMatrix();
	glPopAttrib();
}

void ParticleSystem::setIntensity(float rms)
{
	float ratio = clamp(rms / rmsMax, 0.0f, 1.0f);
	activeParticles = static_cast<int>(static_cast<float>(numParticles) * ratio);
}
