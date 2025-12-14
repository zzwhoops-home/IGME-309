#include "utils.h"

vec3 getVecDirection(vec3 end, vec3 start)
{
	return normalize(end - start);
}

int randInt(int min, int max)
{
	float range = static_cast<float>(max) - static_cast<float>(min);
	return static_cast<int>(min + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * range);
}

float randFloat(float min, float max)
{
	float range = max - min;
	return min + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * range;
}

vec3 getRandomVec(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	return vec3(
		randFloat(xMin, xMax),
		randFloat(yMin, yMax),
		randFloat(yMin, zMax)
	);
}
