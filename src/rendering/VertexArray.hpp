#pragma once
#include "Buffers.hpp"
#include <iostream>

class VertexArray {
public:
	VertexArray(const VertexBuffer& VBO, const std::span<unsigned int>& layout);
	~VertexArray();

	VertexArray(const VertexArray& other) = delete;
	VertexArray& operator=(const VertexArray& other) = delete;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void bind() const;
	void unbind() const;
private:
	unsigned int m_ArrayID;
};