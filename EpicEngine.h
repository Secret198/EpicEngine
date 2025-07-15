// EpicEngine.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"
#include "models/model.h"
#include "shaders/shaderClass.h"
#include "inputHandler.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "imGuiHandler.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "MarchingCubes.h"
#include "Object.h"
#include "PostProcessing.h"
#include <glm/glm.hpp>

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void resizeCallback(GLFWwindow* window, int width, int height);

