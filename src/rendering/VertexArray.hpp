#pragma once
#include "Buffers.hpp"
#include "VertexLayout.hpp"
#include <iostream>

class VertexArray {
public:
	VertexArray(const VertexBuffer& VBO, const VertexLayout& layout);
	~VertexArray();

	VertexArray(const VertexArray& other) = delete;
	VertexArray& operator=(const VertexArray& other) = delete;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void bind() const;
	void unbind() const;

	[[nodiscard]] unsigned int getID() const { return m_ArrayID; }
private:
	unsigned int m_ArrayID;
};