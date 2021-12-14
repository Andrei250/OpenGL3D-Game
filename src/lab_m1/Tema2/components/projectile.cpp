#include "lab_m1/Tema2/components/projectile.h"

Projectile::Projectile(float angle, float translateX, float translateY, float translateZ, float centerX, float centerY, float centerZ) {
	this->angle = angle;
	this->translateX = translateX;
	this->translateY = translateY;
	this->translateZ = translateZ;
	this->centerX = centerX;
	this->centerY = centerY;
	this->centerZ = centerZ;
}

Projectile::~Projectile() {

}

bool Projectile::endLife() {
	return this->maximumDistance <= 0;
}

void Projectile::updateDistance(float distance) {
	this->maximumDistance -= distance;
}
