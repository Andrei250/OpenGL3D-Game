#include "lab_m1/Tema2/components/enemy.h"

using namespace std;
using namespace Tema2ns;

Enemy::Enemy() {
	this->translateX = 0;
	this->translateY = 0;
	this->translateZ = 0;
	this->rotateX = 0;
	this->rotateY = 0;
	this->rotateZ = 0;
	this->initEnemy();
}

Enemy::Enemy(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, int row, int column) {
	this->translateX = translateX;
	this->translateY = translateY;
	this->translateZ = translateZ;
	this->rotateX = rotateX;
	this->rotateY = rotateY;
	this->rotateZ = rotateZ;
	this->toX = translateX;
	this->toZ = translateZ;
	this->row = row;
	this->column = column;
	this->initEnemy();
}

Enemy::~Enemy() {

}

void Enemy::initEnemy() {
	bodyParts.push_back(BodyPart(0.5f, 0.75f, 0.3f, 0, 1.125f, 0, glm::vec3(0.3, 0.2, 0.5))); // main body
	bodyParts.push_back(BodyPart(0.2f, 0.75f, 0.3f, -0.35f, 1.125f, 0.35f, glm::vec3(0.1, 0, 0.9))); // left hand
	bodyParts.push_back(BodyPart(0.2f, 0.75f, 0.3f, 0.15f, 0.375f, 0.15f, glm::vec3(0.7, 0.3, 0.5))); // right foot
	bodyParts.push_back(BodyPart(0.2f, 0.2f, 0.2f, 0, 1.6f, 0, glm::vec3(1, 1, 0))); // head 
}

void Enemy::move() {
	
}

bool Enemy::overLapPoint() {
	return (abs(this->translateX - this->toX) < 0.1) &&
		(abs(this->translateZ - this->toZ) < 0.1);
}
