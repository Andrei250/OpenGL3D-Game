#include "lab_m1/Tema2/components/BodyPart.h"

BodyPart::BodyPart(float scaleX, float scaleY, float scaleZ, float centerX, float centerY, float centerZ, glm::vec3 color) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->scaleZ = scaleZ;
	this->centerX = centerX;
	this->centerY = centerY;
	this->centerZ = centerZ;
	this->color = color;

	if (centerX < 0) {
		isNeg = -1;
	}
	else {
		isNeg = 1;
	}
}

BodyPart::~BodyPart() {

}
