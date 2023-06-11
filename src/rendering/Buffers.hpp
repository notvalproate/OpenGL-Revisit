#pragma once
#include <GL/glew.h>
#include <span>
#include "../util/ErrorHandling.hpp"

class BufferBase {
public:
	template<typename T>
	BufferBase(const std::span<T>& p_Data, unsigned int p_Type) : m_BufferType(p_Type) {
		GLCall(glGenBuffers(1, &m_BufferID));
		GLCall(glBindBuffer(m_BufferType, m_BufferID));
		GLCall(glBufferData(m_BufferType, sizeof(T) * p_Data.size(), p_Data.data(), GL_STATIC_DRAW));
		GLCall(glBindBuffer(m_BufferType, 0));
	}
	~BufferBase();

	BufferBase(const BufferBase& other) = delete;
	BufferBase& operator=(const BufferBase& other) = delete;

	BufferBase(BufferBase&& other) noexcept;
	BufferBase& operator=(BufferBase&& other) noexcept;

	void Bind() const;
	void Unbind() const;

	[[nodiscard]] unsigned int GetID() const { return m_BufferID; }
private:
	unsigned int m_BufferID;
	unsigned int m_BufferType;
};

class VertexBuffer : public BufferBase {
public:
	VertexBuffer(const std::span<float>& p_Data) : BufferBase(p_Data, GL_ARRAY_BUFFER) { }
};

class IndexBuffer : public BufferBase {
public:
	IndexBuffer(const std::span<unsigned int>& p_Data) 
		: BufferBase(p_Data, GL_ELEMENT_ARRAY_BUFFER)
		, m_IndexCount(p_Data.size()) { }

	[[nodiscard]] unsigned int GetCount() const { return m_IndexCount; }
private:
	size_t m_IndexCount;
};