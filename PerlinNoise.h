#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <glm/glm.hpp>
#include <cstdlib>
#include <math.h>

#define IND corner.z * size * size + corner.y * size + corner.x
#define CORNER_TO_POINT_VEC glm::vec3(point.x - (float) corner.x, point.y - (float)corner.y, point.z - (float)corner.z)

class PerlinNoise {
	static const int size = 256;
	glm::vec3* randomVectors = new glm::vec3[size * size * size];	//x : y : z
	const glm::vec3* possibleVectors = new glm::vec3[26] {
		glm::vec3(0.0, 1.0, 0.0),
		glm::vec3(0.0, -1.0, 0.0),
		glm::vec3(1.0, 0.0, 0.0),
		glm::vec3(-1.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 1.0),
		glm::vec3(0.0, 0.0, -1.0),
		glm::vec3(1.0, 1.0, 0.0),
		glm::vec3(1.0, -1.0, 0.0),
		glm::vec3(-1.0, 1.0, 0.0),
		glm::vec3(-1.0, -1.0, 0.0),
		glm::vec3(0.0, 1.0, 1.0),
		glm::vec3(0.0, 1.0, -1.0),
		glm::vec3(0.0, -1.0, 1.0),
		glm::vec3(0.0, -1.0, -1.0),
		glm::vec3(1.0, 0.0, 1.0),
		glm::vec3(1.0, 0.0, -1.0),
		glm::vec3(-1.0, 0.0, 1.0),
		glm::vec3(-1.0, 0.0, -1.0),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(-1.0, 1.0, 1.0),
		glm::vec3(1.0, -1.0, 1.0),
		glm::vec3(1.0, 1.0, -1.0),
		glm::vec3(-1.0, -1.0, 1.0),
		glm::vec3(-1.0, 1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(-1.0, -1.0, -1.0),

	};
	float scale;

	void generateRandomVectors() {
		for (int i = 0; i < pow(size, 3); i++) {
			randomVectors[i] = possibleVectors[rand() % 36];
		}
	}

public:
	PerlinNoise(float scale) {
		scale = scale;
		generateRandomVectors();
	}

	void getValue(glm::vec3 point) {
		glm::ivec3 corner;
		corner.x = (int)point.x;
		corner.y = (int)point.y;
		corner.z = (int)point.z;

		float values[8];
		values[0] = glm::dot(possibleVectors[IND], CORNER_TO_POINT_VEC);


	}

	float trilinearInterpolate() {
		



	}

	~PerlinNoise() {
		delete randomVectors;
		delete possibleVectors;
	}
};

#endif // !PERLIN_NOISE_H
