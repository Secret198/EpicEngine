#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>
#include "Light.h"
#include "../shaders/shaderClass.h"
#include <string>

class PointLight : public Light {
protected:
	int id;

	virtual void sendToShader(Shader& shader) {
		char result[50];
		shader.use();
		std::string idStr = std::to_string(id);

		concatStrings(result, "pointLights[", idStr.c_str(), "].position");
		shader.set3fv(result, position);

		concatStrings(result, "pointLights[", idStr.c_str(), "].ambient");
		shader.set3fv(result, ambient);

		concatStrings(result, "pointLights[", idStr.c_str(), "].diffuse");
		shader.set3fv(result, diffuse);

		concatStrings(result, "pointLights[", idStr.c_str(), "].specular");
		shader.set3fv(result, specular);

		concatStrings(result, "pointLights[", idStr.c_str(), "].constant");
		shader.set1f(result, constant);

		concatStrings(result, "pointLights[", idStr.c_str(), "].linear");
		shader.set1f(result, linear);

		concatStrings(result, "pointLights[", idStr.c_str(), "].quadratic");
		shader.set1f(result, quadratic);
	}

public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	Model lightIcon;
	
	PointLight(int id) 
		: id(id), 
		position(glm::vec3(0.0)), 
		constant(0.1),
		linear(0.1),
		quadratic(0.1),
		lightIcon("E:/projects/EpicEngine/models/LightCube/LightCube.obj"){}

	PointLight(int id, glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: Light(ambient, diffuse, specular),
		id(id),
		position(position),
		constant(constant),
		linear(linear),
		quadratic(quadratic),
		lightIcon("E:/projects/EpicEngine/models/LightCube/LightCube.obj") {}



	void Draw(Shader& lightShader, Shader& iconShader) {
		lightIcon.position = this->position;
		lightIcon.scale = glm::vec3(0.1, 0.1, 0.1);
		sendToShader(lightShader);
		lightIcon.Draw(iconShader, false, GL_TRIANGLES);
	}
};
#endif // !POINT_LIGHT_H
