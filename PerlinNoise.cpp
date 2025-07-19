#include "PerlinNoise.h"

void PerlinNoise::generateRandomVectors() {
	for (int i = 0; i < pow(size, 3); i++) {
		randomVectors[i] = possibleVectors[rand() % 26];
	}
}

glm::vec3 PerlinNoise::getCornerToPointVec(glm::ivec3 corner, glm::vec3 point) {
	return glm::vec3(point.x - (float)corner.x, point.y - (float)corner.y, point.z - (float)corner.z);
}

int PerlinNoise::getVectorIndex(glm::ivec3 corner) {
	return glm::abs(corner.z) * size * size + glm::abs(corner.y) * size + glm::abs(corner.x);
}

float PerlinNoise::lerp(float v1, float v2, float pointCoord) {
	float intPointCoord = (int)pointCoord;
	return v1 + (pointCoord - intPointCoord) * (v2 - v1);
}

PerlinNoise::PerlinNoise() {
	generateRandomVectors();
}

float PerlinNoise::getValue(glm::vec3 point, float scale) {
	point *= scale;
	glm::ivec3 corner;
	corner.x = (int)point.x;
	corner.y = (int)point.y;
	corner.z = (int)point.z;

	return lerp(lerp(lerp(V0, V1, point.x), lerp(V3, V5, point.x), point.z), lerp(lerp(V2, V4, point.x), lerp(V6, V7, point.x), point.z), point.y);
}

PerlinNoise::~PerlinNoise() {
	delete randomVectors;
	delete possibleVectors;
}