#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>
#include "Light.h"
#include "../shaders/shaderClass.h"
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class PointLight : public Light {
protected:
	uint16_t id;
	uint16_t& lightNum;

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

		shader.set1ui("pointLightsNum", lightNum);

	}

public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	Model lightIcon;
	
	PointLight(uint16_t &lightNum, std::vector<PointLight*>& container, uint32_t &objectNum) 
		: Light(),
		position(glm::vec3(0.0)), 
		constant(1.0),
		linear(0.7),
		quadratic(1.8),
		lightIcon("E:/projects/EpicEngine/models/LightCube/LightCube.obj", objectNum),
		id(lightNum),
		lightNum(++lightNum)
	{
		container.push_back(this);
	}

	PointLight(uint16_t& lightNum, std::vector<PointLight*>& container, uint32_t &objectNum, glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: Light(ambient, diffuse, specular),
		position(position),
		constant(constant),
		linear(linear),
		quadratic(quadratic),
		lightIcon("E:/projects/EpicEngine/models/LightCube/LightCube.obj", objectNum),
		id(lightNum),
		lightNum(++lightNum)
	{
		container.push_back(this);
	}

	virtual void ImGuiSection() {
		char name[50];
		concatStrings(name, "Point light ", std::to_string(id).c_str(), "");
		ImGui::Separator();
		ImGui::Text(name);

		ImGui::Separator();
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		concatStrings(name, "x##posPoint", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.x, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "y##posPoint", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.y, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "z##posPoint", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.z, 0.01f);


		ImGui::PushItemWidth(200.0f);
		Light::concatStrings(name, "Ambient##ambPoint", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&ambient, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::SameLine();

		Light::concatStrings(name, "Diffuse##diffPoint", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&diffuse, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::SameLine();

		Light::concatStrings(name, "Specular##specPoint", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&specular, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::PushItemWidth(100.0f);
		concatStrings(name, "Constant##point", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &constant, 0.01f, 0.0f, 10.0f);
		concatStrings(name, "Linear##point", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &linear, 0.01f, 0.0f, 10.0f);
		concatStrings(name, "Quadratic##point", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &quadratic, 0.01f, 0.0f, 10.0f);
	}


	void Draw(Shader& lightShader, Shader& iconShader) {
		lightIcon.position = this->position;
		lightIcon.scale = glm::vec3(0.1, 0.1, 0.1);
		sendToShader(lightShader);
		lightIcon.Draw(iconShader, false, GL_TRIANGLES);
	}
};
#endif // !POINT_LIGHT_H
