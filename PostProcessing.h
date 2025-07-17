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

	const float quadVertices[30]{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
	};

	void CheckForErrors() {
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Error: Framebuffer is not complete!" << std::endl;
		}
	}

	void setupFrameBuffer() {
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scrWidth, scrHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &depthStencilbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthStencilbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scrWidth, scrHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		CheckForErrors();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

		glBindVertexArray(0);
	}

	void resize() {
		std::cout << scrWidth << std::endl;
		//glBindTexture(GL_TEXTURE_2D, colorBuffer);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scrWidth, scrHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		glViewport(0, 0, scrWidth, scrHeight);

	}

public:
	PostProcessing(int screenWidth, int screenHeight, glm::vec4 backgroundColor, bool &postProcToggle, std::vector<Object*>& objects):
		scrWidth(screenWidth),
		scrHeight(screenHeight),
		backColor(backgroundColor),
		postProcessToggle(postProcToggle)
	{
		setupFrameBuffer();
		setupMesh();
		objects.push_back(this);
	}

	void bindFrameBuffer() {

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		resize();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, backColor.g, backColor.b, backColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void drawScreen(Shader* shader) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void ImGuiSection() {
		ImGui::Checkbox("Post processing", &postProcessToggle);
	}

	void setNewWindowSize(int width, int height) {
		scrWidth = width;
		scrHeight = height;
	}
};

#endif // !POST_PROCESSING_H