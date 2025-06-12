#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "../shaders/shaderClass.h"

class Light {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light() : ambient(glm::vec3(1.0)), diffuse(glm::vec3(1.0)), specular(glm::vec3(1.0)) {}
	Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : ambient(amb), diffuse(diff), specular(spec) {}

	virtual void sendToShader(Shader& shader) = 0;

public:
	static void concatStrings(char result[], const char* text1, const char* text2, const char* text3) {
		strcpy(result, text1);
		strcat(result, text2);
		strcat(result, text3);
	}
};

#endif // !LIGHT_H
