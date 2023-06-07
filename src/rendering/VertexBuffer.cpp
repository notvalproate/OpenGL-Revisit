#include "VertexBuffer.hpp"
#include <iostream>

VertexBuffer::VertexBuffer(const std::vector<float>& p_Data) { 
	glGenBuffers(1, &m_BufferID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * p_Data.size(), &p_Data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_BufferID);
}

void VertexBuffer::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}

void VertexBuffer::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
