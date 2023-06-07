#include "VertexArray.hpp"

VertexArray::VertexArray(VertexBuffer& p_VBO) {
	glGenVertexArrays(1, &m_ArrayID);
	glBindVertexArray(m_ArrayID);
	p_VBO.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0); 
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float))); 
	glEnableVertexAttribArray(1); 

	glBindVertexArray(0);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_ArrayID);
}

VertexArray::VertexArray(VertexArray&& other) noexcept {
	*this = std::move(other);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_ArrayID = other.m_ArrayID;
	other.m_ArrayID = 0;

	return *this;
}

void VertexArray::Bind() {
	glBindVertexArray(m_ArrayID);
}

void VertexArray::Unbind() {
	glBindVertexArray(m_ArrayID);
}