#ifndef IMGUIHANDLER_H
#define IMGUIHANDLER_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"

class ImguiHandler {
public:
	static void setupImgui(GLFWwindow* window){
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::StyleColorsDark();
	}

	static void newFrameImgui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	static void showWindow(bool showWindow, GLFWwindow* window, PointLight* lights[256], int lightNum, DirectionalLight& dirLight) {
		if (showWindow) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ImGui::Begin("Light window");
			for (uint16_t i = 0; i < lightNum; i++) {
				lights[i]->ImGuiSection();
			}
			ImGui::End();
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	static void destroyImgui() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};

#endif // !IMGUIHANDLER_H
