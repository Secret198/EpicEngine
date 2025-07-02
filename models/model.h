#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "mesh.h"
#include <stb_image.h>
#include <string>
#include "../shaders/shaderClass.h"
#include "../Lights/Light.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <functional>
#include "../Object.h"

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



	Model(uint32_t& objectNum, std::vector<Object*>& objects) :
		rotationAngle(0.0f),
		matShining(64.0f),
		id(objectNum++)
	{
		initValues();

		objects.push_back(this);
	}

	Model(std::string const path, uint32_t& objectNum) :
		rotationAngle(0.0f),
		matShining(64.0f),
		id(objectNum++)
	{
		loadModel(path);
		initValues();

	}

	Model(std::string const path, uint32_t& objectNum, std::vector<Object*> &objects) :
		rotationAngle(0.0f),
		matShining(64.0f),
		id(objectNum++)
	{
		loadModel(path);
		initValues();

		objects.push_back(this);
	}

	void Draw(Shader* shader, bool customTexture, GLenum drawMode) {
		setTransforms(shader);
		for (int i = 0; i < meshes.size(); i++)
		{
			if (!customTexture) {
				meshes[i].Draw(shader, drawMode);
			}
			else {
				meshes[i].Draw(shader, textureType, textureId, drawMode);
			}
		}
	}

	void Draw(Shader* shader, GLenum type, unsigned int texId, GLenum drawMode) {
		setTransforms(shader);
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader, type, texId, drawMode);
		}
	}

	void loadCubeMap(std::vector<std::string> faces) {
		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

		int width, height, nbrOfChannels;
		for (unsigned int i = 0; i < faces.size(); i++) {
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nbrOfChannels, 0);
			if (data) {
				GLenum format;
				if (nbrOfChannels == 3) {
					format = GL_RGB;
				}
				else if (nbrOfChannels == 4) {
					format = GL_RGBA;
				}
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else {
				std::cout << "Failed to load cubemap at path: " << faces[i] << std::endl;
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		}

		this->textureId = textureId;
		this->textureType = GL_TEXTURE_CUBE_MAP;
	}

	void ImGuiSection() {
		char name[50];
		Light::concatStrings(name, "Object ", std::to_string(id).c_str(), "");
		ImGui::Separator();
		ImGui::Text(name);

		ImGui::Separator();
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);

		Light::concatStrings(name, "x##posObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.x, 0.01f);
		ImGui::SameLine();

		Light::concatStrings(name, "y##posObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.y, 0.01f);
		ImGui::SameLine();

		Light::concatStrings(name, "z##posObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &position.z, 0.01f);


		ImGui::Text("Rotation axis");
		ImGui::SameLine();
		Light::concatStrings(name, "x##rotObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &rotationAxis.x, 0.01f);
		ImGui::SameLine();

		Light::concatStrings(name, "y##rotObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &rotationAxis.y, 0.01f);
		ImGui::SameLine();

		Light::concatStrings(name, "z##rotObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &rotationAxis.z, 0.01f);

		Light::concatStrings(name, "Angle##rotObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &rotationAngle, 0.1f);


		ImGui::Text("Scale");
		ImGui::SameLine();
		Light::concatStrings(name, "x##scaleObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &scale.x, 0.01f);
		ImGui::SameLine();

		Light::concatStrings(name, "y##scaleObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &scale.y, 0.01f);
		ImGui::SameLine();

		Light::concatStrings(name, "z##scaleObj", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &scale.z, 0.01f);


		ImGui::Separator();

		ImGui::PushItemWidth(200.0f);
		Light::concatStrings(name, "Ambient##ambObj", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&matAmbient, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::SameLine();

		Light::concatStrings(name, "Diffuse##diffObj", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&matDiffuse, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);

		ImGui::SameLine();

		Light::concatStrings(name, "Specular##specObj", std::to_string(id).c_str(), "");
		ImGui::ColorPicker3(name, (float*)&matSpecular, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoSidePreview);


		ImGui::PushItemWidth(100.0f);
		Light::concatStrings(name, "Shining##", std::to_string(id).c_str(), "");
		ImGui::DragFloat(name, &matShining, 0.1f, 0.0f, 1000.0f);

		ImGui::Separator();

	}
	
private:

	void initValues() {
		position = glm::vec3(1.0f);
		rotationAxis = glm::vec3(1.0f);
		scale = glm::vec3(1.0f);
		matAmbient = glm::vec3(1.0f);
		matDiffuse = glm::vec3(1.0f);
		matSpecular = glm::vec3(1.0f);
	}

	void loadModel(std::string const path) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP" << import.GetErrorString() << std::endl;
			return;
		}

		directory = path.substr(0, path.find_last_of("/"));

		processNode(scene->mRootNode, scene);
	}
	void processNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indicies;
		std::vector<Texture> textures;

		//Reading vertex positions and normal vectors
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 tempVector;
			tempVector.x = mesh->mVertices[i].x;
			tempVector.y = mesh->mVertices[i].y;
			tempVector.z = mesh->mVertices[i].z;
			vertex.Position = tempVector;

			if (mesh->HasNormals()) {
				tempVector.x = mesh->mNormals[i].x;
				tempVector.y = mesh->mNormals[i].y;
				tempVector.z = mesh->mNormals[i].z;
				vertex.Normal = tempVector;
			}


			if (mesh->mTextureCoords[0]) {
				glm::vec2 tempVector2;

				tempVector2.x = mesh->mTextureCoords[0][i].x;
				tempVector2.y = mesh->mTextureCoords[0][i].y;

				vertex.TexCoords = tempVector2;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}

		//Reading indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indicies.push_back(face.mIndices[j]);
			}
		}

		//Reading the materials
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		
		return Mesh(vertices, indicies, textures);
	}

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0) {
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}


		}

		return textures;
	}

	unsigned int TextureFromFile(const char* path, const std::string& directory) {
		std::string fileName = std::string(path);
		fileName = directory + "/" + fileName;

		unsigned int textureId;
		glGenTextures(1, &textureId);

		int width, height, ComponentsNum;
		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &ComponentsNum, 0);

		if (data) {
			GLenum format;
			switch (ComponentsNum)
			{
				case 1:
					format = GL_RED;
					break;
				case 3:
					format = GL_RGB;
					break;
				case 4:
					format = GL_RGBA;
					break;
				default:
					std::cout << "idk what's going on" << std::endl;
					break;
			}

			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else {
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureId;
	}

	void setTransforms(Shader* shader) {
		shader->use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);

		model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);

		model = glm::scale(model, scale);

		shader->setMatrix4fv("model", model);

		shader->set3fv("material.ambient", matAmbient);
		shader->set3fv("material.diffuse", matDiffuse);
		shader->set3fv("material.specular", matSpecular);
		shader->set1f("material.shining", matShining);

		bool hasDiffuse = false;
		bool hasSpecular = false;
		if (textures_loaded.size() == 0) {
			shader->set1b("textureMaps.hasDiffuseMap", false);
			shader->set1b("textureMaps.hasSpecularTexture", false);
		}
		else {
			for (auto i : textures_loaded) {
				if (i.type == "texture_diffuse") {
					hasDiffuse = true;
				}
				if (i.type == "texture_specular") {
					hasSpecular = true;
				}
			}
			shader->set1b("textureMaps.hasDiffuseMap", hasDiffuse);
			shader->set1b("textureMaps.hasSpecularTexture", hasSpecular);

		}
	}
};