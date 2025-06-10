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
	int id;
	int& lightNum;

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

		shader.set1i("pointLightsNum", lightNum);

	}

public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	Model lightIcon;
	
	PointLight(int id, int &lightNum, std::vector<PointLight*>& container) 
		: id(id), 
		position(glm::vec3(0.0)), 
		constant(1.0),
		linear(0.7),
		quadratic(1.8),
		lightIcon("E:/projects/EpicEngine/models/LightCube/LightCube.obj"),
		lightNum(++lightNum)
	{
		container.push_back(this);
	}

	PointLight(int id, int& lightNum, std::vector<PointLight*>& container, glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: Light(ambient, diffuse, specular),
		id(id),
		position(position),
		constant(constant),
		linear(linear),
		quadratic(quadratic),
		lightIcon("E:/projects/EpicEngine/models/LightCube/LightCube.obj"),
		lightNum(++lightNum)
	{
		container.push_back(this);
	}

	void ImGuiSection() {
		char name[50];
		concatStrings(name, "Point light ", std::to_string(id).c_str(), "");
		ImGui::Separator();
		ImGui::Text(name);

		ImGui::Separator();
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		concatStrings(name, "x##pos", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.x, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "y##pos", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.y, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "z##pos", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.z, 0.01f);


		ImGui::Text("Ambient");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		concatStrings(name, "r##amb", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &ambient.r, 0.01f, 0.0f, 100.0f);
		ImGui::SameLine();

		concatStrings(name, "g##amb", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &ambient.g, 0.01f, 0.0f, 100.0f);
		ImGui::SameLine();

		concatStrings(name, "b##amb", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &ambient.b, 0.01f, 0.0f, 100.0f);


		ImGui::Text("Diffuse");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		concatStrings(name, "r##dif", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &diffuse.r, 0.01f, 0.0f, 100.0f);
		ImGui::SameLine();

		concatStrings(name, "g##dif", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &diffuse.g, 0.01f, 0.0f, 100.0f);
		ImGui::SameLine();

		concatStrings(name, "b##dif", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &diffuse.b, 0.01f, 0.0f, 100.0f);

		ImGui::Text("Specular");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		concatStrings(name, "r##spec", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &specular.r, 0.01f, 0.0f, 100.0f);
		ImGui::SameLine();

		concatStrings(name, "g##spec", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &specular.g, 0.01f, 0.0f, 100.0f);
		ImGui::SameLine();

		concatStrings(name, "b##spec", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &specular.b, 0.01f, 0.0f, 100.0f);

		concatStrings(name, "Constant##", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &constant, 0.01f, 0.0f, 10.0f);
		concatStrings(name, "Linear##", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &linear, 0.01f, 0.0f, 10.0f);
		concatStrings(name, "Quadratic##", std::to_string(id).c_str(), "");
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
