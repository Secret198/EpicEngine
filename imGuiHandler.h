#ifndef IMGUIHANDLER_H
#define IMGUIHANDLER_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Object.h"

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

	static void showWindow(bool showWindow, GLFWwindow* window, PointLight* lights[256], uint32_t lightNum, DirectionalLight& dirLight, std::vector<Object*> &objects) {
		if (showWindow) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//ImGui::ShowDemoWindow();
			ImGui::Begin("Object window");
			int objectNum = objects.size();
			for (uint32_t i = 0u; i < objectNum; i++) {
				objects[i]->ImGuiSection();
			}
			ImGui::End();

			ImGui::Begin("Light window");
			dirLight.ImGuiSection();
			for (uint16_t i = 0u; i < lightNum; i++) {
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
