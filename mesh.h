#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shaderClass.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh {
public:
	//mesh data
	std::vector<Vertex> verticies;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

	Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Texture> textures);

	Mesh();

	void Draw(Shader* shader, GLenum drawMode);

	void Draw(Shader* shader, GLenum textureType, unsigned int textureId, GLenum drawMode);

	void sendToShader();

	void setupMesh();
private:
	unsigned int VBO, EBO;
	

};