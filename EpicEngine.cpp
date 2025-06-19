#include "EpicEngine.h"


using namespace std;

int w_width = 1280;
int w_height = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
InputHandler inpHandler(&camera);

uint16_t pointLightNum = 0u;
uint16_t spotLightNum = 0u;
uint32_t objectNum = 0u;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "EpicEngine", NULL, NULL);

	if (window == NULL) {
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to load opengl" << endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, w_width, w_height);

	ImguiHandler::setupImgui(window);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	Shader lightIconShader("E:/projects/EpicEngine/shaders/blinnPhongVert.glsl", "E:/projects/EpicEngine/shaders/showLight.glsl");
	Shader blinnPhongShader("E:/projects/EpicEngine/shaders/blinnPhongVert.glsl", "E:/projects/EpicEngine/shaders/blinnPhongFrag.glsl");
	blinnPhongShader.use();

	vector<PointLight*> lights;
	vector<Model*> objects;



	Model cube("E:/projects/EpicEngine/models/cube/test.fbx", objectNum, objects);
	cube.position.x = -2.0;

	Model plane("E:/projects/EpicEngine/models/plane/plane.obj", objectNum, objects);
	plane.scale = glm::vec3(5.0);
	plane.position.y = -1.0;

	Model monkey("E:/projects/EpicEngine/models/monkey/monkey.obj", objectNum, objects);
	monkey.position.x = -5.5;
	monkey.matDiffuse = glm::vec3(1.0, 0.2, 0.1);

	DirectionalLight sun(glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f));
	PointLight thePoint(pointLightNum, lights, objectNum);
	SpotLight theSpot(spotLightNum, lights, objectNum);

	//MarchingCubes theCubes(0.3f, 10.0f, blinnPhongShader, objectNum, objects, glm::vec3(-1.0), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));
	MarchingCubes theCubes(1.0f, 10.0f, blinnPhongShader, objectNum, objects, glm::vec3(-2.0), glm::vec3(2.0f, -2.0f, -2.0f), glm::vec3(2.0f, -2.0f, 2.0f), glm::vec3(-2.0f, -2.0f, 2.0f), glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(-2.0f, 2.0f, 2.0f));

	glm::mat4 projection = glm::perspective(camera.Zoom, (float)w_width / (float)w_height, 0.1f, 100.0f);

	//Bind view projection uniform buffers to binding point 0
	glUniformBlockBinding(blinnPhongShader.ID, blinnPhongShader.viewProjBlockIndex, 0);
	glUniformBlockBinding(lightIconShader.ID, lightIconShader.viewProjBlockIndex, 0);

	//Create view projection uniform buffer and set projection matrix
	uint32_t viewProjectionUBO;
	glGenBuffers(1, &viewProjectionUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, viewProjectionUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, viewProjectionUBO);

	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Set callbacks
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetWindowAspectRatio(window, 16, 9);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImguiHandler::newFrameImgui();

		blinnPhongShader.use();
		blinnPhongShader.set3fv("viewPosition", camera.Position);

		glm::mat4 view = camera.GetViewMatrix();
		
		glBindBuffer(GL_UNIFORM_BUFFER, viewProjectionUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		sun.sendToShader(blinnPhongShader);
		theSpot.Draw(blinnPhongShader, lightIconShader);

		thePoint.Draw(blinnPhongShader, lightIconShader);

		theCubes.Draw();

		monkey.Draw(blinnPhongShader, false, GL_TRIANGLES);

		cube.Draw(blinnPhongShader, false, GL_TRIANGLES);
		plane.Draw(blinnPhongShader, false, GL_TRIANGLES);

		inpHandler.execute_key_action(deltaTime, window, lights, pointLightNum + spotLightNum, sun, objects);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	ImguiHandler::destroyImgui();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	inpHandler.key_callback(window, key, scancode, action, mods);
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
	inpHandler.mouse_callback(window, xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	inpHandler.scroll_callback(window, xOffset, yOffset);
}

void resizeCallback(GLFWwindow* window, int width, int height) {
	inpHandler.resize_callback(window, width, height);
}
