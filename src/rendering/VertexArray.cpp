#include "VertexArray.hpp"
#include <iostream>
#include <numeric>
#include "../util/ErrorHandling.hpp"

VertexArray::VertexArray(const VertexBuffer& a_VBO, const std::span<unsigned int>& a_Layout) {
	GLCall(glGenVertexArrays(1, &m_ArrayID));
	GLCall(glBindVertexArray(m_ArrayID));
	a_VBO.bind();

	size_t i = 0;
	size_t offset = 0;
	size_t typeSize = sizeof(float);
	size_t stride = std::accumulate(a_Layout.begin(), a_Layout.end(), 0) * sizeof(float);

	for (const auto& element : a_Layout) {
		GLCall(glVertexAttribPointer(i, element, GL_FLOAT, GL_FALSE, stride, (const void*)offset));
		GLCall(glEnableVertexAttribArray(i++));
		offset += typeSize * element; 
	}

	GLCall(glBindVertexArray(0));
	a_VBO.unbind();
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

void VertexArray::bind() const {
	GLCall(glBindVertexArray(m_ArrayID));
}

void VertexArray::unbind() const {
	GLCall(glBindVertexArray(0));
}