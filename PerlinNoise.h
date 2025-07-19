#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <glm/glm.hpp>
#include <cstdlib>
#include <math.h>


#define CORNER0 corner
#define CORNER1 glm::ivec3(corner.x + 1, corner.y, corner.z)
#define CORNER2 glm::ivec3(corner.x, corner.y + 1, corner.z)
#define CORNER3 glm::ivec3(corner.x, corner.y, corner.z + 1)
#define CORNER4 glm::ivec3(corner.x + 1, corner.y + 1, corner.z)
#define CORNER5 glm::ivec3(corner.x + 1, corner.y, corner.z + 1)
#define CORNER6 glm::ivec3(corner.x, corner.y + 1, corner.z + 1)
#define CORNER7 glm::ivec3(corner.x + 1, corner.y + 1, corner.z + 1)

#define V0 glm::dot(randomVectors[getVectorIndex(CORNER0)], getCornerToPointVec(CORNER0, point))
#define V1 glm::dot(randomVectors[getVectorIndex(CORNER1)], getCornerToPointVec(CORNER1, point))
#define V2 glm::dot(randomVectors[getVectorIndex(CORNER2)], getCornerToPointVec(CORNER2, point))
#define V3 glm::dot(randomVectors[getVectorIndex(CORNER3)], getCornerToPointVec(CORNER3, point))
#define V4 glm::dot(randomVectors[getVectorIndex(CORNER4)], getCornerToPointVec(CORNER4, point))
#define V5 glm::dot(randomVectors[getVectorIndex(CORNER5)], getCornerToPointVec(CORNER5, point))
#define V6 glm::dot(randomVectors[getVectorIndex(CORNER6)], getCornerToPointVec(CORNER6, point))
#define V7 glm::dot(randomVectors[getVectorIndex(CORNER7)], getCornerToPointVec(CORNER7, point))



//0, 1				<-- Corners that make up and edge
//3, 5  
//2, 4
//6, 7



class PerlinNoise {
	static const int size = 64;
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

	void generateRandomVectors();

	glm::vec3 getCornerToPointVec(glm::ivec3 corner, glm::vec3 point);

	int getVectorIndex(glm::ivec3 corner);

	float lerp(float v1, float v2, float pointCoord);

public:
	PerlinNoise();

	float getValue(glm::vec3 point, float scale);



	~PerlinNoise();
};

#endif // !PERLIN_NOISE_H
