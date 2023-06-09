#pragma once
#include "Buffers.hpp"
#include <iostream>

class VertexArray {
public:
	VertexArray(VertexBuffer& p_VBO, const std::span<unsigned int>& p_Layout);
	~VertexArray();

	VertexArray(const VertexArray& other) = delete;
	VertexArray& operator=(const VertexArray& other) = delete;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void Bind();
	void Unbind();
private:
	unsigned int m_ArrayID;
};