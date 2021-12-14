#pragma once

#include <iostream>
#include <vector>
#include "lab_m1/Tema2/components/BodyPart.h"
#include "utils/glm_utils.h"

using namespace std;

namespace Tema2ns {
	class Player {
	 public:
		float translateX, translateY, translateZ;
		float rotateX, rotateY, rotateZ;
		vector<BodyPart> bodyParts;
		float velocity = 7;
		float timer = 0;
		float resetTime;
		int health = 4;

		Player();
		Player(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, float resetTime = 0.3);
		~Player();

		void initPlayer();
		bool canShoot();
		void shoot();
	};
}
