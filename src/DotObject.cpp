#include<DotObject.h>

const int DotObject::moveSpeed = 100;

DotObject::DotObject(const float& xAngle, const float& yAngle, const float& radius) : xAngle(xAngle), yAngle(yAngle), radius(radius), fov(45) {}

const glm::mat4 DotObject::getView() const
{
	glm::vec3 dotPos = getPosition();
	if((yAngle < 90 && yAngle > -90) || (yAngle >= 270 && yAngle <= 360) || (yAngle >= -360 && yAngle <= -270))
		return glm::lookAt(dotPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1, 0));
	else
		return glm::lookAt(dotPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1, 0));
}

const glm::mat4 DotObject::getProjection(const unsigned int& width, const unsigned int& height) const
{
	return glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
}

const glm::mat4 DotObject::getOrtho() const
{
	return glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 100.0f);
}

const glm::vec3 DotObject::getPosition() const
{
	glm::vec3 dotPos = glm::vec3(0, 0, radius);
	glm::mat4 mat(1);
	mat = glm::rotate(mat, glm::radians(xAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(yAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	dotPos = glm::vec3(mat * glm::vec4(dotPos, 1.0f));
	return dotPos;
}

float DotObject::rangeLimit(float val)
{
	while (val > 360)
		val = val - 360;
	while (val < -360)
		val = val + 360;
	return val;
}

void DotObject::moveRight(const float deltaTime)
{
	if ((yAngle < 90 && yAngle > -90) || (yAngle >= 270 && yAngle <= 360) || (yAngle >= -360 && yAngle <= -270))
		xAngle = rangeLimit(xAngle + moveSpeed * deltaTime);
	else
		xAngle = rangeLimit(xAngle - moveSpeed * deltaTime);
}

void DotObject::moveLeft(const float deltaTime)
{
	if ((yAngle < 90 && yAngle > -90) || (yAngle >= 270 && yAngle <= 360) || (yAngle >= -360 && yAngle <= -270))
		xAngle = rangeLimit(xAngle -  moveSpeed * deltaTime);
	else
		xAngle = rangeLimit(xAngle + moveSpeed * deltaTime);
}

void DotObject::moveUp(const float deltaTime)
{
	yAngle = rangeLimit(yAngle - moveSpeed * deltaTime);
}

void DotObject::moveDown(const float deltaTime)
{
	yAngle = rangeLimit(yAngle + moveSpeed * deltaTime);
}

void DotObject::zoom(const float change)
{
	radius += change;
	if (radius < 0.1f)
		radius = 0.1f;
}