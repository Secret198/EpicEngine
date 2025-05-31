#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"
#include <glm/glm.hpp>
#include "../shaders/shaderClass.h"

class DirectionalLight : public Light {
public:
	glm::vec3 direction;

	DirectionalLight() 
		: direction(glm::vec3(0.0, -1.0, 0.0)){}
	DirectionalLight(glm::vec3 direction, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) 
		: Light(amb, diff, spec), 
		direction(direction){}

	void sendToShader(Shader& shader) {
		shader.set3fv("directionalLight.direction", direction);
		shader.set3fv("directionalLight.ambient", ambient);
		shader.set3fv("directionalLight.diffuse", diffuse);
		shader.set3fv("directionalLight.specular", specular);
	}
};

#endif // !DIRECTIONAL_LIGHT_H
