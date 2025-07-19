#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <map>
#include "camera.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "imGuiHandler.h"
#include "Object.h"

class InputHandler {
private:
	Camera* camera;
	std::map<GLenum, bool> key_states;
	std::map<GLenum, Camera_Movement> key_direction_map;
	float lastMouseX;
	float lastMouseY;
	bool firstMove;
	bool showEditWindow;

public:
	InputHandler(Camera* cam);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void execute_key_action(float deltatime, GLFWwindow* window, std::vector<PointLight*> lights, uint8_t lightsNum, DirectionalLight& dirLight, std::vector<Object*>& objects);

	void mouse_callback(GLFWwindow* window, float xPos, float yPos);

	void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

	void resize_callback(GLFWwindow* window, int newWidth, int newHeight);
};

#endif /*INPUT_HANDLER_H*/