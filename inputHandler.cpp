#include "inputHandler.h"

InputHandler::InputHandler(Camera* cam)
	: camera(cam) {
	key_states = {
		{GLFW_KEY_W, false},
		{GLFW_KEY_A, false},
		{GLFW_KEY_S, false},
		{GLFW_KEY_D, false},
		{GLFW_KEY_SPACE, false},
		{GLFW_KEY_LEFT_SHIFT, false},
		{GLFW_KEY_G, false},
		{GLFW_KEY_ESCAPE, false}
	};
	key_direction_map = {
		{GLFW_KEY_W, FORWARD},
		{GLFW_KEY_A, LEFT},
		{GLFW_KEY_S, BACKWARD},
		{GLFW_KEY_D, RIGHT},
		{GLFW_KEY_SPACE, UP},
		{GLFW_KEY_LEFT_SHIFT, DOWN},
	};
	lastMouseX = 0;
	lastMouseY = 0;
	firstMove = true;
	showEditWindow = false;
}

void InputHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key_states.contains(key)) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_G) {
				key_states[key] = !key_states[key];
			}
			else {
				key_states[key] = true;
			}
		}
		else if (action == GLFW_RELEASE && key != GLFW_KEY_G) {
			key_states[key] = false;
		}
	}
}

void InputHandler::execute_key_action(float deltatime, GLFWwindow* window, std::vector<PointLight*> lights, uint8_t lightsNum, DirectionalLight& dirLight, std::vector<Object*>& objects) {
	PointLight** lightsArr = lights.data();
	for (auto const& [key, state] : key_states) {
		if (state && key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
		else if (key == GLFW_KEY_G) {
			ImguiHandler::showWindow(key_states[key], window, lightsArr, lightsNum, dirLight, objects);
		}
		else if (state) {
			camera->ProcessKeyboard(key_direction_map[key], deltatime);
		}
	}
}

void InputHandler::mouse_callback(GLFWwindow* window, float xPos, float yPos) {
	if (firstMove) {
		lastMouseX = xPos;
		lastMouseY = yPos;
		firstMove = false;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent(xPos, yPos);

	float offsetX = xPos - lastMouseX;
	float offsetY = lastMouseY - yPos;

	if (!key_states[GLFW_KEY_G]) {
		camera->ProcessMouseMovement(offsetX, offsetY);
	}
	lastMouseX = xPos;
	lastMouseY = yPos;
}

void InputHandler::scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent(xOffset, yOffset);
}

void InputHandler::resize_callback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}