#include "MarchingCubes.h"

MarchingCubes::MarchingCubes(float isoValue, float noiseScale, float segmentNum, Shader* shaderProgram, uint32_t& objectNum, std::vector<Object*>& objects, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 vert3, glm::vec3 vert4, glm::vec3 vert5, glm::vec3 vert6, glm::vec3 vert7)
	: isoValue(isoValue),
	segmentNum(segmentNum),
	shaderProgram(shaderProgram),
	objectNum(objectNum),
	marchModel(objectNum, objects),
	noise(),
	noiseScale(noiseScale)
{
	boundingBox[0] = vert0;
	boundingBox[1] = vert1;
	boundingBox[2] = vert2;
	boundingBox[3] = vert3;
	boundingBox[4] = vert4;
	boundingBox[5] = vert5;
	boundingBox[6] = vert6;
	boundingBox[7] = vert7;

	objects.push_back(this);

	marchModel.meshes.push_back(Mesh());
	marchModel.meshes[0].setupMesh();
	ConstructMesh();
}

void MarchingCubes::ImGuiSection() {
	char name[50];
	std::string idStr = std::to_string(marchModel.id);
	Light::concatStrings(name, "Iso level##", idStr.c_str(), "");
	ImGui::DragFloat(name, &isoValue, 0.01f);

	Light::concatStrings(name, "Noise scale##", idStr.c_str(), "");
	ImGui::DragFloat(name, &noiseScale, 0.01f);

}

int MarchingCubes::findVertex(std::vector<Vertex>& vect, Vertex& vert) {
	for (int i = 0; i < vect.size(); i++) {
		if (vect[i].Position == vert.Position) {
			return i;
		}
	}
	return -1;
}

int MarchingCubes::findIndex(std::vector<uint32_t>& indices, uint32_t ind) {
	for (int i = 0; i < indices.size(); i++) {
		if (indices[i] == ind) {
			return i;
		}
	}
	return -1;
}

float MarchingCubes::sphereEquation(float x, float y, float z) {
	return x * x + y * y + z * z - 1.0f;
}

uint16_t MarchingCubes::CalcCubeIndex(GridCell& cube) {
	uint16_t cubeIndex = 0;
	for (uint16_t i = 0; i < 8; i++) {
		if (cube.value[i] < isoValue) {
			cubeIndex |= (int)pow(2, i);
		}
	}
	return cubeIndex;
}

void MarchingCubes::calcCubeValues(GridCell& cube) {
	for (int i = 0; i < 8; i++) {
		//cube.value[i] = sphereEquation(cube.vert[i].Position.x, cube.vert[i].Position.y, cube.vert[i].Position.z);
		cube.value[i] = noise.getValue(cube.vert[i].Position, noiseScale);
	}
}

glm::vec3 MarchingCubes::calcNormal(glm::vec3 vert1, glm::vec3 vert2, glm::vec3 vert3) {
	glm::vec3 vector1 = vert2 - vert1;
	glm::vec3 vector2 = vert3 - vert1;

	return glm::cross(vector1, vector2);
}

void MarchingCubes::calcSurfaceNormals(std::map<uint16_t, glm::vec3>& faceVectors, std::map<uint16_t, std::vector<uint16_t>>& adjecentFaces, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
	for (uint16_t i = 0; i < indices.size(); i += 3) {
		faceVectors.insert({ i / 3, calcNormal(vertices[indices[i]].Position, vertices[indices[i + 1]].Position, vertices[indices[i + 2]].Position) });
		if (adjecentFaces.contains(indices[i])) {
			adjecentFaces[indices[i]].push_back(i / 3);
		}
		else {
			adjecentFaces.insert({ indices[i], std::vector<uint16_t>(1, i / 3) });
		}
		if (adjecentFaces.contains(indices[i + 1])) {
			adjecentFaces[indices[i + 1]].push_back(i / 3);
		}
		else {
			adjecentFaces.insert({ indices[i + 1], std::vector<uint16_t>(1, i / 3) });

		}
		if (adjecentFaces.contains(indices[i + 2])) {
			adjecentFaces[indices[i + 2]].push_back(i / 3);
		}
		else {
			adjecentFaces.insert({ indices[i + 2], std::vector<uint16_t>(1, i / 3) });

		}
	}
}

void MarchingCubes::calcSmoothVertexNormals(std::map<uint16_t, glm::vec3>& faceVectors, std::map<uint16_t, std::vector<uint16_t>>& adjecentFaces, std::vector<Vertex>& vertices) {
	for (auto const& [vert, faces] : adjecentFaces) {
		glm::vec3 normalSum = glm::vec3(0.0);
		for (auto i : faces) {
			normalSum += glm::normalize(faceVectors[i]);
		}
		//normalSum /= faces.size();
		vertices[vert].Normal = glm::normalize(normalSum);
	}
}

void MarchingCubes::calcHardVertexNormals(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
	std::vector<uint32_t> setIndices;
	uint16_t indexBufSize = indices.size();
	for (uint16_t i = 0; i < indexBufSize; i += 3) {
		glm::vec3 faceNormal = calcNormal(vertices[indices[i]].Position, vertices[indices[i + 1]].Position, vertices[indices[i + 2]].Position);

		if (findIndex(setIndices, indices[i]) != -1) {
			Vertex newVert;
			newVert.Position = vertices[indices[i]].Position;
			newVert.Normal = faceNormal;
			vertices.push_back(newVert);
			indices.push_back(vertices.size() - 1);
		}
		else {
			vertices[indices[i]].Normal = faceNormal;
			setIndices.push_back(indices[i]);
		}

		if (findIndex(setIndices, indices[i + 1]) != -1) {
			Vertex newVert;
			newVert.Position = vertices[indices[i + 1]].Position;
			newVert.Normal = faceNormal;
			vertices.push_back(newVert);
			indices.push_back(vertices.size() - 1);
		}
		else {
			vertices[indices[i + 1]].Normal = faceNormal;
			setIndices.push_back(indices[i + 1]);
		}

		if (findIndex(setIndices, indices[i + 2]) != -1) {
			Vertex newVert;
			newVert.Position = vertices[indices[i + 2]].Position;
			newVert.Normal = faceNormal;
			vertices.push_back(newVert);
			indices.push_back(vertices.size() - 1);
		}
		else {
			vertices[indices[i + 2]].Normal = faceNormal;
			setIndices.push_back(indices[i + 2]);
		}
	}
}

glm::vec3 MarchingCubes::calcVertexPosition(GridCell& cube, uint16_t sideIndex) {
	return P1 + (isoValue - V1) * (P2 - P1) / (V2 - V1);
}

void MarchingCubes::Draw() {

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	marchModel.Draw(shaderProgram, false, GL_TRIANGLES);
}

void MarchingCubes::ConstructMesh() {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	float sideX = abs(boundingBox[0].x - boundingBox[1].x);
	float sideY = abs(boundingBox[0].y - boundingBox[4].y);
	float sideZ = abs(boundingBox[0].z - boundingBox[3].z);
	float segmentDistX = sideX / segmentNum;
	float segmentDistY = sideY / segmentNum;
	float segmentDistZ = sideZ / segmentNum;


	for (float x = boundingBox[0].x; x < boundingBox[1].x + segmentDistX; x += segmentDistX) {
		for (float y = boundingBox[0].y; y < boundingBox[4].y + segmentDistY; y += segmentDistY) {
			for (float z = boundingBox[0].z; z < boundingBox[3].z + segmentDistZ; z += segmentDistZ) {
				GridCell cube;
				cube.vert[0].Position = glm::vec3(x, y, z);
				cube.vert[1].Position = glm::vec3(x + segmentDistX, y, z);
				cube.vert[2].Position = glm::vec3(x + segmentDistX, y, z + segmentDistZ);
				cube.vert[3].Position = glm::vec3(x, y, z + segmentDistZ);
				cube.vert[4].Position = glm::vec3(x, y + segmentDistY, z);
				cube.vert[5].Position = glm::vec3(x + segmentDistX, y + segmentDistY, z);
				cube.vert[6].Position = glm::vec3(x + segmentDistX, y + segmentDistY, z + segmentDistZ);
				cube.vert[7].Position = glm::vec3(x, y + segmentDistY, z + segmentDistZ);

				calcCubeValues(cube);

				uint16_t cubeIndex = CalcCubeIndex(cube);
				uint16_t sides = edgeTable[cubeIndex];
				uint32_t localIndices[12];

				//Creating new verices
				for (uint16_t l = 0; l < 12; l++) {
					if (sides & (1 << l)) {
						Vertex newVertex;
						newVertex.Position = calcVertexPosition(cube, l);

						uint32_t vertexIndex = findVertex(vertices, newVertex);

						if (vertexIndex == -1) {
							localIndices[l] = vertices.size();
							vertices.push_back(newVertex);
						}
						else {
							localIndices[l] = vertexIndex;
						}
					}
				}

				//Constructing triangles and index buffer
				for (uint16_t l = 0; triTable[cubeIndex][l] != -1; l++) {
					indices.push_back(localIndices[triTable[cubeIndex][l]]);
				}
			}
		}
	}

	std::map<uint16_t, glm::vec3> faceVectors;
	std::map<uint16_t, std::vector<uint16_t>> adjecentFaces;

	calcSurfaceNormals(faceVectors, adjecentFaces, vertices, indices);
	calcSmoothVertexNormals(faceVectors, adjecentFaces, vertices);

	marchModel.meshes[0].indices = indices;
	marchModel.meshes[0].verticies = vertices;

	marchModel.meshes[0].sendToShader();

	Draw();
}

MarchingCubes::~MarchingCubes() {
	delete edgeTable;
	delete edgeTovertex;
}