#pragma once
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Utils
{
	static glm::vec3 makeVector(const std::vector<float>& vertices, const int& index0, const int& index1);
public:
	static std::vector<float>* Normals(const std::vector<float>& vertices);
};