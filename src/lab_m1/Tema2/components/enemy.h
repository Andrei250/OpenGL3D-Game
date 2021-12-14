#pragma once

#include <iostream>
#include <vector>
#include "lab_m1/Tema2/components/BodyPart.h"
#include "utils/glm_utils.h"
#include <math.h>

using namespace std;

namespace Tema2ns {
	class Enemy {
	public:
		float translateX, translateY, translateZ;
		float rotateX, rotateY, rotateZ;
		vector<BodyPart> bodyParts;
		float toX = 0, toZ = 0;
		int row, column;
		float velocity = 10;

		Enemy();
		Enemy(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, int row, int column);
		~Enemy();

		void initEnemy();
		void move();
		bool overLapPoint();
	};
}
