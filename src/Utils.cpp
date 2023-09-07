#include<Utils.h>

glm::vec3 Utils::makeVector(const std::vector<float>& vertices, const int& index0, const int& index1)
{
	float x = vertices[index1 + 0] - vertices[index0 + 0];
	float y = vertices[index1 + 1] - vertices[index0 + 1];
	float z = vertices[index1 + 2] - vertices[index0 + 2];

	return glm::vec3(x, y, z);
}

std::vector<float>* Utils::Normals(const std::vector<float>& vertices)
{
	std::vector<float>* normals = new std::vector<float>(vertices.size());

	for (int i = 0; i < (vertices.size() / 9); i++) {
		int index0 = i * 9;
		int index1 = i * 9 + 3;
		int index2 = i * 9 + 6;

		glm::vec3 vector0 = makeVector(vertices, index0, index1);
		glm::vec3 vector1 = makeVector(vertices, index0, index2);

		glm::vec3 normal = glm::cross(vector0, vector1);
		normal = glm::normalize(normal);

		for (int j = 0; j < 3; ++j) {
			int index = i * 9 + j * 3;
			(*normals)[index + 0] = normal.x;
			(*normals)[index + 1] = normal.y;
			(*normals)[index + 2] = normal.z;
		}
	}

	return normals;
}
