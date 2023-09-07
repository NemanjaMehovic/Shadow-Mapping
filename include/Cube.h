#pragma once

class Cube
{
	static const float data[];
	unsigned int VAO;
	unsigned int VBO;
public:
	Cube();
	~Cube();
	void activate() const;
};