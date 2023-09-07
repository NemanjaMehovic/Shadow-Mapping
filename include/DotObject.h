#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DotObject
{
	static const int moveSpeed;
	float fov;
	float radius;
	float xAngle;
	float yAngle;
	float rangeLimit(float val);
public:
	DotObject(const float& xAngle, const float& yAngle, const float& radius);
	~DotObject() = default;
	const glm::mat4 getView() const;
	const glm::mat4 getProjection(const unsigned int& width, const unsigned int& height) const;
	const glm::mat4 getOrtho() const;
	const glm::vec3 getPosition() const;
	void moveRight(const float deltaTime);
	void moveLeft(const float deltaTime);
	void moveUp(const float deltaTime);
	void moveDown(const float deltaTime);
	void zoom(const float change);
};