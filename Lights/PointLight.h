#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>
#include "Light.h"
#include "../shaderClass.h"
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../model.h"

class PointLight : public Light {
protected:
	uint16_t id;
	uint16_t& lightNum;

	virtual void sendToShader(Shader* shader);

public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	Model lightIcon;
	
	PointLight(uint16_t& lightNum, std::vector<PointLight*>& container, uint32_t& objectNum);


	PointLight(uint16_t& lightNum, std::vector<PointLight*>& container, uint32_t& objectNum, glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);


	virtual void ImGuiSection();


	void Draw(Shader* lightShader, Shader* iconShader);
};
#endif // !POINT_LIGHT_H
