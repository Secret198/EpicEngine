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

	SpotLight(uint16_t &lightNum, std::vector<PointLight*> &container, uint32_t &objectNum)
		: PointLight(lightNum, container, objectNum),
		direction(glm::vec3(0.0, -1.0, 0.0)), 
		cutoffAngle(40.0f), 
		outerCutoffAngle(45.0f){}
	SpotLight(uint16_t &lightNum, std::vector<PointLight*>& container, uint32_t &objectNum, glm::vec3 position, glm::vec3 direction, float cutoffAngle, float outerCutoffAngle, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		:PointLight(lightNum, container, objectNum, position, constant, linear, quadratic, ambient, diffuse, specular),
		direction(direction), 
		cutoffAngle(cutoffAngle), 
		outerCutoffAngle(outerCutoffAngle){}

	void ImGuiSection() override {
		char name[50];
		concatStrings(name, "Spot light ", std::to_string(id).c_str(), "");
		ImGui::Separator();
		ImGui::Text(name);
		ImGui::Separator();

		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		concatStrings(name, "x##posSpot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.x, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "y##posSpot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.y, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "z##posSpot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.z, 0.01f);


		ImGui::Text("Direction");
		ImGui::SameLine();

		concatStrings(name, "x##dirSpot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &direction.x, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "y##dirSpot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &direction.y, 0.01f);
		ImGui::SameLine();

		concatStrings(name, "z##dirSpot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &direction.z, 0.01f);


		ImGui::DragFloat("Cut off angle", &cutoffAngle, 0.1f);
		ImGui::DragFloat("Outer cut off angle", &outerCutoffAngle, 0.1f);


		ImGui::PushItemWidth(200.0f);
		Light::concatStrings(name, "Ambient##ambSpot", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&ambient, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::SameLine();

		Light::concatStrings(name, "Diffuse##diffSpot", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&diffuse, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::SameLine();

		Light::concatStrings(name, "Specular##specSpot", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&specular, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::PushItemWidth(100.0f);

		concatStrings(name, "Constant##spot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &constant, 0.01f, 0.0f, 10.0f);
		concatStrings(name, "Linear##spot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &linear, 0.01f, 0.0f, 10.0f);
		concatStrings(name, "Quadratic##spot", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &quadratic, 0.01f, 0.0f, 10.0f);
	}

protected:
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
		shader.set1f(result, glm::cos(glm::radians(cutoffAngle)));

		concatStrings(result, "spotLights[", idStr.c_str(), "].outerCutoffAngle");
		shader.set1f(result, glm::cos(glm::radians(outerCutoffAngle)));

		shader.set1ui("spotLightsNum", lightNum);

	}

};

#endif // !SPOT_LIGHT_H
