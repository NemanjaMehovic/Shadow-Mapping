#pragma once
#include<string>
#include<glm/glm.hpp>


class Shader
{
	unsigned int shaderId;
	unsigned int createShader(const std::string& shaderFileName, const unsigned int& shaderType) const;
	void checkShaderCompileError(const unsigned int& id, const std::string& shaderName) const;
	void checkShaderLinkerError() const;
	std::string readShaderFileData(const std::string& shaderFileName) const;
public:
	Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	~Shader();
	void activate() const;
	void setMat3(const glm::mat3& transform, const std::string& name) const;
	void setMat4(const glm::mat4& transform, const std::string& name) const;
	void setVec3(const glm::vec3& vec3, const std::string& name) const;
	void setBool(const bool& value, const std::string& name) const;
	void setInt(const int& value, const std::string& name) const;
};
