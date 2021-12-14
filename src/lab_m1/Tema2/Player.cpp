#include "lab_m1/Tema2/Player.h"

using namespace std;
using namespace Tema2ns;

Player::Player() {
	this->translateX = 0;
	this->translateY = 0;
	this->translateZ = 0;
	this->rotateX = 0;
	this->rotateY = 0;
	this->rotateZ = 0;
	this->initPlayer();
}

Player::Player(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, float resetTime) {
	this->translateX = translateX;
	this->translateY = translateY;
	this->translateZ = translateZ;
	this->rotateX = rotateX;
	this->rotateY = rotateY;
	this->rotateZ = rotateZ;
	this->resetTime = resetTime;
	this->initPlayer();
}

Player::~Player() {

}

void Player::initPlayer() {
	bodyParts.push_back(BodyPart(0.5f, 0.75f, 0.3f, 0, 1.125f, 0, glm::vec3(0.6, 1, 0.5))); // main body
	bodyParts.push_back(BodyPart(0.2f, 0.75f, 0.3f, -0.35f, 1.125f, 0.35f, glm::vec3(0, 0, 0.3))); // left hand
	bodyParts.push_back(BodyPart(0.2f, 0.75f, 0.3f, 0.35f, 1.125f, 0.35f, glm::vec3(0, 0, 0.3))); // right hand
	bodyParts.push_back(BodyPart(0.2f, 0.75f, 0.3f, -0.15f, 0.375f, 0.15f, glm::vec3(0.6, 1, 0.5))); // left foot
	bodyParts.push_back(BodyPart(0.2f, 0.75f, 0.3f, 0.15f, 0.375f, 0.15f, glm::vec3(0.6, 1, 0.5))); // right foot
	bodyParts.push_back(BodyPart(0.2f, 0.2f, 0.2f, 0, 1.6f, 0, glm::vec3(0, 0, 0))); // head 
}

bool Player::canShoot() {
	return this->timer <= 0;
}

void Player::shoot() {
	this->timer = resetTime;
}
