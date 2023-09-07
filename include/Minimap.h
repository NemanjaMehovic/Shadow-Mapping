#pragma once

class Minimap
{
	static const float data[];
	unsigned int VAO;
	unsigned int VBO;
public:
	Minimap();
	~Minimap();
	void activate() const;
};