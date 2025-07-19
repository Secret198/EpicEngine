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
	static void setupImgui(GLFWwindow* window);

	static void newFrameImgui();

	static void showWindow(bool showWindow, GLFWwindow* window, PointLight* lights[256], uint32_t lightNum, DirectionalLight& dirLight, std::vector<Object*>& objects);

	static void destroyImgui();
};

#endif // !IMGUIHANDLER_H
