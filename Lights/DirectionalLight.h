#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"
#include <glm/glm.hpp>
#include "../shaderClass.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class DirectionalLight : public Light {
public:
	glm::vec3 direction;

	DirectionalLight();
		
	DirectionalLight(glm::vec3 direction, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);


	void sendToShader(Shader* shader);

	void ImGuiSection();
};

#endif // !DIRECTIONAL_LIGHT_H
