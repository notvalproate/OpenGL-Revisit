#pragma once
#include <GL/glew.h>
#include <span>
#include "../util/ErrorHandling.hpp"

class BufferBase {
public:
	template<typename T>
	BufferBase(const std::span<T>& data, unsigned int type) : m_BufferType(type) {
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glBindBuffer(m_BufferType, m_BufferID));
		GLCall(glBufferData(m_BufferType, data.size_bytes(), data.data(), GL_DYNAMIC_DRAW));
		GLCall(glBindBuffer(m_BufferType, 0));
	}
	virtual ~BufferBase();

	BufferBase(const BufferBase& other) = delete;
	BufferBase& operator=(const BufferBase& other) = delete;

	BufferBase(BufferBase&& other) noexcept;
	BufferBase& operator=(BufferBase&& other) noexcept;

	void bind() const;
	void unbind() const;
private:
	unsigned int m_BufferID;
	unsigned int m_BufferType;
};

class VertexBuffer : public BufferBase {
public:
	VertexBuffer(const std::span<float>& data) : BufferBase(data, GL_ARRAY_BUFFER) { }
};

class IndexBuffer : public BufferBase {
public:
	IndexBuffer(const std::span<unsigned int>& data) 
		: BufferBase(data, GL_ELEMENT_ARRAY_BUFFER)
		, m_IndexCount(data.size()) { }

	[[nodiscard]] unsigned int getCount() const { return m_IndexCount; }
private:
	size_t m_IndexCount;
};