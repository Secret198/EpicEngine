#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>
#include "PointLight.h"
#include "../shaderClass.h"
#include <string>

class SpotLight : public PointLight {;
public:
	glm::vec3 direction;
	float cutoffAngle;
	float outerCutoffAngle;

	SpotLight(uint16_t& lightNum, std::vector<PointLight*>& container, uint32_t& objectNum);

	SpotLight(uint16_t& lightNum, std::vector<PointLight*>& container, uint32_t& objectNum, glm::vec3 position, glm::vec3 direction, float cutoffAngle, float outerCutoffAngle, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);


	void ImGuiSection() override;

protected:
	void sendToShader(Shader* shader) override;

};

#endif // !SPOT_LIGHT_H
