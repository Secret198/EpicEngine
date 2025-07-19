#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	: Light(),
	direction(glm::vec3(0.0, -0.9, 0.0))
{}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
	: Light(amb, diff, spec),
	direction(direction) {
}

void DirectionalLight::sendToShader(Shader* shader)
{
	shader->set3fv("directionalLight.direction", direction);
	shader->set3fv("directionalLight.ambient", ambient);
	shader->set3fv("directionalLight.diffuse", diffuse);
	shader->set3fv("directionalLight.specular", specular);
}

void DirectionalLight::ImGuiSection() {

	ImGui::Text("Directional light");
	ImGui::Separator();
	ImGui::Text("Direction");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.0f);
	ImGui::DragFloat("x##dirDir", &direction.x, 0.1f);
	ImGui::SameLine();
	ImGui::DragFloat("y##dirDir", &direction.y, 0.1f);
	ImGui::SameLine();
	ImGui::DragFloat("z##dirDir", &direction.z, 0.1f);

	ImGui::PushItemWidth(200.0f);
	ImGui::ColorPicker3("Ambient##ambDir", (float*)&ambient, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

	ImGui::SameLine();

	ImGui::ColorPicker3("Ambient##diffDir", (float*)&diffuse, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

	ImGui::SameLine();

	ImGui::ColorPicker3("Ambient##specDir", (float*)&specular, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

	ImGui::PushItemWidth(100.0f);

	ImGui::Separator();
}
