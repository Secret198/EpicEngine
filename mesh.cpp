#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->verticies = verticies;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
	sendToShader();
}

Mesh::Mesh() {
	verticies = std::vector<Vertex>();
	indices = std::vector<unsigned int>();
	textures = std::vector<Texture>();
}
void Mesh::Draw(Shader* shader, GLenum drawMode) {
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse") {
			number += std::to_string(diffuseNum++);
		}
		else if (name == "texture_specular") {
			number += std::to_string(specularNum++);
		}

		glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glActiveTexture(GL_TEXTURE0);


	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(drawMode, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(Shader* shader, GLenum textureType, unsigned int textureId, GLenum drawMode) {
	try {
		if (!textureType || !textureId) {
			throw (textureId);
		}
		glBindTexture(textureType, textureId);


		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(drawMode, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}
	catch (...) {
		std::cout << "Textureid of texturetype is undefined";
	}

}

void Mesh::sendToShader() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}
