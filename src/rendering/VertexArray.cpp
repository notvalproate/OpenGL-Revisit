#include "VertexArray.hpp"
#include <iostream>
#include <numeric>
#include "../util/ErrorHandling.hpp"

VertexArray::VertexArray(VertexBuffer& p_VBO, const std::span<unsigned int>& p_Layout) {
	GLCall(glGenVertexArrays(1, &m_ArrayID));
	GLCall(glBindVertexArray(m_ArrayID));
	p_VBO.Bind();

	size_t i = 0;
	size_t t_Offset = 0;
	size_t t_TypeSize = sizeof(float);
	size_t t_Stride = std::accumulate(p_Layout.begin(), p_Layout.end(), 0) * sizeof(float);

	for (const auto t_Element : p_Layout) {
		GLCall(glVertexAttribPointer(i, t_Element, GL_FLOAT, GL_FALSE, t_Stride, (const void*)t_Offset));
		GLCall(glEnableVertexAttribArray(i++));
		t_Offset += t_TypeSize * t_Element; 
	}

	GLCall(glBindVertexArray(0));
	p_VBO.Unbind();
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_ArrayID));
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

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_ArrayID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}