#include "EpicEngine.h"


using namespace std;

int w_width = 1280;
int w_height = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
InputHandler inpHandler(&camera);


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

	//Test code
	Shader testShader("E:/projects/EpicEngine/shaders/blinnPhongVert.glsl", "E:/projects/EpicEngine/shaders/blinnPhongFrag.glsl");
	testShader.use();

	Model testObj("E:/projects/EpicEngine/models/cube/cube.obj");
	testObj.position.y = 2.0;

	Model plane("E:/projects/EpicEngine/models/plane/plane.obj");
	plane.scale = glm::vec3(5.0);
	plane.position.y = -1.0;

	Model monkey("E:/projects/EpicEngine/models/monkey/monkey.obj");
	monkey.position.x = -1.5;

	PointLight testLight(0);
	testShader.set1i("pointLightsNum", 1);
	testShader.set1i("spotLightsNum", 0);
	testLight.position.z = 2.5;
	testLight.position.y = 0.2;


	/*glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.5f));
	testShader.use();
	testShader.setMatrix4fv("model", model);*/

	glm::mat4 projection = glm::perspective(camera.Zoom, (float)w_width / (float)w_height, 0.1f, 100.0f);

	//Bind view projection uniform buffers to binding point 0
	glUniformBlockBinding(testShader.ID, testShader.viewProjBlockIndex, 0);
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

		testShader.use();

		glm::mat4 view = camera.GetViewMatrix();
		
		glBindBuffer(GL_UNIFORM_BUFFER, viewProjectionUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//test code

		testObj.Draw(testShader, false, GL_TRIANGLES);

		plane.Draw(testShader, false, GL_TRIANGLES);

		monkey.Draw(testShader, false, GL_TRIANGLES);

		testLight.Draw(testShader, lightIconShader);

		inpHandler.execute_key_action(deltaTime, window);

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
