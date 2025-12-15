#pragma once

using namespace std;

class ParticleSystem
{
private:
	// You can modify any values as needed.
	// Define new variables and functions if necessary.

	GLfloat *positions;	 // holds particle positions in the format (x, y, z).
	GLfloat *colors;	 // holds particle colors in the format (r, g, b, a).
	GLfloat *velocities; // contains particles� velocities in the format (x, y, z).
	GLfloat *lifeTimes;	 // represents the current lifetime of every particle.

	int numParticles; // Number of particles in the system
	int activeParticles; // control rain amount
	float rmsMax = 0.2f; // for intensity calculation

	float acceleration[3] = {0.0f, -9.8f, 0.0f}; // Gravity
	float maxLifeTime = 5.0f;					 // The maximum life time the particle could exist

	// speed ranges
	float minSpeedX = -0.5f;
	float maxSpeedX = 0.5f;
	float minSpeedY = -60.0f;
	float maxSpeedY = -20.0f;
	float minSpeedZ = -0.5f;
	float maxSpeedZ = 0.5f;

	// color ranges
	float colorVariation = 0.1f;
	float blueAmount = 0.8f;
	float redGreenAmount = 0.6f;

	// position range
	float xzBound = 20.0f;
	float startY = 50.0f;

public:
	ParticleSystem(int _numParticles);
	void reset(int i);
	void update(float deltaTime, float totalTime);
	void draw();
	void setIntensity(float rms);
	int getNumParticles()
	{
		return numParticles;
	}
};