#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>
#include "PointLight.h"
#include "../shaders/shaderClass.h"
#include <string>

class SpotLight : public PointLight {;
public:
	glm::vec3 direction;
	float cutoffAngle;
	float outerCutoffAngle;

	SpotLight(int id): PointLight(id), direction(glm::vec3(0.0, -1.0, 0.0)), cutoffAngle(40.0f), outerCutoffAngle(45.0f){}
	SpotLight(int id, glm::vec3 position, glm::vec3 direction, float cutoffAngle, float outerCutoffAngle, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		:PointLight(id, position, constant, linear, quadratic, ambient, diffuse, specular), direction(direction), cutoffAngle(cutoffAngle), outerCutoffAngle(outerCutoffAngle){}

	void sendToShader(Shader& shader) override{
		char result[50];

		std::string idStr = std::to_string(id);

		concatStrings(result, "spotLights[", idStr.c_str(), "].position");
		shader.set3fv(result, position);

		concatStrings(result, "spotLights[", idStr.c_str(), "].ambient");
		shader.set3fv(result, ambient);

		concatStrings(result, "spotLights[", idStr.c_str(), "].diffuse");
		shader.set3fv(result, diffuse);

		concatStrings(result, "spotLights[", idStr.c_str(), "].specular");
		shader.set3fv(result, specular);

		concatStrings(result, "spotLights[", idStr.c_str(), "].constant");
		shader.set1f(result, constant);

		concatStrings(result, "spotLights[", idStr.c_str(), "].linear");
		shader.set1f(result, linear);

		concatStrings(result, "spotLights[", idStr.c_str(), "].quadratic");
		shader.set1f(result, quadratic);

		concatStrings(result, "spotLights[", idStr.c_str(), "].direction");
		shader.set3fv(result, direction);

		concatStrings(result, "spotLights[", idStr.c_str(), "].cutoffAngle");
		shader.set1f(result, cutoffAngle);

		concatStrings(result, "spotLights[", idStr.c_str(), "].outerCutoffAngle");
		shader.set1f(result, outerCutoffAngle);
	}

};

#endif // !SPOT_LIGHT_H
