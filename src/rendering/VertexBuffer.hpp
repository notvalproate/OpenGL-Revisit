#pragma once
#include "GL/glew.h"
#include <vector>

class VertexBuffer {
public:
	VertexBuffer(const std::vector<float>& p_Data);
	~VertexBuffer();

	void Bind();
	void Unbind();
private:
	unsigned int m_BufferID;
};