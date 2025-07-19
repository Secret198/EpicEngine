#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "shaderClass.h"
#include "Object.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

class PostProcessing : Object {
private: 
	uint32_t frameBuffer;
	uint32_t colorBuffer;
	uint32_t depthStencilbuffer;

	glm::vec4 backColor;

	int scrWidth;
	int scrHeight;

	uint32_t VBO;
	uint32_t VAO;

	bool& postProcessToggle;
	bool& skyToggle;

	const float quadVertices[30]{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
	};

	void CheckForErrors();

	void setupFrameBuffer();

	void setupMesh();

	void resize();

public:
	PostProcessing(int screenWidth, int screenHeight, glm::vec4 backgroundColor, bool& postProcToggle, bool& skyboxToggle, std::vector<Object*>& objects);


	void bindFrameBuffer();


	void drawScreen(Shader* shader);

	void ImGuiSection();

	void setNewWindowSize(int width, int height);
};

#endif // !POST_PROCESSING_H