#pragma once

#include <iostream>
#include "utils/glm_utils.h"

class BodyPart {
 public:
	 float scaleX, scaleY, scaleZ;
	 float centerX, centerY, centerZ;
	 float rotateX = 0, rotateY = 0, rotateZ = 0;
	 float isNeg;
	 glm::vec3 color;

	 BodyPart(float scaleX, float scaleY, float scaleZ, float centerX, float centerY, float centerZ, glm::vec3 color);
	 ~BodyPart();
};
