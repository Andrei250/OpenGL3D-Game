#pragma once

#include <iostream>
#include "utils/glm_utils.h"

class Projectile {
public:
	float maximumDistance = 15;
	float velocity = 0.2;
	glm::vec3 color;
	float translateX, translateY, translateZ;
	float centerX, centerY, centerZ;
	float angle;

	Projectile(float angle, float translateX, float translateY, float translateZ, float centerX, float centerY, float centerZ);
	~Projectile();
	bool endLife();
	void updateDistance(float distance);
};
