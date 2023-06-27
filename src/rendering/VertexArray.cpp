#include "VertexArray.hpp"
#include <iostream>
#include <numeric>
#include "../util/ErrorHandling.hpp"

VertexArray::VertexArray(const VertexBuffer& VBO, const VertexLayout& layout) {
	GLCall(glGenVertexArrays(1, &m_ArrayID));
	GLCall(glBindVertexArray(m_ArrayID));
	VBO.bind();

	std::size_t i = 0;
	std::size_t offset = 0;
	std::size_t typeSize = sizeof(float);
	std::size_t stride = layout.getStride() * typeSize;

	for (const auto& element : layout.getLayoutArray()) {
		std::size_t width = VertexLayout::getAttributeWidth(element);
		GLCall(glVertexAttribPointer(i, width, GL_FLOAT, GL_FALSE, stride, (const void*)offset));
		GLCall(glEnableVertexAttribArray(i++));
		offset += typeSize * width;
	}

	GLCall(glBindVertexArray(0));
	VBO.unbind();
}

VertexArray::~VertexArray() {
	std::cout << "vertex array deleted";
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