#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "mesh.h"
#include <stb_image.h>
#include <string>
#include "shaderClass.h"
#include "Lights/Light.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <functional>
#include "Object.h"

class Model : Object {
public:
	uint32_t id;
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	glm::vec3 position;
	glm::vec3 rotationAxis;
	float rotationAngle;
	glm::vec3 scale;

	glm::vec3 matAmbient;
	glm::vec3 matDiffuse;
	glm::vec3 matSpecular;
	float matShining;

	GLenum textureType;
	unsigned int textureId;



	Model(uint32_t& objectNum, std::vector<Object*>& objects);


	Model(std::string const path, uint32_t& objectNum);


	Model(std::string const path, uint32_t& objectNum, std::vector<Object*>& objects);


	void Draw(Shader* shader, bool customTexture, GLenum drawMode);

	void Draw(Shader* shader, GLenum type, unsigned int texId, GLenum drawMode);

	void loadCubeMap(std::vector<std::string> faces);

	void ImGuiSection();
	
private:

	void initValues();

	void loadModel(std::string const path);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(const char* path, const std::string& directory);

	void setTransforms(Shader* shader);
};