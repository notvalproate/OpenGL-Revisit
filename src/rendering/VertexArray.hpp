#pragma once
#include "Buffers.hpp"

class VertexArray {
public:
	VertexArray(VertexBuffer& p_VBO);
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